/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/kdpacket.c
 * PURPOSE:         Base functions for the kernel debugger.
 */

#include "kdgdb.h"

/* LOCALS *********************************************************************/
static
VOID
FirstSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData);
static BOOLEAN InException = FALSE;

/* GLOBALS ********************************************************************/
DBGKD_GET_VERSION64 KdVersion;
KDDEBUGGER_DATA64* KdDebuggerDataBlock;
LIST_ENTRY* ProcessListHead;
/* Callbacks used to communicate with KD aside from GDB */
KDP_SEND_HANDLER KdpSendPacketHandler = FirstSendHandler;
KDP_MANIPULATESTATE_HANDLER KdpManipulateStateHandler = NULL;
/* Data describing the current exception */
DBGKD_ANY_WAIT_STATE_CHANGE CurrentStateChange;
CONTEXT CurrentContext;

/* PRIVATE FUNCTIONS **********************************************************/

static
VOID
GetContextSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData
)
{
    DBGKD_MANIPULATE_STATE64* State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;
    const CONTEXT* Context = (const CONTEXT*)MessageData->Buffer;

    if ((PacketType != PACKET_TYPE_KD_STATE_MANIPULATE)
            || (State->ApiNumber != DbgKdGetContextApi)
            || (MessageData->Length < sizeof(*Context)))
    {
        /* Should we bugcheck ? */
        KDDBGPRINT("ERROR: Received wrong packet from KD.\n");
        while (1);
    }

    /* Just copy it */
    RtlCopyMemory(&CurrentContext, Context, sizeof(*Context));
    KdpSendPacketHandler = NULL;
}

static
KDSTATUS
GetContextManipulateHandler(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext
)
{
    State->ApiNumber = DbgKdGetContextApi;
    State->Processor = CurrentStateChange.Processor;
    State->ReturnStatus = STATUS_SUCCESS;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;
    MessageData->Length = 0;

    /* Update the send <-> receive loop handler */
    KdpSendPacketHandler = GetContextSendHandler;
    KdpManipulateStateHandler = NULL;

    return KdPacketReceived;
}

static
VOID
SetContextSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData
)
{
    DBGKD_MANIPULATE_STATE64* State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;

    /* We just confirm that all went well */
    if ((PacketType != PACKET_TYPE_KD_STATE_MANIPULATE)
            || (State->ApiNumber != DbgKdSetContextApi)
            || (State->ReturnStatus != STATUS_SUCCESS))
    {
        /* Should we bugcheck ? */
        while (1);
    }

    KdpSendPacketHandler = NULL;
}

KDSTATUS
SetContextManipulateHandler(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext
)
{
    State->ApiNumber = DbgKdSetContextApi;
    State->Processor = CurrentStateChange.Processor;
    State->ReturnStatus = STATUS_SUCCESS;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;
    MessageData->Length = sizeof(CurrentContext);

    if (MessageData->MaximumLength < sizeof(CurrentContext))
    {
        while (1);
    }

    RtlCopyMemory(MessageData->Buffer, &CurrentContext, sizeof(CurrentContext));

    /* Update the send <-> receive loop handlers */
    KdpSendPacketHandler = SetContextSendHandler;
    KdpManipulateStateHandler = NULL;

    return KdPacketReceived;
}

static
void
send_kd_state_change(DBGKD_ANY_WAIT_STATE_CHANGE* StateChange)
{
    InException = TRUE;

    switch (StateChange->NewState)
    {
    case DbgKdLoadSymbolsStateChange:
    {
        /* We don't care about symbols loading */
        KdpManipulateStateHandler = ContinueManipulateStateHandler;
        break;
    }
    case DbgKdExceptionStateChange:
    {
        PETHREAD Thread = (PETHREAD)(ULONG_PTR)StateChange->Thread;
        /* Save current state for later GDB queries */
        CurrentStateChange = *StateChange;
        KDDBGPRINT("Exception 0x%08x in thread p%p.%p.\n",
            StateChange->u.Exception.ExceptionRecord.ExceptionCode,
            PsGetThreadProcessId(Thread),
            PsGetThreadId(Thread));
        /* Set the current debugged process/thread accordingly */
        current_ptid = ptid_from_thread(Thread);
        gdb_send_exception();
        /* Next receive call will ask for the context */
        KdpManipulateStateHandler = GetContextManipulateHandler;
        break;
    }
    default:
        /* FIXME */
        while (1);
    }
}

