#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <fcntl.h>
#include <wait.h>
#include <unistd.h>
#include <sys/time.h>
#include "luserhw.h"

extern int child_proc;
extern int dev_read_pipe[2], dev_write_pipe[2], int_pipe[2];

int readPortError(struct dev_t *dev, int addr)
{
    fprintf(stderr, "Invalid read on port %x\n", addr);
    return -1;
}

void writePortError(struct dev_t *dev, int addr, int val)
{
    fprintf(stderr, "Invalid write on port %x, value %x\n", addr, val);
}

int defaultReadWord(struct dev_t *dev, int addr)
{
    return dev->inb(dev, addr) | (dev->inb(dev, addr+1) << 8);
}

int defaultReadLong(struct dev_t *dev, int addr)
{
    return dev->inw(dev, addr) | (dev->inw(dev, addr+2) << 16);
}

void defaultWriteWord(struct dev_t *dev, int addr, int val)
{
    dev->outb(dev, addr, val & 0xff);
    dev->outb(dev, addr+1, val >> 8);
}

void defaultWriteLong(struct dev_t *dev, int addr, int val)
{
    dev->outw(dev, addr, val & 0xffff);
    dev->outw(dev, addr+2, val >> 16);
}

struct dev_t *finddev(int addr)
{
    struct dev_t *cur = head;

    while (cur && cur->length)
    {
        
        if (cur->baseAddr <= addr && addr < cur->baseAddr + cur->length)
            break;
        cur = cur->next;
    }
    if (!cur) abort();
    return cur;
}

int in_inst(int size, int addr)
{
    int res = -1;
    struct dev_t *theDevice = finddev(addr);
    addr -= theDevice->baseAddr;

    switch(size)
    {
    case 1:
        res = theDevice->inb(theDevice, addr);
        break;

    case 2:
        res = theDevice->inw(theDevice, addr);
        break;

    case 4:
        res = theDevice->inl(theDevice, addr);
        break;
    }

    return res;
}

void out_inst(int size, int addr, int val)
{
    struct dev_t *theDevice = finddev(addr);
    addr -= theDevice->baseAddr;

    switch(size)
    {
    case 1:
        theDevice->outb(theDevice, addr, val);
        break;

    case 2:
        theDevice->outw(theDevice, addr, val);
        break;

    case 4:
        theDevice->outl(theDevice, addr, val);
        break;
    }
}

int timeLessThan(struct timeval *a, struct timeval *b)
{
    return a->tv_sec < b->tv_sec || 
        ((a->tv_sec == b->tv_sec) && a->tv_usec <= b->tv_usec);
}

/* Take b from a */
int timeSubtract(struct timeval *a, struct timeval *b)
{
    if (timeLessThan(a, b)) 
    {
        a->tv_sec = 0;
        a->tv_usec = 1;
        return 0;
    }
    if (b->tv_usec > a->tv_usec)
    {
        a->tv_sec--;
        a->tv_usec += 1000000;
    }

    a->tv_sec -= b->tv_sec;
    a->tv_usec -= b->tv_usec;
    return 1;
}

void timeAdd(struct timeval *a, struct timeval *b)
{
    a->tv_usec += b->tv_usec;
    if (a->tv_usec > 1000000)
    {
        a->tv_usec -= 1000000;
        a->tv_sec++;
    }
}

void doTimers(struct timeval *tod)
{
    struct interrupt_t *taketimer;

    while (int_head && timeLessThan(&int_head->when, tod))
    {
        taketimer = int_head;
        int_head = int_head->next;
        taketimer->fun(taketimer->dev, taketimer->context);
        free(taketimer);
    }
}

void setAlarm()
{
    struct timeval now;
    struct itimerval val = { };

    gettimeofday(&now, NULL);

    if (int_head)
    { 
        val.it_value = int_head->when;
        timeSubtract(&val.it_value, &now);
    }

    setitimer(ITIMER_REAL, &val, NULL);
}

int timerExp(int sig)
{
    struct timeval now;

    gettimeofday(&now, NULL);
    doTimers(&now);
    setAlarm();

    return 0;
}

int childDied(int sig)
{
    int status;
    fprintf(stderr, "SIGCHLD\n");
    if ((waitpid(child_proc, &status, WNOHANG) == child_proc) &&
        !WIFSTOPPED(status) &&
        !WIFCONTINUED(status))
    {
        fprintf(stderr, "Reaped child process %d (status %x)\n", 
                child_proc, status);
        child_proc = -1;
    }
    return 0;
}

void scheduleInterrupt(struct dev_t *dev, struct timeval *when, interrupt_fun fun, void *context)
{
    struct interrupt_t *newhead = malloc(sizeof(struct interrupt_t));

    memset(newhead, 0, sizeof(*newhead));
    newhead->dev = dev;
    newhead->fun = fun;
    gettimeofday(&newhead->at, NULL);
    newhead->when = newhead->at;
    timeAdd(&newhead->when, when);
    newhead->context = context;
    
    if (!int_head || timeLessThan(&newhead->when, &int_head->when))
    {
        newhead->next = int_head;
        int_head = newhead;
    }
    else
    {
        struct interrupt_t *search = int_head;
        while (search->next && timeLessThan(&search->next->when, &newhead->when))
            search = search->next;
        newhead->next = search->next;
        search->next = newhead;
    }

    setAlarm();
}

void sendInterrupt(char intNum)
{
    if (write(int_pipe[1], &intNum, 1) != 1)
        exit(1);
}

void devLoop()
{
    int readlen;
    int pipe_out_msg[3];
    sigset_t blockedAlarm, notBlocked;

    sigemptyset(&blockedAlarm);
    sigaddset(&blockedAlarm, SIGALRM);
    
    while (child_proc != -1)
    {
        readlen = read(dev_write_pipe[0], &pipe_out_msg, sizeof(pipe_out_msg));
        if (readlen != sizeof(pipe_out_msg)) { usleep(100); continue; }
        
        sigprocmask(SIG_SETMASK, &blockedAlarm, &notBlocked);

        if (pipe_out_msg[0] & 0x80)
        {
            out_inst(pipe_out_msg[0] & 7, pipe_out_msg[1], pipe_out_msg[2]);
            pipe_out_msg[0] &= 0x7f;
            write(dev_read_pipe[1], &pipe_out_msg, sizeof(pipe_out_msg));
        }
        else
        {
            pipe_out_msg[2] = in_inst(pipe_out_msg[0] & 7, pipe_out_msg[1]);
            write(dev_read_pipe[1], &pipe_out_msg, sizeof(pipe_out_msg));
        }
        
        sigprocmask(SIG_SETMASK, &notBlocked, NULL);
    }
}
