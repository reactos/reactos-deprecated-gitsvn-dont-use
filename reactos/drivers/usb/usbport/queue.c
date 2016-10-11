#include "usbport.h"

//#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_CORE
#define NDEBUG_USBPORT_QUEUE
#include "usbdebug.h"

VOID
NTAPI
USBPORT_InsertIdleIrp(IN PIO_CSQ Csq,
                      IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_InsertIdleIrp: Irp - %p\n", Irp);

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     IdleIoCsq);

    InsertTailList(&FdoExtension->IdleIrpList, &Irp->Tail.Overlay.ListEntry);
}

VOID
NTAPI
USBPORT_RemoveIdleIrp(IN PIO_CSQ Csq,
                      IN PIRP Irp)
{
    DPRINT_QUEUE("USBPORT_RemoveIdleIrp: Irp - %p\n", Irp);
    RemoveEntryList(&Irp->Tail.Overlay.ListEntry);
}

PIRP
NTAPI
USBPORT_PeekNextIdleIrp(IN PIO_CSQ Csq,
                        IN PIRP Irp,
                        IN PVOID PeekContext)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY NextEntry;
    PLIST_ENTRY ListHead;
    PIRP NextIrp = NULL;

    DPRINT_QUEUE("USBPORT_PeekNextIdleIrp: Irp - %p, PeekContext - %p\n", Irp, PeekContext);

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     IdleIoCsq);

    ListHead = &FdoExtension->IdleIrpList;

    if (Irp)
    {
        NextEntry = Irp->Tail.Overlay.ListEntry.Flink;
    }
    else
    {
        NextEntry = ListHead->Flink;
    }

    while (NextEntry != ListHead)
    {
        NextIrp = CONTAINING_RECORD(NextEntry,
                                    IRP,
                                    Tail.Overlay.ListEntry);

        if (!PeekContext)
            break;

        NextEntry = NextEntry->Flink;
    }

    return NextIrp;
}

VOID
NTAPI
USBPORT_AcquireIdleLock(IN PIO_CSQ Csq,
                        IN PKIRQL Irql)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_AcquireIdleLock: ... \n");

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     IdleIoCsq);

    KeAcquireSpinLock(&FdoExtension->IdleIoCsqSpinLock, Irql);
}

VOID
NTAPI
USBPORT_ReleaseIdleLock(IN PIO_CSQ Csq,
                        IN KIRQL Irql)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_ReleaseIdleLock: ... \n");

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     IdleIoCsq);

    KeReleaseSpinLock(&FdoExtension->IdleIoCsqSpinLock, Irql);
}

VOID
NTAPI
USBPORT_CompleteCanceledIdleIrp(IN PIO_CSQ Csq,
                                IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_CompleteCanceledIdleIrp: ... \n");

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     IdleIoCsq);

    InterlockedDecrement(&FdoExtension->IdleLockCounter);

    Irp->IoStatus.Status = STATUS_CANCELLED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
}

