#include <ddk/ntddk.h>
#include <ddk/ntifs.h>
#include <ndk/ketypes.h>
#include <ndk/mmtypes.h>
#include <luser/luser.h>
#include <luser/lunix.h>
#include <luser/unixmap.h>
#include <luser/lstorage.h>

char sigstack[32 * 1024];
BOOLEAN interruptsDisabled = FALSE;
BOOLEAN inTrap = FALSE;
int waitingInterrupt = -1;

#define ZF 0x40
#define IF 0x200
#define DF 0x400

void LuserDefaultHandlePageFault(siginfo_t *info, void *addr);
void (*LuserPageFaultHandler)(siginfo_t *info, void *addr) = 
    LuserDefaultHandlePageFault;

void setInterruptsEnabled(__sigset_t *mask, BOOLEAN intEna)
{
    interruptsDisabled = !intEna;
    if (interruptsDisabled)
        sigaddset(mask, SIGIO);
    else
        sigemptyset(mask);
}

#define MEMFD 100

BOOLEAN ReplacePage(ULONG_PTR Address, ULONG_PTR addr)
{
    HARDWARE_PTE SecondLevelPte = MapGetPTEWithRefresh(Address);

    if (SecondLevelPte.Valid)
    {
        ULONG_PTR mmap_res;
        Address = PAGE_ROUND_DOWN(Address);
        //Printf("Replacing page %x\n", Address);
        unix_msync((PVOID)Address, PAGE_SIZE, MS_SYNC);
        mmap_res = unix_mmap
            ((PVOID)Address, 
             PAGE_SIZE, 
             PROT_READ|PROT_WRITE|PROT_EXEC, 
             MAP_SHARED|MAP_FIXED, 
             MEMFD, 
             SecondLevelPte.PageFrameNumber << PAGE_SHIFT);
        if (mmap_res == (ULONG_PTR)-1)
        {
            Printf("Unix fault on address %x at %x\n", 
                   (unsigned int)Address,
                   (unsigned int)addr);
            unix_abort();
        }
        return TRUE;
    }
    return FALSE;
}

void MapDump();
void Suicide(ucontext_t *ucon)
{
    struct sigaction sa = { };
    sigemptyset(&ucon->uc_sigmask);
    sa.sa_sigaction = 0;
    MapDump();
    unix_sigaction(SIGSEGV, &sa, NULL);
    unix_setcontext(ucon);
}

void TakeTrap(int trap, ucontext_t *ucon, int skip);
void LuserDefaultHandlePageFault(siginfo_t *info, void *addr)
{
    ucontext_t *ucon = addr;
    ULONG_PTR Address = (ULONG_PTR)info->si_ptr;

    unix_msync((PVOID)PAGE_ROUND_DOWN(Address), PAGE_SIZE, MS_SYNC);

#if 0
    Printf("Page Fault at %x (addr %x)\n", 
           Address, ucon->uc_mcontext.gregs[REG_EIP]);
#endif

    if (ReplacePage(Address, ucon->uc_mcontext.gregs[REG_EIP]))
    {
        Printf("Returning after successful page replacement (flags %x)\n", ucon->uc_mcontext.gregs[REG_EFL]);
        unix_setcontext(ucon);
    }

#if 0
    HARDWARE_PTE First = MapGetFirstLevelPTE(Address), Second;
    if (!First.Valid)
    {
        Printf("PDE for %x not valid\n", Address);
    }
    else
    {
        Printf("PDE for %08x: %08x\n", Address, First);
        Second = MapGetSecondLevelPTE(First, Address);
        if (!Second.Valid)
            Printf("PTE for %08x not valid\n", Address);
        else
            Printf("PTE for %08x: %08x\n", Address, Second);
    }
#endif

    // Store the address of the instruction that took the trap
    __writecr2(Address);
    TakeTrap(14, ucon, 0);
}

