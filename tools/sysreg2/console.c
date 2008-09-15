#include "sysreg.h"
#include <termios.h>
#include <poll.h>

bool ProcessDebugData(const char* tty, int timeout, int stage )
{
    int ttyfd, i;
    struct termios ttyattr, rawattr;
    bool Ret = true;
    bool KdbgHit = false;

    if ((ttyfd = open(tty, O_NOCTTY | O_RDWR)) < 0)
    {
        printf("error opening tty\n");
        return false;
    }

    if (tcgetattr(STDIN_FILENO, &ttyattr) < 0)
       return false;

    rawattr = ttyattr;
    rawattr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                                     | IGNCR | ICRNL | IXON);
    rawattr.c_lflag &= ~(ICANON | ECHO | ECHONL);
    rawattr.c_oflag &= ~OPOST;
    rawattr.c_cflag &= ~(CSIZE | PARENB);
    rawattr.c_cflag |= CS8;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawattr) < 0)
        return false;

    while (1)
    { 
        int ret;
        struct pollfd fds[] = {
            { STDIN_FILENO, POLLIN, 0 },
            { ttyfd, POLLIN, 0 },
        };

        ret = poll(fds, (sizeof(fds) / sizeof(struct pollfd)), timeout);
        if (ret < 0)
        {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            goto cleanup;
        }
        else if (ret == 0)
        {
            /* timeout */
            printf("timeout\n");
            Ret = false;
            goto cleanup;
        }

        for (i=0; i<(sizeof(fds) / sizeof(struct pollfd)); i++)
        {
            if (!fds[i].revents)
                continue;
            if (fds[i].revents & POLLIN)
            {
                char buf[4096];
                int got, sent = 0;
        
                memset(buf, 0, sizeof(buf));
                got = readln(fds[i].fd, buf, sizeof(buf));
                if (got == -2) /* kernel debugger */
                {
                    if (KdbgHit)
                    {
                        Ret = false;                    
                        goto cleanup;
                    }
                    else
                    {
                        KdbgHit = true;
                        safewrite(ttyfd, "bt\r", 3);
                        continue;
                    }
                }
                else if (got == -3) /* kernel debugger */
                {
                    safewrite(ttyfd, "\r", 1);
                    continue;
                }
                if (got < 0) {
                    goto cleanup;
                }
                if (!got)
                {
                    goto cleanup;
                }

                if (fds[i].fd != STDIN_FILENO)
                {
                    if ((AppSettings.Stage[stage].Checkpoint[0] != '\0') &&
                      (strstr(buf,AppSettings.Stage[stage].Checkpoint) != NULL))
                    { 
                        /* Checkpoint reached,
                         * kill the vm and return success */
                        goto cleanup;
                    } 

                    printf("%s", buf);
                    /*
                    while (sent < got)
                    {
                        int done;
                        if ((done = safewrite(STDOUT_FILENO, 
                                        buf + sent, got - sent)) <= 0)
                        {
                            Ret = false;
                            goto cleanup;
                        }
                        sent += done;
                    }
                    */
                }
                else
                {
                    if (got == 1 && buf[0] == '\33')
                        goto cleanup;
                }
                
            }
        }

    } 


cleanup:
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ttyattr);
    close(ttyfd);
    return Ret;
}

