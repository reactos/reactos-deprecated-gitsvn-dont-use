#ifndef _NTOSKRNL_LUSER_SIGACTION_H
#define _NTOSKRNL_LUSER_SIGACTION_H

/* The proper definitions for Linux's sigaction.
   Copyright (C) 1993-1999, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

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

#define __const
typedef int __sig_atomic_t;
typedef int timer_t;
typedef int sigval_t;
typedef int __ARCH_SI_UID_T;
typedef int __ARCH_SI_BAND_T;
#define _EXTERN_INLINE static inline
#define __user
#define SI_MAX_SIZE     128
#define SI_PAD_SIZE     ((SI_MAX_SIZE/sizeof(int)) - 3)

typedef struct siginfo {
        int si_signo;
        int si_errno;
        int si_code;

        union {
                int _pad[SI_PAD_SIZE];

                /* kill() */
                struct {
                        pid_t _pid;             /* sender's pid */
                        __ARCH_SI_UID_T _uid;   /* sender's uid */
                } _kill;

                /* POSIX.1b timers */
                struct {
                        timer_t _tid;           /* timer id */
                        int _overrun;           /* overrun count */
                        char _pad[sizeof( __ARCH_SI_UID_T) - sizeof(int)];
                        sigval_t _sigval;       /* same as below */
                        int _sys_private;       /* not to be passed to user */
                } _timer;

                /* POSIX.1b signals */
                struct {
                        pid_t _pid;             /* sender's pid */
                        __ARCH_SI_UID_T _uid;   /* sender's uid */
                        sigval_t _sigval;
                } _rt;

                /* SIGCHLD */
                struct {
                        pid_t _pid;             /* which child */
                        __ARCH_SI_UID_T _uid;   /* sender's uid */
                        int _status;            /* exit code */
                        clock_t _utime;
                        clock_t _stime;
                } _sigchld;

                /* SIGILL, SIGFPE, SIGSEGV, SIGBUS */
                struct {
                        void __user *_addr; /* faulting insn/memory ref. */
#ifdef __ARCH_SI_TRAPNO
                        int _trapno;    /* TRAP # which caused the signal */
#endif
                } _sigfault;

                /* SIGPOLL */
                struct {
                        __ARCH_SI_BAND_T _band; /* POLL_IN, POLL_OUT, POLL_MSG */
                        int _fd;
                } _sigpoll;
        } _sifields;
} siginfo_t;

#define si_ptr _sifields._sigfault._addr

/* A `sigset_t' has a bit for each signal.  */
# define _SIGSET_NWORDS 1
typedef int (*__sighandler_t)(int);

typedef struct _sigset_t {
    unsigned long __val[_SIGSET_NWORDS];
} __sigset_t;

struct sigaction {
        void (*sa_sigaction)(int, siginfo_t *, void *);
        __sigset_t sa_mask;
        unsigned long sa_flags;
        void (*sa_restorer)(void);
};

/* Bits in `sa_flags'.  */
#define	SA_NOCLDSTOP  1		 /* Don't send SIGCHLD when children stop.  */
#define SA_NOCLDWAIT  2		 /* Don't create zombie on child death.  */
#define SA_SIGINFO    4		 /* Invoke signal-catching function with
				    three arguments instead of one.  */
/*#if defined __USE_UNIX98 || defined __USE_MISC*/
# define SA_RESTORER  0x04000000 /* Use a restorer function */
# define SA_ONSTACK   0x08000000 /* Use signal stack by using `sa_restorer'. */
# define SA_RESTART   0x10000000 /* Restart syscall on signal return.  */
# define SA_NODEFER   0x40000000 /* Don't automatically block the signal when
				    its handler is being executed.  */
# define SA_RESETHAND 0x80000000 /* Reset to SIG_DFL on entry to handler.  */
/*#endif*/
#ifdef __USE_MISC
# define SA_INTERRUPT 0x20000000 /* Historical no-op.  */

/* Some aliases for the SA_ constants.  */
# define SA_NOMASK    SA_NODEFER
# define SA_ONESHOT   SA_RESETHAND
# define SA_STACK     SA_ONSTACK
#endif

/* Values for the HOW argument to `sigprocmask'.  */
#define	SIG_BLOCK     0		 /* Block signals.  */
#define	SIG_UNBLOCK   1		 /* Unblock signals.  */
#define	SIG_SETMASK   2		 /* Set the set of blocked signals.  */

/* Return a mask that includes the bit for SIG only.  */
# define __sigmask(sig) \
    (((unsigned long int) 1) << (((sig) - 1) % (8 * sizeof (unsigned long int))))

/* Return the word index for SIG.  */
# define __sigword(sig) (((sig) - 1) / (8 * sizeof (unsigned long int)))

/* The POSIX does not specify for handling the whole signal set in one
   command.  This is often wanted and so we define three more functions
   here.  */
