#ifndef FREELDR_LUSERHW_H
#define FREELDR_LUSERHW_H

#include <stdint.h>

struct dev_t {
    const char *name;
    unsigned int baseAddr;
    unsigned int length;
    void *self;
    struct dev_t *next;

    int (*inb)(struct dev_t *dev, int addr);
    int (*inw)(struct dev_t *dev, int addr);
    int (*inl)(struct dev_t *dev, int addr);

    void (*outb)(struct dev_t *dev, int addr, int val);
    void (*outw)(struct dev_t *dev, int addr, int val);
    void (*outl)(struct dev_t *dev, int addr, int val);
};

typedef void (*interrupt_fun)(struct dev_t *device, void *context);

struct interrupt_t {
    struct dev_t *dev;
    interrupt_fun fun;
    struct timeval when, at;
    struct interrupt_t *next;
    void *context;
};

struct poll_t {
    struct dev_t *dev;
    interrupt_fun pollin;
    void *pollin_ctx;
    struct poll_t *next;
};

struct multiboot_t {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
    uint32_t header_addr;
    uint32_t load_addr;
    uint32_t load_end_addr;
    uint32_t bss_end_addr;
    uint32_t entry_addr;
    uint32_t mode_type;
    uint32_t width, height, depth;
};

#define MBT_MAGIC 0x1badb002

struct multiboot_response_t {
    uint32_t flags;
    uint32_t mem_lower, mem_upper; // flags & 1
    uint32_t boot_device; // flags & 2
    uint32_t cmdline; // flags & 4
    uint32_t mods_count, mods_addr; // flags & 8
    uint32_t syms; // flags & 10 | flags & 20
    uint32_t mmap_length, mmap_addr; // flags & 40
    uint32_t drives_length, drives_addr; // flags & 80
    uint32_t config_table; // flags & 100
    uint32_t boot_loader_name; // flags & 200
    uint32_t apm_table; // flags & 400
    struct { // flags & 800
        uint32_t vbe_control_info;
        uint32_t vbe_mode_info;
        uint32_t vbe_mode;
        uint32_t vbe_interface_seg;
        uint32_t vbe_interface_off;
        uint32_t vbe_interface_len;
    } vbe;
};

#define MBT_FLG_MEM    0x001
#define MBT_FLG_BOOT   0x002
#define MBT_FLG_CMD    0x004
#define MBT_FLG_MODS   0x008
#define MBT_FLG_SYMS   0x030
#define MBT_FLG_MMAP   0x040
#define MBT_FLG_DRIVES 0x080
#define MBT_FLG_CONFIG 0x100
#define MBT_FLG_BOOTNM 0x200
#define MBT_FLG_APM    0x400
#define MBT_FLG_VBE    0x800

extern struct interrupt_t *int_head;
extern struct dev_t *head;

extern struct cmos_data cmos;
extern struct pit_data pit_data;
extern struct pic_data pic_data1, pic_data2;
extern struct com_data com_data1;
extern struct ide_data ide_data1;
extern struct pci_data pci_data;

/* General */
int readPortError(struct dev_t *dev, int addr);
void writePortError(struct dev_t *dev, int addr, int val);
int defaultReadWord(struct dev_t *dev, int addr);
int defaultReadLong(struct dev_t *dev, int addr);
void defaultWriteWord(struct dev_t *dev, int addr, int val);
void defaultWriteLong(struct dev_t *dev, int addr, int val);

void scheduleInterrupt(struct dev_t *dev, struct timeval *when, interrupt_fun fun, void *context);
void raiseInterrupt(struct dev_t *dev, char intNum);

int timeLessThan(struct timeval *a, struct timeval *b);
int timeSubtract(struct timeval *a, struct timeval *b);

/* PIT */
int pitReadByte(struct dev_t *dev, int addr);
void pitWriteByte(struct dev_t *dev, int addr, int val);

/* PIC */
int picReadByte(struct dev_t *dev, int addr);
void picWriteByte(struct dev_t *dev, int addr, int val);

/* CMOS */
int cmosReadByte(struct dev_t *dev, int addr);
void cmosWriteByte(struct dev_t *dev, int addr, int val);

/* COM */
int serialReadByte(struct dev_t *dev, int addr);
void serialWriteByte(struct dev_t *dev, int addr, int val);

/* IDE */
int ideReadByte(struct dev_t *dev, int addr);
int ideReadWord(struct dev_t *dev, int addr);
int ideAltReadByte(struct dev_t *dev, int addr);
void ideWriteByte(struct dev_t *dev, int addr, int val);
void ideAltWriteByte(struct dev_t *dev, int addr, int val);
void ideWriteWord(struct dev_t *dev, int addr, int val);
void ideSetup(struct ide_data *dev, int disk_fd);

/* PCI */
int pciReadByte(struct dev_t *dev, int addr);
int pciReadWord(struct dev_t *dev, int addr);
int pciReadDword(struct dev_t *dev, int addr);
void pciWriteByte(struct dev_t *dev, int addr, int val);
void pciWriteWord(struct dev_t *dev, int addr, int val);
void pciWriteDword(struct dev_t *dev, int addr, int val);

#define MEMFD 100
#define CPUFD 101

#endif/*FREELDR_LUSERHW_H*/
