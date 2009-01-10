#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include "luserhw.h"

#define PIT_LATCH_OUT_COUNTER 0
#define PIT_LATCH_MSB 1
#define PIT_LATCH_LSB 2
#define PIT_LATCH_CHAIN 3

#define PIT_COUNT_ONCE 0
#define PIT_COUNT_ONESHOT 1
#define PIT_COUNT_RATE 2
#define PIT_COUNT_SQUARE 3
#define PIT_COUNT_SWSTROBE 4
#define PIT_COUNT_HWSTROBE 5
#define PIT_COUNT_AWAIT 8

#define PITI_LATCH_END 0
#define PITI_LATCH_LSB 1
#define PITI_LATCH_MSB 2

#undef MIN
#define MIN(x,y) ((x)<(y)?(x):(y))
double usecPerTick = 1000000.0 / 1193182.0;

struct pit_counter {
    char mode;

    unsigned int latchValue;
    unsigned int resetValue;

    char latch;
    char intSched;

    clock_t reloaded;
};

struct pit_data {
    struct pit_counter counter[3];
    char control;
} pit_data = {
    { 
        { PIT_COUNT_AWAIT },
        { PIT_COUNT_AWAIT },
        { PIT_COUNT_AWAIT }
    }
};

#define RDTSC(x) \
        __asm__("rdtsc\n" \
                "mov %0,%%ebx\n" \
                "mov %%edx,(%%ebx)\n" \
                "add $4,%%ebx\n" \
                "mov %%eax,(%%ebx)\n" : : "r" (&x) : "eax", "edx")

clock_t x_clock()
{
    static int calibrate = 1;
    static struct timeval start = { };
    static unsigned long long tscdiff = 0;
    struct timeval cur;
    unsigned long long tscval;

    if (calibrate)
    {
        calibrate = 0;
        gettimeofday(&start, NULL);
        cur = start;
        /* Find a time close to a 1-second boundary */
        while (cur.tv_sec == start.tv_sec)
            gettimeofday(&start, NULL);
        RDTSC(tscval);
        cur = start;
        /* Let the full second pass */
        while (cur.tv_sec == start.tv_sec)
            gettimeofday(&cur, NULL);
        /* Now we have an rdtsc diff */
        RDTSC(tscdiff);
        tscdiff -= tscval;
        tscdiff /= 1000000;
    }

    RDTSC(tscval);
    tscval /= tscdiff;
    return tscval;
}

unsigned int pitGetLatchValue(struct pit_counter *pitctr)
{
    clock_t curtime, duration_clock;
    double duration_ticks;
    curtime = x_clock();
    duration_clock = curtime - pitctr->reloaded;
    duration_ticks = (double)duration_clock / (double)usecPerTick;
    return (unsigned int)duration_ticks;
}

int pitReadCounter(struct pit_counter *pitctr, int addr)
{
    int result = 0xff;
    switch (pitctr->latch & 3)
    {
    case PITI_LATCH_LSB:
        result = pitctr->latchValue;
        break;

    case PITI_LATCH_MSB:
        result = pitctr->latchValue >> 8;
        break;
    }

    pitctr->latch >>= 2;
    return result;
}

int pitReadByte(struct dev_t *dev, int addr)
{
    int result;
    struct pit_data *pitdata = dev->self;
    switch (addr)
    {
    case 3:
        return pitdata->control;
    default:
        result = pitReadCounter(&pitdata->counter[addr], addr);
        return result;
    }
}

void pitInterrupt(struct dev_t *dev, void *context);

void pitScheduleInterrupt(struct dev_t *dev, struct pit_counter *counter)
{
    struct timeval next;
    unsigned int latchVal = pitGetLatchValue(counter);

    raiseInterrupt(dev, 0);
    counter->reloaded = x_clock();

    if (counter->intSched < 1)
    {
        ++counter->intSched;
        next.tv_sec = 0;
        next.tv_usec = 10000;
        scheduleInterrupt(dev, &next, pitInterrupt, counter);
    }
}

