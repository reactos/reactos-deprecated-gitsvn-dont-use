/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/gdb_input.c
 * PURPOSE:         Base functions for the kernel debugger.
 */

#include "kdgdb.h"

/* LOCALS *********************************************************************/
/* Keep track of where we are for qfThreadInfo/qsThreadInfo */
static LIST_ENTRY* CurrentProcessEntry;
static LIST_ENTRY* CurrentThreadEntry;

/* GLOBALS ********************************************************************/
BOOLEAN in_stop_mode;
ptid_t current_ptid;

/* PRIVATE FUNCTIONS **********************************************************/
static
ULONG64
hex_to_address(char* buffer)
{
    ULONG64 ret = 0;
    char hex;
    while (*buffer)
    {
        hex = hex_value(*buffer++);
        if (hex < 0)
            return ret;
        ret <<= 4;
        ret += hex;
    }
    return ret;
}

/* H* packets */
static
void
handle_gdb_set_thread(void)
{
    PETHREAD NextThread;

    if(gdb_input[1] != 'g') {
        KDDBGPRINT("Unknown 'H' command: %s\n", gdb_input);
        send_gdb_packet("E");
        return;
    }

    NextThread = find_thread(parse_ptid(&gdb_input[2]));
    if(!NextThread) {
        KDDBGPRINT("No such thread %s\n", gdb_input);
        send_gdb_packet("E");
        return;
    }

    current_ptid = ptid_from_thread(NextThread);
    KDDBGPRINT("Setting debug thread: %s.\n", gdb_input);
    send_gdb_packet("OK");
}

static
void
handle_gdb_thread_alive(void)
{
    ptid_t ptid;
    PETHREAD Thread;

    ptid = parse_ptid(&gdb_input[1]);

    /* We cannot use PsLookupProcessThreadByCid as we could be running at any IRQL.
     * So loop. */
    KDDBGPRINT("Checking if %s is alive.\n", format_ptid(ptid));

    Thread = find_thread(ptid);

    if (Thread != NULL)
        send_gdb_packet("OK");
    else
        send_gdb_packet("E03");
}

/* q* packets */
static
VOID
handle_gdb_query(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    if (strncmp(gdb_input, "qSupported:", 11) == 0)
    {
        send_gdb_packet("PacketSize=4096;multiprocess+;qGetTIBAddr+;");
        return;
    }

    if (strncmp(gdb_input, "qAttached", 9) == 0)
    {
        /* Say no: We didn't attach, we create the process! */
        send_gdb_packet("0");
        return;
    }

    if (strncmp(gdb_input, "qRcmd,", 6) == 0)
    {
        send_gdb_packet("OK");
        return;
    }

    if (strcmp(gdb_input, "qC") == 0)
    {
        send_gdb_packet("QC:%s;", format_ptid(current_ptid));
        return;
    }

    if ((strncmp(gdb_input, "qfThreadInfo", 12) == 0)
            || (strncmp(gdb_input, "qsThreadInfo", 12) == 0))
    {
        BOOLEAN FirstThread = TRUE;
        PEPROCESS Process;
        PETHREAD Thread;
        char gdb_out[1024];
        char* ptr = gdb_out;
        BOOLEAN Resuming = strncmp(gdb_input, "qsThreadInfo", 12) == 0;

        if (Resuming)
        {
            if (CurrentProcessEntry == (LIST_ENTRY*)1)
            {
                /* We're done */
                send_gdb_packet("l");
                CurrentProcessEntry = NULL;
                return;
            }

            if (CurrentThreadEntry == NULL)
                CurrentProcessEntry = CurrentProcessEntry->Flink;
        }
        else
            CurrentProcessEntry = ProcessListHead->Flink;

        if ((CurrentProcessEntry == ProcessListHead) ||
                (CurrentProcessEntry == NULL)) /* Ps is not initialized */
        {
            /* We're almost done. Tell GDB about the idle thread */
            send_gdb_packet("mp1.1");
            CurrentProcessEntry = (LIST_ENTRY*)1;
            return;
        }

        Process = CONTAINING_RECORD(CurrentProcessEntry, EPROCESS, ActiveProcessLinks);

        if (Resuming && CurrentThreadEntry != NULL)
            CurrentThreadEntry = CurrentThreadEntry->Flink;
        else
            CurrentThreadEntry = Process->ThreadListHead.Flink;

        ptr = gdb_out;

        *ptr++ = 'm';
        /* List threads from this process */
        for ( ;
             CurrentThreadEntry != &Process->ThreadListHead;
             CurrentThreadEntry = CurrentThreadEntry->Flink)
        {
            ptid_t ptid;
            Thread = CONTAINING_RECORD(CurrentThreadEntry, ETHREAD, ThreadListEntry);

            /* See if we should add a comma */
            if (FirstThread)
                FirstThread = FALSE;
            else
                *ptr++ = ',';

            ptid = ptid_from_thread(Thread);
            ptr += _snprintf(ptr, 1024 - (ptr - gdb_out), "%s", format_ptid(ptid));
            if (ptr > (gdb_out + 1024))
            {
                /* send what we got */
                send_gdb_packet(gdb_out);
                return;
            }
        }

        /* send the list for this process */
        send_gdb_packet(gdb_out);
        CurrentThreadEntry = NULL;
        return;
    }

    if (strncmp(gdb_input, "qGetTIBAddr:", 6) == 0)
    {
        ptid_t ptid = parse_ptid(strstr(gdb_input, ":") + 1);
        PETHREAD Thread = find_thread(ptid);

        if(!Thread) {
            send_gdb_packet("E");
            return;
        }

        send_gdb_packet("%p", Thread->Tcb.Teb);
        return;
    }

    KDDBGPRINT("KDGDB: Unknown query: %s\n", gdb_input);
    send_gdb_packet("");
    return;
}

