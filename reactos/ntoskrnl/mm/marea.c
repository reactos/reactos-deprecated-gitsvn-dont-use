/*
 * Copyright (C) 1998-2005 ReactOS Team (and the authors from the programmers section)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 *
 * PROJECT:         ReactOS kernel
 * FILE:            ntoskrnl/mm/marea.c
 * PURPOSE:         Implements memory areas
 *
 * PROGRAMMERS:     Rex Jolliff
 *                  David Welch
 *                  Eric Kohl
 *                  Philip Susi
 *                  Casper Hornstrup
 *                  Eric Kohl
 *                  Ge van Geldorp
 *                  Royce Mitchell III
 *                  Aleksey Bragin
 *                  Jason Filby
 *                  Thomas Weidenmueller
 *                  Gunnar Andre' Dalsnes
 *                  Mike Nordell
 *                  Alex Ionescu
 *                  Filip Navara
 *                  Herve Poussineau
 *                  Steven Edwards
 */

/* INCLUDES *****************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <cache/section/newmm.h>
#include <debug.h>

#include "ARM3/miarm.h"

MEMORY_AREA MiStaticMemoryAreas[MI_STATIC_MEMORY_AREAS];
ULONG MiStaticMemoryAreaCount;

MM_AVL_TABLE MiRosKernelVadRoot;
BOOLEAN MiRosKernelVadRootInitialized;

/* FUNCTIONS *****************************************************************/

PMEMORY_AREA NTAPI
MmLocateMemoryAreaByAddress(
    PMMSUPPORT AddressSpace,
    PVOID Address_)
{
    ULONG_PTR StartVpn = (ULONG_PTR)Address_ / PAGE_SIZE;
    PEPROCESS Process;
    PMM_AVL_TABLE Table;
    PMMADDRESS_NODE Node;
    PMEMORY_AREA MemoryArea;
    TABLE_SEARCH_RESULT Result;
    PMMVAD_LONG Vad;

    Process = MmGetAddressSpaceOwner(AddressSpace);
    Table = (Process != NULL) ? &Process->VadRoot : &MiRosKernelVadRoot;

    Result = MiCheckForConflictingNode(StartVpn, StartVpn, Table, &Node);
    if (Result != TableFoundNode)
    {
        return NULL;
    }

    Vad = (PMMVAD_LONG)Node;
    if (Vad->u.VadFlags.Spare == 0)
    {
        /* Check if this is VM VAD */
        if (Vad->ControlArea == NULL)
        {
            /* We store the reactos MEMORY_AREA here */
            MemoryArea = (PMEMORY_AREA)Vad->FirstPrototypePte;
        }
        else
        {
            /* This is a section VAD. Store the MAREA here for now */
            MemoryArea = (PMEMORY_AREA)Vad->u4.Banked;
        }
    }
    else
    {
        MemoryArea = (PMEMORY_AREA)Node;
    }

    return MemoryArea;
}

PMEMORY_AREA
NTAPI
MmLocateMemoryAreaByRegion(
    PMMSUPPORT AddressSpace,
    PVOID Address_,
    ULONG_PTR Length)
{
    ULONG_PTR StartVpn = (ULONG_PTR)Address_ / PAGE_SIZE;
    ULONG_PTR EndVpn = ((ULONG_PTR)Address_ + Length - 1) / PAGE_SIZE;
    PEPROCESS Process;
    PMM_AVL_TABLE Table;
    PMMADDRESS_NODE Node;
    PMEMORY_AREA MemoryArea;
    TABLE_SEARCH_RESULT Result;
    PMMVAD_LONG Vad;

    Process = MmGetAddressSpaceOwner(AddressSpace);
    Table = (Process != NULL) ? &Process->VadRoot : &MiRosKernelVadRoot;

    Result = MiCheckForConflictingNode(StartVpn, EndVpn, Table, &Node);
    if (Result != TableFoundNode)
    {
        return NULL;
    }

    Vad = (PMMVAD_LONG)Node;
    if (Vad->u.VadFlags.Spare == 0)
    {
        /* Check if this is VM VAD */
        if (Vad->ControlArea == NULL)
        {
            /* We store the reactos MEMORY_AREA here */
            MemoryArea = (PMEMORY_AREA)Vad->FirstPrototypePte;
        }
        else
        {
            /* This is a section VAD. Store the MAREA here for now */
            MemoryArea = (PMEMORY_AREA)Vad->u4.Banked;
        }
    }
    else
    {
        MemoryArea = (PMEMORY_AREA)Node;
    }

    ASSERT(MemoryArea != NULL);
    return MemoryArea;
}

