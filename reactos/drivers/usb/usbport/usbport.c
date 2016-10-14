#include "usbport.h"

//#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_MINIPORT
#define NDEBUG_USBPORT_CORE
#define NDEBUG_USBPORT_URB
#define NDEBUG_USBPORT_INTERRUPT
#define NDEBUG_USBPORT_TIMER
#include "usbdebug.h"

LIST_ENTRY USBPORT_MiniPortDrivers = {NULL, NULL};
LIST_ENTRY USBPORT_USB1FdoList = {NULL, NULL};
LIST_ENTRY USBPORT_USB2FdoList = {NULL, NULL};

KSPIN_LOCK USBPORT_SpinLock = 0;
BOOLEAN USBPORT_Initialized = FALSE;

VOID
NTAPI
USBPORT_AddUSB1Fdo(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_AddUSB1Fdo: FdoDevice - %p\n", FdoDevice);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    FdoExtension->Flags |= USBPORT_FLAG_REGISTERED_FDO;

    ExInterlockedInsertTailList(&USBPORT_USB1FdoList,
                                &FdoExtension->ControllerLink,
                                &USBPORT_SpinLock);
}

VOID
NTAPI
USBPORT_AddUSB2Fdo(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_AddUSB2Fdo: FdoDevice - %p\n", FdoDevice);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    FdoExtension->Flags |= USBPORT_FLAG_REGISTERED_FDO;

    ExInterlockedInsertTailList(&USBPORT_USB2FdoList,
                                &FdoExtension->ControllerLink,
                                &USBPORT_SpinLock);
}

VOID
NTAPI
USBPORT_RemoveUSBxFdo(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;

    DPRINT("USBPORT_RemoveUSBxFdo: FdoDevice - %p\n", FdoDevice);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&USBPORT_SpinLock, &OldIrql);
    RemoveEntryList(&FdoExtension->ControllerLink);
    KeReleaseSpinLock(&USBPORT_SpinLock, OldIrql);

    FdoExtension->Flags &= ~USBPORT_FLAG_REGISTERED_FDO;

    FdoExtension->ControllerLink.Flink = NULL;
    FdoExtension->ControllerLink.Blink = NULL;
}

MPSTATUS
NTAPI
USBPORT_NtStatusToMpStatus(NTSTATUS NtStatus)
{
    DPRINT("USBPORT_NtStatusToMpStatus: NtStatus - %x\n", NtStatus);

    if (NtStatus == STATUS_SUCCESS)
    {
        return 0;
    }
    else
    {
        return 8;
    }
}

NTSTATUS
NTAPI
USBPORT_SetRegistryKeyValue(IN PDEVICE_OBJECT DeviceObject,
                            IN HANDLE KeyHandle,
                            IN ULONG Type,
                            IN PCWSTR ValueNameString,
                            IN PVOID Data,
                            IN ULONG DataSize)
{
    UNICODE_STRING ValueName;
    NTSTATUS Status;

    DPRINT("USBPORT_SetRegistryKeyValue: ValueNameString - %S \n", ValueNameString);

    if (KeyHandle)
    {
        Status = IoOpenDeviceRegistryKey(DeviceObject,
                                         PLUGPLAY_REGKEY_DRIVER,
                                         STANDARD_RIGHTS_ALL,
                                         &KeyHandle);
    }
    else
    {
        Status = IoOpenDeviceRegistryKey(DeviceObject,
                                         PLUGPLAY_REGKEY_DEVICE,
                                         STANDARD_RIGHTS_ALL,
                                         &KeyHandle);
    }

    if (NT_SUCCESS(Status))
    {
        RtlInitUnicodeString(&ValueName, ValueNameString);

        Status = ZwSetValueKey(KeyHandle,
                               &ValueName,
                               0,
                               Type,
                               Data,
                               DataSize);

        ZwClose(KeyHandle);
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_GetRegistryKeyValueFullInfo(IN PDEVICE_OBJECT FdoDevice,
                                    IN PDEVICE_OBJECT PdoDevice,
                                    IN ULONG Type,
                                    IN PCWSTR SourceString,
                                    IN ULONG LengthStr,
                                    IN PVOID Buffer,
                                    IN ULONG NumberOfBytes)
{
    NTSTATUS Status;
    PKEY_VALUE_FULL_INFORMATION KeyValue;
    UNICODE_STRING ValueName;
    HANDLE KeyHandle;
    ULONG LengthKey;

    DPRINT("USBPORT_GetRegistryKeyValue: Type - %x, SourceString - %S, LengthStr - %x, Buffer - %p, NumberOfBytes - %x\n",
           Type,
           SourceString,
           LengthStr,
           Buffer,
           NumberOfBytes);

    if (Type)
    {
        Status = IoOpenDeviceRegistryKey(PdoDevice,
                                         PLUGPLAY_REGKEY_DRIVER,
                                         STANDARD_RIGHTS_ALL,
                                         &KeyHandle);
    }
    else
    {
        Status = IoOpenDeviceRegistryKey(PdoDevice,
                                         PLUGPLAY_REGKEY_DEVICE,
                                         STANDARD_RIGHTS_ALL,
                                         &KeyHandle);
    }

    if (NT_SUCCESS(Status))
    {
        RtlInitUnicodeString(&ValueName, SourceString);
        LengthKey = sizeof(KEY_VALUE_FULL_INFORMATION) + LengthStr + NumberOfBytes;

        KeyValue = (PKEY_VALUE_FULL_INFORMATION)ExAllocatePoolWithTag(PagedPool,
                                                                      LengthKey,
                                                                      USB_PORT_TAG);

        if (KeyValue)
        {
            RtlZeroMemory(KeyValue, LengthKey);

            Status = ZwQueryValueKey(KeyHandle,
                                     &ValueName,
                                     KeyValueFullInformation,
                                     KeyValue,
                                     LengthKey,
                                     &LengthKey);

            if (NT_SUCCESS(Status))
            {
                RtlCopyMemory(Buffer,
                              (PUCHAR)KeyValue + KeyValue->DataOffset,
                              NumberOfBytes);
            }

            ExFreePool(KeyValue);
        }

        ZwClose(KeyHandle);
    }

    return Status;
}

MPSTATUS
NTAPI
USBPORT_GetMiniportRegistryKeyValue(IN PVOID Context,
                                    IN ULONG Type,
                                    IN PCWSTR SourceString,
                                    IN SIZE_T LengthStr,
                                    IN PVOID Buffer,
                                    IN SIZE_T NumberOfBytes)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;
    NTSTATUS Status;

    DPRINT("USBPORT_GetMiniportRegistryKeyValue: Context - %p, Type - %x, SourceString - %S, LengthStr - %x, Buffer - %p, NumberOfBytes - %x\n",
           Context,
           Type,
           SourceString,
           LengthStr,
           Buffer,
           NumberOfBytes);

    //DbgBreakPoint();

    //FdoExtension->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtension + sizeof(USBPORT_DEVICE_EXTENSION));
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context -
                                               sizeof(USBPORT_DEVICE_EXTENSION));

    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    Status = USBPORT_GetRegistryKeyValueFullInfo(FdoDevice,
                                                 FdoExtension->CommonExtension.LowerPdoDevice,
                                                 Type,
                                                 SourceString,
                                                 LengthStr,
                                                 Buffer,
                                                 NumberOfBytes);

    return USBPORT_NtStatusToMpStatus(Status);
}

NTSTATUS
NTAPI
USBPORT_GetSetConfigSpaceData(IN PDEVICE_OBJECT FdoDevice,
                              IN BOOLEAN IsReadData,
                              IN PVOID Buffer,
                              IN ULONG Offset,
                              IN ULONG Length)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    ULONG BytesReadWrite;

    DPRINT("USBPORT_ReadWriteConfigSpace ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    BytesReadWrite = Length;

    if (IsReadData)
    {
        RtlZeroMemory(Buffer, Length);

        BytesReadWrite = (*FdoExtension->BusInterface.GetBusData)(FdoExtension->BusInterface.Context,
                                                                  PCI_WHICHSPACE_CONFIG,
                                                                  Buffer,
                                                                  Offset,
                                                                  Length);
    }
    else
    {
        BytesReadWrite = (*FdoExtension->BusInterface.SetBusData)(FdoExtension->BusInterface.Context,
                                                                  PCI_WHICHSPACE_CONFIG,
                                                                  Buffer,
                                                                  Offset,
                                                                  Length);
    }

    if (BytesReadWrite == Length)
    {
        return STATUS_SUCCESS;
    }

    return STATUS_UNSUCCESSFUL;
}

MPSTATUS
NTAPI
USBPORT_ReadWriteConfigSpace(IN PVOID Context,
                             IN BOOLEAN IsReadData,
                             IN PVOID Buffer,
                             IN ULONG Offset,
                             IN ULONG Length)
{
    NTSTATUS Status;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;

    DPRINT("USBPORT_ReadWriteConfigSpace: ... \n");

    //FdoExtension->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtension + sizeof(USBPORT_DEVICE_EXTENSION));
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context - sizeof(USBPORT_DEVICE_EXTENSION));
    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    Status = USBPORT_GetSetConfigSpaceData(FdoDevice,
                                           IsReadData,
                                           Buffer,
                                           Offset,
                                           Length);

    return USBPORT_NtStatusToMpStatus(Status);
}