int *PrepareTrapStack(ucontext_t *ucon, int skip, BOOLEAN errorCode, int ec)
{
    KTSS Tss;
    int *intStack;
    int newSs, newEsp, flags, umode, cs;

    // Fixup old CS value to indicate privelege level
    umode = !(ucon->uc_mcontext.gregs[REG_EIP] & 0x80000000);
    cs = umode ? KGDT_R3_CODE | RPL_MASK : KGDT_R0_CODE;
    //Printf("Taking trap from %s, cs %x\n", umode ? "umode" : "kmode", cs);
    
    // Push requisites for new stack
    if (umode)
    {
        if (!LuserGetTSS(&Tss))
        {
            Printf("Umode trap with no TSS\n");
            unix_abort();
        }
        
        newSs = Tss.Ss0;
        newEsp = Tss.Esp0;
        intStack = (int *)newEsp;

        *--intStack = ucon->uc_mcontext.gregs[REG_SS];
        *--intStack = ucon->uc_mcontext.gregs[REG_ESP];
    }
    else
	intStack = (int *)ucon->uc_mcontext.gregs[REG_ESP];

    flags = ucon->uc_mcontext.gregs[REG_EFL];
    if (interruptsDisabled) flags &= ~IF;
    *--intStack = flags;
    *--intStack = cs;
    *--intStack = ucon->uc_mcontext.gregs[REG_EIP] + skip;

    if (errorCode)
	*--intStack = ec;

    ucon->uc_mcontext.gregs[REG_ESP] = (int)intStack;
    return intStack;
}

void TakeTrap(int trap, ucontext_t *ucon, int skip)
{
    KIDTENTRY Entry;
    BOOLEAN UseErrorCode = FALSE;
    int ErrorCode = 0;

    LuserGetTrapEntry(trap, &Entry);
    
    switch (trap)
    {
    case 13:
        // Put an error code here ... figure out what goes
        UseErrorCode = TRUE;
        break;

    case 14:
        // Access fault uses at least bit 0 for read/write
        UseErrorCode = TRUE;
        break;

    default:
        //Printf("trap %x (from %x)\n", trap, ucon->uc_mcontext.gregs[REG_EIP]);
        break;
    }

    PrepareTrapStack(ucon, skip, UseErrorCode, ErrorCode);

    // Return to the int handler
    ucon->uc_mcontext.gregs[REG_EIP] = 
        (Entry.ExtendedOffset << 16) | Entry.Offset;

    setInterruptsEnabled(&ucon->uc_sigmask, FALSE);

    // Printf("INT#%d Target %x\n", trap, ucon->uc_mcontext.gregs[REG_EIP]);

    inTrap = FALSE;
    unix_setcontext(ucon);
}

void TakeIret(ucontext_t *ucon, int *stackPtr)
{
    int cs, flags, umode;

    ucon->uc_mcontext.gregs[REG_EIP] = *stackPtr++;
    umode = !(ucon->uc_mcontext.gregs[REG_EIP] & 0x80000000);
    cs = *stackPtr++;
    flags = *stackPtr++;
    ucon->uc_mcontext.gregs[REG_EFL] = flags | IF;

    setInterruptsEnabled(&ucon->uc_sigmask, !!(flags & IF));

    if (umode)
    {
        ucon->uc_mcontext.gregs[REG_ESP] = *stackPtr++;
        ucon->uc_mcontext.gregs[REG_SS] = *stackPtr++;

#if 0
        Printf("Iret: New EIP %x New ESP %x (Return to %cMode)\n", 
               ucon->uc_mcontext.gregs[REG_EIP],
               ucon->uc_mcontext.gregs[REG_ESP],
               umode ? 'U' : 'K');
#endif
    }
    else
        ucon->uc_mcontext.gregs[REG_ESP] = (int)stackPtr;
}

int SegTable[8] = {
    REG_ES,
    REG_CS,
    REG_SS,
    REG_DS,
    REG_FS,
    REG_GS,
    -1,
    -1
};
int RegTable[8] = {
    REG_EAX, // 0
    REG_ECX, // 1
    REG_EDX, // 2
    REG_EBX, // 3
    REG_ESP, // 4
    REG_EBP, // 5
    REG_ESI, // 6
    REG_EDI  // 7
};
char *RegNames[8] = {
    "%eax",
    "%ecx",
    "%edx",
    "%ebx",
    "%esp",
    "%ebp",
    "%esi",
    "%edi"
};