static
void
send_kd_debug_io(
    _In_ DBGKD_DEBUG_IO* DebugIO,
    _In_ PSTRING String)
{
    if (InException) {
        KDDBGPRINT("KDGDB: debug_io in exception: %*s\n", String->Length, String->Buffer);
        return;
    }

    switch (DebugIO->ApiNumber)
    {
    case DbgKdPrintStringApi:
        gdb_send_debug_io(String);
        break;
    default:
        /* FIXME */
        while (1);
    }
}

static
void
send_kd_state_manipulate(
    _In_ DBGKD_MANIPULATE_STATE64* State,
    _In_ PSTRING MessageData)
{
    switch (State->ApiNumber)
    {
#if 0
    case DbgKdGetContextApi:
        /* This is an answer to a 'g' GDB request */
        gdb_send_registers((CONTEXT*)MessageData->Buffer);
        return;
#endif
    default:
        /* FIXME */
        while (1);
    }
}

KDSTATUS
ContinueManipulateStateHandler(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext
)
{
    /* Let's go on */
    State->ApiNumber = DbgKdContinueApi;
    State->ReturnStatus = STATUS_SUCCESS; /* ? */
    State->Processor = CurrentStateChange.Processor;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;
    if (MessageData)
        MessageData->Length = 0;
    *MessageLength = 0;
    State->u.Continue.ContinueStatus = STATUS_SUCCESS;

    /* We definitely are at the end of the send <-> receive loop, if any */
    KdpSendPacketHandler = NULL;
    KdpManipulateStateHandler = NULL;
    /* We're not handling an exception anymore */
    InException = FALSE;

    return KdPacketReceived;
}

static
VOID
GetVersionSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData)
{
    DBGKD_MANIPULATE_STATE64* State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;
    LIST_ENTRY* DebuggerDataList;

    /* Confirm that all went well */
    if ((PacketType != PACKET_TYPE_KD_STATE_MANIPULATE)
            || (State->ApiNumber != DbgKdGetVersionApi)
            || !NT_SUCCESS(State->ReturnStatus))
    {
        /* FIXME: should detach from KD and go along without debugging */
        KDDBGPRINT("Wrong packet received after asking for data.\n");
        while(1);
    }

    /* Copy the relevant data */
    RtlCopyMemory(&KdVersion, &State->u.GetVersion64, sizeof(KdVersion));
    DebuggerDataList = (LIST_ENTRY*)(ULONG_PTR)KdVersion.DebuggerDataList;
    KdDebuggerDataBlock = CONTAINING_RECORD(DebuggerDataList->Flink, KDDEBUGGER_DATA64, Header.List);
    ProcessListHead = (LIST_ENTRY*)KdDebuggerDataBlock->PsActiveProcessHead.Pointer;

    /* Now we can get the context for the current state */
    KdpSendPacketHandler = NULL;
    KdpManipulateStateHandler = GetContextManipulateHandler;
}

static
KDSTATUS
GetVersionManipulateStateHandler(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    /* Ask for the version data */
    State->ApiNumber = DbgKdGetVersionApi;
    State->Processor = CurrentStateChange.Processor;
    State->ProcessorLevel = CurrentStateChange.ProcessorLevel;

    /* The next send call will serve this query */
    KdpSendPacketHandler = GetVersionSendHandler;
    KdpManipulateStateHandler = NULL;

    return KdPacketReceived;
}