NTSTATUS
NTAPI
USBPORT_USBDStatusToNtStatus(IN PURB Urb,
                             IN USBD_STATUS USBDStatus)
{
    NTSTATUS Status;

    if (USBD_ERROR(USBDStatus))
    {
        DPRINT("USBPORT_USBDStatusToNtStatus: Urb - %p, USBDStatus - %x\n",
               Urb,
               USBDStatus);
    }

    if (Urb)
        Urb->UrbHeader.Status = USBDStatus;

    switch (USBDStatus)
    {
        case USBD_STATUS_SUCCESS:
            Status = STATUS_SUCCESS;
            break;

        case USBD_STATUS_INSUFFICIENT_RESOURCES:
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;

        case USBD_STATUS_DEVICE_GONE:
            Status = STATUS_DEVICE_NOT_CONNECTED;
            break;

        case USBD_STATUS_CANCELED:
            Status = STATUS_CANCELLED;
            break;

        case USBD_STATUS_NOT_SUPPORTED:
            Status = STATUS_NOT_SUPPORTED;
            break;

        case USBD_STATUS_INVALID_URB_FUNCTION:
        case USBD_STATUS_INVALID_PARAMETER:
        case USBD_STATUS_INVALID_PIPE_HANDLE:
        case USBD_STATUS_BAD_START_FRAME:
            Status = STATUS_INVALID_PARAMETER;
            break;

        default:
            if (USBD_ERROR(USBDStatus))
                Status = STATUS_UNSUCCESSFUL;
            break;
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_Wait(IN PVOID Context,
             IN ULONG Milliseconds)
{
    LARGE_INTEGER Interval = {{0, 0}};

    DPRINT("USBPORT_Wait: Milliseconds - %x\n", Milliseconds);
    Interval.QuadPart -= 10000 * Milliseconds + (KeQueryTimeIncrement() - 1);
    return KeDelayExecutionThread(KernelMode, FALSE, &Interval);
}

VOID
NTAPI
USBPORT_MiniportInterrupts(IN PDEVICE_OBJECT FdoDevice,
                           IN BOOLEAN IsEnable)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN IsLock;
    KIRQL OldIrql;

    DPRINT_INT("USBPORT_MiniportInterrupts: IsEnable - %p\n", IsEnable);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    IsLock = ~(UCHAR)(FdoExtension->MiniPortInterface->Packet.MiniPortFlags >> 6) & 1;

    if (IsLock)
        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);

    if (IsEnable)
    {
        FdoExtension->Flags |= USBPORT_FLAG_INTERRUPT_ENABLED;
        FdoExtension->MiniPortInterface->Packet.EnableInterrupts(FdoExtension->MiniPortExt);
    }
    else
    {
        FdoExtension->MiniPortInterface->Packet.DisableInterrupts(FdoExtension->MiniPortExt);
        FdoExtension->Flags &= ~USBPORT_FLAG_INTERRUPT_ENABLED;
    }

    if (IsLock)
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
}

VOID
NTAPI
USBPORT_SoftInterruptDpc(IN PRKDPC Dpc,
                         IN PVOID DeferredContext,
                         IN PVOID SystemArgument1,
                         IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_SoftInterruptDpc: ... \n");

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (!KeInsertQueueDpc(&FdoExtension->IsrDpc, NULL, (PVOID)1))
    {
        InterlockedDecrement(&FdoExtension->IsrDpcCounter);
    }
}

VOID
NTAPI
USBPORT_SoftInterrupt(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    LARGE_INTEGER DueTime = {{0, 0}};

    DPRINT("USBPORT_SoftInterrupt: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeInitializeTimer(&FdoExtension->TimerSoftInterrupt);

    KeInitializeDpc(&FdoExtension->SoftInterruptDpc,
                    USBPORT_SoftInterruptDpc,
                    FdoDevice);

    DueTime.QuadPart -= 10000 + (KeQueryTimeIncrement() - 1);

    KeSetTimer(&FdoExtension->TimerSoftInterrupt,
               DueTime,
               &FdoExtension->SoftInterruptDpc);
}

VOID
NTAPI
USBPORT_InvalidateControllerHandler(IN PDEVICE_OBJECT FdoDevice,
                                    IN ULONG Type)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_InvalidateControllerHandler: Invalidate Type - %x\n", Type);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    switch (Type)
    {
        case INVALIDATE_CONTROLLER_RESET:
            DPRINT1("USBPORT_InvalidateControllerHandler: INVALIDATE_CONTROLLER_RESET UNIMPLEMENTED. FIXME. \n");
            break;

        case INVALIDATE_CONTROLLER_SURPRISE_REMOVE:
            DPRINT1("USBPORT_InvalidateControllerHandler: INVALIDATE_CONTROLLER_SURPRISE_REMOVE UNIMPLEMENTED. FIXME. \n");
            break;

        case INVALIDATE_CONTROLLER_SOFT_INTERRUPT:
            if (InterlockedIncrement(&FdoExtension->IsrDpcCounter))
            {
                InterlockedDecrement(&FdoExtension->IsrDpcCounter);
            }
            else
            {
                USBPORT_SoftInterrupt(FdoDevice);
            }
            break;
    }
}

ULONG
NTAPI
USBPORT_InvalidateController(IN PVOID Context,
                             IN ULONG Type)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;

    DPRINT("USBPORT_InvalidateController: Invalidate Type - %x\n", Type);

    //FdoExtension->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtension + sizeof(USBPORT_DEVICE_EXTENSION));
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context -
                                               sizeof(USBPORT_DEVICE_EXTENSION));
    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    USBPORT_InvalidateControllerHandler(FdoDevice, Type);

    return 0;
}

ULONG
NTAPI
USBPORT_NotifyDoubleBuffer(IN PVOID Context1,
                           IN PVOID Context2,
                           IN PVOID Buffer,
                           IN SIZE_T Length)
{
    DPRINT1("USBPORT_NotifyDoubleBuffer: UNIMPLEMENTED. FIXME. \n");
    return 0;
}

VOID
NTAPI
USBPORT_WorkerRequestDpc(IN PRKDPC Dpc,
                         IN PVOID DeferredContext,
                         IN PVOID SystemArgument1,
                         IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_WorkerRequestDpc: ... \n");

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (!InterlockedIncrement(&FdoExtension->IsrDpcHandlerCounter))
    {
        USBPORT_DpcHandler(FdoDevice);
    }

    InterlockedDecrement(&FdoExtension->IsrDpcHandlerCounter);
}

VOID
NTAPI
USBPORT_NukeAllEndpoints(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    PLIST_ENTRY EndpointList;
    PUSBPORT_ENDPOINT Endpoint;
    KIRQL OldIrql;

    DPRINT("USBPORT_NukeAllEndpoints \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

    EndpointList = FdoExtension->EndpointList.Flink;

    if (!IsListEmpty(&FdoExtension->EndpointList))
    {
        while (EndpointList && (EndpointList != &FdoExtension->EndpointList))
        {
            Endpoint = CONTAINING_RECORD(EndpointList,
                                         USBPORT_ENDPOINT,
                                         EndpointLink);

            if (!(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
                Endpoint->Flags |= ENDPOINT_FLAG_NUKE;

            EndpointList = Endpoint->EndpointLink.Flink;
        }
    }

    KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);
}

VOID
NTAPI
USBPORT_InvalidateEndpointHandler(IN PDEVICE_OBJECT FdoDevice,
                                  IN PUSBPORT_ENDPOINT Endpoint,
                                  IN ULONG Type)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    PLIST_ENTRY                Entry;
    PLIST_ENTRY                WorkerLink;
    PUSBPORT_ENDPOINT          endpoint;
    KIRQL                      OldIrql;
    BOOLEAN                    IsAddEntry = FALSE;

    DPRINT_CORE("USBPORT_InvalidateEndpointHandler: Endpoint - %p, Type - %x\n", Endpoint, Type);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (Endpoint)
    {
        WorkerLink = &Endpoint->WorkerLink;
        KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

        if ((!WorkerLink->Flink || !WorkerLink->Blink) &&
            !(Endpoint->Flags & ENDPOINT_FLAG_IDLE) &&
            Endpoint->StateLast != 5)
        {
            InsertTailList(&FdoExtension->WorkerList, WorkerLink);
            IsAddEntry = TRUE;
        }

        KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);

        if (Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0)
            Type = 1;
    }
    else
    {
        KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

        Entry = &FdoExtension->EndpointList;

        if (Entry != &FdoExtension->EndpointList)
        {
            while (Entry && Entry != &FdoExtension->EndpointList)
            {
                endpoint = CONTAINING_RECORD(Entry,
                                             USBPORT_ENDPOINT,
                                             EndpointLink);

                if (!endpoint->WorkerLink.Flink || !endpoint->WorkerLink.Blink)
                {
                    if (!(endpoint->Flags & ENDPOINT_FLAG_IDLE) &&
                        !(endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0) &&
                        endpoint->StateLast != 5)
                    {
                        InsertTailList(&FdoExtension->WorkerList, &endpoint->WorkerLink);
                        IsAddEntry = TRUE;
                    }
                }

                Entry = endpoint->EndpointLink.Flink;
            }
        }

        KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);
    }

    if (FdoExtension->Flags & USBPORT_FLAG_HC_SUSPEND)
    {
        Type = 1;
    }
    else if (IsAddEntry == FALSE && Type == 3)
    {
        Type = 0;
    }

    switch (Type)
    {
        case 1:
            USBPORT_SignalWorkerThread(FdoDevice);
            break;

        case 2:
            KeInsertQueueDpc(&FdoExtension->WorkerRequestDpc, NULL, NULL);
            break;

        case 3:
            KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
            FdoExtension->MiniPortInterface->Packet.InterruptNextSOF(FdoExtension->MiniPortExt);
            KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
            break;
    }
}

BOOLEAN
NTAPI
USBPORT_EndpointHasQueuedTransfers(IN PDEVICE_OBJECT FdoDevice,
                                   IN PUSBPORT_ENDPOINT Endpoint,
                                   IN PULONG TransferCount)
{
    PLIST_ENTRY Entry;
    PUSBPORT_TRANSFER Transfer;
    BOOLEAN Result = FALSE;

    DPRINT_CORE("USBPORT_EndpointHasQueuedTransfers: TransferCount - %p\n", TransferCount);

    KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);

    if (!IsListEmpty(&Endpoint->PendingTransferList))
        Result = TRUE;

    if (!IsListEmpty(&Endpoint->TransferList))
    {
        Result = TRUE;

        if (TransferCount)
        {
            *TransferCount = 0;

            for (Entry = Endpoint->TransferList.Flink;
                 Entry && Entry != &Endpoint->TransferList;
                 Entry = Transfer->TransferLink.Flink)
            {
                Transfer = CONTAINING_RECORD(Entry,
                                             USBPORT_TRANSFER,
                                             TransferLink);

                if (Transfer->Flags & TRANSFER_FLAG_SUBMITED)
                {
                    ++*TransferCount;
                }
            }
        }
    }

    KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

    return Result;
}