#define REPCOND_Z 1
#define REPCOND_NZ 2
#define REPCOND_ECX 4
void HandleSpecialInst(siginfo_t *info, void *addr)
{
    int longarg = 1;
    void *mref;
    int repcond = 0;
    int seg_prefix = 0, cdr, reg, cr;
    int page_replace = 0;
    ucontext_t *ucon = addr;
    unsigned char *instByte, *repStart = NULL;
    int repEip = 0;
    __sigset_t unblock, oldmask;

    instByte = (unsigned char *)ucon->uc_mcontext.gregs[REG_EIP];
    
start:
    switch(*instByte)
    {
    case 0x07: // pop %es
        ucon->uc_mcontext.gregs[REG_ESP] += 4;
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0x0f: // Prefix byte
        switch (instByte[1])
        {
        case 0x01:
            switch (instByte[2])
            {
            case 0x15: // lgdt imm32
                memcpy(&mref, instByte + 3, sizeof(void *));
#if 0
                Printf("ptr bytes %x %x %x %x mref %x\n",
                       instByte[6],
                       instByte[5],
                       instByte[4],
                       instByte[3],
                       mref);
#endif
                LuserSetGlobalDescriptorTable(mref);
                ucon->uc_mcontext.gregs[REG_EIP] += 7;
                break;

            case 0x1d: // lidt imm32
                memcpy(&mref, instByte + 3, sizeof(void *));
#if 0
                Printf("ptr bytes %x %x %x %x mref %x\n",
                       instByte[6],
                       instByte[5],
                       instByte[4],
                       instByte[3],
                       mref);
#endif
                LuserSetInterruptDescriptorTable(mref);
                ucon->uc_mcontext.gregs[REG_EIP] += 7;
                break;


            case 0x3d: // invlpg imm
                memcpy(&page_replace, (PVOID)(ucon->uc_mcontext.gregs[REG_EIP]+3), sizeof(PVOID));
                LuserInvalidatePage((PVOID)page_replace);
                ucon->uc_mcontext.gregs[REG_EIP] += 7;
                break;

            case 0x3e: // invlpg %esi
                LuserInvalidatePage((PVOID)ucon->uc_mcontext.gregs[REG_ESI]);
                ucon->uc_mcontext.gregs[REG_EIP] += 3;
                break;

            case 0x3f: // invlpg %edi
                LuserInvalidatePage((PVOID)ucon->uc_mcontext.gregs[REG_EDI]);
                ucon->uc_mcontext.gregs[REG_EIP] += 3;
                break;

            default:
                Printf("Got unknown 0f 01 %x\n", instByte[2]);
                unix_abort();
                break;
            }
            break;
            
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
            if ((instByte[2] & 0xc0) == 0xc0)
            {
                cr = !(instByte[1] & 1);
                cdr = (instByte[2] >> 3) & 7;
                reg = instByte[2] & 7;
                if (!(instByte[1] & 2))
                {
#if 0
                    Printf("0f %x %x -> mov %%%s%d,%%%s\n",
                           instByte[1], instByte[2],
                           cr ? "cr" : "db",
                           cdr,
                           RegNames[reg]);
#endif
                    ucon->uc_mcontext.gregs[RegTable[reg]] = 
                        cr ? LuserReadCR(cdr) : LuserReadDR(cdr);
                }
                else
                {
#if 0
                    Printf("0f %x %x -> mov %%%s,%%%s%d\n",
                           instByte[1], instByte[2],
                           RegNames[reg],
                           cr ? "cr" : "db",
                           cdr);
#endif
                    (cr ? LuserWriteCR : LuserWriteDR)
                        (cdr, ucon->uc_mcontext.gregs[RegTable[reg]]);
                }
                ucon->uc_mcontext.gregs[REG_EIP] += 3;
            }
            else
            {
                Printf("Got unknown 0f %x %x\n", instByte[1], instByte[2]);
                unix_abort();
            }
            break;

        case 0x2e: // cs prefix
            seg_prefix = REG_CS;
            ucon->uc_mcontext.gregs[REG_EIP]++;
            instByte++;
            goto start;

        case 0xa1: // pop %fs
            ucon->uc_mcontext.gregs[REG_FS] = *((int *)ucon->uc_mcontext.gregs[REG_ESP]);
            ucon->uc_mcontext.gregs[REG_ESP] += longarg ? 4 : 2;
            ucon->uc_mcontext.gregs[REG_EIP] += 2;
            break;

        case 0xa9: // pop %gs
            //LuserSetGS(*((int*)ucon->uc_mcontext.gregs[REG_ESP]));
            ucon->uc_mcontext.gregs[REG_ESP] += longarg ? 4 : 2;
            ucon->uc_mcontext.gregs[REG_EIP] += 2;
            break;

        default:
            Printf("Got unknown 0f %x\n", instByte[1]);
            unix_abort();
            break;
        }
        break;

    case 0x1f: // pos %ds
        ucon->uc_mcontext.gregs[REG_ESP] += 4;
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0x64: // fs:
        seg_prefix = REG_FS;
        ucon->uc_mcontext.gregs[REG_EIP]++;
        instByte++;
        //Printf("fs: prefix\n");
        goto start;

    case 0x66: // 16 bit
        longarg = 0;
        ucon->uc_mcontext.gregs[REG_EIP]++;
        instByte++;
        //Printf("16/32 prefix\n");
        goto start;

    case 0x6d: // ins dx,m16
    {
        int posdir = (ucon->uc_mcontext.gregs[REG_EFL] & DF) ? -1 : 1;
        posdir *= longarg ? 4 : 2;
        if (page_replace != (ucon->uc_mcontext.gregs[REG_EDI] & ~0xfff))
        {
            page_replace = ucon->uc_mcontext.gregs[REG_EDI] & ~0xfff;
            ReplacePage
                (page_replace,
                 ucon->uc_mcontext.gregs[REG_EIP]);
        }
        if (longarg)
            *((ULONG *)ucon->uc_mcontext.gregs[REG_EDI]) =
                LuserInDWord(ucon->uc_mcontext.gregs[REG_EDX] & 0xffff);
        else
            *((USHORT *)ucon->uc_mcontext.gregs[REG_EDI]) =
                LuserInWord(ucon->uc_mcontext.gregs[REG_EDX] & 0xffff);
        ucon->uc_mcontext.gregs[REG_EDI] += posdir;
        ucon->uc_mcontext.gregs[REG_EIP]++;        
    }
        break;

    case 0x6e: // outs dx,m8
    {
        int posdir = (ucon->uc_mcontext.gregs[REG_EFL] & DF) ? -1 : 1;
        LuserOutByte
            (ucon->uc_mcontext.gregs[REG_EDX] & 0xffff, 
             *((char *)ucon->uc_mcontext.gregs[REG_ESI]));
        ucon->uc_mcontext.gregs[REG_ESI] += posdir;
        ucon->uc_mcontext.gregs[REG_EIP]++;
    }
        break;

    case 0x6f: // outs dx,m16
    {
        int posdir = (ucon->uc_mcontext.gregs[REG_EFL & DF]) ? -1 : 1;
        posdir *= longarg ? 4 : 2;
        (longarg ? LuserOutDWord : LuserOutWord)
            (ucon->uc_mcontext.gregs[REG_EDX] & 0xffff,
             *((ULONG *)ucon->uc_mcontext.gregs[REG_ESI]));
        ucon->uc_mcontext.gregs[REG_ESI] += posdir;
        ucon->uc_mcontext.gregs[REG_EIP]++;
    }
        break;

    case 0x8e:
        if ((instByte[1] & 0xc0) == 0xc0)
        {
            seg_prefix = SegTable[(instByte[1] >> 3) & 7];
            reg = RegTable[instByte[1] & 7];
            if (seg_prefix == -1)
                unix_abort();
            ucon->uc_mcontext.gregs[seg_prefix] = 
                (ucon->uc_mcontext.gregs[reg] & 0xffff) | 7;
            ucon->uc_mcontext.gregs[REG_EIP] += 2;
            break;
        }
        else
        {
            Printf("Got unknown 8e %x\n", instByte[1]);
            unix_abort();
            break;
        }
        break;

    case 0xcc: // int3
        Printf("Breakpoint instruction at %x\n", instByte);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0xcd: // int imm8
        TakeTrap(instByte[1], ucon, 2);
        break;

    case 0xcf: // iret
        TakeIret(ucon, (int *)ucon->uc_mcontext.gregs[REG_ESP]);
        break;

    case 0xe4: // in imm8,al
        ucon->uc_mcontext.gregs[REG_EAX] =
            (ucon->uc_mcontext.gregs[REG_EAX] & ~0xff) | LuserInByte(instByte[1]);
        ucon->uc_mcontext.gregs[REG_EIP] += 2;
        break;

    case 0xe6: // out imm8,al
        LuserOutByte(instByte[1], ucon->uc_mcontext.gregs[REG_EAX] & 0xff);
        ucon->uc_mcontext.gregs[REG_EIP]+=2;
        break;

    case 0xea: // jmp far seg:addr
        memcpy(&ucon->uc_mcontext.gregs[REG_EIP], instByte + 1, sizeof(int));
        break;

    case 0xec: // in dx,al
        ucon->uc_mcontext.gregs[REG_EAX] = 
            (ucon->uc_mcontext.gregs[REG_EAX] & ~0xff) | 
            LuserInByte(ucon->uc_mcontext.gregs[REG_EDX] & 0xffff);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0xed: // in dx,(e)ax
        if (longarg)
            ucon->uc_mcontext.gregs[REG_EAX] = 
                LuserInDWord(ucon->uc_mcontext.gregs[REG_EDX]);
        else
            ucon->uc_mcontext.gregs[REG_EAX] = 
                (ucon->uc_mcontext.gregs[REG_EAX] & ~0xffff) |
                (LuserInWord(ucon->uc_mcontext.gregs[REG_EDX]) & 0xffff);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;        

    case 0xee: // out dx,al
        LuserOutByte
            (ucon->uc_mcontext.gregs[REG_EDX] & 0xffff, 
             ucon->uc_mcontext.gregs[REG_EAX] & 0xff);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0xef: // out dx,(e)ax
        if (longarg)
            LuserOutDWord
                (ucon->uc_mcontext.gregs[REG_EDX] & 0xffff,
                 ucon->uc_mcontext.gregs[REG_EAX]);
        else
            LuserOutWord
                (ucon->uc_mcontext.gregs[REG_EDX] & 0xffff,
                 ucon->uc_mcontext.gregs[REG_EAX] & 0xffff);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0xf3: // rep
        repStart = ++instByte;
        repcond = REPCOND_ECX;
        repEip = ucon->uc_mcontext.gregs[REG_EIP] + 1;
        break;

    case 0xf4: // hlt
        if (interruptsDisabled)
        {
            Printf("HLT with interrupts disabled at %x!\n", 
                   ucon->uc_mcontext.gregs[REG_EIP]);
            unix_abort();
        }
        ucon->uc_mcontext.gregs[REG_EIP]++;
        if ((waitingInterrupt = LuserReadInterrupt()) == -1)
        {
            // The only place we do something like this ... we want SIGIO to
            // break the sleep
            //Printf("HLT: no interrupts pending\n");
            sigemptyset(&unblock);
            sigaddset(&unblock, SIGIO);
            unix_sigprocmask(SIG_UNBLOCK, &unblock, &oldmask);
            LuserHaltProcessor();
            // We won't return if we got SIGIO
            unix_sigprocmask(SIG_SETMASK, &oldmask, NULL);
        }
        if (waitingInterrupt != -1)
        {
            int inter = waitingInterrupt;
            //Printf("Caught an interrupt (%x)\n", inter);
            waitingInterrupt = -1;
            TakeTrap(inter, ucon, 0);
        }
        break;

    case 0xfa: // cli 
        setInterruptsEnabled(&ucon->uc_sigmask, FALSE);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    case 0xfb: // sti
        setInterruptsEnabled(&ucon->uc_sigmask, TRUE);
        ucon->uc_mcontext.gregs[REG_EIP]++;
        break;

    default:
        Printf("Instruction causing the exception: %x at %x\n", *instByte, instByte);
        unix_abort();
        break;
    }

    if ((repcond & REPCOND_ECX) && (ucon->uc_mcontext.gregs[REG_ECX]--))
    {
        instByte = repStart;
        ucon->uc_mcontext.gregs[REG_EIP] = repEip;
        goto start;
    }

    inTrap = FALSE;
    unix_setcontext(ucon);
}