VOID
NTAPI
MiInsertVad(IN PMMVAD Vad,
            IN PMM_AVL_TABLE VadRoot);

ULONG
NTAPI
MiMakeProtectionMask(
    IN ULONG Protect
);


static VOID
MmInsertMemoryArea(
    PMMSUPPORT AddressSpace,
    PMEMORY_AREA marea)
{
    PEPROCESS Process = MmGetAddressSpaceOwner(AddressSpace);

    marea->VadNode.StartingVpn = marea->StartingVpn;
    marea->VadNode.EndingVpn = marea->EndingVpn;
    marea->VadNode.u.VadFlags.Spare = 1;
    marea->VadNode.u.VadFlags.Protection = MiMakeProtectionMask(marea->Protect);

    /* Build a lame VAD if this is a user-space allocation */
    if (marea->EndingVpn + 1 < (ULONG_PTR)MmSystemRangeStart >> PAGE_SHIFT)
    {
        ASSERT(Process != NULL);
        if (marea->Type != MEMORY_AREA_OWNED_BY_ARM3)
        {
            ASSERT(marea->Type == MEMORY_AREA_SECTION_VIEW || marea->Type == MEMORY_AREA_CACHE);

            /* Insert the VAD */
            MiLockProcessWorkingSetUnsafe(PsGetCurrentProcess(), PsGetCurrentThread());
            MiInsertVad(&marea->VadNode, &Process->VadRoot);
            MiUnlockProcessWorkingSetUnsafe(PsGetCurrentProcess(), PsGetCurrentThread());
            marea->Vad = &marea->VadNode;
        }
    }
    else
    {
        ASSERT(Process == NULL);

        if (!MiRosKernelVadRootInitialized)
        {
            MiRosKernelVadRoot.BalancedRoot.u1.Parent = &MiRosKernelVadRoot.BalancedRoot;
            MiRosKernelVadRoot.Unused = 1;
            MiRosKernelVadRootInitialized = TRUE;
        }

        /* Insert the VAD */
        MiLockWorkingSet(PsGetCurrentThread(), &MmSystemCacheWs);
        MiInsertVad(&marea->VadNode, &MiRosKernelVadRoot);
        MiUnlockWorkingSet(PsGetCurrentThread(), &MmSystemCacheWs);
        marea->Vad = NULL;
    }
}

PVOID NTAPI
MmFindGap(
    PMMSUPPORT AddressSpace,
    ULONG_PTR Length,
    ULONG_PTR Granularity,
    BOOLEAN TopDown)
{
    PEPROCESS Process;
    PMM_AVL_TABLE VadRoot;
    TABLE_SEARCH_RESULT Result;
    PMMADDRESS_NODE Parent;
    ULONG_PTR StartingAddress, HighestAddress;

    Process = MmGetAddressSpaceOwner(AddressSpace);
    VadRoot = Process ? &Process->VadRoot : &MiRosKernelVadRoot;
    if (TopDown)
    {
        /* Find an address top-down */
        HighestAddress = Process ? (ULONG_PTR)MM_HIGHEST_VAD_ADDRESS : (LONG_PTR)-1;
        Result = MiFindEmptyAddressRangeDownTree(Length,
                                                 HighestAddress,
                                                 Granularity,
                                                 VadRoot,
                                                 &StartingAddress,
                                                 &Parent);
    }
    else
    {
        Result = MiFindEmptyAddressRangeInTree(Length,
                                               Granularity,
                                               VadRoot,
                                               &Parent,
                                               &StartingAddress);
    }

    if (Result == TableFoundNode)
    {
        return NULL;
    }

    return (PVOID)StartingAddress;
}

VOID
NTAPI
MiRemoveNode(IN PMMADDRESS_NODE Node,
             IN PMM_AVL_TABLE Table);


/**
 * @name MmFreeMemoryArea
 *
 * Free an existing memory area.
 *
 * @param AddressSpace
 *        Address space to free the area from.
 * @param MemoryArea
 *        Memory area we're about to free.
 * @param FreePage
 *        Callback function for each freed page.
 * @param FreePageContext
 *        Context passed to the callback function.
 *
 * @return Status
 *
 * @remarks Lock the address space before calling this function.
 */

