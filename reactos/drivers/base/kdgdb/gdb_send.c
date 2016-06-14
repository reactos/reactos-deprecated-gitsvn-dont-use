/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/gdb_send.c
 * PURPOSE:         Base functions for the kernel debugger.
 */

#include "kdgdb.h"

/* LOCALS *********************************************************************/
const char hex_chars[] = "0123456789abcdef";

/* PRIVATE FUNCTIONS **********************************************************/
static
unsigned char
exception_code_to_gdb(NTSTATUS code)
{
    unsigned char SigVal;

    switch (code)
    {
    case STATUS_INTEGER_DIVIDE_BY_ZERO:
        SigVal = 8; /* divide by zero */
        break;
    case STATUS_SINGLE_STEP:
    case STATUS_BREAKPOINT:
        SigVal = 5; /* breakpoint */
        break;
    case STATUS_INTEGER_OVERFLOW:
    case STATUS_ARRAY_BOUNDS_EXCEEDED:
        SigVal = 16; /* bound instruction */
        break;
    case STATUS_ILLEGAL_INSTRUCTION:
        SigVal = 4; /* Invalid opcode */
        break;
    case STATUS_STACK_OVERFLOW:
    case STATUS_DATATYPE_MISALIGNMENT:
    case STATUS_ACCESS_VIOLATION:
        SigVal = 11; /* access violation */
        break;
    default:
        SigVal = 7; /* "software generated" */
    }
    return SigVal;
}


static struct {
    char check_sum;
#ifdef KDDEBUG
    BOOLEAN in_packet;
#endif
} gdb_packet;

void
gdb_begin_packet(void)
{
#ifdef KDDEBUG
    if (gdb_packet.in_packet) {
        KDDBGPRINT("gdb_begin_packet called twice\n");
    }
    gdb_packet.in_packet = TRUE;
#endif
    gdb_packet.check_sum = 0;
    KdpSendByte('$');
}

void
gdb_send_byte(char byte)
{
    gdb_packet.check_sum += byte;
    KdpSendByte(byte);
}

void
gdb_send_string(char *Format, ...)
{
    va_list ap;
    int Length;
    CHAR Buffer[4096];

    va_start(ap, Format);
    Length = _vsnprintf(Buffer, sizeof(Buffer), Format, ap);
    va_end(ap);

    gdb_send_binary(Buffer, Length);
}

void
gdb_send_binary(char *buffer, int len)
{
    while(len--)
        gdb_send_byte(*buffer++);
}

void
gdb_end_packet(void)
{
#ifdef KDDEBUG
    if (!gdb_packet.in_packet) {
        KDDBGPRINT("gdb_end_packet called without gdb_begin_packet\n");
    }
    gdb_packet.in_packet = FALSE;
#endif

    KdpSendByte('#');
    KdpSendByte(hex_chars[(gdb_packet.check_sum >> 4) & 0xf]);
    KdpSendByte(hex_chars[gdb_packet.check_sum & 0xf]);
}

/* GLOBAL FUNCTIONS ***********************************************************/
void
send_gdb_packet_binary(char *buf, int len)
{
resend:
    gdb_begin_packet();
    gdb_send_binary(buf, len);
    gdb_end_packet();
    if (gdb_wait_ack() == KdPacketNeedsResend)
        goto resend;
}

void
send_gdb_packet(const char* Format, ...)
{
    va_list ap;
    int Length;
    CHAR Buffer[4096];

    va_start(ap, Format);
    Length = _vsnprintf(Buffer, sizeof(Buffer), Format, ap);
    va_end(ap);

    send_gdb_packet_binary(Buffer, Length);
}

void
send_gdb_memory(
    _In_ VOID* Buffer,
    _In_ size_t Length)
{
    CHAR* ptr;
    size_t len;

resend:
    gdb_begin_packet();

    /* Send the data */
    len = Length;
    ptr = Buffer;
    while (len--)
    {
        gdb_send_byte(hex_chars[(*ptr >> 4) & 0xf]);
        gdb_send_byte(hex_chars[*ptr++ & 0xf]);
    }
    gdb_end_packet();

    if (gdb_wait_ack() == KdPacketNeedsResend)
        goto resend;
}

void
gdb_send_debug_io(
    _In_ PSTRING String)
{
    CHAR* ptr = String->Buffer;
    USHORT Length = String->Length;

    gdb_begin_packet();
    gdb_send_byte('O');

    /* Send the data */
    while (Length--)
    {
        gdb_send_byte(hex_chars[(*ptr >> 4) & 0xf]);
        gdb_send_byte(hex_chars[*ptr++ & 0xf]);
    }
    gdb_end_packet();

    /* Don't wait for an ack here, kd will call PollBreakIn */
}

void
gdb_send_exception(void)
{
    PETHREAD Thread = (PETHREAD)(ULONG_PTR)CurrentStateChange.Thread;

    /* Report to GDB */
    unsigned char code = 5;

    if (CurrentStateChange.NewState == DbgKdExceptionStateChange)
    {
        EXCEPTION_RECORD64* ExceptionRecord = &CurrentStateChange.u.Exception.ExceptionRecord;
        code = exception_code_to_gdb(ExceptionRecord->ExceptionCode);
    }

    send_gdb_packet("T%02xthread:%s;core:%x;", code, format_ptid(ptid_from_thread(Thread)), CurrentStateChange.Processor);
}

void
send_gdb_ntstatus(
    _In_ NTSTATUS Status)
{
    /* Just build a EXX packet and send it */
    send_gdb_packet("E%02x", exception_code_to_gdb(Status));
}