static
VOID
FirstSendHandler(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData)
{
    DBGKD_ANY_WAIT_STATE_CHANGE* StateChange = (DBGKD_ANY_WAIT_STATE_CHANGE*)MessageHeader->Buffer;
    PETHREAD Thread;

    if (PacketType == PACKET_TYPE_KD_DEBUG_IO)
    {
        /* This is not the packet we are waiting for */
        send_kd_debug_io((DBGKD_DEBUG_IO*)MessageHeader->Buffer, MessageData);
        return;
    }

    if (PacketType != PACKET_TYPE_KD_STATE_CHANGE64)
    {
        KDDBGPRINT("First KD packet is not a state change!\n");
        /* FIXME: What should we send back to KD ? */
        while(1);
    }

    KDDBGPRINT("KDGDB: START!\n");

    Thread = (PETHREAD)(ULONG_PTR)StateChange->Thread;

    /* Set up the current state */
    CurrentStateChange = *StateChange;
    current_ptid = ptid_from_thread(Thread);

    KDDBGPRINT("Pid Tid of the first message: %s.\n", format_ptid(current_ptid));

    /* The next receive call will be asking for the version data */
    KdpSendPacketHandler = NULL;
    KdpManipulateStateHandler = GetVersionManipulateStateHandler;
}

/* PUBLIC FUNCTIONS ***********************************************************/

/******************************************************************************
 * \name KdReceivePacket
 * \brief Receive a packet from the KD port.
 * \param [in] PacketType Describes the type of the packet to receive.
 *        This can be one of the PACKET_TYPE_ constants.
 * \param [out] MessageHeader Pointer to a STRING structure for the header.
 * \param [out] MessageData Pointer to a STRING structure for the data.
 * \return KdPacketReceived if successful, KdPacketTimedOut if the receive
 *         timed out, KdPacketNeedsResend to signal that the last packet needs
 *         to be sent again.
 * \note If PacketType is PACKET_TYPE_KD_POLL_BREAKIN, the function doesn't
 *       wait for any data, but returns KdPacketTimedOut instantly if no breakin
 *       packet byte is received.
 * \sa http://www.nynaeve.net/?p=169
 */
KDSTATUS
NTAPI
KdReceivePacket(
    _In_ ULONG PacketType,
    _Out_ PSTRING MessageHeader,
    _Out_ PSTRING MessageData,
    _Out_ PULONG DataLength,
    _Inout_ PKD_CONTEXT KdContext)
{
    DBGKD_MANIPULATE_STATE64* State;

    /* Special handling for breakin packet */
    if (PacketType == PACKET_TYPE_KD_POLL_BREAKIN)
    {
        return KdpPollBreakIn();
    }

    if (PacketType != PACKET_TYPE_KD_STATE_MANIPULATE)
    {
        /* What should we do ? */
        while (1);
    }

    State = (DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer;

    /* Maybe we are in a send<->receive loop that GDB doesn't need to know about */
    if (KdpManipulateStateHandler != NULL)
        return KdpManipulateStateHandler(State, MessageData, DataLength, KdContext);

    return gdb_receive_and_interpret_packet(State, MessageData, DataLength, KdContext);
}

VOID
NTAPI
KdSendPacket(
    IN ULONG PacketType,
    IN PSTRING MessageHeader,
    IN PSTRING MessageData,
    IN OUT PKD_CONTEXT KdContext)
{
    /* Maybe we are in a send <-> receive loop that GDB doesn't need to know about */
    if (KdpSendPacketHandler)
    {
        KdpSendPacketHandler(PacketType, MessageHeader, MessageData);
        return;
    }

    switch (PacketType)
    {
    case PACKET_TYPE_KD_STATE_CHANGE64:
        send_kd_state_change((DBGKD_ANY_WAIT_STATE_CHANGE*)MessageHeader->Buffer);
        return;
    case PACKET_TYPE_KD_DEBUG_IO:
        send_kd_debug_io((DBGKD_DEBUG_IO*)MessageHeader->Buffer, MessageData);
        break;
    case PACKET_TYPE_KD_STATE_MANIPULATE:
        send_kd_state_manipulate((DBGKD_MANIPULATE_STATE64*)MessageHeader->Buffer, MessageData);
        break;
    default:
        /* FIXME */
        while (1);
    }
}

/* EOF */