#if 0
static
void
handle_gdb_registers(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength)
{
    /*
    if (gdb_dbg_thread)
        KDDBGPRINT("Should get registers from other thread!\n");
    */

    State->ApiNumber = DbgKdGetContextApi;
    State->ReturnStatus = STATUS_SUCCESS; /* ? */
    State->Processor = CurrentStateChange.Processor;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;
    if (MessageData)
        MessageData->Length = 0;
    *MessageLength = 0;
}
#endif

static
void
MemorySendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData)
{
    DBGKD_MANIPULATE_STATE64* State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;

    if (PacketType != PACKET_TYPE_KD_STATE_MANIPULATE)
    {
        // KdAssert
        KDDBGPRINT("Wrong packet type (%lu) received after DbgKd VirtualMemoryApi request.\n", PacketType);
        while (1);
    }

    /* Check status */
    if (!NT_SUCCESS(State->ReturnStatus)) {
        send_gdb_ntstatus(State->ReturnStatus);
    } else if (State->ApiNumber == DbgKdReadVirtualMemoryApi) {
        send_gdb_memory(MessageData->Buffer, MessageData->Length);
    } else if (State->ApiNumber == DbgKdWriteVirtualMemoryApi) {
        send_gdb_packet("OK");
    } else {
        KDDBGPRINT("Wrong API number (%lu) after DbgKd VirtualMemoryApi request.\n", State->ApiNumber);
    }
    KdpSendPacketHandler = NULL;
    KdpManipulateStateHandler = NULL;

    /* Reset the TLB */
    __writecr3(PsGetCurrentProcess()->Pcb.DirectoryTableBase[0]);
}

