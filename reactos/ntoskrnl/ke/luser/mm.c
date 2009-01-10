/*
 * PROJECT:         ReactOS Kernel
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            ntoskrnl/ke/i386/cpu.c
 * PURPOSE:         Routines for CPU-level support
 * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
 */

/* INCLUDES *****************************************************************/

#include <ntoskrnl.h>
#define NDEBUG
#include <debug.h>

/* GLOBALS *******************************************************************/

#define MEMFD 101

ULONG 
InterlockedCompareExchangePte
(PULONG PointerPte, ULONG Exchange, ULONG Comperand)
{
    ULONG Result = 
        InterlockedCompareExchange((PLONG)(PointerPte), Exchange, Comperand);
    __invlpg((PVOID)(Exchange & ~0xfff));
    __invlpg(PointerPte);
    return Result;
}

ULONG
InterlockedExchangePte
(PULONG PointerPte, ULONG Value)
{
    ULONG Result = 
        InterlockedExchange((PLONG)(PointerPte), Value);
    __invlpg((PVOID)(Value & ~0xfff));
    __invlpg(PointerPte);
    return Result;
}