VOID
NTAPI
USBPORT_DoneTransfer(IN PUSBPORT_TRANSFER Transfer)
{
    PUSBPORT_ENDPOINT          Endpoint;
    PDEVICE_OBJECT             FdoDevice;
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    PURB                       Urb;
    PIRP                       Irp;
    KIRQL                      CancelIrql;
    KIRQL                      OldIrql;

    DPRINT_CORE("USBPORT_DoneTransfer: Transfer - %p\n", Transfer);

    Endpoint = Transfer->Endpoint;
    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    Urb = Transfer->Urb;
    Irp = Transfer->Irp;

    KeAcquireSpinLock(&FdoExtension->FlushTransferSpinLock, &OldIrql);

    if (Irp)
    {
        IoAcquireCancelSpinLock(&CancelIrql);
        IoSetCancelRoutine(Irp, NULL);
        IoReleaseCancelSpinLock(CancelIrql);

        USBPORT_RemoveActiveTransferIrp(FdoDevice, Irp);
    }

    KeReleaseSpinLock(&FdoExtension->FlushTransferSpinLock, OldIrql);

    USBPORT_USBDStatusToNtStatus(Transfer->Urb, Transfer->USBDStatus);
    USBPORT_CompleteTransfer(Urb, Urb->UrbHeader.Status);

    DPRINT_CORE("USBPORT_DoneTransfer: exit\n");
}

VOID
NTAPI
USBPORT_FlushDoneTransfers(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY DoneTransferList;
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_ENDPOINT Endpoint;
    ULONG TransferCount;
    KIRQL OldIrql;
    BOOLEAN IsHasTransfers;

    DPRINT_CORE("USBPORT_FlushDoneTransfers: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DoneTransferList = &FdoExtension->DoneTransferList;

    while (TRUE)
    {
        KeAcquireSpinLock(&FdoExtension->DoneTransferSpinLock, &OldIrql);

        if (IsListEmpty(DoneTransferList))
            break;

        Transfer = CONTAINING_RECORD(DoneTransferList->Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(DoneTransferList);
        KeReleaseSpinLock(&FdoExtension->DoneTransferSpinLock, OldIrql);

        if (Transfer)
        {
            Endpoint = Transfer->Endpoint;

            USBPORT_DoneTransfer(Transfer);

            IsHasTransfers = USBPORT_EndpointHasQueuedTransfers(FdoDevice, Endpoint, &TransferCount);

            if (IsHasTransfers && !TransferCount)
            {
                USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 2);
            }
        }
    }

    KeReleaseSpinLock(&FdoExtension->DoneTransferSpinLock, OldIrql);
}


VOID
NTAPI
USBPORT_TransferFlushDpc(IN PRKDPC Dpc,
                         IN PVOID DeferredContext,
                         IN PVOID SystemArgument1,
                         IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;

    DPRINT_CORE("USBPORT_TransferFlushDpc: ... \n");
    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    USBPORT_FlushDoneTransfers(FdoDevice);
}

BOOLEAN
NTAPI
USBPORT_QueueDoneTransfer(IN PUSBPORT_TRANSFER Transfer,
                          IN USBD_STATUS USBDStatus)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;

    DPRINT_CORE("USBPORT_QueueDoneTransfer: Transfer - %p, USBDStatus - %p\n", Transfer, USBDStatus);

    FdoDevice = Transfer->Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    RemoveEntryList(&Transfer->TransferLink);
    Transfer->USBDStatus = USBDStatus;

    ExInterlockedInsertTailList(&FdoExtension->DoneTransferList,
                                &Transfer->TransferLink,
                                &FdoExtension->DoneTransferSpinLock);

    return KeInsertQueueDpc(&FdoExtension->TransferFlushDpc, NULL, NULL);
}

VOID
NTAPI
USBPORT_DpcHandler(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_ENDPOINT Endpoint;
    PLIST_ENTRY Entry;
    LIST_ENTRY List;

    DPRINT_CORE("USBPORT_DpcHandler: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    InitializeListHead(&List);

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);
    Entry = FdoExtension->EndpointList.Flink;

    if (!IsListEmpty(&FdoExtension->EndpointList))
    {
        while (Entry && Entry != &FdoExtension->EndpointList)
        {
            Endpoint = CONTAINING_RECORD(Entry,
                                         USBPORT_ENDPOINT,
                                         EndpointLink);

            if (Endpoint->StateLast == USBPORT_ENDPOINT_ACTIVE &&
                !InterlockedIncrement(&Endpoint->LockCounter) &&
                !(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
            {
                InsertTailList(&List, &Endpoint->DispatchLink);

                if (Endpoint->WorkerLink.Flink && Endpoint->WorkerLink.Blink)
                {
                    RemoveEntryList(&Endpoint->WorkerLink);

                    Endpoint->WorkerLink.Flink = NULL;
                    Endpoint->WorkerLink.Blink = NULL;
                }

            }
            else
            {
                InterlockedDecrement(&Endpoint->LockCounter);
            }

            Entry = Endpoint->EndpointLink.Flink;
        }
    }

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

    while (!IsListEmpty(&List))
    {
        Endpoint = CONTAINING_RECORD(List.Flink,
                                     USBPORT_ENDPOINT,
                                     DispatchLink);

        RemoveEntryList(List.Flink);
        Endpoint->DispatchLink.Flink = NULL;
        Endpoint->DispatchLink.Blink = NULL;

        USBPORT_EndpointWorker(Endpoint, TRUE);
        USBPORT_FlushPendingTransfers(Endpoint);
    }

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);

    if (!IsListEmpty(&FdoExtension->WorkerList))
    {
        USBPORT_SignalWorkerThread(FdoDevice);
    }

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

    USBPORT_FlushDoneTransfers(FdoDevice);
}

VOID
NTAPI
USBPORT_IsrDpcHandler(IN PDEVICE_OBJECT FdoDevice,
                      IN BOOLEAN IsDpcHandler)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_ENDPOINT Endpoint;
    PLIST_ENTRY List;
    ULONG FrameNumber;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_IsrDpcHandler: IsDpcHandler - %x\n", IsDpcHandler);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (InterlockedIncrement(&FdoExtension->IsrDpcHandlerCounter))
    {
        KeInsertQueueDpc(&FdoExtension->IsrDpc, NULL, NULL);
        InterlockedDecrement(&FdoExtension->IsrDpcHandlerCounter);
        return;
    }

    for (List = ExInterlockedRemoveHeadList(&FdoExtension->EpStateChangeList,
                                            &FdoExtension->EpStateChangeSpinLock);
         List != NULL;
         List = ExInterlockedRemoveHeadList(&FdoExtension->EpStateChangeList,
                                            &FdoExtension->EpStateChangeSpinLock))
    {
        Endpoint = CONTAINING_RECORD(List,
                                     USBPORT_ENDPOINT,
                                     StateChangeLink);

        DPRINT_CORE("USBPORT_IsrDpcHandler: Endpoint - %p\n", Endpoint);

        KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FrameNumber = FdoExtension->MiniPortInterface->Packet.Get32BitFrameNumber(FdoExtension->MiniPortExt);
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

        if (FrameNumber <= Endpoint->FrameNumber && !(Endpoint->Flags & ENDPOINT_FLAG_NUKE))
        {
            KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

            ExInterlockedInsertHeadList(&FdoExtension->EpStateChangeList,
                                        &Endpoint->StateChangeLink,
                                        &FdoExtension->EpStateChangeSpinLock);

            KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
            FdoExtension->MiniPortInterface->Packet.InterruptNextSOF(FdoExtension->MiniPortExt);
            KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

            break;
        }

        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

        KeAcquireSpinLock(&Endpoint->StateChangeSpinLock, &Endpoint->EndpointStateOldIrql);
        Endpoint->StateLast = Endpoint->StateNext;
        KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);

        DPRINT_CORE("USBPORT_IsrDpcHandler: Endpoint->StateLast - %x\n", Endpoint->StateLast);

        if (IsDpcHandler)
        {
            USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 0);
        }
        else
        {
            USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 1);
        }
    }

    if (IsDpcHandler)
    {
        USBPORT_DpcHandler(FdoDevice);
    }

    InterlockedDecrement(&FdoExtension->IsrDpcHandlerCounter);
}

VOID
NTAPI
USBPORT_IsrDpc(IN PRKDPC Dpc,
               IN PVOID DeferredContext,
               IN PVOID SystemArgument1,
               IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN InterruptEnable;

    DPRINT_INT("USBPORT_IsrDpc: DeferredContext - %p, SystemArgument2 - %p\n", DeferredContext, SystemArgument2);

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (SystemArgument2)
    {
        InterlockedDecrement(&FdoExtension->IsrDpcCounter);
    }

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->MiniportInterruptsSpinLock);
    InterruptEnable = (FdoExtension->Flags & USBPORT_FLAG_INTERRUPT_ENABLED) == USBPORT_FLAG_INTERRUPT_ENABLED;

    FdoExtension->MiniPortInterface->Packet.InterruptDpc(FdoExtension->MiniPortExt,
                                                         InterruptEnable);

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->MiniportInterruptsSpinLock);

    if (FdoExtension->Flags & USBPORT_FLAG_HC_SUSPEND &&
        FdoExtension->TimerFlags & USBPORT_TMFLAG_WAKE)
    {
        USBPORT_CompletePdoWaitWake(FdoDevice);
    }
    else
    {
        USBPORT_IsrDpcHandler(FdoDevice, TRUE);
    }

    DPRINT_INT("USBPORT_IsrDpc: exit\n");
}

