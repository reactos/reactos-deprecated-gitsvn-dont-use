#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ndk/ntndk.h>
#include <luser.h>
#include <lunix.h>
#include <lstorage.h>
#include <unixmap.h>
#include "luserint.h"

#define RING3 3
#define USE_LDT 4

HARDWARE_PTE MapGetFirstLevelPTEWithCR(ULONG_PTR CR, ULONG_PTR Addr);
HARDWARE_PTE MapGetFirstLevelPTE(ULONG_PTR Addr);
HARDWARE_PTE MapGetSecondLevelPTE(HARDWARE_PTE FirstLvlPte, ULONG_PTR Addr);

struct GDTLookupResult {
    ULONG SegLimit, SegBase;
    PKGDTENTRY Entry;
};

void _memcpy(void *dst, void *src, int len);

#define MEMFD 100
#define CPUFD 101
void ReadLuserCPUData(struct LuserCPUData *data)
{
    if (unix_lseek(CPUFD, 0, 0) == -1)
        unix_abort();
    if (unix_read(CPUFD, (char *)data, sizeof(*data)) != sizeof(*data))
        unix_abort();
}

void WriteLuserCPUData(struct LuserCPUData *data)
{
    if (unix_lseek(CPUFD, 0, 0) == -1)
        unix_abort();
    if (unix_write(CPUFD, (char *)data, sizeof(*data)) != sizeof(*data))
        unix_abort();
}

BOOLEAN LookupGDT(struct LuserCPUData *CPU, struct GDTLookupResult *ResultEntry, int seg)
{
    PKGDTENTRY Entry;

    if (seg >= CPU->LuserGdt.Limit) return FALSE;

    Entry = (PKGDTENTRY)(((char *)CPU->LuserGdt.Base) + (8 * (seg >> 3)));
    ResultEntry->SegLimit = (Entry->LimitLow | (Entry->HighWord.Bits.LimitHi << 16));
    ResultEntry->SegBase = 
        Entry->BaseLow | 
        (Entry->HighWord.Bits.BaseMid << 16) | 
        (Entry->HighWord.Bits.BaseHi << 24);

    if (!Entry->HighWord.Bits.Pres)
        return FALSE;

    if (Entry->HighWord.Bits.Granularity)
        ResultEntry->SegLimit *= PAGE_SIZE;

    ResultEntry->Entry = Entry;
    return TRUE;
}

void LuserSetTR(int tr)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    CPU.LuserTr = tr;
    WriteLuserCPUData(&CPU);
}

int LuserGetTR()
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    return CPU.LuserTr;
}

void ZapTLB(unsigned long oldCR, unsigned long newCR)
{
    int i, j;
    HARDWARE_PTE FirstLevelPTE, SecondLevelPTE;
    HARDWARE_PTE OldFirstLevel; // OldSecondLevel;

    for (i = 0; i < (0xbfdf0000 >> 22); i++)
    {
        int addr = i << 22;

        FirstLevelPTE = MapGetFirstLevelPTE(i << 22);
        if (!FirstLevelPTE.Valid)
        {
	    OldFirstLevel = MapGetFirstLevelPTEWithCR(oldCR, i << 22);
	    if (OldFirstLevel.Valid)
	    {
	    	for (j = 0; j < 1024; j++)
	    	{
               	    int thisAddr = addr | (j << 12);
            	    unix_msync((PVOID)thisAddr, PAGE_SIZE, MS_SYNC);
            	    unix_munmap((PVOID)thisAddr, PAGE_SIZE);
		}
	    }
        }
        else
        {
            for (j = 0; j < 1024; j++)
            {
                int thisAddr = addr | (j << 12);
                SecondLevelPTE = MapGetSecondLevelPTE(FirstLevelPTE, j << 12);
                unix_msync((PVOID)thisAddr, 1 << 12, MS_SYNC);
                if (!SecondLevelPTE.Valid)
                {
                    unix_munmap((PVOID)thisAddr, 1 << 12);
                }
                else
                {
                    ULONG_PTR mmap_res = unix_mmap
                        ((PVOID)thisAddr,
                         PAGE_SIZE,
                         PROT_READ|PROT_WRITE|PROT_EXEC,
                         MAP_SHARED|MAP_FIXED,
                                 MEMFD,
                         SecondLevelPTE.PageFrameNumber << PAGE_SHIFT);
                    if (mmap_res == (ULONG_PTR)-1)
                        unix_abort();
                }
            }
        }
    }
}

