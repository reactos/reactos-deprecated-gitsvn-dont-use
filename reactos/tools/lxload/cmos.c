#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "luserhw.h"

struct cmos_data {
    unsigned char buf[256];
    unsigned char addr;
} cmos;

void cmosInterrupt(struct dev_t *dev, void *context)
{
    struct timeval next = { 0, 10000000 / 1024 };
    scheduleInterrupt(dev, &next, cmosInterrupt, context);
    raiseInterrupt(dev, 8);
}

int cmosReadRtc(int what)
{
    struct tm *tm;
    time_t tx;

    time(&tx);
    tm = localtime(&tx);

    switch (what)
    {
    case 0:
        return 16 * (tm->tm_sec / 10) + (tm->tm_sec % 10);

    case 1:
        return 16 * (tm->tm_min / 10) + (tm->tm_min % 10);

    case 2:
        fprintf(stderr, "CMOS: hours %d\n", tm->tm_hour);
        return 16 * (tm->tm_hour / 10) + (tm->tm_hour % 10);
        
    case 3:
        fprintf(stderr, "CMOS: weekday %d\n", tm->tm_wday);
        return tm->tm_wday;

    case 4:
        fprintf(stderr, "CMOS: day-of-month %d\n", tm->tm_mday);
        return 16 * (tm->tm_mday / 10) + (tm->tm_mday % 10);

    case 5:
        fprintf(stderr, "CMOS: month %d\n", tm->tm_mon);
        return 16 * (tm->tm_mon / 10) + (tm->tm_mday % 10);

    case 6:
        fprintf(stderr, "CMOS: year %d\n", tm->tm_year);
        return 16 * ((tm->tm_year - 100) / 10) + ((tm->tm_year - 100) % 10);
    }

    return -1;
}

int cmosReadByte(struct dev_t *dev, int addr)
{
    struct cmos_data *cmdata = (struct cmos_data *)dev->self;
    switch (addr)
    {
    case 0:
        return cmdata->addr;

    case 1:
        switch (cmdata->addr)
        {
        case 0:
        case 2:
        case 4:
        case 6:
            return cmosReadRtc(cmdata->addr / 2);
        case 7:
            return cmosReadRtc(4);
        case 8:
            return cmosReadRtc(5);
        case 9:
            return cmosReadRtc(6);
        default:
            return cmdata->buf[cmdata->addr];
        }
    }

    return -1;
}

void cmosWriteByte(struct dev_t *dev, int addr, int val)
{
    struct timeval next = { 0, 10000000 / 1024 };
    struct cmos_data *cmdata = (struct cmos_data *)dev->self;
    switch (addr)
    {
    case 0:
        cmdata->addr = val;
        break;

    case 1:
        cmdata->buf[cmdata->addr] = val;
        switch (cmdata->addr)
        {
        case 0x0b:
            if (val & 0x40)
                // Start a 1024hz timer interrupt
                scheduleInterrupt(dev, &next, cmosInterrupt, 0);
            break;
        }
        break;
    }
}
