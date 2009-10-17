/*
 * PROJECT:     ReactOS System Regression Testing Utility
 * LICENSE:     GNU GPLv2 or any later version as published by the Free Software Foundation
 * PURPOSE:     Processing the incoming debugging data
 * COPYRIGHT:   Copyright 2008-2009 Christoph von Wittich <christoph_vw@reactos.org>
 *              Copyright 2009 Colin Finck <colin@reactos.org>
 */

#include "sysreg.h"
#define BUFFER_SIZE         512

int ProcessDebugData(const char* tty, int timeout, int stage )
{
    char Buffer[BUFFER_SIZE];
    char CacheBuffer[BUFFER_SIZE];
    char Raddr2LineBuffer[BUFFER_SIZE];
    char* bp;
    int got;
    int Ret = EXIT_DONT_CONTINUE;
    int ttyfd;
    struct termios ttyattr, rawattr;
    unsigned int CacheHits = 0;
    unsigned int i;
    unsigned int KdbgHit = 0;

    /* Initialize CacheBuffer with an empty string */
    *CacheBuffer = 0;

    /* ttyfd is the file descriptor of the virtual COM port */
    if ((ttyfd = open(tty, O_NOCTTY | O_RDWR)) < 0)
    {
        SysregPrintf("error opening tty\n");
        return Ret;
    }

    /* We also monitor STDIN_FILENO, so a user can cancel the process with ESC */
    if (tcgetattr(STDIN_FILENO, &ttyattr) < 0)
    {
        SysregPrintf("tcgetattr failed with error %d\n", errno);
        return Ret;
    }

    rawattr = ttyattr;
    rawattr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                                     | IGNCR | ICRNL | IXON);
    rawattr.c_lflag &= ~(ICANON | ECHO | ECHONL);
    rawattr.c_oflag &= ~OPOST;
    rawattr.c_cflag &= ~(CSIZE | PARENB);
    rawattr.c_cflag |= CS8;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawattr) < 0)
    {
        SysregPrintf("tcsetattr failed with error %d\n", errno);
        return Ret;
    }

    for(;;)
    { 
        struct pollfd fds[] = {
            { STDIN_FILENO, POLLIN, 0 },
            { ttyfd, POLLIN, 0 },
        };

        got = poll(fds, (sizeof(fds) / sizeof(struct pollfd)), timeout);
        if (got < 0)
        {
            /* Just try it again on simple errors */
            if (errno == EINTR || errno == EAGAIN)
                continue;

            SysregPrintf("poll failed with error %d\n", errno);
            goto cleanup;
        }
        else if (got == 0)
        {
            /* timeout */
            SysregPrintf("timeout\n");
            Ret = EXIT_CONTINUE;
            goto cleanup;
        }

        for (i = 0; i < (sizeof(fds) / sizeof(struct pollfd)); i++)
        {
            /* Wait till we get some input from the fd */
            if (!(fds[i].revents & POLLIN))
                continue;

            bp = Buffer;

            /* Read one line or a maximum of 511 bytes into a buffer (leave space for the null character) */
            while (bp - Buffer < (BUFFER_SIZE - 1))
            {
                got = read(fds[i].fd, bp, 1);

                if (got < 0)
                {
                    SysregPrintf("read failed with error %d\n", errno);
                    goto cleanup;
                }
                else if (got == 0)
                {
                    /* No more data */
                    break;
                }

                if (fds[i].fd == STDIN_FILENO)
                {
                    /* break on ESC */
                    if (*bp == '\33')
                        goto cleanup;
                }
                else
                {
                    /* Also break on newlines */
                    if(*bp == '\n')
                        break;

                    /* KDBG doesn't send a newline */
                    if ((strstr(Buffer, "kdb:>")) || 
                        (strstr(Buffer, "--- Press q")))
                        break;
                }
                
                ++bp;
            }

            /* The rest of this logic is just about processing the serial output */
            if(fds[i].fd == STDIN_FILENO)
                continue;

            /* Check whether the message is of zero length */
            if (bp == Buffer)
            {
                /* This can happen when the machine shut down (like after 1st or 2nd stage)
                   or after we got a Kdbg backtrace. */
                Ret = EXIT_CONTINUE;
                goto cleanup;
            }

            /* Null-terminate the line */
            *(++bp) = 0;

            /* Detect whether the same line appears over and over again.
               If that is the case, cancel this test after a specified number of repetitions. */
            if(!strcmp(Buffer, CacheBuffer))
            {
                ++CacheHits;

                if(CacheHits > AppSettings.MaxCacheHits)
                {
                    SysregPrintf("Test seems to be stuck in an endless loop, canceled!\n");
                    Ret = EXIT_CONTINUE;
                    goto cleanup;
                }
            }
            else
            {
                CacheHits = 0;
                memcpy(CacheBuffer, Buffer, bp - Buffer + 1);
            }

            /* Output the line, raddr2line the included addresses if necessary */
            if (KdbgHit == 1 && ResolveAddressFromFile(Raddr2LineBuffer, sizeof(Raddr2LineBuffer), Buffer))
                printf("%s", Raddr2LineBuffer);
            else
                printf("%s", Buffer);

            /* Check for "magic" sequences */
            if (strstr(Buffer, "kdb:>"))
            {
                ++KdbgHit;

                if (KdbgHit == 1)
                {
                    /* We hit Kdbg for the first time, get a backtrace for the log */
                    safewrite(ttyfd, "bt\r", 3);
                    continue;
                }
                else
                {
                    /* We hit it yet another time, give up here */
                    printf("\n");
                    Ret = EXIT_CONTINUE;
                    goto cleanup;
                }
            }
            else if (strstr(Buffer, "--- Press q"))
            {
                /* Send Return to get more data from Kdbg */
                safewrite(ttyfd, "\r", 1);
                continue;
            }
            else if (strstr(Buffer, "SYSREG_ROSAUTOTEST_FAILURE"))
            {
                /* rosautotest itself has problems, so there's no reason to continue */
                goto cleanup;
            }
            else if (*AppSettings.Stage[stage].Checkpoint && strstr(Buffer, AppSettings.Stage[stage].Checkpoint))
            {
                /* We reached a checkpoint, so return success */
                Ret = EXIT_CHECKPOINT_REACHED;
                goto cleanup;
            }
        }
    }


cleanup:
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ttyattr);
    close(ttyfd);

    return Ret;
}