BOOLEAN
NTAPI
USBPORT_InterruptService(IN PKINTERRUPT Interrupt,
                         IN PVOID ServiceContext)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN Result = 0;

    FdoDevice = (PDEVICE_OBJECT)ServiceContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    DPRINT_INT("USBPORT_InterruptService: FdoExtension->Flags - %p\n",
           FdoExtension->Flags);

    if (FdoExtension->Flags & USBPORT_FLAG_INTERRUPT_ENABLED)
    {
        if (FdoExtension->MiniPortFlags & USBPORT_MPFLAG_INTERRUPTS_ENABLED)
        {
            Result = FdoExtension->MiniPortInterface->Packet.InterruptService(FdoExtension->MiniPortExt);

            if (Result)
            {
                KeInsertQueueDpc(&FdoExtension->IsrDpc, NULL, NULL);
            }
        }
    }
    else
    {
        Result = 0;
    }

    DPRINT_INT("USBPORT_InterruptService: return - %x\n", Result);

    return Result;
}

VOID
NTAPI
USBPORT_SignalWorkerThread(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_SignalWorkerThread ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->WorkerThreadEventSpinLock, &OldIrql);
    KeSetEvent(&FdoExtension->WorkerThreadEvent, 1, FALSE);
    KeReleaseSpinLock(&FdoExtension->WorkerThreadEventSpinLock, OldIrql);
}

VOID
NTAPI
USBPORT_WorkerThreadHandler(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY workerList;
    KIRQL OldIrql;
    PUSBPORT_ENDPOINT Endpoint;

    DPRINT_CORE("USBPORT_WorkerThreadHandler: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    USBPORT_FlushAllEndpoints(FdoDevice);

    while (TRUE)
    {
        KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);
        KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);

        workerList = &FdoExtension->WorkerList;

        if (IsListEmpty(workerList))
            break;

        Endpoint = CONTAINING_RECORD(workerList->Flink,
                                     USBPORT_ENDPOINT,
                                     WorkerLink);

        DPRINT_CORE("USBPORT_WorkerThreadHandler: Endpoint - %p\n", Endpoint);

        RemoveHeadList(workerList);
        Endpoint->WorkerLink.Blink = NULL;
        Endpoint->WorkerLink.Flink = NULL;

        KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

        USBPORT_EndpointWorker(Endpoint, FALSE);

        KeLowerIrql(OldIrql);
    }

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);
    KeLowerIrql(OldIrql);

    USBPORT_FlushClosedEndpointList(FdoDevice);
}

VOID
NTAPI
USBPORT_WorkerThread(IN PVOID StartContext)
{
    PDEVICE_OBJECT FdoDevice;
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    LARGE_INTEGER OldTime = {{0, 0}};
    LARGE_INTEGER NewTime = {{0, 0}};
    PRH_INIT_CALLBACK RootHubInitCallback;
    PVOID RootHubInitContext;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_WorkerThread ... \n");

    FdoDevice = (PDEVICE_OBJECT)StartContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    FdoExtension->WorkerThread = KeGetCurrentThread();

    do
    {
        KeQuerySystemTime(&OldTime);

        KeWaitForSingleObject(&FdoExtension->WorkerThreadEvent,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);

        KeQuerySystemTime(&NewTime);

        KeAcquireSpinLock(&FdoExtension->WorkerThreadEventSpinLock, &OldIrql);
        KeResetEvent(&FdoExtension->WorkerThreadEvent);
        KeReleaseSpinLock(&FdoExtension->WorkerThreadEventSpinLock, OldIrql);
        DPRINT_CORE("USBPORT_WorkerThread: run \n");

        if (FdoExtension->MiniPortFlags & USBPORT_MPFLAG_INTERRUPTS_ENABLED)
        {
            USBPORT_DoSetPowerD0(FdoDevice);

            if (FdoExtension->Flags & USBPORT_FLAG_RH_INIT_CALLBACK)
            {
                DPRINT_CORE("USBPORT_WorkerThread: RootHubInitCallback \n");

                PdoDevice = FdoExtension->RootHubPdo;

                if (PdoDevice)
                {
                    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

                    RootHubInitContext = PdoExtension->RootHubInitContext;
                    RootHubInitCallback = PdoExtension->RootHubInitCallback;

                    PdoExtension->RootHubInitCallback = NULL;
                    PdoExtension->RootHubInitContext = NULL;

                    if (RootHubInitCallback)
                        RootHubInitCallback(RootHubInitContext);
                }

                FdoExtension->Flags &= ~USBPORT_FLAG_RH_INIT_CALLBACK;

                DPRINT_CORE("USBPORT_WorkerThread: end RootHubInitCallback \n");
            }
        }

        USBPORT_WorkerThreadHandler(FdoDevice);
    }
    while (!(FdoExtension->Flags & USBPORT_FLAG_WORKER_THREAD_ON));

    PsTerminateSystemThread(0);
}

NTSTATUS
NTAPI
USBPORT_CreateWorkerThread(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    NTSTATUS Status;

    DPRINT("USBPORT_CreateWorkerThread ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    FdoExtension->Flags &= ~USBPORT_FLAG_WORKER_THREAD_ON;

    KeInitializeEvent(&FdoExtension->WorkerThreadEvent,
                      NotificationEvent,
                      FALSE);

    Status = PsCreateSystemThread(&FdoExtension->WorkerThreadHandle,
                                  THREAD_ALL_ACCESS,
                                  NULL,
                                  NULL,
                                  NULL,
                                  USBPORT_WorkerThread,
                                  (PVOID)FdoDevice);

    return Status;
}

VOID
NTAPI
USBPORT_TimerDpc(IN PRKDPC Dpc,
                 IN PVOID DeferredContext,
                 IN PVOID SystemArgument1,
                 IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    LARGE_INTEGER DueTime = {{0, 0}};
    ULONG TimerFlags;
    ULONG TimeIncrement;
    PTIMER_WORK_QUEUE_ITEM IdleQueueItem;

    DPRINT_TIMER("USBPORT_TimerDpc: Dpc - %p, DeferredContext - %p, SystemArgument1 - %p, SystemArgument2 - %p\n",
           Dpc,
           DeferredContext,
           SystemArgument1,
           SystemArgument2);

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    PdoDevice = FdoExtension->RootHubPdo;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    TimerFlags = FdoExtension->TimerFlags;

    if (PdoDevice &&
        (PdoExtension->RootHubInitCallback != NULL) &&
        !(FdoExtension->Flags & USBPORT_FLAG_RH_INIT_CALLBACK))
    {
        FdoExtension->Flags |= USBPORT_FLAG_RH_INIT_CALLBACK;
        USBPORT_SignalWorkerThread(FdoDevice);
    }

    USBPORT_IsrDpcHandler(FdoDevice, FALSE);

    if (FdoExtension->IdleLockCounter > -1 &&
        !(TimerFlags & USBPORT_TMFLAG_IDLE_QUEUEITEM_ON))
    {
        IdleQueueItem = (PTIMER_WORK_QUEUE_ITEM)ExAllocatePoolWithTag(NonPagedPool,
                                                                      sizeof(TIMER_WORK_QUEUE_ITEM),
                                                                      USB_PORT_TAG);

        DPRINT1("USBPORT_DM_TimerDpc: IdleLockCounter - %x, IdleQueueItem - %p\n",
                FdoExtension->IdleLockCounter,
                IdleQueueItem);

        if (IdleQueueItem)
        {
            RtlZeroMemory(IdleQueueItem, sizeof(TIMER_WORK_QUEUE_ITEM));

            IdleQueueItem->WqItem.WorkerRoutine = (PWORKER_THREAD_ROUTINE)USBPORT_DoIdleNotificationCallback;
            IdleQueueItem->WqItem.Parameter = (PVOID)IdleQueueItem;
            IdleQueueItem->FdoDevice = FdoDevice;

            FdoExtension->TimerFlags |= USBPORT_TMFLAG_IDLE_QUEUEITEM_ON;

            ExQueueWorkItem((PWORK_QUEUE_ITEM)IdleQueueItem, 0);
        }
    }

    if (TimerFlags & 1)
    {
        TimeIncrement = KeQueryTimeIncrement();
        DueTime.QuadPart -= 10000 * FdoExtension->TimerValue + (TimeIncrement - 1);

        KeSetTimer(&FdoExtension->TimerObject,
                   DueTime,
                   &FdoExtension->TimerDpc);
    }

    DPRINT_TIMER("USBPORT_TimerDpc: exit\n");
}

BOOLEAN
NTAPI
USBPORT_StartTimer(IN PDEVICE_OBJECT FdoDevice,
                   IN ULONG Time)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    LARGE_INTEGER DueTime = {{0, 0}};
    ULONG TimeIncrement;
    BOOLEAN Result;

    DPRINT_TIMER("USBPORT_StartTimer: FdoDevice - %p, Time - %x\n",
           FdoDevice,
           Time);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    TimeIncrement = KeQueryTimeIncrement();

    FdoExtension->TimerFlags |= 1;
    FdoExtension->TimerValue = Time;

    KeInitializeTimer(&FdoExtension->TimerObject);
    KeInitializeDpc(&FdoExtension->TimerDpc, USBPORT_TimerDpc, FdoDevice);

    DueTime.QuadPart -= 10000 * Time + (TimeIncrement - 1);

    Result = KeSetTimer(&FdoExtension->TimerObject,
                        DueTime,
                        &FdoExtension->TimerDpc);

    return Result;
}

