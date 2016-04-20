/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/gdb_input.c
 * PURPOSE:         Base functions for the kernel debugger.
 */

#include "kdgdb.h"

/* LOCALS *********************************************************************/
static ULONG_PTR gdb_run_tid;
/* Keep track of where we are for qfThreadInfo/qsThreadInfo */
static LIST_ENTRY* CurrentProcessEntry;
static LIST_ENTRY* CurrentThreadEntry;

/* GLOBALS ********************************************************************/
BOOLEAN in_stop_mode;
UINT_PTR gdb_dbg_pid;
UINT_PTR gdb_dbg_tid;

/* PRIVATE FUNCTIONS **********************************************************/
static
UINT_PTR
hex_to_tid(char* buffer)
{
    ULONG_PTR ret = 0;
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
#define hex_to_pid hex_to_tid

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
    switch (gdb_input[1])
    {
    case 'c':
        if (strcmp(&gdb_input[2], "-1") == 0)
            gdb_run_tid = (ULONG_PTR)-1;
        else
            gdb_run_tid = hex_to_tid(&gdb_input[2]);
        send_gdb_packet("OK");
        break;
    case 'g':
        KDDBGPRINT("Setting debug thread: %s.\n", gdb_input);
        if (strncmp(&gdb_input[2], "p-1", 3) == 0)
        {
            gdb_dbg_pid = (UINT_PTR)-1;
            gdb_dbg_tid = (UINT_PTR)-1;
        }
        else
        {
            char* ptr = strstr(gdb_input, ".") + 1;
            gdb_dbg_pid = hex_to_pid(&gdb_input[3]);
            if (strncmp(ptr, "-1", 2) == 0)
                gdb_dbg_tid = (UINT_PTR)-1;
            else
                gdb_dbg_tid = hex_to_tid(ptr);
        }
        send_gdb_packet("OK");
        break;
    default:
        KDDBGPRINT("KDGBD: Unknown 'H' command: %s\n", gdb_input);
        send_gdb_packet("");
    }
}

static
void
handle_gdb_thread_alive(void)
{
    ULONG_PTR Pid, Tid;
    PETHREAD Thread;

    Pid = hex_to_pid(&gdb_input[2]);
    Tid = hex_to_tid(strstr(gdb_input, ".") + 1);

    /* We cannot use PsLookupProcessThreadByCid as we could be running at any IRQL.
     * So loop. */
    KDDBGPRINT("Checking if p%p.%p is alive.\n", Pid, Tid);

    Thread = find_thread(Pid, Tid);

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
        send_gdb_packet("PacketSize=4096;multiprocess+;");
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
        char gdb_out[64];
        sprintf(gdb_out, "QC:p%"PRIxPTR".%"PRIxPTR";",
            handle_to_gdb_pid(PsGetThreadProcessId((PETHREAD)(ULONG_PTR)CurrentStateChange.Thread)),
            handle_to_gdb_tid(PsGetThreadId((PETHREAD)(ULONG_PTR)CurrentStateChange.Thread)));
        send_gdb_packet(gdb_out);
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
            Thread = CONTAINING_RECORD(CurrentThreadEntry, ETHREAD, ThreadListEntry);

            /* See if we should add a comma */
            if (FirstThread)
            {
                FirstThread = FALSE;
            }
            else
            {
                *ptr++ = ',';
            }

            ptr += _snprintf(ptr, 1024 - (ptr - gdb_out),
                "p%p.%p",
                handle_to_gdb_pid(Process->UniqueProcessId),
                handle_to_gdb_tid(Thread->Cid.UniqueThread));
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
    if ((gdb_dbg_pid != 0) && gdb_pid_to_handle(gdb_dbg_pid) != PsGetCurrentProcessId())
    {
        __writecr3(PsGetCurrentProcess()->Pcb.DirectoryTableBase[0]);
    }
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

    /* Set the TLB according to the process being read. Pid 0 means any process. */
    if ((gdb_dbg_pid != 0) && gdb_pid_to_handle(gdb_dbg_pid) != PsGetCurrentProcessId())
    {
        PEPROCESS AttachedProcess = find_process(gdb_dbg_pid);
        if (AttachedProcess == NULL)
        {
            KDDBGPRINT("The current GDB debug thread is invalid!");
            send_gdb_packet("E03");
            return GdbContinue;
        }
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

static
KDSTATUS
handle_gdb_v(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    if (strncmp(gdb_input, "vCont", 5) == 0)
    {
        if (gdb_input[5] == '?')
        {
            /* Report what we support */
            send_gdb_packet("vCont;c;C;s;S");
            return GdbContinue;
        }

        if (strcmp(gdb_input, "vCont;c") == 0)
        {
            DBGKM_EXCEPTION64* Exception = NULL;

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
        }
    }

    KDDBGPRINT("Unhandled 'v' packet: %s\n", gdb_input);
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

    in_stop_mode = TRUE;

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
            /* Let gdb do soft breakpoints itself */
            send_gdb_packet("");
            break;
        default:
            /* We don't know how to handle this request. Maybe this is something for KD */
            State->ReturnStatus = STATUS_NOT_SUPPORTED;
            KDDBGPRINT("Unsupported GDB command: %s.\n", gdb_input);
            goto exit;
        }
    };

exit:
    in_stop_mode = FALSE;
    return Status;
}