static
GDBSTATUS
handle_gdb_mem(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    ULONG64 Address;
    ULONG64 Length;

    State->ReturnStatus = STATUS_SUCCESS; /* ? */
    State->Processor = CurrentStateChange.Processor;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;

    MessageData->Length = 0;
    *MessageLength = 0;

    /* Set the TLB according to the process being read */
    if (PsGetCurrentProcess() != find_process(current_ptid))
    {
        PEPROCESS AttachedProcess = find_process(current_ptid);
        if (AttachedProcess == NULL)
        {
            KDDBGPRINT("The current GDB debug thread is invalid!\n");
            send_gdb_packet("E03");
            return GdbContinue;
        }
        KDDBGPRINT("Switching page table to %s\n", format_ptid(current_ptid));
        __writecr3(AttachedProcess->Pcb.DirectoryTableBase[0]);
    }

    Address = hex_to_address(&gdb_input[1]);
    Length = hex_to_address(strstr(&gdb_input[1], ",") + 1);

    if(gdb_input[0] == 'm') {
        State->ApiNumber = DbgKdReadVirtualMemoryApi;
        State->u.ReadMemory.TargetBaseAddress = Address;
        State->u.ReadMemory.TransferCount = Length;
    } else /* X */ {
        State->ApiNumber = DbgKdWriteVirtualMemoryApi;
        State->u.WriteMemory.TargetBaseAddress = Address;
        State->u.WriteMemory.TransferCount = Length;
        MessageData->Length = Length;
        MessageData->Buffer = strstr(gdb_input, ":") + 1;
    }

    /* KD will reply with KdSendPacket. Catch it */
    KdpSendPacketHandler = MemorySendHandler;

    return GdbStop;
}

static ULONG active_breakpoints[KD_BREAKPOINT_MAX];

static
void
BreakpointSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData)
{
    DBGKD_MANIPULATE_STATE64* State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;

    KdpSendPacketHandler = NULL;

    if (State->ApiNumber == DbgKdWriteBreakPointApi) {
        ULONG handle = State->u.WriteBreakPoint.BreakPointHandle;
        ULONG address = State->u.WriteBreakPoint.BreakPointAddress;

        if (!NT_SUCCESS(State->ReturnStatus)) {
            KDDBGPRINT("KDDBG: Failed to add breakpoint: %x\n", State->ReturnStatus);
            send_gdb_packet("E");
            return;
        }

        active_breakpoints[handle - 1] = address;
        KDDBGPRINT("KDDBG: Breakpoint %lx added as %lx\n", address, handle);
    } else { /* DbgKdRestoreBreakPointApi */
        /* Kd seems to remove hit breakpoints, and gdb seems to add
         * breakpoints on every vCont, so ignore failures */
        ULONG handle = State->u.RestoreBreakPoint.BreakPointHandle;
        active_breakpoints[handle - 1] = 0;
        KDDBGPRINT("KDDBG: Breakpoint %lx removed\n", handle);
    }

    send_gdb_packet("OK");
}

static
GDBSTATUS
handle_gdb_breakpoint(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    ULONG address = hex_to_address(&gdb_input[3]);

    if (gdb_input[1] != '0' || strstr(gdb_input, ";")) {
        /* Only unconditional software breakpoints */
        KDDBGPRINT("KDDBG: Unsupported breakpoint %s\n", gdb_input);
        send_gdb_packet("");
        return GdbContinue;
    }

    if (address <= (ULONG)MmHighestUserAddress) {
        /* Kernel mode breakpoints only */
        KDDBGPRINT("KDDBG: Breakpoint address %lx in userspace\n", address);
        send_gdb_packet("E");
        return GdbContinue;
    }

    if (gdb_input[0] == 'Z') {
        State->ApiNumber = DbgKdWriteBreakPointApi;
        State->u.WriteBreakPoint.BreakPointAddress = address;
    } else {
        ULONG handle;

        for (handle = 0; handle < KD_BREAKPOINT_MAX; handle++)
            if (active_breakpoints[handle] == address) break;

        if (handle == KD_BREAKPOINT_MAX) {
            /* We don't have that breakpoint */
            send_gdb_packet("E");
            return GdbContinue;
        }

        /* Breakpoint handles number from 1 to 32 inclusive,
         * offset by 1 so we can store them in active_breakpoints */
        State->ApiNumber = DbgKdRestoreBreakPointApi;
        State->u.RestoreBreakPoint.BreakPointHandle = handle + 1;
    }

    State->ReturnStatus = STATUS_SUCCESS; /* ? */
    State->Processor = CurrentStateChange.Processor;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;

    MessageData->Length = 0;
    *MessageLength = 0;

    KdpSendPacketHandler = BreakpointSendHandler;
    return GdbStop;
}