NTSTATUS NTAPI
MmFreeMemoryArea(
    PMMSUPPORT AddressSpace,
    PMEMORY_AREA MemoryArea,
    PMM_FREE_PAGE_FUNC FreePage,
    PVOID FreePageContext)
{
    ULONG_PTR Address;
    PVOID EndAddress;

    /* Make sure we own the address space lock! */
    ASSERT(CONTAINING_RECORD(AddressSpace, EPROCESS, Vm)->AddressCreationLock.Owner == KeGetCurrentThread());

    /* Check magic */
    ASSERT(MemoryArea->Magic == 'erAM');

    if (MemoryArea->Type != MEMORY_AREA_OWNED_BY_ARM3)
    {
        PEPROCESS CurrentProcess = PsGetCurrentProcess();
        PEPROCESS Process = MmGetAddressSpaceOwner(AddressSpace);

        if (Process != NULL &&
                Process != CurrentProcess)
        {
            KeAttachProcess(&Process->Pcb);
        }

        EndAddress = MM_ROUND_UP(MA_GetEndingAddress(MemoryArea), PAGE_SIZE);
        for (Address = MA_GetStartingAddress(MemoryArea);
                Address < (ULONG_PTR)EndAddress;
                Address += PAGE_SIZE)
        {
            BOOLEAN Dirty = FALSE;
            SWAPENTRY SwapEntry = 0;
            PFN_NUMBER Page = 0;

            if (MmIsPageSwapEntry(Process, (PVOID)Address))
            {
                MmDeletePageFileMapping(Process, (PVOID)Address, &SwapEntry);
            }
            else
            {
                MmDeleteVirtualMapping(Process, (PVOID)Address, &Dirty, &Page);
            }
            if (FreePage != NULL)
            {
                FreePage(FreePageContext, MemoryArea, (PVOID)Address,
                         Page, SwapEntry, (BOOLEAN)Dirty);
            }
#if (_MI_PAGING_LEVELS == 2)
            /* Remove page table reference */
            ASSERT(KeGetCurrentIrql() <= APC_LEVEL);
            if ((SwapEntry || Page) && ((PVOID)Address < MmSystemRangeStart))
            {
                ASSERT(AddressSpace != MmGetKernelAddressSpace());
                if (MiQueryPageTableReferences((PVOID)Address) == 0)
                {
                    /* No PTE relies on this PDE. Release it */
                    KIRQL OldIrql = KeAcquireQueuedSpinLock(LockQueuePfnLock);
                    PMMPDE PointerPde = MiAddressToPde(Address);
                    ASSERT(PointerPde->u.Hard.Valid == 1);
                    MiDeletePte(PointerPde, MiPdeToPte(PointerPde), Process, NULL);
                    ASSERT(PointerPde->u.Hard.Valid == 0);
                    KeReleaseQueuedSpinLock(LockQueuePfnLock, OldIrql);
                }
            }
#endif
        }

        if (Process != NULL &&
                Process != CurrentProcess)
        {
            KeDetachProcess();
        }

        //if (MemoryArea->VadNode.StartingVpn < (ULONG_PTR)MmSystemRangeStart >> PAGE_SHIFT
        if (MemoryArea->Vad)
        {
            ASSERT(MemoryArea->EndingVpn + 1 < (ULONG_PTR)MmSystemRangeStart >> PAGE_SHIFT);
            ASSERT(MemoryArea->Type == MEMORY_AREA_SECTION_VIEW || MemoryArea->Type == MEMORY_AREA_CACHE);

            /* MmCleanProcessAddressSpace might have removed it (and this would be MmDeleteProcessAdressSpace) */
            ASSERT(MemoryArea->VadNode.u.VadFlags.Spare != 0);
            if (((PMMVAD)MemoryArea->Vad)->u.VadFlags.Spare == 1)
            {
                MiRemoveNode((PMMADDRESS_NODE)&MemoryArea->VadNode, &Process->VadRoot);
            }

            MemoryArea->Vad = NULL;
        }
        else
        {
            MiRemoveNode((PMMADDRESS_NODE)&MemoryArea->VadNode, &MiRosKernelVadRoot);
        }
    }

#if DBG
    MemoryArea->Magic = 'daeD';
#endif
    ExFreePoolWithTag(MemoryArea, TAG_MAREA);

    DPRINT("MmFreeMemoryAreaByNode() succeeded\n");

    return STATUS_SUCCESS;
}

/**
 * @name MmCreateMemoryArea
 *
 * Create a memory area.
 *
 * @param AddressSpace
 *        Address space to create the area in.
 * @param Type
 *        Type of the memory area.
 * @param BaseAddress
 *        Base address for the memory area we're about the create. On
 *        input it contains either 0 (auto-assign address) or preferred
 *        address. On output it contains the starting address of the
 *        newly created area.
 * @param Length
 *        Length of the area to allocate.
 * @param Attributes
 *        Protection attributes for the memory area.
 * @param Result
 *        Receives a pointer to the memory area on successful exit.
 *
 * @return Status
 *
 * @remarks Lock the address space before calling this function.
 */

