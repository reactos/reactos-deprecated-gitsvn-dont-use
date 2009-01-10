#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <assert.h>
#include "luserhw.h"

#define IDE_MEDIA_STATUS 0xda
#define IDE_READ_SECTOR 0x20
#define IDE_WRITE_SECTOR 0x30
#define IDE_GET_PARAMS 0xec

struct ide_data {
    char status;
    char interrupt;
    int diskfd;
    int targetid;
    char error;
    int cylinders, heads, sectors;
    int sector;
    unsigned long long totalsize;
    unsigned int transferIdx;
    short transfer[256];
    unsigned char regbits[5];
    char command;
    char dodrq;
} ide_data1 = { 0x50, 14, -1 };

int ideReadByte(struct dev_t *dev, int addr)
{
    struct ide_data *idedata = dev->self;
    switch (addr)
    {
    case 1:
        return idedata->error;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        return idedata->regbits[addr-2];

    case 7:
        return idedata->status;
    }

    return -1;
}

int ideAltReadByte(struct dev_t *dev, int addr)
{
    struct ide_data *idedata = dev->self;
    if (addr)
        return idedata->error;
    else
        return idedata->status;
}

int ideReadWord(struct dev_t *dev, int addr)
{
    struct ide_data *idedata = dev->self;
    int result = -1;
    switch (addr)
    {
    case 0:
        if (idedata->status & 8)
        {
            //idedata->status |= 0x80;
            result = idedata->transfer[idedata->transferIdx++];
            if (idedata->transferIdx == 0x100)
            {
                if (--idedata->regbits[0])
                {
                    idedata->transferIdx = 0;
                    idedata->sector++;
                    //fprintf(stderr, "IDE: Read sector %d\n", idedata->sector);
                    lseek(idedata->diskfd, idedata->sector << 9, 0);
                    read(idedata->diskfd, idedata->transfer, 1<<9);
                    raiseInterrupt(dev, idedata->interrupt);
                }
                else
                {
                    idedata->status = 0x50;
                    //fprintf(stderr, "IDE: read %d complete (%d words read)\n", idedata->sector, idedata->transferIdx);
                    idedata->transferIdx = 0;
                }
            }
        }
        return result;

    default:
        return defaultReadWord(dev, addr);
    }
}

