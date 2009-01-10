#ifndef _NTOSKRNL_LUSER_UNIX_H
#define _NTOSKRNL_LUSER_UNIX_H

#include <stdarg.h>

#ifndef _PID_T_
#define _PID_T_
typedef int pid_t;
#endif

#ifndef _CLOCK_T_DEFINED
#define _CLOCK_T_DEFINED
typedef long clock_t;
#endif

#ifndef TIMESPEC_DEFINED
#define TIMESPEC_DEFINED
struct timespec {
    time_t tv_sec;
    long tv_nsec;
};
#endif

#ifndef _OFF_T_
#define _OFF_T_
typedef long off_t;
#endif

#include "sigaction.h"

#undef O_RDONLY
#define O_RDONLY 0
#undef O_RDWR
#define O_RDWR 2

#undef SIGILL
#define SIGILL 4
#undef SIGTRAP
#define SIGTRAP 5
#undef SIGABRT
#define SIGABRT 6
#undef SIGBUS
#define SIGBUS 7
#undef SIGUSR1
#define SIGUSR1 10
#undef SIGSEGV
#define SIGSEGV 11
#undef SIGPIPE
#define SIGPIPE 13
#undef SIGALRM
#define SIGALRM 14
#undef SIGIO
#define SIGIO 29

#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4

#define MAP_SHARED 1
#define MAP_FIXED 0x10

#define MS_SYNC 4

#undef errno
extern int errno;

struct pt_regs {
    long ebx;
    long ecx;
    long edx;
    long esi;
    long edi;
    long ebp;
    long eax;
    int  xds;
    int  xes;
    int  xfs;
    /* int  xgs; */
    long orig_eax;
    long eip;
    int  xcs;
    long eflags;
    long esp;
    int  xss;
};

struct modify_ldt_ldt_s {
        unsigned int  entry_number;
        unsigned long base_addr;
        unsigned int  limit;
        unsigned int  seg_32bit:1;
        unsigned int  contents:2;
        unsigned int  read_exec_only:1;
        unsigned int  limit_in_pages:1;
        unsigned int  seg_not_present:1;
        unsigned int  useable:1;
};

void unix_exit(int code);
int unix_read(int fd, char *buf, int len);
int unix_write(int fd, char *buf, int len);
int unix_open(char *name, int flags, int mode);
int unix_close(int fd);
int unix_lseek(int fd, int target, int origin);
int unix_getpid();
int unix_kill(int pid, int signal);
void unix_abort();
int unix_sigaction(int sig, struct sigaction *oldsig, struct sigaction *newsig);
int unix_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off);
int unix_munmap(void *addr, size_t len);
int unix_read_ldt(void *ptr, unsigned long count);
int unix_write_ldt(struct modify_ldt_ldt_s *ldtmod, unsigned long count);
int unix_mprotect(void *addr, size_t len, int prot);
int unix_sigaltstack(const stack_t *newstack, stack_t *oldstack);
int unix_sigprocmask(int how, const __sigset_t *set, __sigset_t *oldset);
int unix_msync(void *addr, size_t len, int flags);
int unix_getcontext(ucontext_t *context);
void unix_setcontext(const ucontext_t *context);
void unix_nanosleep(struct timespec *ts, struct timespec *ret);

#endif/*_NTOSKRNL_LUSER_UNIX_H*/
