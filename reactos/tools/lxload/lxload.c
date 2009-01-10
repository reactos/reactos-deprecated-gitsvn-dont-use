#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <SDL/SDL.h>
#include <libs/luser/lstorage.h>
#include "luserhw.h"

int child_proc;
int dev_read_pipe[2], dev_write_pipe[2], int_pipe[2];

struct RGBTriplet { char b,g,r; } *char_surf;
struct RGBTriplet color[16] = {
    { 0x00, 0x00, 0x00 },
    { 0x80, 0x00, 0x00 },
    { 0x00, 0x80, 0x00 },
    { 0x80, 0x80, 0x00 },
    { 0x00, 0x00, 0x80 },
    { 0x80, 0x00, 0x80 },
    { 0x00, 0x80, 0x80 },
    { 0x80, 0x80, 0x80 },
    { 0x40, 0x40, 0x40 },
    { 0xff, 0x40, 0x40 },
    { 0x40, 0xff, 0x40 },
    { 0xff, 0xff, 0x40 },
    { 0x40, 0x40, 0xff },
    { 0xff, 0x40, 0xff },
    { 0x40, 0xff, 0xff },
    { 0xff, 0xff, 0xff }
};

typedef void (*start_function)();

struct interrupt_t *int_head = NULL;

int timerExp(int);
int childDied(int);
void devLoop();

void callFreeldr(struct multiboot_t *mbthdr, struct multiboot_response_t *resp)
{
    fprintf(stderr, "multiboot_info %p\n", resp);
    __asm__
        ("mov %1,%%ecx\n\t"
         "mov %0,%%ebx\n\t"
         "mov $0x2badb002,%%eax\n\t"
         "call *%%ecx\n\t"
         : : "r" (resp), "r" (mbthdr->entry_addr));
}

