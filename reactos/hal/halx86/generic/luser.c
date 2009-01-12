/*
 * PROJECT:         ReactOS HAL
 * LICENSE:         GPL - See COPYING in the top level directory
 * FILE:            hal/halx86/generic/luser.c
 * PURPOSE:         HAL Luser
 * PROGRAMMERS:     arty
 */

/* INCLUDES ******************************************************************/

#include <hal.h>
#define NDEBUG
#include <debug.h>

BOOLEAN 
NTAPI
HalpBiosDisplayReset(VOID)
{
    return TRUE;
}

VOID
NTAPI
CheckPcr(int eax)
{
    char buf[128];
    sprintf(buf, "CurrentIrql %d (eax %x)\n", KeGetCurrentIrql(), eax);
    HalDisplayString(buf);
}