void ideWriteByte(struct dev_t *dev, int addr, int val)
{
    int oldtarget;
    struct ide_data *idedata = dev->self;
    switch (addr)
    {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        idedata->regbits[addr-2] = val;
        oldtarget = idedata->targetid;
        idedata->targetid = !!(idedata->regbits[4] & 0x10);
        if (oldtarget != idedata->targetid)
        {
            if (idedata->targetid)
            {
                fprintf(stderr, "IDE: Slave\n");
                idedata->status = 1;
                idedata->error = 2;
            }
            else
            {
                fprintf(stderr, "IDE: Master\n");
                idedata->status = 0x50;
                idedata->error = 0;
            }
        }
        break;

    case 7:
        if (idedata->targetid)
        {
            fprintf(stderr, "IDE: Command targeted at wrong unit\n");
            return;
        }

        idedata->error = 0;
        idedata->status = 0x50;

        if ((val != IDE_READ_SECTOR) && 
            (val != IDE_WRITE_SECTOR) &&
            (val != IDE_GET_PARAMS))
            fprintf(stderr, "IDE COMMAND %x\n", val);
        switch (val)
        {
        case IDE_READ_SECTOR:
        case IDE_READ_SECTOR + 8:
            if (idedata->regbits[4] & 0x40)
            {
                idedata->sector =
                    ((idedata->regbits[4] & 15) << 24) |
                    ((idedata->regbits[3] & 0xff) << 16) |
                    ((idedata->regbits[2] & 0xff) << 8) |
                    (idedata->regbits[1] & 0xff);
            }
            else
            {
                int c = idedata->regbits[2] + (idedata->regbits[3] << 8);
                int h = idedata->regbits[4] & 15;
                int s = (idedata->regbits[1] & 0x3f) - 1;
                //fprintf(stderr, "IDE: read sector C%d H%d S%d\n", c,h,s);
                idedata->sector = 
                    (c * idedata->heads * idedata->sectors) +
                    (h * idedata->sectors) +
                    (s);
            }
            if (!idedata->regbits[0]) return;
            //fprintf(stderr, "IDE: Read sector %d\n", idedata->sector);
            lseek(idedata->diskfd, idedata->sector << 9, 0);
            read(idedata->diskfd, idedata->transfer, 1<<9);
            idedata->status = 0x08;
            raiseInterrupt(dev, idedata->interrupt);
            break;

        case IDE_WRITE_SECTOR:
            if (idedata->regbits[4] & 0x40)
            {
                idedata->sector =
                    ((idedata->regbits[4] & 15) << 24) |
                    ((idedata->regbits[3] & 0xff) << 16) |
                    ((idedata->regbits[2] & 0xff) << 8) |
                    (idedata->regbits[1] & 0xff);
            }
            else
            {
                int c = idedata->regbits[2] + (idedata->regbits[3] << 8);
                int h = idedata->regbits[4] & 15;
                int s = (idedata->regbits[1] & 0x3f) - 1;
                //fprintf(stderr, "IDE: read sector C%d H%d S%d\n", c,h,s);
                idedata->sector = 
                    (c * idedata->heads * idedata->sectors) +
                    (h * idedata->sectors) +
                    (s);
            }
            if (!idedata->regbits[0]) return;
            //fprintf(stderr, "IDE: Read sector %d\n", idedata->sector);
            idedata->status = 0x08;
            break;            

        case IDE_GET_PARAMS:
            idedata->transferIdx = 0;
            memset(idedata->transfer, 0, sizeof(idedata->transfer));
            idedata->transfer[1] = idedata->cylinders;
            idedata->transfer[3] = idedata->heads;
            idedata->transfer[6] = idedata->sectors;
            strcpy((char *)&idedata->transfer[10], "1234");
            strcpy((char *)&idedata->transfer[23], "1234");
            strcpy((char *)&idedata->transfer[27], "Simulated IDE");
            idedata->transfer[49] = 0x200;
            idedata->status = 0x08;
            break;

        case IDE_MEDIA_STATUS:
            fprintf(stderr, "Media status ... not sure\n");
            break;
        }
        break;
    }
}

void ideAltWriteByte(struct dev_t *dev, int addr, int val)
{
}

void ideWriteWord(struct dev_t *dev, int addr, int val)
{
    struct ide_data *idedata = dev->self;
    if (addr == 0)
    {
        if (idedata->status & 0x8)
        {
            idedata->transfer[idedata->transferIdx++] = val;
            if (idedata->transferIdx == 0x100)
            {
                lseek(idedata->diskfd, idedata->sector << 9, 0);
                write(idedata->diskfd, idedata->transfer, 1<<9);

                if (--idedata->regbits[0])
                {
                    idedata->transferIdx = 0;
                    idedata->sector++;
                    //fprintf(stderr, "IDE: Read sector %d\n", idedata->sector);
                }
                else
                {
                    idedata->status = 0x50;
                    //fprintf(stderr, "IDE: read %d complete (%d words read)\n", idedata->sector, idedata->transferIdx);
                    idedata->transferIdx = 0;
                }
                raiseInterrupt(dev, idedata->interrupt);
            }
        }
    }
    else
        defaultWriteWord(dev, addr, val);
}

void ideSetup(struct ide_data *idedata, int diskfd)
{
    struct stat statbuf;
    fstat(diskfd, &statbuf);
    idedata->diskfd = diskfd;
    idedata->totalsize = statbuf.st_size;
    idedata->heads = 16;
    idedata->sectors = 63;
    idedata->cylinders = statbuf.st_size / 512 / idedata->heads / idedata->sectors;
}