PUSBPORT_COMMON_BUFFER_HEADER
NTAPI
USBPORT_AllocateCommonBuffer(IN PDEVICE_OBJECT FdoDevice,
                             IN SIZE_T BufferLength)
{
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer = NULL;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDMA_ADAPTER DmaAdapter;
    SIZE_T HeaderSize;
    ULONG Length = 0;
    ULONG LengthPadded;
    PHYSICAL_ADDRESS LogicalAddress;
    ULONG_PTR BaseVA;
    ULONG_PTR StartBufferVA;
    ULONG_PTR StartBufferPA;

    DPRINT("USBPORT_AllocateCommonBuffer: FdoDevice - %p, BufferLength - %p\n",
           FdoDevice,
           BufferLength);

    if (BufferLength == 0)
        goto Exit;

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DmaAdapter = FdoExtension->DmaAdapter;

    HeaderSize = sizeof(USBPORT_COMMON_BUFFER_HEADER);
    Length = ROUND_TO_PAGES(BufferLength + HeaderSize);
    LengthPadded = Length - (BufferLength + HeaderSize);

    BaseVA = (ULONG_PTR)DmaAdapter->DmaOperations->AllocateCommonBuffer(DmaAdapter, // IN PDMA_ADAPTER DmaAdapter
                                                                        Length, // IN ULONG Length,
                                                                        &LogicalAddress,  // OUT PPHYSICAL_ADDRESS LogicalAddress
                                                                        TRUE); // IN BOOLEAN CacheEnabled

    if (!BaseVA)
        goto Exit;

    StartBufferVA = BaseVA & 0xFFFFF000;
    StartBufferPA = LogicalAddress.LowPart & 0xFFFFF000;

    HeaderBuffer = (PUSBPORT_COMMON_BUFFER_HEADER)(StartBufferVA +
                                                   BufferLength +
                                                   LengthPadded);

    HeaderBuffer->Length = Length;
    HeaderBuffer->BaseVA = BaseVA;
    HeaderBuffer->LogicalAddress = LogicalAddress; // PHYSICAL_ADDRESS

    HeaderBuffer->BufferLength = BufferLength + LengthPadded;
    HeaderBuffer->VirtualAddress = StartBufferVA;
    HeaderBuffer->PhysicalAddress = StartBufferPA;

    RtlZeroMemory((PVOID)StartBufferVA, BufferLength + LengthPadded);

Exit:
    return HeaderBuffer;
}

VOID
NTAPI
USBPORT_FreeCommonBuffer(IN PDEVICE_OBJECT FdoDevice,
                         IN PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDMA_ADAPTER DmaAdapter;

    DPRINT("USBPORT_FreeCommonBuffer: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DmaAdapter = FdoExtension->DmaAdapter;

    DmaAdapter->DmaOperations->FreeCommonBuffer(FdoExtension->DmaAdapter,
                                                HeaderBuffer->Length,
                                                HeaderBuffer->LogicalAddress,
                                                (PVOID)HeaderBuffer->VirtualAddress,
                                                TRUE);
}

PUSBPORT_MINIPORT_INTERFACE
NTAPI
USBPORT_FindMiniPort(IN PDRIVER_OBJECT DriverObject)
{
    KIRQL OldIrql;
    PLIST_ENTRY List;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface = NULL;

    DPRINT("USBPORT_FindMiniPort: ... \n");

    KeAcquireSpinLock(&USBPORT_SpinLock, &OldIrql);

    for (List = USBPORT_MiniPortDrivers.Flink;
         List != &USBPORT_MiniPortDrivers;
         List = List->Flink)
    {
        MiniPortInterface = CONTAINING_RECORD(List,
                                              USBPORT_MINIPORT_INTERFACE,
                                              DriverLink);

        if (MiniPortInterface->DriverObject == DriverObject)
        {
            DPRINT("USBPORT_FindMiniPort: find MiniPortInterface - %p\n",
                   MiniPortInterface);
            break;
        }
    }

    KeReleaseSpinLock(&USBPORT_SpinLock, OldIrql);

    return MiniPortInterface;
}

NTSTATUS
NTAPI
USBPORT_AddDevice(IN PDRIVER_OBJECT DriverObject,
                  IN PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS Status;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;
    ULONG DeviceNumber = 0;
    WCHAR CharDeviceName[64];
    UNICODE_STRING DeviceName;
    PDEVICE_OBJECT DeviceObject;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_AddDevice: DriverObject - %p, PhysicalDeviceObject - %p\n",
           DriverObject,
           PhysicalDeviceObject);

    MiniPortInterface = USBPORT_FindMiniPort(DriverObject);
    if (!MiniPortInterface)
    {
        DPRINT("USBPORT_AddDevice: USBPORT_FindMiniPort not found MiniPortInterface\n");
        return STATUS_UNSUCCESSFUL;
    }

    while (TRUE)
    {
        // Construct device name
        swprintf(CharDeviceName, L"\\Device\\USBFDO-%d", DeviceNumber);
        RtlInitUnicodeString(&DeviceName, CharDeviceName);

        // Create device
        Status = IoCreateDevice(DriverObject,
                                sizeof(USBPORT_DEVICE_EXTENSION) + MiniPortInterface->Packet.MiniPortExtensionSize,
                                &DeviceName,
                                FILE_DEVICE_CONTROLLER,
                                0,
                                FALSE,
                                &DeviceObject);

        // Check for success
        if (NT_SUCCESS(Status)) break;

        // Is there a device object with that same name
        if ((Status == STATUS_OBJECT_NAME_EXISTS) ||
            (Status == STATUS_OBJECT_NAME_COLLISION))
        {
            // Try the next name
            DeviceNumber++;
            continue;
        }

        // Bail out on other errors
        if (!NT_SUCCESS(Status))
        {
            DPRINT1("USBPORT_AddDevice: failed to create %wZ, Status %x\n",
                    &DeviceName,
                    Status);

            return Status;
        }
    }

    DPRINT("USBPORT_AddDevice: created device %p <%wZ>, Status %x\n",
           DeviceObject,
           &DeviceName,
           Status);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

    RtlZeroMemory(FdoExtension, sizeof(USBPORT_DEVICE_EXTENSION));

    FdoExtension->CommonExtension.SelfDevice = DeviceObject;
    FdoExtension->CommonExtension.LowerPdoDevice = PhysicalDeviceObject;
    FdoExtension->CommonExtension.IsPDO = FALSE;
    FdoExtension->CommonExtension.LowerDevice = IoAttachDeviceToDeviceStack(DeviceObject,
                                                                            PhysicalDeviceObject);

    FdoExtension->CommonExtension.DevicePowerState = PowerDeviceD3;

    FdoExtension->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtension +
                                        sizeof(USBPORT_DEVICE_EXTENSION));

    FdoExtension->MiniPortInterface = MiniPortInterface;
    FdoExtension->FdoNameNumber = DeviceNumber;

    InitializeListHead(&FdoExtension->EndpointList);
    InitializeListHead(&FdoExtension->DoneTransferList);
    InitializeListHead(&FdoExtension->WorkerList);
    InitializeListHead(&FdoExtension->EpStateChangeList);
    InitializeListHead(&FdoExtension->MapTransferList);
    InitializeListHead(&FdoExtension->DeviceHandleList);
    InitializeListHead(&FdoExtension->IdleIrpList);
    InitializeListHead(&FdoExtension->BadRequestList);
    InitializeListHead(&FdoExtension->EndpointClosedList);

    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    return Status;
}

VOID
NTAPI
USBPORT_Unload(IN PDRIVER_OBJECT DriverObject)
{
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;

    DPRINT("USBPORT_Unload: FIXME!\n");

    MiniPortInterface = USBPORT_FindMiniPort(DriverObject);
    if (!MiniPortInterface)
    {
        DPRINT("USBPORT_Unload: CRITICAL ERROR!!! USBPORT_FindMiniPort not found MiniPortInterface\n");
        ASSERT(FALSE);
    }

    DPRINT1("USBPORT_Unload: UNIMPLEMENTED. FIXME. \n");
    // ...
    //MiniPortInterface->DriverUnload(DriverObject); // Call MiniPort _HCI_Unload
    // ...
}

ULONG
NTAPI
USBPORT_MiniportCompleteTransfer(IN PVOID MiniPortExtension,
                                 IN PVOID MiniPortEndpoint,
                                 IN PVOID TransferParameters,
                                 IN USBD_STATUS USBDStatus,
                                 IN ULONG TransferLength)
{
    PUSBPORT_TRANSFER Transfer;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_MiniportCompleteTransfer: USBDStatus - %x, TransferLength - %x\n",
                USBDStatus,
                TransferLength);

    Transfer = CONTAINING_RECORD(TransferParameters,
                                 USBPORT_TRANSFER,
                                 TransferParameters);

    FdoDevice = Transfer->Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Transfer->CompletedTransferLen = TransferLength;

    RemoveEntryList(&Transfer->TransferLink);

    Transfer->USBDStatus = USBDStatus;

    ExInterlockedInsertTailList(&FdoExtension->DoneTransferList,
                                &Transfer->TransferLink,
                                &FdoExtension->DoneTransferSpinLock);

    return KeInsertQueueDpc(&FdoExtension->TransferFlushDpc, NULL, NULL);
}

ULONG
NTAPI
USBPORT_CompleteIsoTransfer(IN PVOID MiniPortExtension,
                            IN PVOID MiniPortEndpoint,
                            IN PVOID TransferParameters,
                            IN ULONG TransferLength)
{
    DPRINT1("USBPORT_CompleteIsoTransfer: UNIMPLEMENTED. FIXME.\n");
    return 0;
}

VOID
NTAPI
USBPORT_AsyncTimerDpc(IN PRKDPC Dpc,
                      IN PVOID DeferredContext,
                      IN PVOID SystemArgument1,
                      IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_ASYNC_CALLBACK_DATA AsyncCallbackData;

    DPRINT("USBPORT_AsyncTimerDpc: ... \n");

    AsyncCallbackData = (PUSBPORT_ASYNC_CALLBACK_DATA)DeferredContext;
    FdoDevice = AsyncCallbackData->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    (*(ASYNC_TIMER_CALLBACK *)AsyncCallbackData->CallbackFunction)(FdoExtension->MiniPortExt,
                                                                   &AsyncCallbackData->CallbackContext);

    ExFreePool(AsyncCallbackData);
}

