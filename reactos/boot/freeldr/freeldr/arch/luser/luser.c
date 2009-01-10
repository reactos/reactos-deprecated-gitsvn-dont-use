// Linux User arch for freeldr by arty
#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ddk/ioaccess.h>
#include <ndk/ntndk.h>
#include <ndk/ketypes.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <arc/arc.h>
#include <reactos/rosldr.h>
#include <mm.h>
#include <disk.h>
#include <video.h>
#include <fs.h>
#include <machine.h>
#include <libs/luser/luser.h>
#include <libs/luser/lunix.h>
#include <arch.h>

#undef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#undef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))

#define FOURCC(w,x,y,z) (((w)<<24)|((x)<<16)|((y)<<8)|(z))
#define FLATPTR(seg,addr) ((void *)((((int)seg)<<4)+((int)addr)))
#define FLATDWRD(dwd) FLATPTR(((dwd)>>16),((dwd)&0xffff))
#define MAKEFLAT(ptr) \
    (((((ULONG_PTR)ptr) & ~15) << 12) | (((ULONG_PTR)ptr) & 15))
#define KERNEL_BASE_PHYS 0x800000

char sigstack[32 * 1024];
unsigned char *ideport = (unsigned char *)0x1f0;
int cylinders, heads, sectors;

typedef unsigned char U8;
typedef unsigned long U32;

#define MEMFD 100
void SetupLuser()
{
    LuserRegisterSigstack();
    LuserRegisterSegv();
    unix_mmap
        ((void *)0x80800000,
         0x1000 * 1536,
         PROT_READ | PROT_WRITE | PROT_EXEC,
         MAP_FIXED | MAP_SHARED,
         MEMFD,
         0x800000);
}

void __stdcall ShowMe(int x)
{
    Printf("%x -> %x\n", __builtin_return_address(0), x);
}

void EnableA20()
{
}

void SoftReboot()
{
    unix_exit(0);
}

void ChainLoadBiosBootSectorCode()
{
    unix_abort();
}

void ide_rdy()
{
    while (!(READ_PORT_UCHAR(ideport+7) & 0x40));
}

void ide_bsy()
{
    while (READ_PORT_UCHAR(ideport+7) & 0x80);
}

void ide_drq()
{
    while (!(READ_PORT_UCHAR(ideport+7) & 8));
}

void ide_get_desc(int *cylinders, int *heads, int *sectors)
{
    int i;
    short diskbuf[0x100];

    ide_rdy();
    ide_bsy();

    WRITE_PORT_UCHAR(ideport+2, 1);
    WRITE_PORT_UCHAR(ideport+3, 0);
    WRITE_PORT_UCHAR(ideport+4, 0);
    WRITE_PORT_UCHAR(ideport+5, 0);
    WRITE_PORT_UCHAR(ideport+6, 0);
    WRITE_PORT_UCHAR(ideport+7, 0xec);

    ide_drq();
    for (i = 0; i < 256; i++)
        diskbuf[i] = READ_PORT_USHORT((unsigned short *)ideport);

    *cylinders = diskbuf[1];
    *heads = diskbuf[3];
    *sectors = diskbuf[6];
}

void ide_read_sector(unsigned short *sector, int blocknum)
{
    int i;

    ide_rdy();
    ide_bsy();

    WRITE_PORT_UCHAR(ideport+2,1);
    WRITE_PORT_UCHAR(ideport+3,blocknum);
    WRITE_PORT_UCHAR(ideport+4,blocknum>>8);
    WRITE_PORT_UCHAR(ideport+5,blocknum>>16);
    WRITE_PORT_UCHAR(ideport+6,((blocknum>>24) & 15) | 0xe0);
    WRITE_PORT_UCHAR(ideport+7,0x20);

    ide_drq();
    for (i = 0; i < 256; i++)
        sector[i] = READ_PORT_USHORT((unsigned short *)ideport);
}

char *VBEStrdup(char **allocptr, char *str)
{
    char *thestr = strcpy(*allocptr, str);
    *allocptr += strlen(str) + 1;
    return thestr;
}

void *VBEAlloc(char **allocptr, int size)
{
    char *result = *allocptr;
    *allocptr += size;
    return result;
}

