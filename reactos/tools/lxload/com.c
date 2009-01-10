#include <stdio.h>
#include <unistd.h>
#include <poll.h>
#include <sys/time.h>
#include "luserhw.h"

#define COM_IER_RDRFULL 1
#define COM_IER_THREMPTY 2

#define COM_IIR_INTPENDING 1
#define COM_IIR_WHATMASK 0xe
#define COM_IIR_MSRCHANGE 0
#define COM_IIR_THREMPTY 2
#define COM_IIR_RDRFULL 4
#define COM_IIR_LSRCHANGE 6

#define COM_LSR_RDRFULL 1
#define COM_LSR_THREMPTY 0x20

#define COM_MCR_LOOP 16

#define fullAddr(addr,comdata) ((addr)+(((comdata)->lsr & 0x80) ? 8 : 0))

struct com_data {
    int intNum;
    char rx, tx;
    char ier;
    char iir;
    char lcr;
    char mcr;
    char lsr;
    char msr;
    char scr;
    int baudDiv;
    int started;
} com_data1 = { 4 };

void comPoll(struct dev_t *dev, struct com_data *comdata)
{
    int canread = 0;
    struct pollfd pfd = { };

    pfd.fd = 0; // stdin
    pfd.events = POLLIN;
    if (poll(&pfd, 1, 0) == -1) pfd.revents = 0;
    canread = !!(pfd.revents & POLLIN);

    if (canread)
    {
        if (read(0, &comdata->rx, 1) != -1)
        {
            comdata->lsr |= COM_LSR_RDRFULL;
            comdata->iir = 
                COM_IIR_RDRFULL | 
                ((comdata->ier & COM_IER_RDRFULL) ? COM_IIR_INTPENDING : 0);
        }
    }
    else
    {
        comdata->lsr |= COM_LSR_THREMPTY;
        comdata->iir = 
            COM_IIR_THREMPTY |
            ((comdata->tx && (comdata->ier & COM_IER_THREMPTY)) ? 
             COM_IIR_INTPENDING : 0);
        comdata->tx = 0;
    }

    if (comdata->iir & COM_IIR_INTPENDING)
    {
        fprintf(stderr, "raiseInterrupt %d\n", comdata->intNum);
        raiseInterrupt(dev, comdata->intNum);
    }
}

void comPollInt(struct dev_t *dev, void *context)
{
    struct timeval when = { 0, 100 * 1000 };
    struct com_data *comdata = context;
    if (!(comdata->iir & COM_IIR_INTPENDING))
        comPoll(dev, comdata);
    scheduleInterrupt(dev, &when, comPollInt, comdata);
}

int serialReadByte(struct dev_t *dev, int addr)
{
    char sx;
    struct com_data *comdata = dev->self;

    switch (fullAddr(addr,comdata))
    {
    case 0:
        sx = comdata->rx;
        comdata->lsr &= ~COM_LSR_RDRFULL;
        comPoll(dev, comdata);
        return sx;

    case 8:
        return comdata->baudDiv;

    case 1:
        return comdata->ier;

    case 9:
        return comdata->baudDiv >> 8;

    case 2:
    case 10:
        sx = comdata->iir;
        comPoll(dev, comdata);
        return sx;

    case 3:
    case 11:
        return comdata->lcr;
        
    case 4:
    case 12:
        return comdata->mcr;

    case 5:
    case 13:
        return comdata->lsr;
        
    case 6:
    case 14:
        return comdata->msr;

    case 7:
    case 15:
        return comdata->scr;
    }

    return -1;
}

void serialWriteByte(struct dev_t *dev, int addr, int val)
{
    char sx;
    struct com_data *comdata = dev->self;

    if (!comdata->started)
    {
        struct timeval tv = { 0, 100 * 1000 };
        comdata->started = 1;
        scheduleInterrupt(dev, &tv, comPollInt, comdata);
    }

    switch (fullAddr(addr, comdata))
    {
    case 0:
        sx = val;
        comdata->tx = 1;
        comdata->lsr &= ~COM_LSR_THREMPTY;
        write(1, &sx, 1);
        comPoll(dev, comdata);
        break;

    case 8:
        comdata->baudDiv = (comdata->baudDiv & ~0xff) | (val & 0xff);
        break;

    case 1:
        comdata->ier = val;
        comPoll(dev, comdata);
        break;

    case 9:
        comdata->baudDiv = (comdata->baudDiv & 0xff) | ((val & 0xff) << 8);
        break;

    case 2:
    case 10:
        break;

    case 3:
    case 11:
        comdata->lcr = val;
        break;

    case 4:
    case 12:
        comdata->mcr = val;
        if (comdata->mcr & COM_MCR_LOOP)
            comdata->msr = comdata->mcr << 4;
        break;

    case 5:
    case 13:
        break;

    case 6:
    case 14:
        break;

    case 7:
    case 15:
        comdata->scr = val;
        break;
    }
}
