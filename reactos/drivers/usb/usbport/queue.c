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

PIRP
NTAPI
USBPORT_FindIrpInTable(IN PUSBPORT_IRP_TABLE IrpTable,
                       IN PIRP Irp)
{
    ULONG ix;
    PIRP irp;

    DPRINT_CORE("USBPORT_FindIrpInTable: IrpTable - %p, Irp - %p\n", IrpTable, Irp);

    ASSERT(IrpTable != NULL);

    do
    {
        for (ix = 0; ix < 0x200; ix++)
        {
            irp = IrpTable->irp[ix];

            if (irp && irp == Irp)
            {
                return irp;
            }
        }

        IrpTable = IrpTable->LinkNextTable;
    }
    while (IrpTable->LinkNextTable);

    DPRINT_CORE("USBPORT_FindIrpInTable: Not found!!!\n");
    return NULL;
}

PIRP
NTAPI
USBPORT_FindActiveTransferIrp(IN PDEVICE_OBJECT FdoDevice,
                              IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_FindActiveTransferIrp: Irp - %p\n", Irp);
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    return USBPORT_FindIrpInTable(FdoExtension->ActiveIrpTable, Irp);
}

VOID
NTAPI
USBPORT_CancelActiveTransferIrp(IN PDEVICE_OBJECT DeviceObject,
                                IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    PURB Urb;
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_ENDPOINT Endpoint;
    PIRP irp;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_CancelTransferIrp: Irp - %p\n", Irp);

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    IoReleaseCancelSpinLock(Irp->CancelIrql);

    KeAcquireSpinLock(&FdoExtension->FlushTransferSpinLock, &OldIrql);

    irp = USBPORT_FindActiveTransferIrp(FdoDevice, Irp);

    if (irp)
    {
        IoStack = IoGetCurrentIrpStackLocation(irp);
        Urb = (PURB)(IoStack->Parameters.Others.Argument1);
        Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
        Endpoint = Transfer->Endpoint;

        DPRINT_CORE("USBPORT_CancelTransferIrp: irp - %p, Urb - %p, Transfer - %p\n", irp, Urb, Transfer);

        KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);
        Transfer->Flags |= TRANSFER_FLAG_CANCELED;
        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

        KeReleaseSpinLock(&FdoExtension->FlushTransferSpinLock, OldIrql);

        USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 1);
        return;
    }

    KeReleaseSpinLock(&FdoExtension->FlushTransferSpinLock, OldIrql);
}

VOID
NTAPI
USBPORT_FlushAbortList(IN PUSBPORT_ENDPOINT Endpoint)
{
    DPRINT_CORE("USBPORT_FlushCancelList: FIXME unimplemented\n");
}

