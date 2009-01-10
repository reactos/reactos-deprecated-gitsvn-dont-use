/* Wrapper for setting errno.
 *
 * Copyright (C) 2000-2006 Erik Andersen <andersen@uclibc.org>
 *
 * Licensed under the LGPL v2.1, see the file COPYING.LIB in this tarball.
 */

/* This routine is jumped to by all the syscall handlers, to stash
 * an error number into errno.  */

/* This version uses a lot of magic and relies heavily on x86
 * calling convention ... The advantage is that this is the same
 * size as the previous __syscall_error() but all the .S functions
 * need just one instruction.
 *
 * Local .S files have to set %eax to the negative errno value
 * and then jump to this function.  The neglected return to caller
 * and return value of -1 is taken care of here so we don't have to
 * worry about it in the .S functions.
 *
 * We have to stash the errno from %eax in a local stack var because
 * __set_errno will prob call a function thus clobbering %eax on us.
 */

#include <ddk/ntddk.h>
#include <luser/luser.h>
//#include <errno.h>
//#include <features.h>

int errno;
/*int __syscall_error(void) attribute_hidden;*/
int _syscall_error(void)
{
	register int edx __asm__ ("%edx");
	__asm__ ("mov %eax, %edx\n\t"
		 "negl %edx");
	/*__set_errno (edx);*/
#if 0
        Printf("Error %d from syscall at %x\n",
               edx, __builtin_return_address(0));
#endif
	errno = edx;
	return -1;
}
