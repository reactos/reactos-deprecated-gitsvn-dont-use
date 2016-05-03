/*
 * COPYRIGHT:       GPL, see COPYING in the top level directory
 * PROJECT:         ReactOS kernel
 * FILE:            drivers/base/kddll/utils.c
 * PURPOSE:         Misc helper functions.
 */

#include "kdgdb.h"

/*
 * We cannot use PsLookupProcessThreadByCid or alike as we could be running at any IRQL.
 * So we have to loop over the process list.
 */

/* pid/tid 0 is special in gdb, and pid/tid 1 aren't valid in reactos
 * So lets map them to each other */
static HANDLE gdb_tid_to_handle(long Tid)
{
    return (HANDLE)(Tid == 1 ? 0 : Tid);
}
#define gdb_pid_to_handle gdb_tid_to_handle

static UINT_PTR handle_to_gdb_tid(HANDLE Handle)
{
    return (UINT_PTR)Handle == 0 ? 1 : (UINT_PTR)Handle;
}
#define handle_to_gdb_pid handle_to_gdb_tid


static int
hex_to_int(char* buffer)
{
    int ret = 0;
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

char *
format_ptid(
    _In_ ptid_t ptid)
{
    static char buf[512];

    if (ptid.pid < 0)
        return "p-1";

    if (ptid.tid < 0) {
        sprintf(buf, "p%x", ptid.pid);
    } else {
        sprintf(buf, "p%x.%x", ptid.pid, ptid.tid);
    }

    return buf;
}

ptid_t
parse_ptid(
    _In_ char *input)
{
    char *buf = input;
    ptid_t ret = {-1, -1};

    if(buf[0] != 'p')
        return ret;

    if(buf[1] == '-')
        return ret;

    ret.pid = hex_to_int(&buf[1]);

    buf = strstr(buf, ".");

    if(!buf || buf[1] == '-')
        return ret;

    ret.tid = hex_to_int(&buf[1]);

    return ret;
}

ptid_t
ptid_from_thread(
    _In_ PETHREAD Thread)
{
    ptid_t ret;

    ret.pid = handle_to_gdb_pid(PsGetThreadProcessId(Thread)),
    ret.tid = handle_to_gdb_tid(PsGetThreadId(Thread));

    return ret;
}

BOOLEAN
ptid_compare(
    _In_ ptid_t a,
    _In_ ptid_t b)
{
    if (a.pid == 0)
        return TRUE;

    if (a.pid != b.pid)
        return FALSE;

    if (a.tid == 0)
        return TRUE;

    return a.tid == b.tid;
}

PEPROCESS
find_process(
    _In_ ptid_t ptid)
{
    HANDLE ProcessId = gdb_pid_to_handle(ptid.pid);
    LIST_ENTRY* ProcessEntry;
    PEPROCESS Process;

    /* pid 0 == any process, return system which is always pid 4 */
    if (ptid.pid == 0)
        return find_process((ptid_t){.pid = 4});

    for (ProcessEntry = ProcessListHead->Flink;
            ProcessEntry != ProcessListHead;
            ProcessEntry = ProcessEntry->Flink)
    {
        Process = CONTAINING_RECORD(ProcessEntry, EPROCESS, ActiveProcessLinks);
        if (Process->UniqueProcessId == ProcessId)
            return Process;
    }

    return NULL;
}

PETHREAD
find_thread(
    _In_ ptid_t ptid)
{
    HANDLE ThreadId = gdb_tid_to_handle(ptid.tid);
    PETHREAD Thread;
    PEPROCESS Process;
    LIST_ENTRY* ThreadEntry;

    /* tid -1 == all threads, shouldn't happen */
    if (ptid.tid < 0)
        return NULL;

    Process = find_process(ptid);
    if (!Process)
        return NULL;

    for (ThreadEntry = Process->ThreadListHead.Flink;
            ThreadEntry != &Process->ThreadListHead;
            ThreadEntry = ThreadEntry->Flink)
    {
        Thread = CONTAINING_RECORD(ThreadEntry, ETHREAD, ThreadListEntry);
        /* For GDB, Tid == 0 means any thread */
        if ((Thread->Cid.UniqueThread == ThreadId) || (ptid.tid == 0))
            return Thread;
    }

    return NULL;
}
