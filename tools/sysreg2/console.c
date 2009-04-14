#include "sysreg.h"

int ProcessDebugData(const char* tty, int timeout, int stage )
{
    char buf[512];
    char rbuf[512];
    char* bp;
    int got;
    int ttyfd, i;
    struct termios ttyattr, rawattr;
    int Ret = EXIT_NONCONTINUABLE_ERROR;
    int KdbgHit = 0;

    /* ttyfd is the file descriptor of the virtual COM port */
    if ((ttyfd = open(tty, O_NOCTTY | O_RDWR)) < 0)
    {
        SysregPrintf("error opening tty\n");
        return false;
    }

    /* We also monitor STDIN_FILENO, so a user can cancel the process with ESC */
    if (tcgetattr(STDIN_FILENO, &ttyattr) < 0)
    {
        SysregPrintf("tcgetattr failed with error %d\n", errno);
        return false;
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
        return false;
    }

    while (1)
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
            Ret = EXIT_ERROR;
            goto cleanup;
        }

        for (i = 0; i < (sizeof(fds) / sizeof(struct pollfd)); i++)
        {
            /* Wait till we get some input from the fd */
            if (!(fds[i].revents & POLLIN))
                continue;

            bp = buf;

            /* Read one line or a maximum of 511 bytes into a buffer (leave space for the null character) */
            while (bp - buf < sizeof(buf) - 1)
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

                /* Also break on newlines */
                if(*bp == '\n')
                    break;

                ++bp;
            }

            if (bp == buf)
            {
                /* This usually means the machine shut down (like in 1st or 2nd stage) */
                Ret = EXIT_SHUTDOWN;
                goto cleanup;
            }

            *(++bp) = 0;

            /* Now check the output */
            if (fds[i].fd == STDIN_FILENO)
            {
                /* Check whether the user pressed ESC and cancel in that case */
                if (strchr(buf, '\33'))
                    goto cleanup;
            }
            else
            {
                /* Check for "magic" sequences */
                if (strstr(buf, "kdb:>"))
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
                        Ret = EXIT_ERROR;
                        goto cleanup;
                    }
                }
                else if (strstr(buf, "--- Press q"))
                {
                    /* Send Return to get more data from Kdbg */
                    safewrite(ttyfd, "\r", 1);
                    continue;
                }
                else if (strstr(buf, "SYSREG_ROSAUTOTEST_FAILURE"))
                {
                    /* rosautotest itself has problems, so there's no reason to continue */
                    goto cleanup;
                }
                else if (*AppSettings.Stage[stage].Checkpoint && strstr(buf, AppSettings.Stage[stage].Checkpoint))
                {
                    /* We reached a checkpoint, so return success */
                    Ret = EXIT_CHECKPOINT_REACHED;
                    goto cleanup;
                }

                if (ResolveAddressFromFile(rbuf, sizeof(rbuf), buf))
                    printf("%s", rbuf);
                else
                    printf("%s", buf);
            }
        }
    }


cleanup:
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ttyattr);
    close(ttyfd);

    return Ret;
}

