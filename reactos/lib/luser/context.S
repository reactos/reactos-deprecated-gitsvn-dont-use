/* Save current context.
   Copyright (C) 2001, 2002, 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2001.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <ucontext_i.h>

#define __NR_sigprocmask 126
#define SYSCALL_ERROR_LABEL __syscall_error

        .globl __syscall_error
        
        .text

        .globl _unix_getcontext
_unix_getcontext:   
	/* Load address of the context data structure.  */
	movl	4(%esp), %eax

	/* Return value of getcontext.  EAX is the only register whose
	   value is not preserved.  */
	movl	$0, oEAX(%eax)

	/* Save the 32-bit register values and the return address.  */
	movl	%ecx, oECX(%eax)
	movl	%edx, oEDX(%eax)
	movl	%edi, oEDI(%eax)
	movl	%esi, oESI(%eax)
	movl	%ebp, oEBP(%eax)
	movl	(%esp), %ecx
	movl	%ecx, oEIP(%eax)
	leal	4(%esp), %ecx		/* Exclude the return address.  */
	movl	%ecx, oESP(%eax)
	movl	%ebx, oEBX(%eax)

	/* Save the FS segment register.  We don't touch the GS register
	   since it is used for threads.  */
	xorl	%edx, %edx
	movw	%fs, %dx
	movl	%edx, oFS(%eax)

	/* We have separate floating-point register content memory on the
	   stack.  We use the __fpregs_mem block in the context.  Set the
	   links up correctly.  */
        /* We use this for priveleged instructions and the kernel is already
         * already conscious of using floating point registers.  Hence we
         * disable fp use here */
#if 0
	leal	oFPREGSMEM(%eax), %ecx
	movl	%ecx, oFPREGS(%eax)
	/* Save the floating-point context.  */
	fnstenv	(%ecx)
	/* And load it right back since the processor changes the mask.
	   Intel thought this opcode to be used in interrupt handlers which
	   would block all exceptions.  */
	fldenv	(%ecx)
#endif
        
	/* Save the current signal mask.  */
	pushl	%ebx
	leal	oSIGMASK(%eax), %edx
	xorl	%ecx, %ecx
	movl	$SIG_BLOCK, %ebx
	movl	$__NR_sigprocmask, %eax
	// ENTER_KERNEL
        int     $0x80
	popl	%ebx
	cmpl	$-4095, %eax		/* Check %eax for error.  */
	jae	SYSCALL_ERROR_LABEL	/* Jump to error handler if error.  */

	/* All done, return 0 for success.  */
	xorl	%eax, %eax
	ret

        .globl _unix_setcontext
_unix_setcontext:   
	/* Load address of the context data structure.  */
	movl	4(%esp), %eax

	/* Get the current signal mask.  Note that we preserve EBX in case
	   the system call fails and we return from the function with an
	   error.  */
	pushl	%ebx
	xorl	%edx, %edx
	leal	oSIGMASK(%eax), %ecx
	movl	$SIG_SETMASK, %ebx
	movl	$__NR_sigprocmask, %eax
	// ENTER_KERNEL
        int     $0x80
	popl	%ebx
	cmpl	$-4095, %eax		/* Check %eax for error.  */
	jae	SYSCALL_ERROR_LABEL	/* Jump to error handler if error.  */

	/* EAX was modified, reload it.  */
	movl	4(%esp), %eax

        /* @See above regarding floating point */
#if 0
	/* Restore the floating-point context.  Not the registers, only the
	   rest.  */
	movl	oFPREGS(%eax), %ecx
	fldenv	(%ecx)
#endif

	/* Restore the FS segment register.  We don't touch the GS register
	   since it is used for threads.  */
	movl	oFS(%eax), %ecx
	movw	%cx, %fs

	/* Fetch the address to return to.  */
	movl	oEIP(%eax), %ecx

	/* Load the new stack pointer.  */
	movl	oESP(%eax), %esp

	/* Push the return address on the new stack so we can return there.  */
	pushl	%ecx

	/* Load the values of all the 32-bit registers (except ESP).
	   Since we are loading from EAX, it must be last.  */
	movl	oEDI(%eax), %edi
	movl	oESI(%eax), %esi
	movl	oEBP(%eax), %ebp
	movl	oEBX(%eax), %ebx
	movl	oEDX(%eax), %edx
	movl	oECX(%eax), %ecx
	movl	oEAX(%eax), %eax

	/* The following 'ret' will pop the address of the code and jump
	   to it.  */

	ret
