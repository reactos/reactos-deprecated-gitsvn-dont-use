/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            ntoskrnl/mm/i386/page.c
 * PURPOSE:         Low level memory managment manipulation
 *
 * PROGRAMMERS:     David Welch (welch@cwcom.net)
 */

/* INCLUDES ***************************************************************/

#include <ntoskrnl.h>
#include <reactos/libs/luser/luser.h>
#include <reactos/libs/luser/lunix.h>
#include <reactos/libs/luser/unixmap.h>
#define NDEBUG
#include <debug.h>

#if defined (ALLOC_PRAGMA)
#pragma alloc_text(INIT, MmInitGlobalKernelPageDirectory)
#pragma alloc_text(INIT, MiInitPageDirectoryMap)
#endif

#define MEMFD 100

/* GLOBALS *****************************************************************/

#define PA_BIT_PRESENT   (0)
#define PA_BIT_READWRITE (1)
#define PA_BIT_USER      (2)
#define PA_BIT_WT        (3)
#define PA_BIT_CD        (4)
#define PA_BIT_ACCESSED  (5)
#define PA_BIT_DIRTY     (6)
#define PA_BIT_GLOBAL	 (8)

#define PA_PRESENT   (1 << PA_BIT_PRESENT)
#define PA_READWRITE (1 << PA_BIT_READWRITE)
#define PA_USER      (1 << PA_BIT_USER)
#define PA_DIRTY     (1 << PA_BIT_DIRTY)
#define PA_WT        (1 << PA_BIT_WT)
#define PA_CD        (1 << PA_BIT_CD)
#define PA_ACCESSED  (1 << PA_BIT_ACCESSED)
#define PA_GLOBAL    (1 << PA_BIT_GLOBAL)

#ifndef LUSER
#define HYPERSPACE		(0xc0400000)
#define IS_HYPERSPACE(v)	(((ULONG)(v) >= HYPERSPACE && (ULONG)(v) < HYPERSPACE + 0x400000))
#else
#define HYPERSPACE		(0xa0400000)
#define IS_HYPERSPACE(v)	(((ULONG)(v) >= HYPERSPACE && (ULONG)(v) < HYPERSPACE + 0x400000))
#endif

ULONG MmGlobalKernelPageDirectory[1024];

#define PTE_TO_PFN(X)  ((X) >> PAGE_SHIFT)
#define PFN_TO_PTE(X)  ((X) << PAGE_SHIFT)

#define IS_THIS_PROCESS(Process) (GetCR3ForProcess(Process) == (__readcr3() & ~0xfff))

#if defined(__GNUC__)
#define PTE_TO_PAGE(X) ((LARGE_INTEGER)(LONGLONG)(PAGE_MASK(X)))
#else
__inline LARGE_INTEGER PTE_TO_PAGE(ULONG npage)
{
    LARGE_INTEGER dummy;
    dummy.QuadPart = (LONGLONG)(PAGE_MASK(npage));
    return dummy;
}
#endif

/* FUNCTIONS ***************************************************************/

BOOLEAN MmUnmapPageTable(PULONG Pt);

#define PTE_MODE_REPLACE 0
#define PTE_MODE_OR 1
#define PTE_MODE_INVMASK 2

ULONG_PTR
GetCR3ForProcess(PEPROCESS Process)
{
    ULONG CR3 = Process ? Process->Pcb.DirectoryTableBase[0] : __readcr3();
    DPRINT("GetCR3ForProcess(%x) => %x\n", Process, CR3);
    return CR3;
}


VOID
WritePDE(ULONG_PTR Table, ULONG Entry, ULONG Value, BOOLEAN Global)
{
    DPRINT("WritePDE(%x,%x,%x)\n", Table, Entry, Value);
    unix_lseek(MEMFD, (Table & ~0xfff) + Entry * sizeof(LONG), 0);
    unix_write(MEMFD, (char *)&Value, sizeof(Value));
    if (Global)
    {
        MmGlobalKernelPageDirectory[Entry] = Value;
        unix_msync(MmGlobalKernelPageDirectory, PAGE_SIZE, MS_SYNC);
    }
}

NTSTATUS
AllocatePDE(ULONG_PTR CR3, PVOID Addr, PPFN_TYPE PdePfn)
{
    NTSTATUS Status;
    HARDWARE_PTE First;
    Status = MmRequestPageMemoryConsumer(MC_NPPOOL, FALSE, PdePfn);
    if (!NT_SUCCESS(Status))
    {
        DPRINT("Failed to get a page %x\n", Status);
        return Status;
    }
    First.Valid = 1;
    First.PageFrameNumber = *PdePfn;
    WritePDE(CR3, (((ULONG_PTR)Addr) >> 22) & 0x3ff, *((PULONG)&First), Addr >= MmSystemRangeStart);
    DPRINT("AllocatePDE(%x,%x) => %x\n", CR3, Addr, *PdePfn);
    return STATUS_SUCCESS;
}