VOID
NTAPI
USBPORT_FlushCancelList(IN PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_TRANSFER Transfer;
    PIRP Irp;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_FlushCancelList: FIXME FlushAbortList\n");

    FdoDevice = Endpoint->FdoDevice;

    while (!IsListEmpty(&Endpoint->CancelList))
    {
        Transfer = CONTAINING_RECORD(Endpoint->CancelList.Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(&Endpoint->CancelList);

        Irp = Transfer->Irp;

        if (Irp)
        {
            DPRINT1("USBPORT_FlushCancelList: Irp - %p\n", Irp);
            IoAcquireCancelSpinLock(&OldIrql);
            IoSetCancelRoutine(Irp, NULL);
            IoReleaseCancelSpinLock(OldIrql);

            USBPORT_RemoveActiveTransferIrp(FdoDevice, Irp);
        }

        USBPORT_CompleteTransfer(Transfer->Urb, USBD_STATUS_CANCELED);
    }

    USBPORT_FlushAbortList(Endpoint);
}

VOID
NTAPI
USBPORT_FlushPendingTransfers(IN PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN IsMapTransfer;
    BOOLEAN IsEnd = FALSE;
    PLIST_ENTRY List;
    PUSBPORT_TRANSFER Transfer;
    KIRQL PrevIrql;
    PURB Urb;
    PIRP Irp;
    PIRP irp;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_FlushPendingTransfers: Endpoint - %p\n", Endpoint);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (InterlockedCompareExchange(&Endpoint->FlushPendingLock, 1, 0))
    {
      DPRINT1("USBPORT_FlushPendingTransfers: Endpoint Locked \n");
      return;
    }

    while (TRUE)
    {
        IsMapTransfer = 0;

        if (!(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
        {
            if (!IsListEmpty(&Endpoint->TransferList))
            {
                List = Endpoint->TransferList.Flink;

                while (List && List != &Endpoint->TransferList)
                {
                    Transfer = CONTAINING_RECORD(List, USBPORT_TRANSFER, TransferLink);

                    if (!(Transfer->Flags & TRANSFER_FLAG_SUBMITED))
                    {
                        IsEnd = TRUE;
                        goto Worker;
                    }

                    List = Transfer->TransferLink.Flink;
                }
            }
        }

        List = Endpoint->PendingTransferList.Flink;

        if (IsListEmpty(&Endpoint->PendingTransferList) || List == NULL)
        {
            IsEnd = TRUE;
            goto Worker;
        }

        Transfer = CONTAINING_RECORD(List, USBPORT_TRANSFER, TransferLink);

        if (Transfer->Irp)
        {
            DPRINT_CORE("USBPORT_FlushPendingTransfers: Transfer->Irp->CancelRoutine - %p\n", Transfer->Irp->CancelRoutine);
        }

        if (Transfer->Irp && (IoSetCancelRoutine(Transfer->Irp, NULL) == NULL))
        {
            DPRINT_CORE("USBPORT_FlushPendingTransfers: Transfer->Irp - %p\n", Transfer->Irp);
            Transfer = NULL;
            IsEnd = TRUE;
        }

        if (!Transfer)
        {
            if (IsMapTransfer)
            {
                USBPORT_FlushMapTransfers(FdoDevice);
                goto Next;
            }

            goto Worker;
        }

        Irp = Transfer->Irp;
        Urb = Transfer->Urb;

        RemoveEntryList(&Transfer->TransferLink);
        Transfer->TransferLink.Flink = NULL;
        Transfer->TransferLink.Blink = NULL;

        irp = Irp;

        if (Irp)
        {
            irp = USBPORT_RemovePendingTransferIrp(FdoDevice, Irp);
        }

        KeAcquireSpinLock(&FdoExtension->FlushTransferSpinLock, &OldIrql);

        if (irp)
        {
            IoSetCancelRoutine(irp, USBPORT_CancelActiveTransferIrp);

            if (Irp->Cancel && IoSetCancelRoutine(irp, NULL))
            {
                DPRINT_CORE("USBPORT_FlushPendingTransfers: irp - %p\n", irp);
                KeReleaseSpinLock(&FdoExtension->FlushTransferSpinLock, OldIrql);
                USBPORT_CompleteTransfer(Transfer->Urb, USBD_STATUS_CANCELED);
                goto Worker;
            }

            USBPORT_FindUrbInIrpTable(FdoExtension->ActiveIrpTable, Urb, irp);
            USBPORT_InsertIrpInTable(FdoExtension->ActiveIrpTable, irp);
        }

        IsMapTransfer = USBPORT_QueueActiveUrbToEndpoint(Endpoint, Urb);

        KeReleaseSpinLock(&FdoExtension->FlushTransferSpinLock, OldIrql);

        if (IsMapTransfer)
        {
            USBPORT_FlushMapTransfers(FdoDevice);

            if (IsEnd)
                goto Next;
        }

Worker:
        KeRaiseIrql(DISPATCH_LEVEL, &PrevIrql);
        USBPORT_EndpointWorker(Endpoint, FALSE);
        KeLowerIrql(PrevIrql);

Next:
        if (IsEnd)
        {
            InterlockedDecrement(&Endpoint->FlushPendingLock);
            DPRINT_CORE("USBPORT_FlushPendingTransfers: Endpoint Unlocked. Exit\n");
            return;
        }
    }
}

VOID
NTAPI
USBPORT_CancelPendingTransferIrp(IN PDEVICE_OBJECT DeviceObject,
                                 IN PIRP Irp)
{
    DPRINT_CORE("USBPORT_CancelPendingTransferIrp: UNIMPLEMENTED. FIXME.\n");
    IoReleaseCancelSpinLock(Irp->CancelIrql);
}

VOID
NTAPI
USBPORT_QueuePendingUrbToEndpoint(IN PUSBPORT_ENDPOINT Endpoint,
                                  IN PURB Urb)
{
    PUSBPORT_TRANSFER Transfer;

    DPRINT_CORE("USBPORT_QueuePendingUrbToEndpoint: Endpoint - %p, Urb - %p\n", Endpoint, Urb);

    Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
    //FIXME USBPORT_ResetEndpointIdle();
    InsertTailList(&Endpoint->PendingTransferList, &Transfer->TransferLink);
    Urb->UrbHeader.Status = USBD_STATUS_PENDING;
}

BOOLEAN
NTAPI
USBPORT_QueueActiveUrbToEndpoint(IN PUSBPORT_ENDPOINT Endpoint,
                                 IN PURB Urb)
{
    PUSBPORT_TRANSFER Transfer;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;

    DPRINT_CORE("USBPORT_QueueActiveUrbToEndpoint: Endpoint - %p, Urb - %p\n", Endpoint, Urb);

    Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (Transfer->TransferParameters.TransferBufferLength == 0 ||
        !(Endpoint->Flags & ENDPOINT_FLAG_DMA_TYPE))
    {
      InsertTailList(&Endpoint->TransferList, &Transfer->TransferLink);
      DPRINT("USBPORT_QueueActiveUrbToEndpoint: return FALSE\n");
      return FALSE;
    }

    InsertTailList(&FdoExtension->MapTransferList, &Transfer->TransferLink);

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Transfer->Urb->UrbHeader.UsbdDeviceHandle;
    InterlockedIncrement(&DeviceHandle->DeviceHandleLock);

    DPRINT("USBPORT_QueueActiveUrbToEndpoint: return TRUE\n");
    return TRUE;
}

VOID
NTAPI
USBPORT_QueuePendingTransferIrp(IN PIRP Irp)
{
    PIO_STACK_LOCATION IoStack;
    PURB Urb;
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_ENDPOINT Endpoint;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_QueuePendingTransferIrp: Irp - %p\n", Irp);

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    Urb = (PURB)(IoStack->Parameters.Others.Argument1);

    Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
    Endpoint = Transfer->Endpoint;

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Irp->IoStatus.Status = STATUS_PENDING;
    IoMarkIrpPending(Irp);

    IoSetCancelRoutine(Irp, USBPORT_CancelPendingTransferIrp);

    if (Irp->Cancel && !IoSetCancelRoutine(Irp, NULL))
    {
        USBPORT_CompleteTransfer(Urb, USBD_STATUS_CANCELED);
    }
    else
    {
        USBPORT_InsertIrpInTable(FdoExtension->PendingIrpTable, Irp);
        USBPORT_QueuePendingUrbToEndpoint(Endpoint, Urb);
    }
}

VOID
NTAPI
USBPORT_QueueTransferUrb(IN PURB Urb)
{
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_ENDPOINT Endpoint;
    PIRP Irp;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;

    DPRINT_CORE("USBPORT_QueueTransferUrb: Urb - %p\n", Urb);

    if (Urb->UrbControlTransfer.TransferFlags & USBD_DEFAULT_PIPE_TRANSFER)
        Urb->UrbHeader.Function = URB_FUNCTION_CONTROL_TRANSFER;

    Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
    Endpoint = Transfer->Endpoint;

    Endpoint->Flags &= ~ENDPOINT_FLAG_QUEUENE_EMPTY;

    Transfer->TransferParameters.TransferBufferLength = Urb->UrbControlTransfer.TransferBufferLength;
    Transfer->TransferParameters.TransferFlags = Urb->UrbControlTransfer.TransferFlags;

    Transfer->TransferBufferMDL = Urb->UrbControlTransfer.TransferBufferMDL;
    Transfer->Direction = ((Urb->UrbControlTransfer.TransferFlags & 1) == 0) + 1;

    if (Endpoint->EndpointProperties.TransferType == USBPORT_TRANSFER_TYPE_CONTROL)
    {
        RtlCopyMemory(&Transfer->TransferParameters.SetupPacket,
                      Urb->UrbControlTransfer.SetupPacket,
                      sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));
    }

    DPRINT_URB("... URB TransferBufferLength - %x\n",
           Urb->UrbControlTransfer.TransferBufferLength);

    Urb->UrbControlTransfer.TransferBufferLength = 0;

    Irp = Transfer->Irp;

    if (Irp)
    {
        USBPORT_QueuePendingTransferIrp(Irp);
    }
    else
    {
        USBPORT_QueuePendingUrbToEndpoint(Endpoint, Urb);
    }

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;
    InterlockedDecrement(&DeviceHandle->DeviceHandleLock);

    USBPORT_FlushPendingTransfers(Endpoint);

    DPRINT_URB("... URB TransferBufferLength - %x\n",
           Urb->UrbControlTransfer.TransferBufferLength);

    if (Urb->UrbControlTransfer.TransferBufferLength)
    {
        ULONG Buffer;
        ULONG BufferLength;
        ULONG BufferEnd;
        ULONG ix;

        Buffer = (ULONG)Urb->UrbControlTransfer.TransferBuffer;
        BufferLength = Urb->UrbControlTransfer.TransferBufferLength;
        BufferEnd = Buffer + BufferLength;

        DPRINT_URB("URB TransferBuffer - %p\n", Buffer);

        for (ix = 0; (Buffer + ix * 4) <= BufferEnd; ix++)
        {
            DPRINT_URB("Buffer[%02X] - %p\n", ix, *(PULONG)(Buffer + ix * 4));
        }
    }
}

VOID
NTAPI
USBPORT_FlushAllEndpoints(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    PLIST_ENTRY Entry;
    PUSBPORT_ENDPOINT Endpoint;
    LIST_ENTRY List;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_FlushAllEndpoints: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

    InitializeListHead(&List);

    if (!IsListEmpty(&FdoExtension->EndpointList) && FdoExtension->EndpointList.Flink)
    {
        Entry = FdoExtension->EndpointList.Flink;

        while (Entry != &FdoExtension->EndpointList)
        {
            Endpoint = CONTAINING_RECORD(Entry, USBPORT_ENDPOINT, EndpointLink);

            if (Endpoint->StateLast != 5)
            {
                InsertTailList(&List, &Endpoint->FlushLink);
            }

            Entry = Endpoint->EndpointLink.Flink;

            if (!Entry)
                break;
        }
    }

    KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);

    while (TRUE)
    {
        if (IsListEmpty(&List))
            break;

        Endpoint = CONTAINING_RECORD(List.Flink, USBPORT_ENDPOINT, FlushLink);

        RemoveHeadList(&List);

        Endpoint->FlushLink.Flink = NULL;
        Endpoint->FlushLink.Blink = NULL;

        if (!IsListEmpty(&Endpoint->PendingTransferList))
        {
            USBPORT_FlushPendingTransfers(Endpoint);
        }
    }

    DPRINT_CORE("USBPORT_FlushAllEndpoints: exit\n");
}

ULONG
NTAPI
USBPORT_KillEndpointActiveTransfers(IN PDEVICE_OBJECT FdoDevice,
                                    IN PUSBPORT_ENDPOINT Endpoint)
{
    PLIST_ENTRY ActiveList;
    PUSBPORT_TRANSFER Transfer;
    ULONG KilledTransfers = 0;

    DPRINT("USBPORT_KillEndpointActiveTransfers \n");

    ActiveList = Endpoint->TransferList.Flink;

    if (!IsListEmpty(&Endpoint->TransferList))
    {
        while (ActiveList && ActiveList != &Endpoint->TransferList)
        {
            ++KilledTransfers;

            Transfer = CONTAINING_RECORD(ActiveList,
                                         USBPORT_TRANSFER,
                                         TransferLink);

            Transfer->Flags |= TRANSFER_FLAG_ABORTED;

            ActiveList = Transfer->TransferLink.Flink;
        }
    }

    USBPORT_FlushPendingTransfers(Endpoint);
    USBPORT_FlushCancelList(Endpoint);

    return KilledTransfers;
}

VOID
NTAPI
USBPORT_FlushController(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    PLIST_ENTRY Entry;
    PUSBPORT_ENDPOINT Endpoint;
    ULONG KilledTransfers = 0;
    PLIST_ENTRY EndpointList;
    KIRQL OldIrql;
    LIST_ENTRY FlushList;

    DPRINT("USBPORT_FlushController \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    EndpointList = &FdoExtension->EndpointList;

    while (TRUE)
    {
        KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

        InitializeListHead(&FlushList);

        Entry = EndpointList->Flink;

        if (!IsListEmpty(EndpointList))
        {
            while (Entry && Entry != EndpointList)
            {
                Endpoint = CONTAINING_RECORD(Entry, USBPORT_ENDPOINT, EndpointLink);

                if (Endpoint->StateLast != 4 && Endpoint->StateLast != 5)
                {
                    InterlockedIncrement(&Endpoint->LockCounter);
                    InsertTailList(&FlushList, &Endpoint->FlushControllerLink);
                }

                Entry = Endpoint->EndpointLink.Flink;
            }
        }

        KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);

        if (IsListEmpty(&FlushList))
            break;

        do
        {
            Endpoint = CONTAINING_RECORD(FlushList.Flink, USBPORT_ENDPOINT, FlushControllerLink);

            RemoveHeadList(&FlushList);
            KilledTransfers += USBPORT_KillEndpointActiveTransfers(FdoDevice, Endpoint);
            InterlockedDecrement(&Endpoint->LockCounter);
        }
        while (!IsListEmpty(&FlushList));

        if (!KilledTransfers)
            break;

        USBPORT_Wait(FdoDevice, 100);
    }
}