ULONG
NTAPI
USBPORT_RequestAsyncCallback(IN PVOID Context,
                             IN ULONG TimerValue,
                             IN PVOID Buffer,
                             IN SIZE_T Length,
                             IN ULONG Callback)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_ASYNC_CALLBACK_DATA AsyncCallbackData;
    LARGE_INTEGER DueTime = {{0, 0}};

    DPRINT("USBPORT_RequestAsyncCallback: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context -
                                               sizeof(USBPORT_DEVICE_EXTENSION));

    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    AsyncCallbackData = (PUSBPORT_ASYNC_CALLBACK_DATA)ExAllocatePoolWithTag(NonPagedPool,
                                                                            sizeof(USBPORT_ASYNC_CALLBACK_DATA) + Length,
                                                                            USB_PORT_TAG);

    if (!AsyncCallbackData)
    {
        DPRINT1("USBPORT_RequestAsyncCallback: Not allocated AsyncCallbackData!\n");
        return 0;
    }

    RtlZeroMemory(AsyncCallbackData,
                  sizeof(USBPORT_ASYNC_CALLBACK_DATA) + Length);

    if (Length)
    {
        RtlCopyMemory(&AsyncCallbackData->CallbackContext, Buffer, Length);
    }

    AsyncCallbackData->FdoDevice = FdoDevice;
    AsyncCallbackData->CallbackFunction = (ASYNC_TIMER_CALLBACK *)Callback;

    KeInitializeTimer(&AsyncCallbackData->AsyncTimer);

    KeInitializeDpc(&AsyncCallbackData->AsyncTimerDpc,
                    USBPORT_AsyncTimerDpc,
                    AsyncCallbackData);

    DueTime.QuadPart -= (KeQueryTimeIncrement() - 1) + 10000 * TimerValue;

    KeSetTimer(&AsyncCallbackData->AsyncTimer,
               DueTime,
               &AsyncCallbackData->AsyncTimerDpc);

    return 0;
}

PVOID
NTAPI
USBPORT_GetMappedVirtualAddress(IN PVOID PhysicalAddress,
                                IN PVOID MiniPortExtension,
                                IN PVOID MiniPortEndpoint)
{
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
    PUSBPORT_ENDPOINT Endpoint;
    ULONG Offset;
    ULONG_PTR VirtualAddress;

    DPRINT_CORE("USBPORT_GetMappedVirtualAddress ... \n");

    Endpoint = (PUSBPORT_ENDPOINT)((ULONG_PTR)MiniPortEndpoint -
                                   sizeof(USBPORT_ENDPOINT));

    if (!Endpoint)
    {
        ASSERT(FALSE);
    }

    HeaderBuffer = Endpoint->HeaderBuffer;

    Offset = (ULONG_PTR)PhysicalAddress - HeaderBuffer->PhysicalAddress;
    VirtualAddress = HeaderBuffer->VirtualAddress + Offset;

    return (PVOID)VirtualAddress;
}

ULONG
NTAPI
USBPORT_InvalidateEndpoint(IN PVOID Context1,
                           IN PVOID Context2)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_ENDPOINT Endpoint;

    DPRINT_CORE("USBPORT_InvalidateEndpoint: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context1 -
                                               sizeof(USBPORT_DEVICE_EXTENSION));

    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    Endpoint = (PUSBPORT_ENDPOINT)((ULONG_PTR)Context2 -
                                   sizeof(USBPORT_ENDPOINT));

    if (Context2)
    {
        USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 0);
    }
    else
    {
        USBPORT_InvalidateEndpointHandler(FdoDevice, NULL, 0);
    }

    return 0;
}

VOID
NTAPI
USBPORT_CompleteTransfer(IN PURB Urb,
                         IN USBD_STATUS TransferStatus)
{
    struct _URB_CONTROL_TRANSFER *UrbTransfer;
    PUSBPORT_TRANSFER Transfer;
    NTSTATUS Status;
    PIRP Irp;
    KIRQL OldIrql;
    PRKEVENT Event;
    BOOLEAN WriteToDevice;
    BOOLEAN IsFlushSuccess;
    PMDL Mdl;
    ULONG_PTR CurrentVa;
    SIZE_T TransferLength;
    PUSBPORT_ENDPOINT Endpoint;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT_CORE("USBPORT_CompleteTransfer: Urb - %p, TransferStatus - %p\n",
           Urb,
           TransferStatus);

    UrbTransfer = &Urb->UrbControlTransfer;
    Transfer = (PUSBPORT_TRANSFER)UrbTransfer->hca.Reserved8[0];

    Transfer->USBDStatus = TransferStatus;
    Status = USBPORT_USBDStatusToNtStatus(Urb, TransferStatus);

    UrbTransfer->TransferBufferLength = Transfer->CompletedTransferLen;

    if (Transfer->Flags & TRANSFER_FLAG_DMA_MAPPED)
    {
        Endpoint = Transfer->Endpoint;
        FdoDevice = Endpoint->FdoDevice;
        FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

        WriteToDevice = Transfer->Direction == 2;
        Mdl = UrbTransfer->TransferBufferMDL;
        CurrentVa = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);
        TransferLength = UrbTransfer->TransferBufferLength;

        IsFlushSuccess = FdoExtension->DmaAdapter->DmaOperations->FlushAdapterBuffers(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                                      Mdl, // IN PMDL Mdl,
                                                                                      Transfer->MapRegisterBase, // IN PVOID MapRegisterBase,
                                                                                      (PVOID)CurrentVa, // IN PVOID CurrentVa,
                                                                                      TransferLength, // IN ULONG Length,
                                                                                      WriteToDevice); // IN BOOLEAN WriteToDevice

        if (!IsFlushSuccess)
        {
            DPRINT("USBPORT_CompleteTransfer: no FlushAdapterBuffers !!!\n");
            ASSERT(FALSE);
        }

        KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

        FdoExtension->DmaAdapter->DmaOperations->FreeMapRegisters(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                  Transfer->MapRegisterBase, // PVOID MapRegisterBase,
                                                                  Transfer->NumberOfMapRegisters); // ULONG NumberOfMapRegisters

        KeLowerIrql(OldIrql);
    }

    if (Urb->UrbHeader.UsbdFlags & USBD_FLAG_ALLOCATED_MDL)
    {
        IoFreeMdl(Transfer->TransferBufferMDL);
        Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_MDL;
    }

    Urb->UrbControlTransfer.hca.Reserved8[0] = NULL;
    Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_TRANSFER;

    Irp = Transfer->Irp;

    if (Irp)
    {
        Irp->IoStatus.Status = Status;
        Irp->IoStatus.Information = 0;

        KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        KeLowerIrql(OldIrql);
    }

    Event = Transfer->Event;

    if (Event)
        KeSetEvent(Event, 1, FALSE);

    ExFreePool(Transfer);

    DPRINT_CORE("USBPORT_CompleteTransfer: exit\n");
}

VOID
NTAPI
USBPORT_DmaEndpointWorker(PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_TRANSFER Transfer;
    ULONG OldState;
    ULONG NewState;
    ULONG CurrentState;
    PLIST_ENTRY List;
    ULONG Result;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_DmaEndpointWorker ... \n");

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    OldState = Endpoint->StateLast;

    if (OldState == USBPORT_ENDPOINT_ACTIVE)
    {
        CurrentState = Endpoint->StateLast;
        ASSERT(CurrentState == USBPORT_ENDPOINT_ACTIVE);

        List = Endpoint->TransferList.Flink;
        if (!IsListEmpty(&Endpoint->TransferList))
        {
            while (List && (List != &Endpoint->TransferList))
            {
                Transfer = CONTAINING_RECORD(List,
                                             USBPORT_TRANSFER,
                                             TransferLink);

                if (!(Transfer->Flags & TRANSFER_FLAG_SUBMITED))
                {
                    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);

                    Result = FdoExtension->MiniPortInterface->Packet.SubmitTransfer(FdoExtension->MiniPortExt,
                                                                                    (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                                                                    &Transfer->TransferParameters,
                                                                                    Transfer->MiniportTransfer,
                                                                                    &Transfer->SgList);
                    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

                    if (Result)
                        goto Exit;

                    Transfer->Flags |= TRANSFER_FLAG_SUBMITED;
                    CurrentState = USBPORT_ENDPOINT_ACTIVE;
                }

                List = Transfer->TransferLink.Flink;
            }
        }

Exit:
        NewState = CurrentState;
    }

    if (NewState != OldState)
    {
        USBPORT_SetEndpointState(Endpoint, NewState);
    }

    DPRINT_CORE("USBPORT_DmaEndpointWorker exit \n");
}

BOOLEAN
NTAPI
USBPORT_EndpointWorker(IN PUSBPORT_ENDPOINT Endpoint,
                       IN BOOLEAN Flag)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_EndpointWorker: Endpoint - %p, Flag - %x\n",
           Endpoint,
           Flag);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (Flag == FALSE)
    {
        if (InterlockedIncrement(&Endpoint->LockCounter))
        {
            InterlockedDecrement(&Endpoint->LockCounter);
            DPRINT_CORE("USBPORT_EndpointWorker: LockCounter > 0\n");
            return TRUE;
        }
    }

    KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);

    if (Endpoint->StateLast == 5)
    {
        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);
        InterlockedDecrement(&Endpoint->LockCounter);
        DPRINT_CORE("USBPORT_EndpointWorker: State == 5. return FALSE\n");
        return FALSE;
    }

    if ((Endpoint->Flags & (ENDPOINT_FLAG_ROOTHUB_EP0 | 0x00000008)) == 0)
    {
        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FdoExtension->MiniPortInterface->Packet.PollEndpoint(FdoExtension->MiniPortExt,
                                                             (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)));
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
    }

    if (Endpoint->StateLast == USBPORT_ENDPOINT_CLOSED)
    {
        KeAcquireSpinLock(&Endpoint->StateChangeSpinLock, &Endpoint->EndpointStateOldIrql);
        Endpoint->StateLast = 5;
        Endpoint->StateNext = 5;
        KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);

        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

        KefAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);
        ExInterlockedInsertTailList(&FdoExtension->EndpointClosedList,
                                    &Endpoint->CloseLink,
                                    &FdoExtension->EndpointClosedSpinLock);
        KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

        InterlockedDecrement(&Endpoint->LockCounter);
        DPRINT_CORE("USBPORT_EndpointWorker: State == USBPORT_ENDPOINT_CLOSED. return FALSE\n");
        return FALSE;
    }

    if (!IsListEmpty(&Endpoint->PendingTransferList) ||
        !IsListEmpty(&Endpoint->TransferList) ||
        !IsListEmpty(&Endpoint->CancelList))
    {
        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

        KeAcquireSpinLock(&Endpoint->StateChangeSpinLock, &Endpoint->EndpointStateOldIrql);
        if (Endpoint->StateLast == Endpoint->StateNext)
        {
            KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);

            if (Endpoint->EndpointWorker)
            {
                USBPORT_DmaEndpointWorker(Endpoint);
            }
            else
            {
                USBPORT_RootHubEndpointWorker(Endpoint);
            }

            USBPORT_FlushAbortList(Endpoint);

            InterlockedDecrement(&Endpoint->LockCounter);
            DPRINT_CORE("USBPORT_EndpointWorker: return FALSE\n");
            return FALSE;
        }

        InterlockedDecrement(&Endpoint->LockCounter);
        DPRINT_CORE("USBPORT_EndpointWorker: return TRUE\n");
        return TRUE;
    }

    KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

    USBPORT_FlushAbortList(Endpoint);

    InterlockedDecrement(&Endpoint->LockCounter);
    DPRINT_CORE("USBPORT_EndpointWorker: return FALSE\n");
    return FALSE;
}