VOID
WritePTE(ULONG_PTR PageFrameNumber, PVOID Address, ULONG Value)
{
    DPRINT("WritePTE(%x,%x,%x)\n", PageFrameNumber, Address, Value);
    unix_lseek(MEMFD, (PageFrameNumber << PAGE_SHIFT) + ((((ULONG_PTR)Address) >> 12) & 0x3ff) * sizeof(LONG), 0);
    unix_write(MEMFD, (char *)&Value, sizeof(Value));
}

static ULONG MmGetPageEntryForProcess(PEPROCESS Process, PVOID Address)
{
    HARDWARE_PTE First, Second;
    ULONG PageEntry;
    First = MapGetFirstLevelPTEWithCR(GetCR3ForProcess(Process), (ULONG_PTR)Address);
    if (!First.Valid)
    {
        First = *((HARDWARE_PTE*)&MmGlobalKernelPageDirectory[((ULONG_PTR)Address >> 22) & 0x3ff]);
        // No main mapping either
        if (!First.Valid) return 0;
    }
    Second = MapGetSecondLevelPTE(First, (ULONG_PTR)Address);
    PageEntry = *((PULONG)&Second);
    DPRINT("MmGetPageEntryForProcess(%x,%x) => %x\n", Process, Address, PageEntry);
    return PageEntry;
}