void LuserWriteCR(int cr, unsigned long newCR)
{
    unsigned long oldCR;
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    oldCR = CPU.LuserCr[cr];
    CPU.LuserCr[cr] = newCR;
    WriteLuserCPUData(&CPU);
    // Clobber 'TLB'
    if (cr == 3)
    {
        //Printf("Zapping CR: newCR %x\n", newCR);
        ZapTLB(oldCR, newCR);
    }
}

unsigned long LuserReadCR(int cr)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    return CPU.LuserCr[cr];
}

void LuserWriteDR(int dr, unsigned long newDR)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    CPU.LuserDr[dr] = newDR;
    WriteLuserCPUData(&CPU);
}

unsigned long LuserReadDR(int dr)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    return CPU.LuserDr[dr];
}

void LuserSetGlobalDescriptorTable(void *newGdt)
{
    int i;
    struct LuserCPUData CPU;
    struct modify_ldt_ldt_s ldtmod;
    KGDTENTRY *gdtent;
    Printf("Loading new GDT from %x\n", newGdt);
    ReadLuserCPUData(&CPU);
    memcpy(&CPU.LuserGdt.Limit, newGdt, sizeof(USHORT) + sizeof(ULONG));
    WriteLuserCPUData(&CPU);
    Printf("New GDT: %x:%x\n", CPU.LuserGdt.Base, CPU.LuserGdt.Limit);
    for (i = 0; i < CPU.LuserGdt.Limit; i += 8)
    {
        gdtent = (KGDTENTRY *)(CPU.LuserGdt.Base + i);
        // Fill in ldt entry
	ldtmod.entry_number = i / 8;
	ldtmod.base_addr = gdtent->BaseLow | (gdtent->HighWord.Bytes.BaseMid << 16) | (gdtent->HighWord.Bytes.BaseHi << 24);
	ldtmod.limit = gdtent->LimitLow | (gdtent->HighWord.Bits.LimitHi << 16);
	ldtmod.limit_in_pages = gdtent->HighWord.Bits.Granularity;
	ldtmod.contents = 0;
	ldtmod.seg_32bit = gdtent->HighWord.Bits.Default_Big;
	ldtmod.seg_not_present = !gdtent->HighWord.Bits.Pres;
	ldtmod.useable = !ldtmod.seg_not_present;
	ldtmod.read_exec_only = 0;

	if (ldtmod.useable)
	    Printf("LDT: [%x] -> %x:%x\n", ldtmod.entry_number, ldtmod.base_addr, ldtmod.limit * (ldtmod.limit_in_pages ? (1<<PAGE_SHIFT) : 1));
	
        if (unix_write_ldt(&ldtmod, sizeof(ldtmod)) == -1)
	    unix_abort();
    }
    Printf("Wrote %d shadow LDT entries\n", i / 8);
}

void LuserGetGlobalDescriptorTable(void *gdtSt)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    memcpy(gdtSt, &CPU.LuserGdt.Limit, sizeof(USHORT) + sizeof(ULONG));
}

void LuserSetLocalDescriptorTable(int ldtSelector)
{
    struct GDTLookupResult result;
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    if (!LookupGDT(&CPU, &result, ldtSelector))
    {
    	Printf("LDT Set Failed\n");
    	return;
    }
    CPU.LuserLdtSel = ldtSelector;
    CPU.LuserLdt.Limit = result.SegLimit;
    CPU.LuserLdt.Base = result.SegBase;
    WriteLuserCPUData(&CPU);
}

