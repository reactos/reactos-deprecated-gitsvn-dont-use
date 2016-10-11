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

VOID
NTAPI
USBPORT_InsertBadRequest(IN PIO_CSQ Csq,
                         IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_InsertBadRequest: Irp - %p\n", Irp);

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     BadRequestIoCsq);

    InsertTailList(&FdoExtension->BadRequestList, &Irp->Tail.Overlay.ListEntry);
}

VOID
NTAPI
USBPORT_RemoveBadRequest(IN PIO_CSQ Csq,
                         IN PIRP Irp)
{
    DPRINT_QUEUE("USBPORT_RemoveBadRequest: Irp - %p\n", Irp);
    RemoveEntryList(&Irp->Tail.Overlay.ListEntry);
}

PIRP
NTAPI
USBPORT_PeekNextBadRequest(IN PIO_CSQ Csq,
                           IN PIRP Irp,
                           IN PVOID PeekContext)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY NextEntry;
    PLIST_ENTRY ListHead;
    PIRP NextIrp = NULL;

    DPRINT_QUEUE("USBPORT_PeekNextBadRequest: Irp - %p, PeekContext - %p\n", Irp, PeekContext);

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     BadRequestIoCsq);

    ListHead = &FdoExtension->BadRequestList;

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
USBPORT_AcquireBadRequestLock(IN PIO_CSQ Csq,
                              IN PKIRQL Irql)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_AcquireBadRequestLock: ... \n");

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     BadRequestIoCsq);

    KeAcquireSpinLock(&FdoExtension->BadRequestIoCsqSpinLock, Irql);
}

VOID
NTAPI
USBPORT_ReleaseBadRequestLock(IN PIO_CSQ Csq,
                              IN KIRQL Irql)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_ReleaseBadRequestLock: ... \n");

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     BadRequestIoCsq);

    KeReleaseSpinLock(&FdoExtension->BadRequestIoCsqSpinLock, Irql);
}

VOID
NTAPI
USBPORT_CompleteCanceledBadRequest(IN PIO_CSQ Csq,
                                   IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_QUEUE("USBPORT_CompleteCanceledBadRequest: Irp - %p\n", Irp);

    FdoExtension = CONTAINING_RECORD(Csq,
                                     USBPORT_DEVICE_EXTENSION,
                                     BadRequestIoCsq);

    InterlockedDecrement(&FdoExtension->BadRequestLockCounter);

    Irp->IoStatus.Status = STATUS_CANCELLED;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
}

VOID
NTAPI
USBPORT_InsertIrpInTable(IN PUSBPORT_IRP_TABLE IrpTable,
                         IN PIRP Irp)
{
    ULONG ix;

    DPRINT_CORE("USBPORT_InsertIrpInTable: IrpTable - %p, Irp - %p\n", IrpTable, Irp);

    ASSERT(IrpTable != NULL);

Start:

    for (ix = 0; ix < 0x200; ix++)
    {
        if (IrpTable->irp[ix] == NULL)
        {
            IrpTable->irp[ix] = Irp;

            if (ix > 0)
            {
                DPRINT_CORE("USBPORT_InsertIrpInTable: ix - %x\n", ix);
            }

            return;
        }
    }

    if (ix != 0x200)
    {
        KeBugCheckEx(0xFE, 1, 0, 0, 0);
    }

    IrpTable->LinkNextTable = ExAllocatePoolWithTag(NonPagedPool, sizeof(USBPORT_IRP_TABLE), USB_PORT_TAG);

    if (IrpTable->LinkNextTable == NULL)
    {
        KeBugCheckEx(0xFE, 1, 0, 0, 0);
    }

    RtlZeroMemory(IrpTable->LinkNextTable, sizeof(USBPORT_IRP_TABLE));
    IrpTable = IrpTable->LinkNextTable;

    goto Start;
}

PIRP
NTAPI
USBPORT_RemoveIrpFromTable(IN PUSBPORT_IRP_TABLE IrpTable,
                           IN PIRP Irp)
{
    ULONG ix;

    DPRINT_CORE("USBPORT_RemoveIrpFromTable: IrpTable - %p, Irp - %p\n", IrpTable, Irp);

    ASSERT(IrpTable != NULL);

    while (TRUE)
    {
        ix = 0;

        for (ix = 0; ix < 0x200; ix++)
        {
            if (IrpTable->irp[ix] == Irp)
            {
                IrpTable->irp[ix] = NULL;

                if (ix > 0)
                {
                    DPRINT1("USBPORT_RemoveIrpFromTable: ix - %x\n", ix);
                }

                return Irp;
            }
        }

        if (IrpTable->LinkNextTable == 0)
            break;

        IrpTable = IrpTable->LinkNextTable;
        continue;
    }

    DPRINT1("USBPORT_RemoveIrpFromTable: return NULL. ix - %x\n", ix);
    return NULL;
}

PIRP
NTAPI
USBPORT_RemoveActiveTransferIrp(IN PDEVICE_OBJECT FdoDevice,
                                IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_RemoveActiveTransferIrp: Irp - %p\n", Irp);
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    return USBPORT_RemoveIrpFromTable(FdoExtension->ActiveIrpTable, Irp);
}

PIRP
NTAPI
USBPORT_RemovePendingTransferIrp(IN PDEVICE_OBJECT FdoDevice,
                                 IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_RemovePendingTransferIrp: Irp - %p\n", Irp);
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    return USBPORT_RemoveIrpFromTable(FdoExtension->PendingIrpTable, Irp);
}

VOID
NTAPI
USBPORT_FindUrbInIrpTable(IN PUSBPORT_IRP_TABLE IrpTable,
                          IN PURB Urb,
                          IN PIRP Irp)
{
    ULONG ix; 
    PIRP irp;
    PURB urbIn;
    PIO_STACK_LOCATION IoStack;

    DPRINT_CORE("USBPORT_FindUrbInIrpTable: IrpTable - %p, Urb - %p, Irp - %p\n", IrpTable, Urb, Irp);

    ASSERT(IrpTable != NULL);

    do
    {
        for (ix = 0; ix < 0x200; ix++)
        {
            irp = IrpTable->irp[ix];

            if (irp)
            {
                IoStack = IoGetCurrentIrpStackLocation(irp);
                urbIn = (PURB)(IoStack->Parameters.Others.Argument1);

                if (urbIn == Urb)
                {
                    if (irp == Irp)
                    {
                        KeBugCheckEx(0xFE, 4, (ULONG_PTR)irp, (ULONG_PTR)urbIn, 0);
                    }

                    KeBugCheckEx(0xFE, 2, (ULONG_PTR)irp, (ULONG_PTR)Irp, (ULONG_PTR)urbIn);
                }
            }
        }

        IrpTable = IrpTable->LinkNextTable;
    }
    while (IrpTable);
}
