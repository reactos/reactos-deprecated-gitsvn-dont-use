#include "sysreg.h"
#include <sys/sysinfo.h>

int readln(int fd, char* buffer, int size)
{
    char* bp = buffer, ch;
    int got;

    while ((bp - buffer < size) && (got = read(fd, bp, 1)))
    {
        if (fd == STDIN_FILENO)  
        {
            if (*bp == '\33')
                return 1;
        }
        else
        {
             if (strstr(buffer, "kdb:>"))
                 return -2;
        }
        if (*bp++ == '\n')
            return (bp - buffer);
    }
    if (got < 0)
        return -1;
    if (bp - buffer == size)
        while (read(fd, &ch, 1))
    return (bp - buffer);
}

ssize_t safewrite(int fd, const void *buf, size_t count)
{
    size_t nwritten = 0;
    while (count > 0) {
        ssize_t r = write(fd, buf, count);

        if (r < 0 && errno == EINTR)
            continue;
        if (r < 0)
            return r;
        if (r == 0)
            return nwritten;
        buf = (const char *)buf + r;
        count -= r;
        nwritten += r;
    }
    return nwritten;
}

char * ReadFile (const char *filename)
{
    char *buffer = NULL, *oldbuffer;
    int len = 0, fd, r;
    char b[1024];

    fd = open (filename, O_RDONLY);
    if (fd == -1) 
        return NULL;

    for (;;) {
        r = read (fd, b, sizeof b);
        if (r == -1) 
        {
          if (buffer) free (buffer);
          close(fd);
          return NULL;
        }
        if (r == 0) break;  /* end of file. */
        oldbuffer = buffer;
        buffer = realloc (buffer, len+r);
        if (buffer == NULL) {
            /* out of memory */
            close(fd);
            return NULL;
        }
        memcpy (buffer+len, b, r);
        len += r;
    }

    buffer = realloc (buffer, len+1);
    if (buffer == NULL) 
    {
        /* out of memory */
        close(fd);
        return NULL;
    }
    buffer[len] = '\0';
    close (fd);
    return buffer;
}