int Int386(int inter, REGS *in, REGS *out)
{
    struct LuserCPUData cpu;
    PBIOS_MEMORY_MAP Map;
    PI386_DISK_ADDRESS_PACKET DiskPacket;
    PI386_EXTENDED_DRIVE_GEOMETRY DiskGeom;
    PVESA_CHECK_SVGA_INFO VesaInfo;
    unsigned short *sectorbuf;
    char *vbeAlloc;
    USHORT *vidmodes;
    int tmp, sectornum, c,h,s;

    ReadLuserCPUData(&cpu);

    switch(inter)
    {
    case 0x10:
        switch (in->b.ah)
        {
        case 0x00:
            Printf("Change to video mode %d\n", in->b.al);
            break;

        case 0x01: // Set cursor shape
            break;

        case 0x0e:
            Printf("%c", in->b.al);
            break;

        case 0x4f:
            switch (in->w.ax)
            {
            case 0x4f00:
                out->b.ah = 0;
                VesaInfo = FLATPTR(in->x.es, in->w.di);
                VesaInfo->VesaMagic = FOURCC('V','E','S','A');
                VesaInfo->Version = 0x100;
                vbeAlloc = (char *)&VesaInfo->VBEReserved[0];
                VesaInfo->OEMName = MAKEFLAT(VBEStrdup(&vbeAlloc, "sim vesa"));
                VesaInfo->Flags = 2;
                vidmodes = (USHORT *)VBEAlloc(&vbeAlloc, sizeof(USHORT));
                *vidmodes = -1;
                VesaInfo->ModePointer = MAKEFLAT(vidmodes);
                VesaInfo->VideoMemoryBlocks = 1 + ((640 * 480 * 3) >> 16);
                break;
            case 0x4f01:
                break;
            default: goto unhandled;
            }
            break;
            /* Fallthrough*/
        default: goto unhandled;
        }
        break;
        
    case 0x12:
        switch (in->w.ax)
        {
        case 0:
            out->w.ax = 640;
            break;

        default: goto unhandled;
        }
        break;

    case 0x13:
        switch (in->b.ah)
        {
        case 0x2:
            tmp = out->b.al;
            c = (in->b.ch & 0xff) | ((in->b.cl & 0xc0) << 2);
            h = (in->b.dh & 0xff);
            s = (in->b.cl & 0x3f) - 1;
            sectornum = (c * sectors * heads) + (h * sectors) + s;
            //Printf("Disk: Reading %d,%d,%d -> %d (ptr %x)\n", c,h,s,sectornum,FLATPTR(in->x.es, in->w.bx));
            out->b.al = in->b.al;
            while (out->b.al--)
            {
                ide_read_sector(FLATPTR(in->x.es, in->w.bx), sectornum++);
                in->w.bx += 512;
            }
            out->b.al = tmp;
            out->w.flags &= ~I386FLAG_CF;            
            break;

        case 0x8:
            if (in->b.dl != 0x80)
            {
                out->w.flags |= I386FLAG_CF;
                break;
            }
            ide_get_desc(&cylinders, &heads, &sectors);
            out->b.ch = cylinders;
            out->b.cl = sectors | (cylinders >> 6);
            out->b.dh = heads;
            out->b.dl = 1;
            out->w.flags &= ~I386FLAG_CF;
            break;

        case 0x41:
            if (in->b.dl != 0x80)
            {
                out->w.flags |= I386FLAG_CF;
                break;
            }
            out->b.ah = 1;
            out->b.al = 0;
            out->w.bx = 0xaa55;
            out->w.cx = 1;
            out->w.flags &= ~I386FLAG_CF;
            break;

        case 0x42:
            DiskPacket = FLATPTR(in->x.ds,in->w.si);
            sectornum = DiskPacket->LBABlockCount + DiskPacket->LBAStartBlock;
            sectorbuf = FLATPTR(DiskPacket->TransferBufferSegment, DiskPacket->TransferBufferOffset);
            for (tmp = DiskPacket->LBAStartBlock; tmp < sectornum; tmp++)
            {
                ide_read_sector(sectorbuf, tmp);
                sectorbuf += 0x100;
            }
            out->w.ax = 0;
            out->w.flags &= ~I386FLAG_CF;
            break;

        case 0x48:
            if (in->b.dl != 0x80)
            {
                out->w.flags |= I386FLAG_CF;
                break;
            }
            DiskGeom = FLATPTR(in->x.ds, in->w.si);
            DiskGeom->Flags = 0;
            DiskGeom->Cylinders = cylinders;
            DiskGeom->Heads = heads;
            DiskGeom->Sectors = sectors;
            DiskGeom->TotalSectors = cylinders * heads * sectors;
            DiskGeom->EDDConfig = -1;
            out->w.flags &= ~I386FLAG_CF;
            break;

        default: goto unhandled;
        }
        break;

    case 0x15:
        switch (in->w.ax)
        {
        case 0xe801: // extended memory size
            out->w.ax = MAX(15,(cpu.MemSize >> 10) - 1);
            out->w.bx = MIN(0, (cpu.MemSize - (16 * 1024 * 1024)) >> 16);
            out->w.cx = out->w.ax;
            out->w.dx = out->w.bx;
            out->w.flags &= ~I386FLAG_CF;
            break;

        case 0xe820: // memory map
            out->x.eax = 0xe820;
            out->x.ebx = 0;
            out->x.ecx = sizeof(BIOS_MEMORY_MAP);
            out->x.edx = FOURCC('S','M','A','P');
            out->w.flags &= ~I386FLAG_CF;
            out->x.edi = in->x.edi;
            out->x.es = in->x.es;
            Map = (PBIOS_MEMORY_MAP)FLATPTR(out->x.es,out->w.di);
            //Printf("Map is at %x (%x:%x)\n", Map, out->x.es, out->w.di);
            Map->BaseAddress = 0x1000;
            Map->Length = cpu.MemSize - 0x1000;
            Map->Type = 1;
            break;

        case 0x5300: // apm
            out->w.flags |= I386FLAG_CF;
            break;

        default: goto unhandled;
        }
        break;

    case 0x16:
        switch (in->b.ah)
        {
        default: goto unhandled;
        }
        break;

    case 0x1a:
        switch (in->w.ax)
        {
        case 0xb101:
            out->w.flags |= I386FLAG_CF;
#if 0
            out->b.ah = 0;
            out->b.al = 1; // pci hardware characteristics: config access 1
            out->b.bh = 1;
            out->b.bl = 0;
            out->b.cl = 1;
            out->x.edi = 0;
            out->x.edx = FOURCC(' ','I','C','P');
#endif
            break;

        default: goto unhandled;
        }
        break;

    default: goto unhandled;
    }

    return 0;

unhandled:
    Printf("Unsupported call to Int386 from %x, int %x (ax %x)\n", 
           __builtin_return_address(0), inter, in->w.ax);
    unix_abort();
    return 1;
}

int PnpBiosSupported()
{
    return 0;
}

int PnpBiosGetDeviceNodeCount()
{
    return 0;
}

U32 PnpBiosGetDeviceNode(U8 *NodeId, U8 *NodeBuffer)
{
    return 0;
}

void BootOldLinuxKernel()
{
}

void BootNewLinuxKernel()
{
}