void HandleSigio(int signal, siginfo_t *info, void *addr)
{
    ucontext_t *ucon = addr;
    int interrupt;

    // Don't do anything if we just filled the interrupt slot
    if (inTrap && waitingInterrupt != -1) return;

    interrupt = LuserReadInterrupt();
    if (interrupt == -1) 
    {
        //Printf("Failed to read an interrupt!\n");
        return;
    }

    if (!inTrap)
    {
        //Printf("SIGIO: Taking trap %x\n", interrupt);
        TakeTrap(interrupt, ucon, 0);
    }
    else
        waitingInterrupt = interrupt;
}

void HandleSegv(int signal, siginfo_t *info, void *addr)
{
    ucontext_t *ucon = addr;

    inTrap = TRUE;

    switch (signal)
    {
    case SIGTRAP:
        Printf("SIGTRAP %x\n", ucon->uc_mcontext.gregs[REG_EIP]);
        //ucon->uc_mcontext.gregs[REG_EIP]++;
        unix_setcontext(ucon);
        break;

    case SIGPIPE:
        break;

    case SIGBUS:
        switch (info->si_code)
        {
        case 2:
            LuserPageFaultHandler(info, addr);
            break;
        }
        break;

    case SIGSEGV:
        switch (info->si_code)
        {
        case 1:
            LuserPageFaultHandler(info, addr);
            break;
            
        case 0x80:
            HandleSpecialInst(info, addr);
            break;
        }
        break;

    default:
        Printf("Unhandled signal %d (si_code %x, addr %x)\n", signal, info->si_code, addr);
        break;
    }
    inTrap = FALSE;
}