NTSTATUS NTAPI
MmCreateMemoryArea(PMMSUPPORT AddressSpace,
                   ULONG Type,
                   PVOID *BaseAddress,
                   ULONG_PTR Length,
                   ULONG Protect,
                   PMEMORY_AREA *Result,
                   ULONG AllocationFlags,
                   ULONG Granularity)
{
    ULONG_PTR tmpLength;
    PMEMORY_AREA MemoryArea;
    ULONG_PTR EndingAddress;

    DPRINT("MmCreateMemoryArea(Type 0x%lx, BaseAddress %p, "
           "*BaseAddress %p, Length %p, AllocationFlags %x, "
           "Result %p)\n",
           Type, BaseAddress, *BaseAddress, Length, AllocationFlags,
           Result);

    /* Is this a static memory area? */
    if (Type & MEMORY_AREA_STATIC)
    {
        /* Use the static array instead of the pool */
        ASSERT(MiStaticMemoryAreaCount < MI_STATIC_MEMORY_AREAS);
        MemoryArea = &MiStaticMemoryAreas[MiStaticMemoryAreaCount++];
    }
    else
    {
        /* Allocate the memory area from nonpaged pool */
        MemoryArea = ExAllocatePoolWithTag(NonPagedPool,
                                           sizeof(MEMORY_AREA),
                                           TAG_MAREA);
    }

    if (!MemoryArea)
    {
        DPRINT1("Not enough memory.\n");
        return STATUS_NO_MEMORY;
    }

    RtlZeroMemory(MemoryArea, sizeof(MEMORY_AREA));
    MemoryArea->Type = Type & ~MEMORY_AREA_STATIC;
    MemoryArea->Protect = Protect;
    MemoryArea->Flags = AllocationFlags;
    MemoryArea->Magic = 'erAM';
    MemoryArea->DeleteInProgress = FALSE;

    if (*BaseAddress == 0)
    {
        tmpLength = (ULONG_PTR)MM_ROUND_UP(Length, PAGE_SIZE);
        *BaseAddress = MmFindGap(AddressSpace,
                                 tmpLength,
                                 Granularity,
                                 (AllocationFlags & MEM_TOP_DOWN) == MEM_TOP_DOWN);
        if ((*BaseAddress) == 0)
        {
            DPRINT("No suitable gap\n");
            if (!(Type & MEMORY_AREA_STATIC)) ExFreePoolWithTag(MemoryArea, TAG_MAREA);
            return STATUS_NO_MEMORY;
        }

        MemoryArea->StartingVpn = (ULONG_PTR)*BaseAddress >> PAGE_SHIFT;
        MemoryArea->EndingVpn = ((ULONG_PTR)*BaseAddress + tmpLength - 1) >> PAGE_SHIFT;
        MmInsertMemoryArea(AddressSpace, MemoryArea);
    }
    else
    {
        EndingAddress = ((ULONG_PTR)*BaseAddress + Length - 1) | (PAGE_SIZE - 1);
        *BaseAddress = ALIGN_DOWN_POINTER_BY(*BaseAddress, Granularity);
        tmpLength = EndingAddress + 1 - (ULONG_PTR)*BaseAddress;

        if (!MmGetAddressSpaceOwner(AddressSpace) && *BaseAddress < MmSystemRangeStart)
        {
            ASSERT(FALSE);
            if (!(Type & MEMORY_AREA_STATIC)) ExFreePoolWithTag(MemoryArea, TAG_MAREA);
            return STATUS_ACCESS_VIOLATION;
        }

        if (MmGetAddressSpaceOwner(AddressSpace) &&
                (ULONG_PTR)(*BaseAddress) + tmpLength > (ULONG_PTR)MmSystemRangeStart)
        {
            DPRINT("Memory area for user mode address space exceeds MmSystemRangeStart\n");
            if (!(Type & MEMORY_AREA_STATIC)) ExFreePoolWithTag(MemoryArea, TAG_MAREA);
            return STATUS_ACCESS_VIOLATION;
        }

        /* No need to check ARM3 owned memory areas, the range MUST be free */
        if (MemoryArea->Type != MEMORY_AREA_OWNED_BY_ARM3)
        {
            if (MmLocateMemoryAreaByRegion(AddressSpace,
                                           *BaseAddress,
                                           tmpLength) != NULL)
            {
                DPRINT("Memory area already occupied\n");
                if (!(Type & MEMORY_AREA_STATIC)) ExFreePoolWithTag(MemoryArea, TAG_MAREA);
                return STATUS_CONFLICTING_ADDRESSES;
            }
        }

        MemoryArea->StartingVpn = (ULONG_PTR)*BaseAddress >> PAGE_SHIFT;
        MemoryArea->EndingVpn = ((ULONG_PTR)*BaseAddress + tmpLength - 1) >> PAGE_SHIFT;
        MmInsertMemoryArea(AddressSpace, MemoryArea);
    }

    *Result = MemoryArea;

    DPRINT("MmCreateMemoryArea() succeeded (%p)\n", *BaseAddress);
    return STATUS_SUCCESS;
}