IO_ALLOCATION_ACTION
NTAPI
USBPORT_MapTransfer(IN PDEVICE_OBJECT FdoDevice,
                    IN PIRP Irp,
                    IN PVOID MapRegisterBase,
                    IN PVOID Context)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDMA_ADAPTER DmaAdapter;
    PUSBPORT_TRANSFER Transfer;
    PURB Urb;
    PUSBPORT_ENDPOINT Endpoint;
    PMDL Mdl;
    ULONG_PTR CurrentVa;
    PVOID MappedSystemVa;
    PUSBPORT_SCATTER_GATHER_LIST sgList;
    SIZE_T CurrentLength;
    ULONG ix;
    BOOLEAN WriteToDevice;
    PHYSICAL_ADDRESS PhAddr = {{0, 0}};
    PHYSICAL_ADDRESS PhAddress = {{0, 0}};
    SIZE_T TransferLength;
    SIZE_T SgCurrentLength;
    SIZE_T ElementLength;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;

    DPRINT_CORE("USBPORT_MapTransfer: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DmaAdapter = FdoExtension->DmaAdapter;

    Transfer = (PUSBPORT_TRANSFER)Context;

    Urb = Transfer->Urb;
    Endpoint = Transfer->Endpoint;
    TransferLength = Transfer->TransferParameters.TransferBufferLength;

    Mdl = Urb->UrbControlTransfer.TransferBufferMDL;
    CurrentVa = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);
    Transfer->SgList.CurrentVa = CurrentVa;

    Mdl->MdlFlags |= MDL_MAPPING_CAN_FAIL;

    if (Mdl->MdlFlags & (MDL_SOURCE_IS_NONPAGED_POOL | MDL_MAPPED_TO_SYSTEM_VA))
        MappedSystemVa = Mdl->MappedSystemVa;
    else
        MappedSystemVa = MmMapLockedPages(Mdl, KernelMode);

    Transfer->SgList.MappedSystemVa = MappedSystemVa;

    Mdl->MdlFlags &= ~MDL_MAPPING_CAN_FAIL;

    sgList = &Transfer->SgList;
    sgList->Flags = 0;

    Transfer->MapRegisterBase = MapRegisterBase;

    ix = 0;
    CurrentLength = 0;

    do
    {
        WriteToDevice = Transfer->Direction == 2;
        ASSERT(Transfer->Direction != 0);

        PhAddress = DmaAdapter->DmaOperations->MapTransfer(DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                           Mdl, // IN PMDL Mdl,
                                                           MapRegisterBase, // IN PVOID MapRegisterBase,
                                                           (PVOID)CurrentVa, // IN PVOID CurrentVa,
                                                           (PULONG)&TransferLength, // IN OUT PULONG Length,
                                                           WriteToDevice); // IN BOOLEAN WriteToDevice

        DPRINT_CORE("USBPORT_MapTransfer: PhAddress.LowPart - %p, PhAddress.HighPart - %x, TransferLength - %x\n",
               PhAddress.LowPart,
               PhAddress.HighPart,
               TransferLength);

        PhAddress.HighPart = 0;
        SgCurrentLength = TransferLength;

        do
        {
            ElementLength = 0x1000 - (PhAddress.LowPart & 0xFFF);

            if (ElementLength > SgCurrentLength)
                ElementLength = SgCurrentLength;

            DPRINT_CORE("USBPORT_MapTransfer: PhAddress.LowPart - %p, HighPart - %x, ElementLength - %x\n",
                   PhAddress.LowPart,
                   PhAddress.HighPart,
                   ElementLength);

            sgList->SgElement[ix].SgPhysicalAddress = PhAddress;
            sgList->SgElement[ix].SgTransferLength = ElementLength;
            sgList->SgElement[ix].SgOffset = CurrentLength + (TransferLength - SgCurrentLength);

            PhAddress.LowPart += ElementLength;
            SgCurrentLength -= ElementLength;

            ++ix;
        }
        while (SgCurrentLength);

        if ((PhAddr.LowPart == PhAddress.LowPart) &&
            (PhAddr.HighPart == PhAddress.HighPart))
        {
            ASSERT(FALSE);
        }

        PhAddr = PhAddress;

        CurrentLength += TransferLength;
        CurrentVa += TransferLength;

        TransferLength = Transfer->TransferParameters.TransferBufferLength - CurrentLength;
    }
    while (CurrentLength != Transfer->TransferParameters.TransferBufferLength);

    Transfer->SgList.SgElementCount = ix;
    Transfer->Flags |= TRANSFER_FLAG_DMA_MAPPED;

    ASSERT(Transfer->TransferParameters.TransferBufferLength <=
           Endpoint->EndpointProperties.MaxTransferSize);

    InsertTailList(&Endpoint->TransferList, &Transfer->TransferLink);

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;
    InterlockedDecrement(&DeviceHandle->DeviceHandleLock);

    return DeallocateObjectKeepRegisters;
}

VOID
NTAPI
USBPORT_FlushMapTransfers(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY MapTransferList;
    PUSBPORT_TRANSFER Transfer;
    ULONG NumMapRegisters;
    PMDL Mdl;
    SIZE_T TransferBufferLength;
    ULONG_PTR VirtualAddr;
    KIRQL OldIrql;
    NTSTATUS Status;

    DPRINT_CORE("USBPORT_FlushMapTransfers: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

    while (TRUE)
    {
        MapTransferList = &FdoExtension->MapTransferList;

        if (IsListEmpty(&FdoExtension->MapTransferList))
        {
            KeLowerIrql(OldIrql);
            return;
        }

        Transfer = CONTAINING_RECORD(MapTransferList->Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(MapTransferList);

        Mdl = Transfer->Urb->UrbControlTransfer.TransferBufferMDL;
        TransferBufferLength = Transfer->TransferParameters.TransferBufferLength;
        VirtualAddr = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);

        NumMapRegisters = ADDRESS_AND_SIZE_TO_SPAN_PAGES(VirtualAddr,
                                                         TransferBufferLength);

        Transfer->NumberOfMapRegisters = NumMapRegisters;

        Status = FdoExtension->DmaAdapter->DmaOperations->AllocateAdapterChannel(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                                 FdoDevice, // IN PDEVICE_OBJECT DeviceObject,
                                                                                 NumMapRegisters, // IN ULONG NumberOfMapRegisters,
                                                                                 (PDRIVER_CONTROL)USBPORT_MapTransfer, // IN PDRIVER_CONTROL ExecutionRoutine,
                                                                                 Transfer); // IN PVOID Context

        if (!NT_SUCCESS(Status))
            ASSERT(FALSE);
    }

    KeLowerIrql(OldIrql);
}

USBD_STATUS
NTAPI
USBPORT_AllocateTransfer(IN PDEVICE_OBJECT FdoDevice,
                         IN PURB Urb,
                         IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                         IN PIRP Irp,
                         IN PRKEVENT Event)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    SIZE_T TransferLength;
    PMDL Mdl;
    ULONG_PTR VirtualAddr;
    ULONG PagesNeed = 0;
    SIZE_T PortTransferLength;
    SIZE_T FullTransferLength;
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_PIPE_HANDLE PipeHandle;
    USBD_STATUS USBDStatus;

    DPRINT_CORE("USBPORT_AllocateTransfer: FdoDevice - %p, Urb - %p, DeviceHandle - %p, Irp - %p, Event - %p\n",
           FdoDevice,
           Urb,
           DeviceHandle,
           Irp,
           Event);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    TransferLength = Urb->UrbControlTransfer.TransferBufferLength;
    PipeHandle = Urb->UrbControlTransfer.PipeHandle;

    if (TransferLength)
    {
        Mdl = Urb->UrbControlTransfer.TransferBufferMDL;
        VirtualAddr = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);

        PagesNeed = ADDRESS_AND_SIZE_TO_SPAN_PAGES(VirtualAddr,
                                                   TransferLength);
    }

    PortTransferLength = sizeof(USBPORT_TRANSFER) +
                         PagesNeed * sizeof(USBPORT_SCATTER_GATHER_ELEMENT);

    FullTransferLength = PortTransferLength +
                         FdoExtension->MiniPortInterface->Packet.MiniPortTransferSize;

    Transfer = ExAllocatePoolWithTag(NonPagedPool,
                                     FullTransferLength,
                                     USB_PORT_TAG);

    if (Transfer)
    {
        RtlZeroMemory(Transfer, FullTransferLength);

        Transfer->Irp = Irp;
        Transfer->Urb = Urb;
        Transfer->Endpoint = PipeHandle->Endpoint;
        Transfer->Event = Event;
        Transfer->PortTransferLength = PortTransferLength;
        Transfer->FullTransferLength = FullTransferLength;

        Transfer->MiniportTransfer = (PVOID)((ULONG_PTR)Transfer +
                                             PortTransferLength);

        Urb->UrbControlTransfer.hca.Reserved8[0] = Transfer;
        Urb->UrbHeader.UsbdFlags |= USBD_FLAG_ALLOCATED_TRANSFER;
        USBDStatus = USBD_STATUS_SUCCESS;
    }
    else
    {
        USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
    }

    DPRINT_CORE("USBPORT_AllocateTransfer: return USBDStatus - %x\n", USBDStatus);
    return USBDStatus;
}