static
GDBSTATUS
handle_gdb_v(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    if (strncmp(gdb_input, "vCont?", 6) == 0)
    {
        /* Report what we support */
        send_gdb_packet("vCont;c;");
        return GdbContinue;
    }

    if (strncmp(gdb_input, "vCont;", 6) == 0)
    {
        DBGKM_EXCEPTION64* Exception = NULL;
        ptid_t ptid = {.pid = -1};

        if (strstr(gdb_input, ":"))
          ptid = parse_ptid(strstr(gdb_input, ":") + 1);

        switch(gdb_input[6])
        {
        case 'c':
            if (ptid.pid != -1) {
                /* Can't resume individual threads */
                send_gdb_packet("E Can't resume individual threads");
                return GdbContinue;
            }

            /* Tell GDB everything is fine, we will handle it */
            send_gdb_packet("OK");

            if (CurrentStateChange.NewState == DbgKdExceptionStateChange)
                Exception = &CurrentStateChange.u.Exception;

            /* See if we should update the program counter (unlike windbg, gdb doesn't do it for us) */
            if (Exception && (Exception->ExceptionRecord.ExceptionCode == STATUS_BREAKPOINT)
                    && (Exception->ExceptionRecord.ExceptionInformation[0] == 0))
            {
                ULONG_PTR ProgramCounter;

                /* So we must get past the breakpoint instruction */
                ProgramCounter = KdpGetContextPc(&CurrentContext);
                KdpSetContextPc(&CurrentContext, ProgramCounter + KD_BREAKPOINT_SIZE);

                SetContextManipulateHandler(State, MessageData, MessageLength, KdContext);
                KdpManipulateStateHandler = ContinueManipulateStateHandler;
                return GdbStop;
            }

            ContinueManipulateStateHandler(State, MessageData, MessageLength, KdContext);
            return GdbStop;
        default:
            /* We can't handle this one, error */
            KDDBGPRINT("Unhandled 'vCont' packet: %s\n", gdb_input);
            send_gdb_packet("E Unsupported: %s", gdb_input);
            return GdbContinue;
        }
    }

    KDDBGPRINT("Unhandled 'v' packet: %s\n", gdb_input);
    send_gdb_packet("E");
    return GdbContinue;
}

/* GLOBAL FUNCTIONS ***********************************************************/
KDSTATUS
gdb_receive_and_interpret_packet(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    KDSTATUS Status;
    GDBSTATUS GdbStatus = GdbContinue;

    while(GdbStatus == GdbContinue)
    {
        Status = gdb_receive_packet(KdContext);

        if(Status != KdPacketReceived)
            break;

        switch (gdb_input[0])
        {
        case '?':
            /* Send the Status */
            gdb_send_exception();
            break;
        case 'g':
            gdb_send_registers(State, MessageData, MessageLength, KdContext);
            break;
        case 'H':
            handle_gdb_set_thread();
            break;
        case 'm':
        case 'X':
            GdbStatus = handle_gdb_mem(State, MessageData, MessageLength, KdContext);
            break;
        case 'p':
            gdb_send_register(State, MessageData, MessageLength, KdContext);
            break;
        case 'Q':
        case 'q':
            handle_gdb_query(State, MessageData, MessageLength, KdContext);
            break;
        case 'T':
            handle_gdb_thread_alive();
            break;
        case 'v':
            GdbStatus = handle_gdb_v(State, MessageData, MessageLength, KdContext);
            break;
        case 'z':
        case 'Z':
            GdbStatus = handle_gdb_breakpoint(State, MessageData, MessageLength, KdContext);
            break;
        case '!':
            /* We're an extended-mode stub */
            send_gdb_packet("OK");
            break;
        default:
            /* We don't know how to handle this request. Maybe this is something for KD */
            State->ReturnStatus = STATUS_NOT_SUPPORTED;
            KDDBGPRINT("Unsupported GDB command: %s.\n", gdb_input);
            goto exit;
        }
    };

exit:
    return Status;
}