int main(int argc, char **argv)
{
    int i, disk_fd, mem_fd, cpu_fd;
    int use_x = 0;
    int mem_size = 128 * 1024 * 1024, vid_size = 640 * 480 * 3;
    const char *freeldr_path = NULL;
    const char *disk_path = NULL;
    const char *cmdline = "";
    char *freeldr_data;
    char mem_path[FOPEN_MAX] = "/tmp/memXXXXXX";
    char cpu_path[FOPEN_MAX] = "/tmp/cpuXXXXXX";
    struct stat statbuf;
    void *vidsurf;
    SDL_Surface *video, *char_temp;
    FILE *freeldr;
    struct sigaction sa;
    sigset_t unmask;
    struct multiboot_t *freeldr_hdr, hdr;
    struct multiboot_response_t response = { };
    struct LuserCPUData cpu = { };

    for (i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))
            cmdline = argv[++i];
        else if (!strcmp(argv[i], "-d"))
            disk_path = argv[++i];
        else if (!strcmp(argv[i], "-f"))
            freeldr_path = argv[++i];
        else if (!strcmp(argv[i], "-m"))
            mem_size = atoi(argv[++i]);
        else if (!strcmp(argv[i], "-x"))
            use_x = 1;
    }

    // We share the mem file
    mem_fd = mkstemp(mem_path);

    if (ftruncate(mem_fd, mem_size + vid_size) == -1)
    {
        perror("truncate");
        exit(1);
    }
    if ((mem_fd != MEMFD) && (dup2(mem_fd,MEMFD) == -1))
    {
        perror("memfd");
        exit(1);
    }
    unlink(mem_path);

    if ((cpu_fd = mkstemp(cpu_path)) == -1)
    {
        perror(cpu_path);
        exit(1);
    }
    if ((cpu_fd != CPUFD) && (dup2(cpu_fd,CPUFD) == -1))
    {
        perror("cpufd");
        exit(1);
    }
    unlink(cpu_path);

    if (mem_fd != MEMFD) close(mem_fd);
    if (cpu_fd != CPUFD) close(cpu_fd);

    if ((disk_fd = open(disk_path, O_RDWR)) == -1)
    {
        perror(disk_path);
        exit(1);
    }

    ideSetup(&ide_data1, disk_fd);

    if (pipe(dev_read_pipe) == -1)
    {
        perror("read_pipe");
        exit(1);
    }

    if (pipe(dev_write_pipe) == -1)
    {
        perror("write_pipe");
        exit(1);
    }

    if (pipe(int_pipe) == -1)
    {
        perror("int_pipe\n");
        exit(1);
    }

    // pull in freeldr
    if (stat(freeldr_path, &statbuf) == -1)
    {
        perror(freeldr_path);
        exit(1);
    }

    freeldr = fopen(freeldr_path, "rb");
    
    if (!freeldr)
    {
        perror(freeldr_path);
        exit(1);
    }
    
    freeldr_data = malloc(statbuf.st_size);
    if (!freeldr_data)
    {
        fprintf(stderr, "Could not allocate %d bytes for freeldr\n",
                (int)statbuf.st_size);
        exit(1);
    }

    if (fread(freeldr_data, 1, statbuf.st_size, freeldr) != statbuf.st_size)
    {
        perror("fread");
        exit(1);
    }
    
    fclose(freeldr);
    
    for (i = 0, freeldr_hdr = (struct multiboot_t *)(freeldr_data + i); 
         i < statbuf.st_size - sizeof(*freeldr_hdr) && 
             freeldr_hdr->magic != MBT_MAGIC; 
         i += sizeof(uint32_t), freeldr_hdr = 
             (struct multiboot_t *)(freeldr_data + i));

    if (freeldr_hdr->magic != MBT_MAGIC)
    {
        fprintf(stderr, "Could not find multiboot magic in freeldr\n");
        exit(1);
    }

    hdr = *freeldr_hdr;

    if (lseek(MEMFD, hdr.load_addr, 0) == -1)
    {
        fprintf(stderr, "Could not lseek to %x in mem\n", hdr.load_addr);
        exit(1);
    }
    
    if (write(MEMFD, freeldr_data, statbuf.st_size) != statbuf.st_size)
    {
        fprintf(stderr, "Could not write freeldr into mem\n");
        exit(1);
    }
    
    free(freeldr_data);

    if ((child_proc = fork()))
    {
        // parent
        close(dev_write_pipe[1]);
        close(dev_read_pipe[0]);
        close(int_pipe[0]);

        sigemptyset(&sa.sa_mask);
        sigaddset(&sa.sa_mask, SIGALRM);
        sigaddset(&sa.sa_mask, SIGIO);
        sa.sa_sigaction = (void *)timerExp;
        sa.sa_flags = SA_RESTART;
        sigaction(SIGALRM, &sa, NULL);

        sa.sa_flags = 0;
        sa.sa_sigaction = (void *)childDied;
        sigaction(SIGCHLD, &sa, NULL);

        // if X, create the display surface
        if (use_x)
        {
            if ((vidsurf = mmap
                 (NULL, 
                  640 * 480 * 3, 
                  PROT_READ | PROT_WRITE, 
                  MAP_SHARED,
                  MEMFD,
                  mem_size + 0x1000)) == MAP_FAILED)
            {
                fprintf(stderr, "Could not map video on IO side\n");
                exit(1);
            }
            
            SDL_Init(SDL_INIT_VIDEO);
            SDL_EnableUNICODE(1);
            video = SDL_SetVideoMode
                (640, 480, 24, SDL_SWSURFACE | SDL_DOUBLEBUF);
            if (!video) 
            {
                fprintf(stderr, "use_x but can't open SDL\n");
                exit(1);
            }
            char_temp = SDL_CreateRGBSurfaceFrom
                (vidsurf, 640, 480, 24, 640 * 3, 0xff0000, 0xff00, 0xff, 0);
            char_surf = vidsurf;
        }

        devLoop();
    }
    else
    {
        cpu.MemSize = mem_size;
        cpu.IntPipeRead = int_pipe[0];
        cpu.DevPipeRead = dev_read_pipe[0];
        cpu.DevPipeWrite = dev_write_pipe[1];

        if (write(CPUFD, &cpu, sizeof(cpu)) == -1)
        {
            perror("cpu_write");
            exit(1);
        }

        if (mmap
            ((void *)0x1000, 
             mem_size - 0x1000, 
             PROT_READ | PROT_WRITE | PROT_EXEC,
             MAP_SHARED | MAP_FIXED,
             MEMFD,
             0x1000) == MAP_FAILED)
        {
            fprintf(stderr, "Could not map mem into target space\n");
            exit(1);
        }

        close(dev_write_pipe[0]);
        close(dev_read_pipe[1]);
        close(int_pipe[1]);

        fcntl(int_pipe[0], F_SETOWN, getpid());
        fcntl(int_pipe[0], F_SETFL, O_ASYNC|O_NONBLOCK|O_RDONLY);

        sigemptyset(&unmask);
        sigaddset(&unmask, SIGIO);
        sigprocmask(SIG_SETMASK, &unmask, NULL);

        response.mem_lower = 0;
        response.mem_upper = mem_size;
        response.flags |= MBT_FLG_MEM;

        response.cmdline = (uint32_t)cmdline;
        response.flags |= MBT_FLG_CMD;

        response.boot_device = 0x8000 << 16;
        response.flags |= MBT_FLG_BOOT;

        callFreeldr(&hdr, &response);
    }

    return 0;
}
