#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ndk/ntndk.h>
#include <luser.h>
#include <lunix.h>
#include <luserint.h>

int syscall(int nr, ...);

#define __NR_exit 1
void unix_exit(int code)
{
    syscall(__NR_exit, code);
}

#define __NR_fork 2
int unix_fork()
{
    return syscall(__NR_fork);
}

#define __NR_read 3
int unix_read(int fd, char *buf, int len)
{
    return syscall(__NR_read, fd, buf, len);
}

#define __NR_write 4
int unix_write(int fd, char *buf, int len)
{
    return syscall(__NR_write, fd, buf, len);
}

#define __NR_open 5
int unix_open(char *file, int omode, int perm)
{
    return syscall(__NR_open, file, omode, perm);
}

#define __NR_close 6
int unix_close(int fd)
{
    return syscall(__NR_close, fd);
}

#define __NR_execve 11
int unix_execve(const char *path, const char **argv, const char **envp)
{
    return syscall(__NR_execve, path, argv, envp);
}

#define __NR_lseek 19
int unix_lseek(int fd, int target, int origin)
{
    return syscall(__NR_lseek, fd, target, origin);
}

#define __NR_getpid 20
int unix_getpid()
{
    return syscall(__NR_getpid);
}

#define __NR_kill 37
int unix_kill(int pid, int signal)
{
    return syscall(__NR_kill, pid, signal);
}

void unix_abort()
{
    Printf("abort() called from %x\n", __builtin_return_address(0));
    unix_kill(unix_getpid(), SIGABRT);
}

void _memcpy(void *dst, void *src, int len)
{
    char *dst_ = dst, *src_ = src, *end = dst_ + len;
    while(dst_ < end)
        *dst_++ = *src_++;
}

extern void _restore();
#define __NR_sigaction 67
int unix_sigaction(int sig, struct sigaction *newsig, struct sigaction *oldsig)
{
    struct sigaction kact;
    if (newsig)
    {
        _memcpy(&kact, newsig, sizeof(kact));
        /*kact.sa_flags |= SA_RESTORER;*/
        /*kact.sa_restorer = _restore;*/
        newsig = &kact;
    }
    return syscall
        (__NR_sigaction, sig, newsig, oldsig);
}

#define __NR_mmap 90
int unix_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t off)
{
    struct { void *addr; size_t len; int prot, flags, fd; off_t off; }
    mmap_args = { addr, len, prot, flags, fd, off };
    return syscall(__NR_mmap, &mmap_args);
}

#define __NR_munmap 91
int unix_munmap(void *addr, size_t len)
{
    return syscall(__NR_munmap, addr, len);
}

#define __NR_sigreturn 119
__asm__(
    ".text\n"
    "  .align 8\n"
    "  .globl __restore\n"
    "__restore:\n"
    "  popl %eax\n"
    "  movl $173, %eax\n"
    "  int $0x80\n");

#define __NR_modify_ldt 123
int unix_read_ldt(void *ptr, unsigned long count)
{
    return syscall(__NR_modify_ldt, 0, ptr, count);
}

int unix_write_ldt(struct modify_ldt_ldt_s *ptr, unsigned long count)
{
    return syscall(__NR_modify_ldt, 1, ptr, count);
}

#define __NR_mprotect 125
int unix_mprotect(void *addr, size_t len, int prot)
{
    return syscall(__NR_mprotect, addr, len, prot);
}

#define __NR_sigprocmask 126
int unix_sigprocmask(int how, const __sigset_t *set, __sigset_t *oldset)
{
    return syscall(__NR_sigprocmask, how, set, oldset);
}

#define __NR_msync 144
int unix_msync(void *addr, size_t len, int flags)
{
    return syscall(__NR_msync, addr, len, flags);
}

#define __NR_nanosleep 162
void unix_nanosleep(struct timespec *ts, struct timespec *ret)
{
    syscall(__NR_nanosleep, ts, ret);
}

#define __NR_sigaltstack 186
int unix_sigaltstack(const stack_t *newstack, stack_t *oldstack)
{
    return syscall(__NR_sigaltstack, newstack, oldstack);
}

void Printf(const char *fmt, ...)
{
    int len;
    va_list args;
    char buf[128];
    va_start(args, fmt);
    len = _vsprintf(buf, fmt, args);
    va_end(args);
    unix_write(1, buf, len);
}