void pitMaybeResetCount
(struct dev_t *dev, struct pit_counter *pitctr, int fromMode)
{
    struct timeval next = { };
    struct pit_data *pitdata = dev->self;

    if (!(fromMode || pitctr->mode == PIT_COUNT_AWAIT)) return;

    if (pitctr->resetValue == 0)
        pitctr->resetValue = 1<<16;
    
    //pitctr->reloaded = x_clock();
    pitScheduleInterrupt(dev, pitctr);
}

void pitInterrupt(struct dev_t *dev, void *context)
{
    struct pit_counter *pitctr = context;
    struct pit_data *pitdata = dev->self;
    switch (pitctr->mode)
    {
    case PIT_COUNT_RATE:
    case PIT_COUNT_SQUARE:
    case PIT_COUNT_HWSTROBE:
        pitMaybeResetCount(dev, pitctr, 1);
        break;

    case PIT_COUNT_ONCE:
    case PIT_COUNT_ONESHOT:
        pitctr->mode = PIT_COUNT_AWAIT;
        break;
    }

    if (pitctr->intSched > 0)
        --pitctr->intSched;

    pitScheduleInterrupt(dev, pitctr);
}

void pitWriteCounter
(struct dev_t *dev, struct pit_counter *pitctr, int val)
{
    switch (pitctr->latch & 3)
    {
    case PITI_LATCH_LSB:
        pitctr->latchValue = (pitctr->latchValue & ~0xff) | val;
        break;

    case PITI_LATCH_MSB:
        pitctr->latchValue = (pitctr->latchValue & 0xff) | (val << 8);
        pitctr->resetValue = pitctr->latchValue;
        if (!pitctr->resetValue) pitctr->resetValue = 1 << 16;
        pitMaybeResetCount(dev, pitctr, 1);
        break;
    }

    pitctr->latch >>= 2;
}

void pitControlCounter(struct dev_t *dev, struct pit_counter *pitctr, int latchmode, int countmode, int bcd)
{
    int oldmode = pitctr->mode;
    struct pit_data *pitdata = dev->self;
    assert(!bcd);

    switch (latchmode)
    {
    case PIT_LATCH_OUT_COUNTER:
        pitctr->latchValue = 
            pitctr->resetValue - 
            (pitGetLatchValue(pitctr) % pitctr->resetValue);
#if 0
        fprintf(stderr, "PIT: latching %d ticks (%d usec)\n",
                pitctr->latchValue,
                x_clock() - pitctr->reloaded);
#endif

    case PIT_LATCH_CHAIN:
        pitctr->latch = PITI_LATCH_LSB | (PITI_LATCH_MSB << 2);
        break;

    case PIT_LATCH_MSB:
        pitctr->latch = PITI_LATCH_MSB;
        break;

    case PIT_LATCH_LSB:
        pitctr->latch = PITI_LATCH_LSB;
        break;
    }

    if (latchmode && countmode != oldmode)
    {
        pitctr->mode = countmode;
        pitMaybeResetCount(dev, pitctr, 1);
    }
}

void pitWriteControl(struct dev_t *dev)
{
    struct pit_data *pitdata = dev->self;
    int ctsel = (pitdata->control >> 6) & 3;
    int latchmode = (pitdata->control >> 4) & 3;
    int countmode = (pitdata->control >> 1) & 7;
    int bcd = pitdata->control & 1;

    if (ctsel < 3)
        pitControlCounter
            (dev, &pitdata->counter[ctsel], latchmode, countmode, bcd);
}

void pitWriteByte(struct dev_t *dev, int addr, int val)
{
    struct pit_data *pitdata = dev->self;
    switch (addr)
    {
    case 0:
    case 1:
    case 2:
        pitWriteCounter(dev, &pitdata->counter[addr], val);
        break;
    case 3:
        pitdata->control = val;
        pitWriteControl(dev);
        break;
    }
}