#   define __sigisemptyset(set) \
  (__extension__ ({ int __cnt = _SIGSET_NWORDS;                               \
                    const __sigset_t *__set = (set);                          \
                    int __ret = __set->__val[--__cnt];                        \
                    while (!__ret && --__cnt >= 0)                            \
                        __ret = __set->__val[__cnt];                          \
                    __ret == 0; }))
#   define __sigandset(dest, left, right) \
  (__extension__ ({ int __cnt = _SIGSET_NWORDS;                               \
                    __sigset_t *__dest = (dest);                              \
                    const __sigset_t *__left = (left);                        \
                    const __sigset_t *__right = (right);                      \
                    while (--__cnt >= 0)                                      \
                      __dest->__val[__cnt] = (__left->__val[__cnt]            \
                                              & __right->__val[__cnt]);       \
                    0; }))
#   define __sigorset(dest, left, right) \
  (__extension__ ({ int __cnt = _SIGSET_NWORDS;                               \
                    __sigset_t *__dest = (dest);                              \
                    const __sigset_t *__left = (left);                        \
                    const __sigset_t *__right = (right);                      \
                    while (--__cnt >= 0)                                      \
                      __dest->__val[__cnt] = (__left->__val[__cnt]            \
                                              | __right->__val[__cnt]);       \
                    0; }))
/* These functions needn't check for a bogus signal number -- error
   checking is done in the non __ versions.  */

#  define __SIGSETFN(NAME, BODY, CONST)                                       \
  _EXTERN_INLINE int                                                          \
  NAME (CONST __sigset_t *__set, int __sig)                                   \
  {                                                                           \
    unsigned long int __mask = __sigmask (__sig);                             \
    unsigned long int __word = __sigword (__sig);                             \
    return BODY;                                                              \
  }

#define sigemptyset(__set) memset(__set, 0, sizeof(*__set))
__SIGSETFN (sigismember, (__set->__val[__word] & __mask) ? 1 : 0, __const)
__SIGSETFN (sigaddset, ((__set->__val[__word] |= __mask), 0), )
__SIGSETFN (sigdelset, ((__set->__val[__word] &= ~__mask), 0), )

#  undef __SIGSETFN

typedef struct {
    void *ss_sp;
    int ss_flags;
    int ss_size;
} stack_t;

/* Type for general register.  */
typedef int greg_t;

/* Number of general registers.  */
#define NGREG	19

/* Container for all general registers.  */
typedef greg_t gregset_t[NGREG];

/* Number of each register is the `gregset_t' array.  */
enum
{
  REG_GS = 0,
# define REG_GS		REG_GS
  REG_FS,
# define REG_FS		REG_FS
  REG_ES,
# define REG_ES		REG_ES
  REG_DS,
# define REG_DS		REG_DS
  REG_EDI,
# define REG_EDI	REG_EDI
  REG_ESI,
# define REG_ESI	REG_ESI
  REG_EBP,
# define REG_EBP	REG_EBP
  REG_ESP,
# define REG_ESP	REG_ESP
  REG_EBX,
# define REG_EBX	REG_EBX
  REG_EDX,
# define REG_EDX	REG_EDX
  REG_ECX,
# define REG_ECX	REG_ECX
  REG_EAX,
# define REG_EAX	REG_EAX
  REG_TRAPNO,
# define REG_TRAPNO	REG_TRAPNO
  REG_ERR,
# define REG_ERR	REG_ERR
  REG_EIP,
# define REG_EIP	REG_EIP
  REG_CS,
# define REG_CS		REG_CS
  REG_EFL,
# define REG_EFL	REG_EFL
  REG_UESP,
# define REG_UESP	REG_UESP
  REG_SS
# define REG_SS	REG_SS
};

struct _libc_fpreg
{
  unsigned short int significand[4];
  unsigned short int exponent;
};

struct _libc_fpstate
{
  unsigned long int cw;
  unsigned long int sw;
  unsigned long int tag;
  unsigned long int ipoff;
  unsigned long int cssel;
  unsigned long int dataoff;
  unsigned long int datasel;
  struct _libc_fpreg _st[8];
  unsigned long int status;
};

/* Structure to describe FPU registers.  */
typedef struct _libc_fpstate *fpregset_t;

/* Context to describe whole processor state.  */
typedef struct
  {
    gregset_t gregs;
    /* Note that fpregs is a pointer.  */
    fpregset_t fpregs;
    unsigned long __reserved1 [8];
} mcontext_t;

/* Userlevel context.  */
typedef struct ucontext
  {
    unsigned long int uc_flags;
    struct ucontext *uc_link;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
    __sigset_t uc_sigmask;
    struct _libc_fpstate __fpregs_mem;
  } ucontext_t;

#endif/*_NTOSKRNL_LUSER_SIGACTION_H*/
