/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/kddll.h
 * PURPOSE:         Base definitions for the kernel debugger.
 */

#ifndef _KDGDB_H_
#define _KDGDB_H_

#define NOEXTAPI
#include <ntifs.h>
#include <halfuncs.h>
#include <stdio.h>
#include <arc/arc.h>
#include <inttypes.h>
#include <windbgkd.h>
#include <kddll.h>

#include <pstypes.h>

#define KDDEBUG /* uncomment to enable debugging this dll */

#ifndef KDDEBUG
#define KDDBGPRINT(...)
#else
extern ULONG KdpDbgPrint(const char* Format, ...);
#define KDDBGPRINT KdpDbgPrint
#endif

/* for gdb_receive_and_interpret_packet */
typedef ULONG GDBSTATUS;

#define GdbStop 0
#define GdbContinue 1

/* GDB doesn't like pid - tid 0, so +1 them */
FORCEINLINE HANDLE gdb_tid_to_handle(UINT_PTR Tid)
{
    return (HANDLE)(Tid - 1);
}
#define gdb_pid_to_handle gdb_tid_to_handle
FORCEINLINE UINT_PTR handle_to_gdb_tid(HANDLE Handle)
{
    return (UINT_PTR)Handle + 1;
}
#define handle_to_gdb_pid handle_to_gdb_tid

/* Format for printing pid/tid pair */
#define PIDTID "p%" PRIxPTR ".%" PRIxPTR

FORCEINLINE
VOID
InitManipulateFromStateChange(
    _In_ ULONG ApiNumber,
    _In_ const DBGKD_ANY_WAIT_STATE_CHANGE* StateChange,
    _Out_ DBGKD_MANIPULATE_STATE64* Manipulate)
{
    Manipulate->ApiNumber = ApiNumber;
    Manipulate->Processor = StateChange->Processor;
    Manipulate->ProcessorLevel = StateChange->ProcessorLevel;
}

/* Callbacks to simulate a KdReceive <-> KdSend loop without GDB being aware of it */
typedef VOID (*KDP_SEND_HANDLER)(
    _In_ ULONG PacketType,
    _In_ PSTRING MessageHeader,
    _In_ PSTRING MessageData
);
typedef KDSTATUS (*KDP_MANIPULATESTATE_HANDLER)(
    _Out_ DBGKD_MANIPULATE_STATE64* State,
    _Out_ PSTRING MessageData,
    _Out_ PULONG MessageLength,
    _Inout_ PKD_CONTEXT KdContext
);

/* gdb_input.c */
extern BOOLEAN in_stop_mode;
extern UINT_PTR gdb_dbg_tid;
extern UINT_PTR gdb_dbg_pid;
extern KDSTATUS gdb_receive_and_interpret_packet(_Out_ DBGKD_MANIPULATE_STATE64* State, _Out_ PSTRING MessageData, _Out_ PULONG MessageLength, _Inout_ PKD_CONTEXT KdContext);

/* gdb_receive.c */
extern CHAR gdb_input[];
KDSTATUS NTAPI gdb_receive_packet(_Inout_ PKD_CONTEXT KdContext);
char hex_value(char ch);

/* gdb_send.c */
void send_gdb_packet(_In_ CHAR* Buffer);
void send_gdb_memory(_In_ VOID* Buffer, size_t Length);
void gdb_send_debug_io(_In_ PSTRING String);
void gdb_send_exception(void);
void send_gdb_ntstatus(_In_ NTSTATUS Status);
extern const char hex_chars[];

/* kdcom.c */
KDSTATUS NTAPI KdpPollBreakIn(VOID);
VOID NTAPI KdpSendByte(_In_ UCHAR Byte);
KDSTATUS NTAPI KdpReceiveByte(_Out_ PUCHAR OutByte);

/* kdpacket.c */
extern DBGKD_ANY_WAIT_STATE_CHANGE CurrentStateChange;
extern CONTEXT CurrentContext;
extern DBGKD_GET_VERSION64 KdVersion;
extern KDDEBUGGER_DATA64* KdDebuggerDataBlock;
extern LIST_ENTRY* ProcessListHead;
extern KDP_SEND_HANDLER KdpSendPacketHandler;
extern KDP_MANIPULATESTATE_HANDLER KdpManipulateStateHandler;
/* Commone ManipulateState handlers */
extern KDSTATUS ContinueManipulateStateHandler(_Out_ DBGKD_MANIPULATE_STATE64* State, _Out_ PSTRING MessageData, _Out_ PULONG MessageLength, _Inout_ PKD_CONTEXT KdContext);
extern KDSTATUS SetContextManipulateHandler(_Out_ DBGKD_MANIPULATE_STATE64* State, _Out_ PSTRING MessageData, _Out_ PULONG MessageLength, _Inout_ PKD_CONTEXT KdContext);
extern PEPROCESS TheIdleProcess;
extern PETHREAD TheIdleThread;

/* utils.c */
extern PEPROCESS find_process( _In_ UINT_PTR Pid);
extern PETHREAD find_thread(_In_ UINT_PTR Pid, _In_ UINT_PTR Tid);

/* arch_sup.c */
extern VOID gdb_send_register(_Out_ DBGKD_MANIPULATE_STATE64* State, _Out_ PSTRING MessageData, _Out_ PULONG MessageLength, _Inout_ PKD_CONTEXT KdContext);
extern VOID gdb_send_registers(_Out_ DBGKD_MANIPULATE_STATE64* State, _Out_ PSTRING MessageData, _Out_ PULONG MessageLength, _Inout_ PKD_CONTEXT KdContext);

/* Architecture specific defines. See ntoskrnl/include/internal/arch/ke.h */
#ifdef _M_IX86
#  define KdpGetContextPc(Context) \
    ((Context)->Eip)
#  define KdpSetContextPc(Context, ProgramCounter) \
    ((Context)->Eip = (ProgramCounter))
#  define KD_BREAKPOINT_SIZE        sizeof(UCHAR)
#else
#  error "Please define relevant macros for your architecture"
#endif

#endif /* _KDGDB_H_ */