NTSTATUS
NTAPI
USBPORT_PdoScsi(IN PDEVICE_OBJECT PdoDevice,
                IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    ULONG IoCtl;
    NTSTATUS Status;

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);
    IoCtl = IoStack->Parameters.DeviceIoControl.IoControlCode;

    DPRINT("USBPORT_PdoScsi: PdoDevice - %p, Irp - %p, IoCtl - %x\n",
           PdoDevice,
           Irp,
           IoCtl);

    if (IoCtl == IOCTL_INTERNAL_USB_SUBMIT_URB)
    {
        PURB Urb = (PURB)IoStack->Parameters.Others.Argument1;
        return USBPORT_HandleSubmitURB(PdoDevice, Irp, Urb);
    }

    if (IoCtl == IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO\n");

        if (IoStack->Parameters.Others.Argument1)
            *(PVOID *)IoStack->Parameters.Others.Argument1 = PdoDevice;

        if (IoStack->Parameters.Others.Argument2)
            *(PVOID *)IoStack->Parameters.Others.Argument2 = PdoDevice;

        Status = STATUS_SUCCESS;
        goto Exit;
    }

    if (IoCtl == IOCTL_INTERNAL_USB_GET_HUB_COUNT)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_HUB_COUNT\n");

        if (IoStack->Parameters.Others.Argument1)
            *(PULONG)IoStack->Parameters.Others.Argument1 = *(PULONG)IoStack->Parameters.Others.Argument1 + 1;

        Status = STATUS_SUCCESS;
        goto Exit;
    }

    if (IoCtl == IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE\n");

        if (IoStack->Parameters.Others.Argument1)
            *(PVOID *)IoStack->Parameters.Others.Argument1 = &PdoExtension->DeviceHandle;

        Status = STATUS_SUCCESS;
        goto Exit;
    }

    if (IoCtl == IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION\n");
        return USBPORT_IdleNotification(PdoDevice, Irp);
    }

    DPRINT("USBPORT_PdoScsi: INVALID INTERNAL DEVICE CONTROL\n");
    Status = STATUS_INVALID_DEVICE_REQUEST;

Exit:
    Irp->IoStatus.Status = Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}

NTSTATUS
NTAPI
USBPORT_Dispatch(IN PDEVICE_OBJECT DeviceObject,
                 IN PIRP Irp)
{
    PUSBPORT_COMMON_DEVICE_EXTENSION DeviceExtension;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status = STATUS_SUCCESS;

    DeviceExtension = (PUSBPORT_COMMON_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);

    switch (IoStack->MajorFunction)
    {
        case IRP_MJ_DEVICE_CONTROL: // 14
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO IRP_MJ_DEVICE_CONTROL. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_PdoDeviceControl(DeviceObject, Irp);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO IRP_MJ_DEVICE_CONTROL. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_FdoDeviceControl(DeviceObject, Irp);
            }

            break;

        case IRP_MJ_SCSI: // 15 IRP_MJ_NTERNAL_DEVICE_CONTROL:
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO IRP_MJ_SCSI. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_PdoScsi(DeviceObject, Irp);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO IRP_MJ_SCSI. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_FdoScsi(DeviceObject, Irp);
            }

            break;

        case IRP_MJ_POWER: // 22
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO IRP_MJ_POWER. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_PdoPower(DeviceObject, Irp);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO IRP_MJ_POWER. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_FdoPower(DeviceObject, Irp);
            }

            break;

        case IRP_MJ_SYSTEM_CONTROL: // 23
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO IRP_MJ_SYSTEM_CONTROL. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Irp->IoStatus.Status = Status;
                IoCompleteRequest(Irp, IO_NO_INCREMENT);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO IRP_MJ_SYSTEM_CONTROL. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                IoSkipCurrentIrpStackLocation(Irp);
                Status = IoCallDriver(DeviceExtension->LowerDevice, Irp);
            }

            break;

        case IRP_MJ_PNP: // 27
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO IRP_MJ_PNP. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_PdoPnP(DeviceObject, Irp);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO IRP_MJ_PNP. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);

                Status = USBPORT_FdoPnP(DeviceObject, Irp);
            }

            break;

        case IRP_MJ_CREATE: // 0
        case IRP_MJ_CLOSE: // 2
            DPRINT("USBPORT_Dispatch: IRP_MJ_CREATE | IRP_MJ_CLOSE\n");
            Irp->IoStatus.Status = Status;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            break;

        default:
            if (DeviceExtension->IsPDO)
            {
                DPRINT("USBPORT_Dispatch: PDO unhandled IRP_MJ_???. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);
            }
            else
            {
                DPRINT("USBPORT_Dispatch: FDO unhandled IRP_MJ_???. Major - %d, Minor - %d\n",
                       IoStack->MajorFunction,
                       IoStack->MinorFunction);
            }

            Status = STATUS_INVALID_DEVICE_REQUEST;
            Irp->IoStatus.Status = Status;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            break;
    }

    DPRINT("USBPORT_Dispatch: Status - %x\n", Status);
    return Status;
}

ULONG
NTAPI
USBPORT_GetHciMn(VOID)
{
    return 0x10000001;
}

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(IN PDRIVER_OBJECT DriverObject,
                              IN ULONG Version,
                              IN PUSBPORT_REGISTRATION_PACKET RegPacket)
{
    NTSTATUS Status;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;

    DPRINT("USBPORT_RegisterUSBPortDriver: DriverObject - %p, Version - %p, RegPacket - %p\n",
           DriverObject,
           Version,
           RegPacket);

    DPRINT("USBPORT_RegisterUSBPortDriver: sizeof(USBPORT_MINIPORT_INTERFACE) - %x\n",
           sizeof(USBPORT_MINIPORT_INTERFACE));

    DPRINT("USBPORT_RegisterUSBPortDriver: sizeof(USBPORT_DEVICE_EXTENSION)   - %x\n",
           sizeof(USBPORT_DEVICE_EXTENSION));

    if (Version < 100) // 100 - USB1.1; 200 - USB2.0
    {
        return STATUS_UNSUCCESSFUL;
    }

    if (!USBPORT_Initialized)
    {
        InitializeListHead(&USBPORT_MiniPortDrivers);
        InitializeListHead(&USBPORT_USB1FdoList);
        InitializeListHead(&USBPORT_USB2FdoList);

        KeInitializeSpinLock(&USBPORT_SpinLock);
        USBPORT_Initialized = TRUE;
    }

    MiniPortInterface = (PUSBPORT_MINIPORT_INTERFACE)ExAllocatePoolWithTag(NonPagedPool,
                                                                           sizeof(USBPORT_MINIPORT_INTERFACE),
                                                                           USB_PORT_TAG);
    if (!MiniPortInterface)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(MiniPortInterface, sizeof(USBPORT_MINIPORT_INTERFACE));

    MiniPortInterface->DriverObject = DriverObject;
    MiniPortInterface->DriverUnload = DriverObject->DriverUnload;

    ExInterlockedInsertTailList(&USBPORT_MiniPortDrivers,
                                &MiniPortInterface->DriverLink,
                                &USBPORT_SpinLock);

    DriverObject->DriverExtension->AddDevice = (PDRIVER_ADD_DEVICE)USBPORT_AddDevice;
    DriverObject->DriverUnload = (PDRIVER_UNLOAD)USBPORT_Unload;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = (PDRIVER_DISPATCH)USBPORT_Dispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = (PDRIVER_DISPATCH)USBPORT_Dispatch;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = (PDRIVER_DISPATCH)USBPORT_Dispatch;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = (PDRIVER_DISPATCH)USBPORT_Dispatch; // [== IRP_MJ_SCSI]
    DriverObject->MajorFunction[IRP_MJ_PNP] = (PDRIVER_DISPATCH)USBPORT_Dispatch;
    DriverObject->MajorFunction[IRP_MJ_POWER] = (PDRIVER_DISPATCH)USBPORT_Dispatch;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = (PDRIVER_DISPATCH)USBPORT_Dispatch;

    RegPacket->UsbPortDbgPrint = USBPORT_DbgPrint;
    RegPacket->UsbPortTestDebugBreak = USBPORT_TestDebugBreak;
    RegPacket->UsbPortAssertFailure = USBPORT_AssertFailure;
    RegPacket->UsbPortGetMiniportRegistryKeyValue = USBPORT_GetMiniportRegistryKeyValue;
    RegPacket->UsbPortInvalidateRootHub = USBPORT_InvalidateRootHub;
    RegPacket->UsbPortInvalidateEndpoint = USBPORT_InvalidateEndpoint;
    RegPacket->UsbPortCompleteTransfer = USBPORT_MiniportCompleteTransfer;
    RegPacket->UsbPortCompleteIsoTransfer = USBPORT_CompleteIsoTransfer;
    RegPacket->UsbPortLogEntry = USBPORT_LogEntry;
    RegPacket->UsbPortGetMappedVirtualAddress = USBPORT_GetMappedVirtualAddress;
    RegPacket->UsbPortRequestAsyncCallback = USBPORT_RequestAsyncCallback;
    RegPacket->UsbPortReadWriteConfigSpace = USBPORT_ReadWriteConfigSpace;
    RegPacket->UsbPortWait = USBPORT_Wait;
    RegPacket->UsbPortInvalidateController = USBPORT_InvalidateController;
    RegPacket->UsbPortBugCheck = USBPORT_BugCheck;
    RegPacket->UsbPortNotifyDoubleBuffer = USBPORT_NotifyDoubleBuffer;

    RtlCopyMemory(&MiniPortInterface->Packet,
                  RegPacket,
                  sizeof(USBPORT_REGISTRATION_PACKET));

    Status = STATUS_SUCCESS;
    return Status;
}

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    return STATUS_SUCCESS;
}