void LuserRegisterSigstack()
{
    stack_t newstack = { };

    newstack.ss_sp = sigstack;
    newstack.ss_flags = 0;
    newstack.ss_size = sizeof(sigstack);

    if (unix_sigaltstack(&newstack, NULL) == -1)
        unix_abort();

    memset(sigstack, 0, sizeof(sigstack));
}

void LuserRegisterSegv()
{
    /* We must do everything on the stack here, cause it's the only
     * memory we can trust... */
    struct sigaction newsa = { };

    newsa.sa_flags = SA_ONSTACK | SA_SIGINFO;
    newsa.sa_sigaction = HandleSegv;
    sigemptyset(&newsa.sa_mask);
    sigaddset(&newsa.sa_mask, SIGIO);
    sigaddset(&newsa.sa_mask, SIGUSR1);

    if (unix_sigaction(SIGSEGV, &newsa, NULL) == -1)
        unix_abort();
    if (unix_sigaction(SIGPIPE, &newsa, NULL) == -1)
        unix_abort();
    if (unix_sigaction(SIGTRAP, &newsa, NULL) == -1)
        unix_abort();
    if (unix_sigaction(SIGBUS,  &newsa, NULL) == -1)
        unix_abort();
#if 0
    if (unix_sigaction(SIGILL,  &newsa, NULL) == -1)
        unix_abort();
#endif

    /* Interrupt */
    newsa.sa_sigaction = HandleSigio;
    if (unix_sigaction(SIGIO, &newsa, NULL) == -1)
        unix_abort();
}