VOID
NTAPI
MiRosCleanupMemoryArea(
    PEPROCESS Process,
    PMMVAD Vad)
{
    PMEMORY_AREA MemoryArea;
    PVOID BaseAddress;
    NTSTATUS Status;

    /* We must be called from MmCleanupAddressSpace and nowhere else!
       Make sure things are as expected... */
    ASSERT(Process == PsGetCurrentProcess());
    ASSERT(Process->VmDeleted == TRUE);
    ASSERT(((PsGetCurrentThread()->ThreadsProcess == Process) &&
            (Process->ActiveThreads == 1)) ||
           (Process->ActiveThreads == 0));

    /* We are in cleanup, we don't need to synchronize */
    MmUnlockAddressSpace(&Process->Vm);

    MemoryArea = (PMEMORY_AREA)Vad;
    BaseAddress = (PVOID)MA_GetStartingAddress(MemoryArea);

    if (MemoryArea->Type == MEMORY_AREA_SECTION_VIEW)
    {
        Status = MiRosUnmapViewOfSection(Process, BaseAddress, Process->ProcessExiting);
    }
    else if (MemoryArea->Type == MEMORY_AREA_CACHE)
    {
        Status = MmUnmapViewOfCacheSegment(&Process->Vm, BaseAddress);
    }
    else
    {
        /* There shouldn't be anything else! */
        ASSERT(FALSE);
    }

    /* Make sure this worked! */
    ASSERT(NT_SUCCESS(Status));

    /* Lock the address space again */
    MmLockAddressSpace(&Process->Vm);
}

VOID
NTAPI
MmDeleteProcessAddressSpace2(IN PEPROCESS Process);

NTSTATUS
NTAPI
MmDeleteProcessAddressSpace(PEPROCESS Process)
{
    KIRQL OldIrql;
    PVOID Address;

    DPRINT("MmDeleteProcessAddressSpace(Process %p (%s))\n", Process,
           Process->ImageFileName);

#ifndef _M_AMD64
    OldIrql = MiAcquireExpansionLock();
    RemoveEntryList(&Process->MmProcessLinks);
    MiReleaseExpansionLock(OldIrql);
#endif
    MmLockAddressSpace(&Process->Vm);

    /* There should not be any memory areas left! */
    ASSERT(Process->Vm.WorkingSetExpansionLinks.Flink == NULL);

#if (_MI_PAGING_LEVELS == 2)
    {
        KIRQL OldIrql;
        PMMPDE pointerPde;
        /* Attach to Process */
        KeAttachProcess(&Process->Pcb);

        /* Acquire PFN lock */
        OldIrql = KeAcquireQueuedSpinLock(LockQueuePfnLock);

        for (Address = MI_LOWEST_VAD_ADDRESS;
                Address < MM_HIGHEST_VAD_ADDRESS;
                Address =(PVOID)((ULONG_PTR)Address + (PAGE_SIZE * PTE_COUNT)))
        {
            /* At this point all references should be dead */
            if (MiQueryPageTableReferences(Address) != 0)
            {
                DPRINT1("Process %p, Address %p, UsedPageTableEntries %lu\n",
                        Process,
                        Address,
                        MiQueryPageTableReferences(Address));
                ASSERT(MiQueryPageTableReferences(Address) == 0);
            }

            pointerPde = MiAddressToPde(Address);
            /* Unlike in ARM3, we don't necesarrily free the PDE page as soon as reference reaches 0,
             * so we must clean up a bit when process closes */
            if (pointerPde->u.Hard.Valid)
                MiDeletePte(pointerPde, MiPdeToPte(pointerPde), Process, NULL);
            ASSERT(pointerPde->u.Hard.Valid == 0);
        }

        /* Release lock */
        KeReleaseQueuedSpinLock(LockQueuePfnLock, OldIrql);

        /* Detach */
        KeDetachProcess();
    }
#endif

    MmUnlockAddressSpace(&Process->Vm);

    DPRINT("Finished MmDeleteProcessAddressSpace()\n");
    MmDeleteProcessAddressSpace2(Process);
    return(STATUS_SUCCESS);
}

/* EOF */