int LuserGetLocalDescriptorTable()
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    return CPU.LuserLdtSel;
}

void LuserSetInterruptDescriptorTable(void *newIdt)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    memcpy(&CPU.LuserIdt.Limit, newIdt, sizeof(USHORT) + sizeof(ULONG));
    WriteLuserCPUData(&CPU);
}

void LuserGetInterruptDescriptorTable(void *idtSt)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    memcpy(idtSt, &CPU.LuserIdt.Limit, sizeof(USHORT) + sizeof(ULONG));
}

void LuserRefreshFS()
{
    int i = 0x3f;
    struct LuserCPUData CPU;
    struct modify_ldt_ldt_s ldtmod;
    KGDTENTRY *gdtent;

    ReadLuserCPUData(&CPU);
    gdtent = (KGDTENTRY *)(CPU.LuserGdt.Base + (i & ~7));
    // Fill in ldt entry
    ldtmod.entry_number = i / 8;
    ldtmod.base_addr = gdtent->BaseLow | (gdtent->HighWord.Bytes.BaseMid << 16) | (gdtent->HighWord.Bytes.BaseHi << 24);
    ldtmod.limit = gdtent->LimitLow | (gdtent->HighWord.Bits.LimitHi << 16);
    ldtmod.limit_in_pages = gdtent->HighWord.Bits.Granularity;
    ldtmod.contents = 0;
    ldtmod.seg_32bit = gdtent->HighWord.Bits.Default_Big;
    ldtmod.seg_not_present = !gdtent->HighWord.Bits.Pres;
    ldtmod.useable = !ldtmod.seg_not_present;
    ldtmod.read_exec_only = 0;
    
    //Printf("LDT: [%x] -> %x:%x\n", ldtmod.entry_number, ldtmod.base_addr, ldtmod.limit * (ldtmod.limit_in_pages ? (1<<PAGE_SHIFT) : 1));

    if (unix_write_ldt(&ldtmod, sizeof(ldtmod)) == -1)
        unix_abort();    
}

void MapDump();
int  ReplacePage(ULONG_PTR ptr, ULONG_PTR from);
HARDWARE_PTE MapGetPTEWithRefresh(ULONG_PTR Addr);

void LuserInvalidatePage(void *addr)
{
    ULONG_PTR ptr = PAGE_ROUND_DOWN((ULONG_PTR)addr);
    unix_msync((void *)ptr, PAGE_SIZE, MS_SYNC);
    MapGetPTEWithRefresh(ptr);
    ReplacePage(ptr, (ULONG_PTR)__builtin_return_address(0));
}

void LuserWrmsr(unsigned long Msr, unsigned long ValHi, unsigned long ValLo)
{
    struct LuserCPUData CPU;
    ULONGLONG val = ValHi;
    val = val << 32 | ValLo;
    ReadLuserCPUData(&CPU);
    CPU.LuserMsr[Msr] = val;
    WriteLuserCPUData(&CPU);
}

void LuserRdmsr(unsigned long Msr, unsigned long *ValHi, unsigned long *ValLo)
{
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    *ValHi = CPU.LuserMsr[Msr] >> 32;
    *ValLo = CPU.LuserMsr[Msr];
}

void LuserGetTrapEntry(int Trap, void *IdtEntry)
{
    PKIDTENTRY LocalEntry;
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    LocalEntry = (PKIDTENTRY)(((char *)CPU.LuserIdt.Base) + (8 * Trap));
    _memcpy(IdtEntry, LocalEntry, sizeof(*LocalEntry));
}

int LuserGetTSS(void *Tss)
{
    struct GDTLookupResult result;
    struct LuserCPUData CPU;
    ReadLuserCPUData(&CPU);
    if (!CPU.LuserTr) return 0;
    if (LookupGDT(&CPU, &result, CPU.LuserTr))
        _memcpy(Tss, (void *)result.SegBase, sizeof(KTSS));
    return 1;
}