VOID
WritePTEForProcess(PEPROCESS Process, PVOID Address, ULONG Entry, ULONG Mode)
{
    ULONG Pte, CR3 = GetCR3ForProcess(Process), PDEntry;
    PFN_TYPE Pde;
    HARDWARE_PTE First;
    __sigset_t sigmask = { }, oldmask = { };

    DPRINT("WritePTEForProcess(CR3 %x,%x,%x,%x,%d)\n", CR3, Process, Address, Entry, Mode);

    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    First = MapGetFirstLevelPTEWithCR(CR3, (ULONG_PTR)Address);
    PDEntry = (((ULONG_PTR)Address) >> 22) & 0x3ff;

    if (!First.Valid)
    {
        if (!NT_SUCCESS(AllocatePDE(CR3, Address, &Pde)))
            unix_abort();
        First = MapGetFirstLevelPTEWithCR(CR3, (ULONG_PTR)Address);
        if (!First.Valid)
            unix_abort();
    }

    if (Address >= MmSystemRangeStart)
    {
        MmGlobalKernelPageDirectory[PDEntry] = *((PULONG)&First);
        unix_msync(MmGlobalKernelPageDirectory, PAGE_SIZE, MS_SYNC);
    }

    Pte = MmGetPageEntryForProcess(Process, Address);

    switch (Mode)
    {
    	case PTE_MODE_REPLACE: Pte = Entry; break;
	case PTE_MODE_OR: Pte |= Entry; break;
	case PTE_MODE_INVMASK: Pte &= ~Entry; break;
    }
    WritePTE(First.PageFrameNumber, Address, Pte);

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

#if 0
VOID
MiFlushTlb(PVOID Address)
{
    DPRINT("Zap -> %x\n", Address);
    __invlpg(Address);
}
#endif

PULONG
MmGetPageDirectory(VOID)
{
    return (PULONG)(ULONG_PTR)__readcr3();
}

static ULONG
ProtectToPTE(ULONG flProtect)
{
    ULONG Attributes = 0;
    
    if (flProtect & (PAGE_NOACCESS|PAGE_GUARD))
    {
        Attributes = 0;
    }
    else if (flProtect & PAGE_IS_WRITABLE)
    {
        Attributes = PA_PRESENT | PA_READWRITE;
    }
    else if (flProtect & (PAGE_IS_READABLE | PAGE_IS_EXECUTABLE))
    {
        Attributes = PA_PRESENT;
    }
    else
    {
        DPRINT1("Unknown main protection type.\n");
        KeBugCheck(MEMORY_MANAGEMENT);
    }
    
    if (flProtect & PAGE_SYSTEM)
    {
    }
    else
    {
        Attributes = Attributes | PA_USER;
    }
    if (flProtect & PAGE_NOCACHE)
    {
        Attributes = Attributes | PA_CD;
    }
    if (flProtect & PAGE_WRITETHROUGH)
    {
        Attributes = Attributes | PA_WT;
    }
    return(Attributes);
}

NTSTATUS
NTAPI
Mmi386ReleaseMmInfo(PEPROCESS Process)
{
    PUSHORT LdtDescriptor;
    ULONG LdtBase;
    PULONG PageDir;
    ULONG i, CR3;
    
    DPRINT("Mmi386ReleaseMmInfo(Process %x)\n",Process);
    
    CR3 = GetCR3ForProcess(Process);

    LdtDescriptor = (PUSHORT) &Process->Pcb.LdtDescriptor;
    LdtBase = LdtDescriptor[1] |
    ((LdtDescriptor[2] & 0xff) << 16) |
    ((LdtDescriptor[3] & ~0xff) << 16);
    
    DPRINT("LdtBase: %x\n", LdtBase);
    
    if (LdtBase)
    {
        ExFreePool((PVOID) LdtBase);
    }
    
    PageDir = MmCreateHyperspaceMapping(PTE_TO_PFN(CR3));
    for (i = 0; i < ADDR_TO_PDE_OFFSET(MmSystemRangeStart); i++)
    {
        if (PageDir[i] != 0)
        {
            MmReleasePageMemoryConsumer(MC_NPPOOL, PTE_TO_PFN(PageDir[i]));
        }
    }
    MmReleasePageMemoryConsumer(MC_NPPOOL, PTE_TO_PFN(PageDir[ADDR_TO_PDE_OFFSET(HYPERSPACE)]));
    MmDeleteHyperspaceMapping(PageDir);
    MmReleasePageMemoryConsumer(MC_NPPOOL, PTE_TO_PFN(CR3));

    Process->Pcb.DirectoryTableBase[0] = 0;
    Process->Pcb.DirectoryTableBase[1] = 0;

    DPRINT("Finished Mmi386ReleaseMmInfo()\n");
    return(STATUS_SUCCESS);
}

NTSTATUS
NTAPI
MmInitializeHandBuiltProcess(IN PEPROCESS Process,
                             IN PULONG DirectoryTableBase)
{
    /* Share the directory base with the idle process */
    DirectoryTableBase[0] = PsGetCurrentProcess()->Pcb.DirectoryTableBase[0];
    DirectoryTableBase[1] = PsGetCurrentProcess()->Pcb.DirectoryTableBase[1];

    /* Initialize the Addresss Space */
    KeInitializeGuardedMutex(&Process->AddressCreationLock);
    Process->VadRoot.BalancedRoot.u1.Parent = NULL;

    /* The process now has an address space */
    Process->HasAddressSpace = TRUE;
    return STATUS_SUCCESS;
}

BOOLEAN
NTAPI
MmCreateProcessAddressSpace(IN ULONG MinWs,
                            IN PEPROCESS Process,
                            IN PULONG DirectoryTableBase)
{
    NTSTATUS Status;
    ULONG i, j, DirEnt;
    PFN_TYPE Pfn[2];
    
    DPRINT("MmCreateProcessAddressSpace(%x,%x,%x)\n", MinWs, Process, DirectoryTableBase);
    
    for (i = 0; i < 2; i++)
    {
        Status = MmRequestPageMemoryConsumer(MC_NPPOOL, FALSE, &Pfn[i]);
        if (!NT_SUCCESS(Status))
        {
            for (j = 0; j < i; j++)
            {
                MmReleasePageMemoryConsumer(MC_NPPOOL, Pfn[j]);
            }
            
            return FALSE;
        }
    }
    

    for (i = ADDR_TO_PDE_OFFSET(MmSystemRangeStart); i < 1024; i++)
    {
        DirEnt = MmGlobalKernelPageDirectory[i];
    	WritePDE(Pfn[0] << PAGE_SHIFT, i, DirEnt, FALSE);
    }

    WritePDE(Pfn[0] << PAGE_SHIFT, ADDR_TO_PDE_OFFSET(PAGETABLE_MAP), PFN_TO_PTE(Pfn[0]) | PA_PRESENT | PA_READWRITE, FALSE);
    WritePDE(Pfn[0] << PAGE_SHIFT, ADDR_TO_PDE_OFFSET(HYPERSPACE), PFN_TO_PTE(Pfn[1]) | PA_PRESENT | PA_READWRITE, FALSE);

    DirectoryTableBase[0] = PFN_TO_PTE(Pfn[0]);
    DirectoryTableBase[1] = 0;
    DPRINT("Finished MmCopyMmInfo(): 0x%x\n", DirectoryTableBase[0]);
    return TRUE;
}

VOID
NTAPI
MmDeletePageTable(PEPROCESS Process, PVOID Address)
{
    int i;
    ULONG_PTR Addr;
    PEPROCESS CurrentProcess = PsGetCurrentProcess();
    __sigset_t sigmask = { }, oldmask = { };
    
    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    DPRINT("MmDeletePageTable(%x,%x)\n", Process, Address);
    if (Process != NULL && Process != CurrentProcess)
    {
        KeAttachProcess(&Process->Pcb);
    }
    
    WritePDE(GetCR3ForProcess(Process), ADDR_TO_PDE_OFFSET(Address), 0, Address >= MmSystemRangeStart);

    if (IS_THIS_PROCESS(Process) || Address >= MmSystemRangeStart)
    {
        for (i = 0; i < 1024; i++)
        {
            Addr = (((ULONG_PTR)Address) & ~0x3fffff) + (i << PAGE_SHIFT);
            unix_msync((PVOID)Addr, PAGE_SIZE, MS_SYNC);
            unix_munmap((PVOID)Addr, PAGE_SIZE);
        }
    }

    if (Address >= MmSystemRangeStart)
    {
        KeBugCheck(MEMORY_MANAGEMENT);
        //       MmGlobalKernelPageDirectory[ADDR_TO_PDE_OFFSET(Address)] = 0;
    }
    if (Process != NULL && Process != CurrentProcess)
    {
        KeDetachProcess();
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

PFN_TYPE
NTAPI
MmGetPfnForProcess(PEPROCESS Process,
                   PVOID Address)
{
    ULONG Entry;
    PFN_TYPE Result;
    Entry = MmGetPageEntryForProcess(Process, Address);
    if (!(Entry & PA_PRESENT))
    {
        return 0;
    }
    Result = PTE_TO_PFN(Entry);
    DPRINT("MmGetPfnForProcess(%x,%x) => %x\n", Process, Address, Result);
    return Result;
}

VOID
NTAPI
MmDisableVirtualMapping(PEPROCESS Process, PVOID Address, BOOLEAN* WasDirty, PPFN_TYPE Page)
/*
 * FUNCTION: Delete a virtual mapping
 */
{
    ULONG Pte, Addr;
    __sigset_t sigmask = { }, oldmask = { };

    DPRINT("MmDisableVirtualMapping(%x,%x)\n", Process, Address);

    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    Pte = MmGetPageEntryForProcess(Process, Address);
    WritePTEForProcess(Process, Address, PA_DIRTY, PTE_MODE_INVMASK);

    /*
     * Return some information to the caller
     */
    if (WasDirty != NULL)
    {
        *WasDirty = Pte & PA_DIRTY;
    }
    if (Page != NULL)
    {
        *Page = PTE_TO_PFN(Pte);
    }

    if (IS_THIS_PROCESS(Process) || Address >= MmSystemRangeStart)
    {
        Addr = PAGE_ROUND_DOWN((ULONG_PTR)Address);
        unix_msync((PVOID)Addr, PAGE_SIZE, MS_SYNC);
        unix_munmap((PVOID)Addr, PAGE_SIZE);
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

VOID
NTAPI
MmRawDeleteVirtualMapping(PVOID Address)
{
    DPRINT("MmRawDeleteVirtualMapping(%x)\n", Address);
    MmDeleteVirtualMapping(PsGetCurrentProcess(), Address, FALSE, NULL, NULL);
}

VOID
NTAPI
MmDeleteVirtualMapping(PEPROCESS Process, PVOID Address, BOOLEAN FreePage,
                       BOOLEAN* WasDirty, PPFN_TYPE Page)
/*
 * FUNCTION: Delete a virtual mapping
 */
{
    ULONG Pte, Addr;
    PFN_TYPE Pfn;
    __sigset_t sigmask = { }, oldmask = { };

    DPRINT("MmDeleteVirtualMapping(%x, %x, %d, %x, %x)\n",
           Process, Address, FreePage, WasDirty, Page);

    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    Pte = MmGetPageEntryForProcess(Process, Address);
    WritePTEForProcess(Process, Address, 0, PTE_MODE_REPLACE);
    
    if (IS_THIS_PROCESS(Process) || Address >= MmSystemRangeStart)
    {
        Addr = PAGE_ROUND_DOWN((ULONG_PTR)Address);
        unix_msync((PVOID)Addr, PAGE_SIZE, MS_SYNC);
        unix_munmap((PVOID)Addr, PAGE_SIZE);
    }

    Pfn = Pte >> PAGE_SHIFT;

    /*
     * Return some information to the caller
     */
    if (WasDirty != NULL)
    {
        *WasDirty = Pte & PA_DIRTY ? TRUE : FALSE;
    }
    if (Page != NULL)
    {
        *Page = Pfn;
    }

    if (FreePage && !!(Pte & PA_PRESENT))
    {
        MmReleasePageMemoryConsumer(MC_NPPOOL, Pfn);
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

VOID
NTAPI
MmDeletePageFileMapping(PEPROCESS Process, PVOID Address,
                        SWAPENTRY* SwapEntry)
/*
 * FUNCTION: Delete a virtual mapping
 */
{
    *SwapEntry = MmGetPageEntryForProcess(Process, Address) >> 1;
    MmDeleteVirtualMapping(Process, Address, FALSE, NULL, NULL);
}

void ZapTLB(ULONG_PTR, ULONG_PTR);

BOOLEAN
Mmi386MakeKernelPageTableGlobal(PVOID PAddress)
{
    NTSTATUS Status;
    PFN_TYPE Page;
    ULONG Offset = ADDR_TO_PDE_OFFSET(PAddress);
    ULONG CR3 = __readcr3();
    HARDWARE_PTE First = MapGetFirstLevelPTEWithCR(CR3, (ULONG_PTR)PAddress);
    __sigset_t sigmask = { }, oldmask = { };

    DPRINT("Mmi386MakeKernelPageTableGlobal(%x, Offset=%x)\n", PAddress, Offset);
    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    if (!(MmGlobalKernelPageDirectory[Offset] & PA_PRESENT))
    {
        Status = MmRequestPageMemoryConsumer(MC_NPPOOL, FALSE, &Page);
        if (!NT_SUCCESS(Status))
            KeBugCheck(MEMORY_MANAGEMENT);
        MmGlobalKernelPageDirectory[Offset] = PFN_TO_PTE(Page) | PA_PRESENT | PA_READWRITE;
        unix_msync(MmGlobalKernelPageDirectory, PAGE_SIZE, MS_SYNC);
        WritePDE(CR3, Offset, MmGlobalKernelPageDirectory[Offset], FALSE);
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
    
    return !First.Valid;
}

BOOLEAN
NTAPI
MmIsDirtyPage(PEPROCESS Process, PVOID Address)
{
    return MmGetPageEntryForProcess(Process, Address) & PA_DIRTY ? TRUE : FALSE;
}

VOID
NTAPI
MmSetCleanPage(PEPROCESS Process, PVOID Address)
{
    WritePTEForProcess(Process, Address, PA_DIRTY, PTE_MODE_INVMASK);
}

VOID
NTAPI
MmSetDirtyPage(PEPROCESS Process, PVOID Address)
{
    WritePTEForProcess(Process, Address, PA_DIRTY, PTE_MODE_OR);
}

VOID
NTAPI
MmEnableVirtualMapping(PEPROCESS Process, PVOID Address)
{
    WritePTEForProcess(Process, Address, PA_PRESENT, PTE_MODE_OR);
}

BOOLEAN
NTAPI
MmIsPagePresent(PEPROCESS Process, PVOID Address)
{
    return MmGetPageEntryForProcess(Process, Address) & PA_PRESENT ? TRUE : FALSE;
}

BOOLEAN
NTAPI
MmIsPageSwapEntry(PEPROCESS Process, PVOID Address)
{
    ULONG Entry;
    Entry = MmGetPageEntryForProcess(Process, Address);
    return !(Entry & PA_PRESENT) && Entry != 0 ? TRUE : FALSE;
}

NTSTATUS
NTAPI
MmCreateVirtualMappingForKernel(PVOID Address,
                                ULONG flProtect,
                                PPFN_TYPE Pages,
                                ULONG PageCount)
{
    ULONG Attributes;
    ULONG i, Result;
    PVOID Addr;
    HARDWARE_PTE First;
    PFN_TYPE PdePfn;
    BOOLEAN NoExecute = FALSE;
    __sigset_t sigmask = {}, oldmask = {};
    
    DPRINT("MmCreateVirtualMappingForKernel(%x, %x, %x, %d)\n",
           Address, flProtect, Pages, PageCount);
    
    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    if (Address < MmSystemRangeStart)
    {
        DPRINT1("MmCreateVirtualMappingForKernel is called for user space\n");
        KeBugCheck(MEMORY_MANAGEMENT);
    }
    
    Attributes = ProtectToPTE(flProtect);
    if (Attributes & 0x80000000)
    {
        NoExecute = TRUE;
    }
    Attributes &= 0xfff;
    if (Ke386GlobalPagesEnabled)
    {
        Attributes |= PA_GLOBAL;
    }
    
    Addr = (PVOID)PAGE_ROUND_DOWN((ULONG_PTR)Address);
    
    for (i = 0; i < PageCount; i++, Addr = (PVOID)((ULONG_PTR)Addr + PAGE_SIZE))
    {
        First = *((HARDWARE_PTE*)&MmGlobalKernelPageDirectory[(((ULONG_PTR)Addr) >> 22) & 0x3ff]);
	if (!First.Valid)
	{
            NTSTATUS Status;

            Status = AllocatePDE(__readcr3(), Addr, &PdePfn);

            if (!NT_SUCCESS(Status))
                KeBugCheck(MEMORY_MANAGEMENT);
        }
        WritePTEForProcess(NULL, Addr, Attributes | (Pages[i] << PAGE_SHIFT), PTE_MODE_REPLACE);
        DPRINT("Hard mapping %x to %x\n", Addr, Pages[i]<<PAGE_SHIFT);
        unix_msync(Addr, PAGE_SIZE, MS_SYNC);
        unix_munmap(Addr, PAGE_SIZE);
        Result = unix_mmap(Addr,
                  PAGE_SIZE,
                  PROT_READ|PROT_WRITE|PROT_EXEC,
                  MAP_SHARED|MAP_FIXED,
                  MEMFD,
                  Pages[i]<<PAGE_SHIFT);
        if (Result == ((ULONG)-1))
            unix_abort();
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
    
    return(STATUS_SUCCESS);
}

NTSTATUS
NTAPI
MmCreatePageFileMapping(PEPROCESS Process,
                        PVOID Address,
                        SWAPENTRY SwapEntry)
{
    DPRINT("MmCreatePageFileMapping(%x,%x,%x)\n", Process, Address, SwapEntry);
    WritePTEForProcess(Process, Address, SwapEntry << 1, PTE_MODE_REPLACE);
    return(STATUS_SUCCESS);
}


NTSTATUS
NTAPI
MmCreateVirtualMappingUnsafe(PEPROCESS Process,
                             PVOID Address,
                             ULONG flProtect,
                             PPFN_TYPE Pages,
                             ULONG PageCount)
{
    ULONG Attributes;
    PVOID Addr;
    ULONG i;
    ULONG_PTR CR3;
    PFN_TYPE PdePfn;
    BOOLEAN NoExecute = FALSE;
    HARDWARE_PTE First;
    __sigset_t sigmask = {}, oldmask = {};

    DPRINT("MmCreateVirtualMappingUnsafe(%x, %x, %x, %x (%x), %d)\n",
           Process, Address, flProtect, Pages, *Pages, PageCount);
    
    if (Process == NULL)
    {
        if (Address < MmSystemRangeStart)
        {
            DPRINT1("No process\n");
            KeBugCheck(MEMORY_MANAGEMENT);
        }
        if (PageCount > 0x10000 ||
            (ULONG_PTR) Address / PAGE_SIZE + PageCount > 0x100000)
        {
            DPRINT1("Page count to large\n");
            KeBugCheck(MEMORY_MANAGEMENT);
        }

        return MmCreateVirtualMappingForKernel(Address, flProtect, Pages, PageCount);
    }
    else
    {
        if (Address >= MmSystemRangeStart)
        {
            DPRINT1("Setting kernel address with process context\n");
            KeBugCheck(MEMORY_MANAGEMENT);
        }
        if (PageCount > (ULONG_PTR)MmSystemRangeStart / PAGE_SIZE ||
            (ULONG_PTR) Address / PAGE_SIZE + PageCount >
            (ULONG_PTR)MmSystemRangeStart / PAGE_SIZE)
        {
            DPRINT1("Page Count to large\n");
            KeBugCheck(MEMORY_MANAGEMENT);
        }
    }
    
    unix_sigprocmask(SIG_SETMASK, NULL, &sigmask);
    sigaddset(&sigmask, SIGIO);
    unix_sigprocmask(SIG_SETMASK, &sigmask, &oldmask);
    
    CR3 = GetCR3ForProcess(Process);

    Attributes = ProtectToPTE(flProtect);
    if (Attributes & 0x80000000)
    {
        NoExecute = TRUE;
    }
    Attributes &= 0xfff;
    if (Address >= MmSystemRangeStart)
    {
        Attributes &= ~PA_USER;
        if (Ke386GlobalPagesEnabled)
        {
            Attributes |= PA_GLOBAL;
        }
    }
    else
    {
        Attributes |= PA_USER;
    }
    
    Addr = (PVOID)PAGE_ROUND_DOWN((ULONG_PTR)Address);
    for (i = 0; i < PageCount; i++, Addr = (PVOID)((ULONG_PTR)Addr + PAGE_SIZE))
    {
        DPRINT("Installing page %x as address %x\n", Pages[i], Addr);
        First = MapGetFirstLevelPTEWithCR(CR3, (ULONG_PTR)Address);
	if (!First.Valid)
	{
            NTSTATUS Status;

            Status = AllocatePDE(CR3, Addr, &PdePfn);

            if (!NT_SUCCESS(Status))
                KeBugCheck(MEMORY_MANAGEMENT);
        }

        WritePTEForProcess(Process, Addr, Attributes | (Pages[i] << PAGE_SHIFT) | Attributes, PTE_MODE_REPLACE);

#if 0
        DPRINT("Mapped page %x\n", Pages[i]);
        if (IS_THIS_PROCESS(Process) || (PVOID)Addr >= MmSystemRangeStart)
        {
            ULONG Result;
            DPRINT("Hard mapping %x to %x\n", Addr, Pages[i]<<PAGE_SHIFT);
            unix_msync(Addr, PAGE_SIZE, MS_SYNC);
            unix_munmap(Addr, PAGE_SIZE);
            Result = unix_mmap(Addr,
                               PAGE_SIZE,
                               PROT_READ|PROT_WRITE|PROT_EXEC,
                               MAP_SHARED|MAP_FIXED,
                               MEMFD,
                               Pages[i]<<PAGE_SHIFT);            
            if (Result == ((ULONG)-1))
                unix_abort();
        }
#endif
    }

    unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
    
    return(STATUS_SUCCESS);
}

NTSTATUS
NTAPI
MmCreateVirtualMapping(PEPROCESS Process,
                       PVOID Address,
                       ULONG flProtect,
                       PPFN_TYPE Pages,
                       ULONG PageCount)
{
    ULONG i;
    
    for (i = 0; i < PageCount; i++)
    {
        if (!MmIsPageInUse(Pages[i]))
        {
            DPRINT1("Page at address %x not in use\n", PFN_TO_PTE(Pages[i]));
            KeBugCheck(MEMORY_MANAGEMENT);
        }
    }
    
    return(MmCreateVirtualMappingUnsafe(Process,
                                        Address,
                                        flProtect,
                                        Pages,
                                        PageCount));
}

ULONG
NTAPI
MmGetPageProtect(PEPROCESS Process, PVOID Address)
{
    ULONG Entry;
    ULONG Protect;
    
    Entry = MmGetPageEntryForProcess(Process, Address);
    
    if (!(Entry & PA_PRESENT))
    {
        Protect = PAGE_NOACCESS;
    }
    else
    {
        if (Entry & PA_READWRITE)
        {
            Protect = PAGE_READWRITE;
        }
        else
        {
            Protect = PAGE_EXECUTE_READ;
        }
        if (Entry & PA_CD)
        {
            Protect |= PAGE_NOCACHE;
        }
        if (Entry & PA_WT)
        {
            Protect |= PAGE_WRITETHROUGH;
        }
        if (!(Entry & PA_USER))
        {
            Protect |= PAGE_SYSTEM;
        }
        
    }
    return(Protect);
}

VOID
NTAPI
MmSetPageProtect(PEPROCESS Process, PVOID Address, ULONG flProtect)
{
    ULONG Attributes = 0, Pte;
    BOOLEAN NoExecute = FALSE;
    
    DPRINT("MmSetPageProtect(Process %x  Address %x  flProtect %x)\n",
           Process, Address, flProtect);
    
    Attributes = ProtectToPTE(flProtect);
    if (Attributes & 0x80000000)
    {
        NoExecute = TRUE;
    }
    Attributes &= 0xfff;
    if (Address >= MmSystemRangeStart)
    {
        Attributes &= ~PA_USER;
        if (Ke386GlobalPagesEnabled)
        {
            Attributes |= PA_GLOBAL;
        }
    }
    else
    {
        Attributes |= PA_USER;
    }
    
    Pte = MmGetPageEntryForProcess(Process, Address);
    WritePTEForProcess(Process, Address, (Pte & ~0xfff) | Attributes, PTE_MODE_REPLACE);
}

/*
 * @implemented
 */
PHYSICAL_ADDRESS NTAPI
MmGetPhysicalAddress(PVOID vaddr)
/*
 * FUNCTION: Returns the physical address corresponding to a virtual address
 */
{
    PHYSICAL_ADDRESS p;
    ULONG Pte;
    
    DPRINT("MmGetPhysicalAddress(vaddr %x)\n", vaddr);
    if (IS_HYPERSPACE(vaddr))
    {
        p.QuadPart = *((PULONG)MiAddressToPte(vaddr)) >> PAGE_SHIFT;
        p.u.LowPart |= (ULONG_PTR)vaddr & (PAGE_SIZE - 1);
        DPRINT("Returning %x from hyperspace\n", (ULONG)p.QuadPart);
        return p;
    }

    Pte = MmGetPageEntryForProcess(NULL, vaddr);
    if (Pte == 0)
    {
        Mmi386MakeKernelPageTableGlobal(vaddr);
        Pte = MmGetPageEntryForProcess(NULL, vaddr);
    }

    DPRINT("Got page entry %x\n", Pte);
    if (Pte != 0 && Pte & PA_PRESENT)
    {
        p.QuadPart = PAGE_MASK(Pte);
        p.u.LowPart |= (ULONG_PTR)vaddr & (PAGE_SIZE - 1);
    }
    else
    {
        p.QuadPart = 0;
        DPRINT("Not Mapped!\n");
    }
    return p;
}

PVOID
NTAPI
MmCreateHyperspaceMapping(PFN_TYPE Page)
{
    PVOID Address;
    ULONG i, Result;
    
    ULONG Entry;
    PULONG Pte;
    Entry = PFN_TO_PTE(Page) | PA_PRESENT | PA_READWRITE;
    Pte = (PULONG)MiAddressToPte(HYPERSPACE) + Page % 1024;
    if (Page & 1024)
    {
        for (i = Page % 1024; i < 1024; i++, Pte++)
        {
            if (0 == InterlockedCompareExchange((PLONG)Pte, (LONG)Entry, 0))
            {
                break;
            }
        }
        if (i >= 1024)
        {
            Pte = (PULONG)MiAddressToPte(HYPERSPACE);
            for (i = 0; i < Page % 1024; i++, Pte++)
            {
                if (0 == InterlockedCompareExchange((PLONG)Pte, (LONG)Entry, 0))
                {
                    break;
                }
            }
            if (i >= Page % 1024)
            {
                KeBugCheck(MEMORY_MANAGEMENT);
            }
        }
    }
    else
    {
        for (i = Page % 1024; (LONG)i >= 0; i--, Pte--)
        {
            if (0 == InterlockedCompareExchange((PLONG)Pte, (LONG)Entry, 0))
            {
                break;
            }
        }
        if ((LONG)i < 0)
        {
            Pte = (PULONG)MiAddressToPte(HYPERSPACE) + 1023;
            for (i = 1023; i > Page % 1024; i--, Pte--)
            {
                if (0 == InterlockedCompareExchange((PLONG)Pte, (LONG)Entry, 0))
                {
                    break;
                }
            }
            if (i <= Page % 1024)
            {
                KeBugCheck(MEMORY_MANAGEMENT);
            }
        }
    }
    Address = (PVOID)((ULONG_PTR)HYPERSPACE + i * PAGE_SIZE);
    unix_msync(Address, PAGE_SIZE, MS_SYNC);
    unix_munmap(Address, PAGE_SIZE);
    Result = unix_mmap(Address,
                       PAGE_SIZE,
                       PROT_READ|PROT_WRITE|PROT_EXEC,
                       MAP_SHARED|MAP_FIXED,
                       MEMFD,
                       *Pte & ~0xfff);
    if (Result == ((ULONG)-1))
        unix_abort();    
    return Address;
}

PFN_TYPE
NTAPI
MmDeleteHyperspaceMapping(PVOID Address)
{
    PFN_TYPE Pfn;
    ULONG Entry;
    
    ASSERT (IS_HYPERSPACE(Address));
    
    unix_msync(Address, PAGE_SIZE, MS_SYNC);

    Entry = InterlockedExchangePte(MiAddressToPte(Address), 0);
    Pfn = PTE_TO_PFN(Entry);

    unix_munmap(Address, PAGE_SIZE);
    return Pfn;
}

VOID
INIT_FUNCTION
NTAPI
MmInitGlobalKernelPageDirectory(VOID)
{
    ULONG i;
    PULONG CurrentPageDirectory = (PULONG)PAGEDIRECTORY_MAP;
    
    DPRINT("MmInitGlobalKernelPageDirectory()\n");
    
    for (i = ADDR_TO_PDE_OFFSET(MmSystemRangeStart); i < 1024; i++)
    {
        if (i != ADDR_TO_PDE_OFFSET(PAGETABLE_MAP) &&
            i != ADDR_TO_PDE_OFFSET(HYPERSPACE) &&
            0 == MmGlobalKernelPageDirectory[i] && 0 != CurrentPageDirectory[i])
        {
            MmGlobalKernelPageDirectory[i] = CurrentPageDirectory[i];
            if (Ke386GlobalPagesEnabled)
            {
                MmGlobalKernelPageDirectory[i] |= PA_GLOBAL;
                CurrentPageDirectory[i] |= PA_GLOBAL;
            }
            unix_msync(MmGlobalKernelPageDirectory, PAGE_SIZE, MS_SYNC);
        }
    }
}

VOID
INIT_FUNCTION
NTAPI
MiInitPageDirectoryMap(VOID)
{
    MEMORY_AREA* kernel_map_desc = NULL;
    MEMORY_AREA* hyperspace_desc = NULL;
    PHYSICAL_ADDRESS BoundaryAddressMultiple;
    PVOID BaseAddress;
    NTSTATUS Status;
    
    DPRINT("MiInitPageDirectoryMap()\n");
    
    BoundaryAddressMultiple.QuadPart = 0;
    BaseAddress = (PVOID)PAGETABLE_MAP;
    Status = MmCreateMemoryArea(MmGetKernelAddressSpace(),
                                MEMORY_AREA_SYSTEM,
                                &BaseAddress,
                                0x400000,
                                PAGE_READWRITE,
                                &kernel_map_desc,
                                TRUE,
                                0,
                                BoundaryAddressMultiple);
    if (!NT_SUCCESS(Status))
    {
        KeBugCheck(MEMORY_MANAGEMENT);
    }
    BaseAddress = (PVOID)HYPERSPACE;
    Status = MmCreateMemoryArea(MmGetKernelAddressSpace(),
                                MEMORY_AREA_SYSTEM,
                                &BaseAddress,
                                0x400000,
                                PAGE_READWRITE,
                                &hyperspace_desc,
                                TRUE,
                                0,
                                BoundaryAddressMultiple);
    if (!NT_SUCCESS(Status))
    {
        KeBugCheck(MEMORY_MANAGEMENT);
    }
}

VOID
NTAPI
MmUpdatePageDir(PEPROCESS Process, PVOID Address, ULONG Size)
{
    ULONG CR3 = __readcr3(), i;
    ULONG_PTR Start = ADDR_TO_PDE_OFFSET(Address);
    ULONG_PTR End = ADDR_TO_PDE_OFFSET((PVOID)(Start + Size));

    //DPRINT("MmUpdatePageDir(%x,%x,%x)\n", Process, Address, Size);

    if (Address < MmSystemRangeStart)
    {
        KeBugCheck(MEMORY_MANAGEMENT);
    }

    for (i = Start; i < End; i++)
    {
        WritePDE(CR3, i, MmGlobalKernelPageDirectory[i], (PVOID)i > MmSystemRangeStart);
    }
    
    //DPRINT("Page Dir updated!\n");
}

/* EOF */
