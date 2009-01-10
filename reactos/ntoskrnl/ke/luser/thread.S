#include <asm.h>
.intel_syntax noprefix

.globl _KiLuserSetupStackAndInitializeKernel@24
.func KiLuserSetupStackAndInitializeKernel@24
_KiLuserSetupStackAndInitializeKernel@24:

    /* Save current stack */
    mov esi, esp

    /* Setup the new stack */
    mov esp, [esp+12]
    sub esp, NPX_FRAME_LENGTH + KTRAP_FRAME_ALIGN + KTRAP_FRAME_LENGTH
    push CR0_EM + CR0_TS + CR0_MP

    /* Copy all parameters to the new stack */
    push [esi+24]
    push [esi+20]
    push [esi+16]
    push [esi+12]
    push [esi+8]
    push [esi+4]
    xor ebp, ebp
    call _KiInitializeKernel@24

    /* Set the priority of this thread to 0 */
    push KPCR_CURRENT_THREAD
    call _LuserReadFSDword
    add esp,4
        
    mov ebx, eax
    mov byte ptr [ebx+KTHREAD_PRIORITY], 0

    /* Force interrupts enabled and lower IRQL back to DISPATCH_LEVEL */
    push ebx
        
    mov ecx, DISPATCH_LEVEL
    call @KfLowerIrql@4
        
    pop ebx

    /* Set the right wait IRQL */
    mov byte ptr [ebx+KTHREAD_WAIT_IRQL], DISPATCH_LEVEL;

    /* Jump into the idle loop */
    jmp @KiIdleLoop@0
.endfunc
