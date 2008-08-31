#include "sysreg.h"
#include <termios.h>
#include <poll.h>

void ProcessDebugData(const char* tty, int timeout, int stage )
{
    int ttyfd, i;
    struct termios ttyattr, rawattr;

    if ((ttyfd = open(tty, O_NOCTTY | O_RDWR)) < 0)
    {
        printf("error opening tty\n");
        return;
    }

    if (tcgetattr(STDIN_FILENO, &ttyattr) < 0)
       return;

    rawattr = ttyattr;
    rawattr.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                                     | IGNCR | ICRNL | IXON);
    rawattr.c_lflag &= ~(ICANON | ECHO | ECHONL);
    rawattr.c_oflag &= ~OPOST;
    rawattr.c_cflag &= ~(CSIZE | PARENB);
    rawattr.c_cflag |= CS8;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &rawattr) < 0)
        return;

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

                got = read(fds[i].fd, buf, sizeof(buf));
                if (got < 0)
                    goto cleanup;
                if (!got || got == 1 && buf[0] == '\33')
                    goto cleanup;

                if (fds[i].fd != STDIN_FILENO)
                {
                    while (sent < got)
                    {
                        int done;
                        if ((done = safewrite(STDOUT_FILENO, 
                                        buf + sent, got -sent)) <= 0)
                        {
                            goto cleanup;
                        }
                        sent += done;
                    }
                }
            }
        }

    } 


cleanup:
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &ttyattr);
    close(ttyfd);


}

