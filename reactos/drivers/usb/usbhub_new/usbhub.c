#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_Wait(IN ULONG Milliseconds)
{
    LARGE_INTEGER Interval = {{0, 0}};

    DPRINT("USBH_Wait: Milliseconds - %x\n", Milliseconds);
    Interval.QuadPart -= 10000 * Milliseconds + (KeQueryTimeIncrement() - 1);
    Interval.HighPart = -1;
    return KeDelayExecutionThread(KernelMode, FALSE, &Interval);
}

VOID
NTAPI
USBH_CompleteIrp(IN PIRP Irp,
                 IN NTSTATUS CompleteStatus)
{
    DPRINT("USBH_CompleteIrp: Irp - %p, CompleteStatus - %x\n",
           Irp,
           CompleteStatus);

    Irp->IoStatus.Status = CompleteStatus;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
}

NTSTATUS
NTAPI
USBH_PassIrp(IN PDEVICE_OBJECT DeviceObject,
             IN PIRP Irp)
{
    DPRINT("USBH_PassIrp: DeviceObject - %p, Irp - %p\n",
           DeviceObject,
           Irp);

    IoSkipCurrentIrpStackLocation(Irp);
    return IoCallDriver(DeviceObject, Irp);
}

NTSTATUS
NTAPI
USBH_SyncIrpComplete(IN PDEVICE_OBJECT DeviceObject,
                     IN PIRP Irp,
                     IN PVOID Context)
{
    PUSBHUB_URB_TIMEOUT_CONTEXT HubTimeoutContext;
    KIRQL OldIrql;
    BOOLEAN TimerCancelled;

    DPRINT("USBH_SyncIrpComplete: ... \n");

    HubTimeoutContext = (PUSBHUB_URB_TIMEOUT_CONTEXT)Context;

    KeAcquireSpinLock(&HubTimeoutContext->UrbTimeoutSpinLock, &OldIrql);
    HubTimeoutContext->IsNormalCompleted = TRUE;
    TimerCancelled = KeCancelTimer(&HubTimeoutContext->UrbTimeoutTimer);
    KeReleaseSpinLock(&HubTimeoutContext->UrbTimeoutSpinLock, OldIrql);

    if (TimerCancelled)
    {
        KeSetEvent(&HubTimeoutContext->UrbTimeoutEvent,
                   EVENT_INCREMENT,
                   FALSE);
    }

    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBH_WriteFailReasonID(IN PDEVICE_OBJECT DeviceObject,
                       IN ULONG Data)
{
    NTSTATUS Status;
    WCHAR SourceString[64];
    HANDLE KeyHandle;
    UNICODE_STRING DestinationString;

    DPRINT("USBH_WriteFailReason: ID - %x\n", Data);

    swprintf(SourceString, L"FailReasonID");

    Status = IoOpenDeviceRegistryKey(DeviceObject,
                                     PLUGPLAY_REGKEY_DEVICE,
                                     STANDARD_RIGHTS_ALL,
                                     &KeyHandle);

    if (NT_SUCCESS(Status))
    {
        RtlInitUnicodeString(&DestinationString, SourceString);

        return ZwSetValueKey(KeyHandle,
                             &DestinationString,
                             0,
                             REG_DWORD,
                             &Data,
                             sizeof(Data));

        ZwClose(KeyHandle);
    }

    return Status;
}

VOID
NTAPI
USBH_UrbTimeoutDPC(IN PKDPC Dpc,
                   IN PVOID DeferredContext,
                   IN PVOID SystemArgument1,
                   IN PVOID SystemArgument2)
{
    PUSBHUB_URB_TIMEOUT_CONTEXT HubTimeoutContext;
    KIRQL OldIrql;
    BOOL IsCompleted;

    DPRINT("USBH_TimeoutDPC ... \n");

    HubTimeoutContext = (PUSBHUB_URB_TIMEOUT_CONTEXT)DeferredContext;

    KeAcquireSpinLock(&HubTimeoutContext->UrbTimeoutSpinLock, &OldIrql);
    IsCompleted = HubTimeoutContext->IsNormalCompleted;
    KeReleaseSpinLock(&HubTimeoutContext->UrbTimeoutSpinLock, OldIrql);

    if (!IsCompleted)
    {
        IoCancelIrp(HubTimeoutContext->Irp);
    }

    KeSetEvent(&HubTimeoutContext->UrbTimeoutEvent,
               EVENT_INCREMENT,
               FALSE);
}

NTSTATUS
NTAPI
USBH_SyncSubmitUrb(IN PDEVICE_OBJECT DeviceObject,
                   IN PURB Urb)
{
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock;
    PIRP Irp;
    PIO_STACK_LOCATION IoStack;
    PUSBHUB_URB_TIMEOUT_CONTEXT HubTimeoutContext;
    BOOLEAN IsWaitTimeout = FALSE;
    LARGE_INTEGER DueTime = {{0, 0}};
    NTSTATUS Status;

    DPRINT("USBH_SyncSubmitUrb: ... \n");

    Urb->UrbHeader.UsbdDeviceHandle = NULL;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_SUBMIT_URB,
                                        DeviceObject,
                                        NULL,
                                        0,
                                        NULL,
                                        0,
                                        TRUE,
                                        &Event,
                                        &IoStatusBlock);

    if (!Irp)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
    }

    IoStack = IoGetNextIrpStackLocation(Irp);
    IoStack->Parameters.Others.Argument1 = Urb;

    HubTimeoutContext = ExAllocatePoolWithTag(NonPagedPool,
                                              sizeof(USBHUB_URB_TIMEOUT_CONTEXT),
                                              USB_HUB_TAG);

    if (HubTimeoutContext)
    {
        HubTimeoutContext->Irp = Irp;
        HubTimeoutContext->IsNormalCompleted = FALSE;

        KeInitializeEvent(&HubTimeoutContext->UrbTimeoutEvent,
                          NotificationEvent,
                          FALSE);

        KeInitializeSpinLock(&HubTimeoutContext->UrbTimeoutSpinLock);
        KeInitializeTimer(&HubTimeoutContext->UrbTimeoutTimer);

        KeInitializeDpc(&HubTimeoutContext->UrbTimeoutDPC,
                        USBH_UrbTimeoutDPC,
                        HubTimeoutContext);

        DueTime.QuadPart -= 5000 * 10000; // Timeout 5 sec.

        KeSetTimer(&HubTimeoutContext->UrbTimeoutTimer,
                   DueTime,
                   &HubTimeoutContext->UrbTimeoutDPC);

        IoSetCompletionRoutine(Irp,
                               USBH_SyncIrpComplete,
                               HubTimeoutContext,
                               TRUE,
                               TRUE,
                               TRUE);

        IsWaitTimeout = TRUE;
    }

    Status = IoCallDriver(DeviceObject, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);
    }
    else
    {
        IoStatusBlock.Status = Status;
    }

    if (IsWaitTimeout)
    {
        KeWaitForSingleObject(&HubTimeoutContext->UrbTimeoutEvent,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);

       ExFreePool(HubTimeoutContext);
    }

    Status = IoStatusBlock.Status;

    return Status;
}

NTSTATUS
NTAPI
USBH_FdoSyncSubmitUrb(IN PDEVICE_OBJECT FdoDevice,
                      IN PURB Urb)
{
    PUSBHUB_FDO_EXTENSION HubExtension;

    DPRINT("USBH_FdoSyncSubmitUrb: FdoDevice - %p, Urb - %p\n",
           FdoDevice,
           Urb);

    HubExtension = (PUSBHUB_FDO_EXTENSION)FdoDevice->DeviceExtension;
    return USBH_SyncSubmitUrb(HubExtension->LowerDevice, Urb);
}

NTSTATUS
NTAPI
USBH_Transact(IN PUSBHUB_FDO_EXTENSION HubExtension,
              IN PVOID TransferBuffer,
              IN ULONG BufferLen,
              IN BOOLEAN Direction,
              IN USHORT Function,
              IN BM_REQUEST_TYPE RequestType,
              IN UCHAR Request,
              IN USHORT RequestValue,
              IN USHORT RequestIndex)
{
    struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST * Urb;
    ULONG TransferFlags;
    PVOID Buffer = NULL;
    NTSTATUS Status;

    DPRINT("USBH_Transact: ... \n");

    if (BufferLen)
    {
        Buffer = ExAllocatePoolWithTag(NonPagedPool,
                                       (BufferLen + sizeof(ULONG)) & ~((sizeof(ULONG) - 1)),
                                       USB_HUB_TAG);

        if (!Buffer)
        {
            return STATUS_INSUFFICIENT_RESOURCES;
        }
    }

    Urb = ExAllocatePoolWithTag(NonPagedPool,
                                sizeof(struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST),
                                USB_HUB_TAG);

    if (!Urb)
    {
        if (Buffer)
        {
            ExFreePool(Buffer);
        }

        return STATUS_INSUFFICIENT_RESOURCES;
    }

    if (Direction)
    {
        if (BufferLen)
        {
            RtlCopyMemory(Buffer, TransferBuffer, BufferLen);
        }

        TransferFlags = USBD_TRANSFER_DIRECTION_OUT;
    }
    else
    {
        if (BufferLen)
        {
            RtlZeroMemory(TransferBuffer, BufferLen);
        }

        TransferFlags = USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK;
    }

    Urb->Hdr.Length = sizeof(struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST);
    Urb->Hdr.Function = Function;
    Urb->Hdr.UsbdDeviceHandle = NULL;

    Urb->TransferFlags = TransferFlags;
    Urb->TransferBuffer = BufferLen != 0 ? Buffer : NULL;
    Urb->TransferBufferLength = BufferLen;
    Urb->TransferBufferMDL = NULL;
    Urb->UrbLink = NULL;

    Urb->RequestTypeReservedBits = RequestType.B;
    Urb->Request = Request;
    Urb->Value = RequestValue;
    Urb->Index = RequestIndex;

    Status = USBH_FdoSyncSubmitUrb(HubExtension->Common.SelfDevice, (PURB)Urb);

    if (!Direction && BufferLen)
    {
        RtlCopyMemory(TransferBuffer, Buffer, BufferLen);
    }

    if (Buffer)
    {
        ExFreePool(Buffer);
    }

    ExFreePool(Urb);

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncResetPort(IN PUSBHUB_FDO_EXTENSION HubExtension,
                   IN USHORT Port)
{
    USBHUB_PORT_STATUS PortStatus;
    KEVENT Event;
    LARGE_INTEGER Timeout = {{0, 0}};
    ULONG ix;
    NTSTATUS Status;

    DPRINT("USBH_SyncResetPort: Port - %x\n", Port);

    InterlockedIncrement(&HubExtension->PendingRequestCount);

    KeWaitForSingleObject(&HubExtension->HubPortSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    Status = USBH_SyncGetPortStatus(HubExtension,
                                    Port,
                                    &PortStatus,
                                    4);

    if (NT_SUCCESS(Status) && !PortStatus.UsbPortStatus.ConnectStatus)
    {
        Status = STATUS_UNSUCCESSFUL;
        goto Exit;
    }

    HubExtension->HubFlags |= USBHUB_FDO_FLAG_RESET_PORT_LOCK;
    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    ix = 0;

    while (TRUE)
    {
        BM_REQUEST_TYPE RequestType;

        InterlockedExchange((PLONG)&HubExtension->pResetPortEvent,
                            (LONG)&Event);

        RequestType.B = 0;//0x23
        RequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
        RequestType._BM.Type = BMREQUEST_CLASS;
        RequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;

        Status = USBH_Transact(HubExtension,
                               NULL,
                               0,
                               1, // to device
                               URB_FUNCTION_CLASS_OTHER,
                               RequestType,
                               USB_REQUEST_SET_FEATURE,
                               USBHUB_FEATURE_PORT_RESET,
                               Port);

        Timeout.QuadPart -= 5000 * 10000;

        if (!NT_SUCCESS(Status))
        {
            InterlockedExchange((PLONG)&HubExtension->pResetPortEvent, 0);

            USBH_Wait(10);
            HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_RESET_PORT_LOCK;

            goto Exit;
        }

        Status = KeWaitForSingleObject(&Event,
                                       Suspended,
                                       KernelMode,
                                       FALSE,
                                       &Timeout);

        if (Status != STATUS_TIMEOUT)
        {
            break;
        }

        Status = USBH_SyncGetPortStatus(HubExtension,
                                        Port,
                                        &PortStatus,
                                        4);

        if (!NT_SUCCESS(Status) ||
            !PortStatus.UsbPortStatus.ConnectStatus ||
            ix >= 3)
        {
            InterlockedExchange((PLONG)&HubExtension->pResetPortEvent, 0);

            USBH_Wait(10);
            HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_RESET_PORT_LOCK;

            Status = STATUS_DEVICE_DATA_ERROR;
            goto Exit;
        }

        ++ix;

        KeInitializeEvent(&Event, NotificationEvent, FALSE);
    }

    if (HubExtension->HubFlags & USBHUB_FDO_FLAG_USB20_HUB)
    {
        Status = USBH_SyncGetPortStatus(HubExtension,
                                        Port,
                                        &PortStatus,
                                        4);

        if (!NT_SUCCESS(Status) && !PortStatus.UsbPortStatus.ConnectStatus)
        {
            USBH_Wait(10);
            HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_RESET_PORT_LOCK;

            Status = STATUS_DEVICE_DATA_ERROR;
        }
    }

Exit:

    KeReleaseSemaphore(&HubExtension->HubPortSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    if (!InterlockedDecrement(&HubExtension->PendingRequestCount))
    {
        KeSetEvent(&HubExtension->PendingRequestEvent,
                   EVENT_INCREMENT,
                   FALSE);
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_GetDeviceType(IN PUSBHUB_FDO_EXTENSION HubExtension,
                   IN PUSB_DEVICE_HANDLE DeviceHandle,
                   OUT USB_DEVICE_TYPE * OutDeviceType)
{
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;
    PUSB_DEVICE_INFORMATION_0 DeviceInfo;
    SIZE_T DeviceInformationBufferLength;
    USB_DEVICE_TYPE DeviceType = Usb11Device;
    ULONG dummy;
    NTSTATUS Status;

    DPRINT("USBH_GetDeviceType: ... \n");

    QueryDeviceInformation = HubExtension->BusInterface.QueryDeviceInformation;

    if (!QueryDeviceInformation)
    {
        DPRINT1("USBH_GetDeviceType: no QueryDeviceInformation()\n");
        return STATUS_NOT_IMPLEMENTED;
    }

    DeviceInformationBufferLength = sizeof(USB_DEVICE_INFORMATION_0);

    while (TRUE)
    {
        DeviceInfo = ExAllocatePoolWithTag(PagedPool,
                                           DeviceInformationBufferLength,
                                           USB_HUB_TAG);

        if (!DeviceInfo)
        {
            DPRINT1("USBH_GetDeviceType: ExAllocatePoolWithTag() failed\n");
            Status = STATUS_INSUFFICIENT_RESOURCES;
            break;
        }

        DeviceInfo->InformationLevel = 0;

        Status = QueryDeviceInformation(HubExtension->BusInterface.BusContext,
                                        DeviceHandle,
                                        DeviceInfo,
                                        DeviceInformationBufferLength,
                                        &dummy);

        if (Status != STATUS_BUFFER_TOO_SMALL)
        {
            if (NT_SUCCESS(Status))
            {
                DeviceType = DeviceInfo->DeviceType;
            }

            ExFreePool(DeviceInfo);
            break;
        }

        DeviceInformationBufferLength = DeviceInfo->ActualLength;
        ExFreePool(DeviceInfo);
    }

    if (OutDeviceType)
    {
        *OutDeviceType = DeviceType;
        DPRINT("USBH_GetDeviceType: DeviceType - %x\n", DeviceType);
    }

    return Status;
}

NTSTATUS
NTAPI
USBHUB_GetExtendedHubInfo(IN PUSBHUB_FDO_EXTENSION HubExtension,
                          IN PUSB_EXTHUB_INFORMATION_0 HubInfoBuffer)
{
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    ULONG dummy = 0;

    DPRINT("USBHUB_GetExtendedHubInfo: ... \n");

    GetExtendedHubInformation = HubExtension->BusInterface.GetExtendedHubInformation;

    return GetExtendedHubInformation(HubExtension->BusInterface.BusContext,
                                     HubExtension->LowerPDO,
                                     HubInfoBuffer,
                                     sizeof(USB_EXTHUB_INFORMATION_0),
                                     &dummy);
}

NTSTATUS
NTAPI
USBH_SyncGetRootHubPdo(IN PDEVICE_OBJECT DeviceObject,
                       IN OUT PDEVICE_OBJECT * OutPdo1,
                       IN OUT PDEVICE_OBJECT * OutPdo2)
{
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock;
    PIRP Irp;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status;
    NTSTATUS result;

    DPRINT("USBH_SyncGetRootHubPdo: ... \n");

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO,
                                        DeviceObject,
                                        NULL,
                                        0,
                                        NULL,
                                        0,
                                        TRUE,
                                        &Event,
                                        &IoStatusBlock);

    if (!Irp)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    IoStack = IoGetNextIrpStackLocation(Irp);
    IoStack->Parameters.Others.Argument1 = OutPdo1;
    IoStack->Parameters.Others.Argument2 = OutPdo2;

    result = IoCallDriver(DeviceObject, Irp);

    if (result == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);
    }
    else
    {
        IoStatusBlock.Status = result;
    }

    Status = IoStatusBlock.Status;

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncGetHubCount(IN PDEVICE_OBJECT DeviceObject,
                     IN OUT PULONG OutHubCount)
{
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock;
    PIRP Irp;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status;
    NTSTATUS result;

    DPRINT("USBH_SyncGetHubCount: *OutHubCount - %x\n", *OutHubCount);

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_GET_HUB_COUNT,
                                        DeviceObject,
                                        NULL,
                                        0,
                                        NULL,
                                        0,
                                        TRUE,
                                        &Event,
                                        &IoStatusBlock);

    if (!Irp)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    IoStack = IoGetNextIrpStackLocation(Irp);
    IoStack->Parameters.Others.Argument1 = OutHubCount;

    result = IoCallDriver(DeviceObject, Irp);

    if (result == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);
    }
    else
    {
        IoStatusBlock.Status = result;
    }

    Status = IoStatusBlock.Status;

    return Status;
}

PUSB_DEVICE_HANDLE
NTAPI
USBH_SyncGetDeviceHandle(IN PDEVICE_OBJECT DeviceObject)
{
    PIRP Irp;
    KEVENT Event;
    IO_STATUS_BLOCK IoStatusBlock;
    PUSB_DEVICE_HANDLE DeviceHandle = NULL;
    PIO_STACK_LOCATION IoStack;

    DPRINT("USBH_SyncGetDeviceHandle: ... \n");

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Irp = IoBuildDeviceIoControlRequest(IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE,
                                        DeviceObject,
                                        NULL,
                                        0,
                                        NULL,
                                        0,
                                        TRUE,
                                        &Event,
                                        &IoStatusBlock);

    if (!Irp)
    {
        DPRINT1("USBH_SyncGetDeviceHandle: Irp - NULL!\n");
        return NULL;
    }

    IoStack = IoGetNextIrpStackLocation(Irp);
    IoStack->Parameters.Others.Argument1 = &DeviceHandle;

    if (IoCallDriver(DeviceObject, Irp) == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);
    }

    return DeviceHandle;
}

NTSTATUS
NTAPI
USBH_GetDeviceDescriptor(IN PDEVICE_OBJECT DeviceObject,
                         IN PUSB_DEVICE_DESCRIPTOR HubDeviceDescriptor)
{
    struct _URB_CONTROL_DESCRIPTOR_REQUEST * Urb;
    NTSTATUS Status;

    DPRINT("USBH_GetDeviceDescriptor: ... \n");

    Urb = ExAllocatePoolWithTag(NonPagedPool,
                                sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                                USB_HUB_TAG);

    if (!Urb)
    {
        DPRINT1("USBH_SyncGetDeviceHandle: Urb - NULL!\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST));

    Urb->Hdr.Function = URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE;
    Urb->Hdr.Length = sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST);

    Urb->TransferBufferLength = sizeof(USB_DEVICE_DESCRIPTOR);
    Urb->TransferBuffer = HubDeviceDescriptor;
    Urb->DescriptorType = USB_DEVICE_DESCRIPTOR_TYPE;

    Status = USBH_FdoSyncSubmitUrb(DeviceObject, (PURB)Urb);

    ExFreePool(Urb);

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncGetDeviceConfigurationDescriptor(IN PDEVICE_OBJECT DeviceObject,
                                          IN PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor,
                                          IN ULONG NumberOfBytes,
                                          IN PULONG OutLength)
{
    PCOMMON_DEVICE_EXTENSION DeviceExtension;
    struct _URB_CONTROL_DESCRIPTOR_REQUEST * Urb;
    NTSTATUS Status;

    DPRINT("USBH_SyncGetDeviceConfigurationDescriptor: ... \n");

    DeviceExtension = (PCOMMON_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

    if (OutLength)
    {
        *OutLength = 0;
    }

    Urb = ExAllocatePoolWithTag(NonPagedPool,
                                sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                                USB_HUB_TAG);

    if (!Urb)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST));

    Urb->Hdr.Function = URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE;
    Urb->Hdr.Length = sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST);

    Urb->TransferBufferLength = NumberOfBytes;
    Urb->TransferBuffer = ConfigDescriptor;
    Urb->DescriptorType = USB_CONFIGURATION_DESCRIPTOR_TYPE;

    if (DeviceExtension->ExtensionType == USBH_EXTENSION_TYPE_HUB ||
        DeviceExtension->ExtensionType == USBH_EXTENSION_TYPE_PARENT)
    {
        Status = USBH_FdoSyncSubmitUrb(DeviceObject, (PURB)Urb);
    }
    else
    {
        Status = USBH_SyncSubmitUrb(DeviceObject, (PURB)Urb);
    }

    if (OutLength)
    {
        *OutLength = Urb->TransferBufferLength;
    }

    if (Urb)
    {
        ExFreePool(Urb);
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_GetConfigurationDescriptor(IN PDEVICE_OBJECT DeviceObject,
                                IN PUSB_CONFIGURATION_DESCRIPTOR * pConfigurationDescriptor)
{
    PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor;
    NTSTATUS Status;
    ULONG Length;

    DPRINT("USBH_GetConfigurationDescriptor: ... \n");

    ConfigDescriptor = ExAllocatePoolWithTag(NonPagedPool, 0xFF, USB_HUB_TAG);

    if (!ConfigDescriptor)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ErrorExit;
    }

    Status = USBH_SyncGetDeviceConfigurationDescriptor(DeviceObject,
                                                       ConfigDescriptor,
                                                       0xFF,
                                                       &Length);

    if (!NT_SUCCESS(Status))
    {
        goto ErrorExit;
    }

    if (Length < sizeof(USB_CONFIGURATION_DESCRIPTOR))
    {
        Status = STATUS_DEVICE_DATA_ERROR;
        goto ErrorExit;
    }

    if (Length < ConfigDescriptor->wTotalLength)
    {
        Status = STATUS_DEVICE_DATA_ERROR;
        goto ErrorExit;
    }

    if (ConfigDescriptor->wTotalLength > 0xFF)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ErrorExit;
    }

    *pConfigurationDescriptor = ConfigDescriptor;

     return Status;

ErrorExit:

    if (ConfigDescriptor)
    {
        ExFreePool(ConfigDescriptor);
    }

    *pConfigurationDescriptor = NULL;

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncGetHubDescriptor(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PUSB_EXTHUB_INFORMATION_0 ExtendedHubInfo;
    ULONG NumberPorts;
    PUSBHUB_PORT_DATA PortData;
    USHORT RequestValue;
    ULONG NumberOfBytes;
    NTSTATUS Status;
    PUSB_HUB_DESCRIPTOR HubDescriptor;
    ULONG ix;

    DPRINT("USBH_SyncGetHubDescriptor: ... \n");

    ExtendedHubInfo = ExAllocatePoolWithTag(NonPagedPool,
                                            sizeof(USB_EXTHUB_INFORMATION_0),
                                            USB_HUB_TAG);

    if (!ExtendedHubInfo)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ErrorExit;
    }

    Status = USBHUB_GetExtendedHubInfo(HubExtension, ExtendedHubInfo);

    if (!NT_SUCCESS(Status))
    {
        ExFreePool(ExtendedHubInfo);
    }

    NumberOfBytes = sizeof(USB_HUB_DESCRIPTOR);

    HubDescriptor = ExAllocatePoolWithTag(NonPagedPool,
                                          NumberOfBytes,
                                          USB_HUB_TAG);

    if (!HubDescriptor)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ErrorExit;
    }

    RequestValue = 0;

    while (TRUE)
    {
        while (TRUE)
        {
            BM_REQUEST_TYPE RequestType;

            RequestType.B = 0;//0xA0
            RequestType._BM.Recipient = 0;
            RequestType._BM.Type = 0;
            RequestType._BM.Dir = 0;

            Status = USBH_Transact(HubExtension,
                                   HubDescriptor,
                                   NumberOfBytes,
                                   0,
                                   URB_FUNCTION_CLASS_DEVICE,
                                   RequestType,
                                   USB_REQUEST_GET_DESCRIPTOR,
                                   RequestValue,
                                   0);

            if (NT_SUCCESS(Status))
            {
                break;
            }

            RequestValue = 0x2900; // Hub DescriptorType - 0x29
        }

        if (HubDescriptor->bDescriptorLength <= NumberOfBytes)
        {
            break;
        }

        NumberOfBytes = HubDescriptor->bDescriptorLength;
        ExFreePool(HubDescriptor);

        HubDescriptor = ExAllocatePoolWithTag(NonPagedPool,
                                              NumberOfBytes,
                                              USB_HUB_TAG);

        if (!HubDescriptor)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto ErrorExit;
        }
    }

    NumberPorts = HubDescriptor->bNumberOfPorts;

    if (HubExtension->PortData)
    {
        PortData = HubExtension->PortData;

        if (HubDescriptor->bNumberOfPorts)
        {
            for (ix = 0; ix < NumberPorts; ix++)
            {
                PortData[ix].PortStatus.AsULONG = 0;

                if (ExtendedHubInfo)
                {
                    PortData[ix].PortAttributes = ExtendedHubInfo->Port[ix].PortAttributes;
                }
                else
                {
                    PortData[ix].PortAttributes = 0;
                }

                PortData[ix].ConnectionStatus = NoDeviceConnected;

                if (PortData[ix].DeviceObject != NULL)
                {
                    PortData[ix].ConnectionStatus = DeviceConnected;
                }
            }
        }
    }
    else
    {
        PortData = NULL;

        if (HubDescriptor->bNumberOfPorts)
        {
            PortData = ExAllocatePoolWithTag(NonPagedPool,
                                             NumberPorts * sizeof(USBHUB_PORT_DATA),
                                             USB_HUB_TAG);
        }

        if (!PortData)
        {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto ErrorExit;
        }

        RtlZeroMemory(PortData, NumberPorts * sizeof(USBHUB_PORT_DATA));

        if (NumberPorts)
        {
            for (ix = 0; ix < NumberPorts; ix++)
            {
                PortData[ix].ConnectionStatus = NoDeviceConnected;

                if (ExtendedHubInfo)
                {
                    PortData[ix].PortAttributes = ExtendedHubInfo->Port[ix].PortAttributes;
                }
            }
       }
    }

    if (!NT_SUCCESS(Status))
    {
        goto ErrorExit;
    }

    HubExtension->HubDescriptor = HubDescriptor;

    HubExtension->PortData = PortData;

    if (ExtendedHubInfo)
    {
        ExFreePool(ExtendedHubInfo);
    }

    return Status;

ErrorExit:

    if (HubDescriptor)
    {
        ExFreePool(HubDescriptor);
    }

    if (ExtendedHubInfo)
    {
        ExFreePool(ExtendedHubInfo);
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncGetStringDescriptor(IN PDEVICE_OBJECT DeviceObject,
                             IN UCHAR Index,
                             IN USHORT LanguageId,
                             IN PUSB_STRING_DESCRIPTOR Descriptor,
                             IN ULONG NumberOfBytes,
                             IN PULONG OutLength,
                             IN BOOLEAN IsValidateLength)
{
    struct _URB_CONTROL_DESCRIPTOR_REQUEST * Urb;
    ULONG TransferedLength;
    NTSTATUS Status;

    DPRINT("USBH_SyncGetStringDescriptor: Index - %x, LanguageId - %x\n",
           Index,
           LanguageId);

    Urb = ExAllocatePoolWithTag(NonPagedPool,
                                sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST),
                                USB_HUB_TAG);

    if (!Urb)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST));

    Urb->Hdr.Function = URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE;
    Urb->Hdr.Length = sizeof(struct _URB_CONTROL_DESCRIPTOR_REQUEST);

    Urb->TransferBuffer = Descriptor;
    Urb->TransferBufferLength = NumberOfBytes;

    Urb->Index = Index;
    Urb->DescriptorType = USB_STRING_DESCRIPTOR_TYPE;
    Urb->LanguageId = LanguageId;

    Status = USBH_SyncSubmitUrb(DeviceObject, (PURB)Urb);

    if (!NT_SUCCESS(Status))
    {
        ExFreePool(Urb);
        return Status;
    }

    TransferedLength = Urb->TransferBufferLength;

    if (TransferedLength > NumberOfBytes)
    {
        Status = STATUS_DEVICE_DATA_ERROR;
    }

    if (!NT_SUCCESS(Status))
    {
        ExFreePool(Urb);
        return Status;
    }

    if (OutLength)
    {
        *OutLength = TransferedLength;
    }

    if (IsValidateLength && TransferedLength != Descriptor->bLength)
    {
        Status = STATUS_DEVICE_DATA_ERROR;
    }

    ExFreePool(Urb);

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncGetStatus(IN PDEVICE_OBJECT DeviceObject,
                   IN PUSHORT OutStatus,
                   IN USHORT Function,
                   IN USHORT RequestIndex)
{
    struct _URB_CONTROL_GET_STATUS_REQUEST * Urb;
    NTSTATUS NtStatus;
    USHORT UsbStatus;

    DPRINT("USBH_SyncGetStatus: ... \n");

    Urb = ExAllocatePoolWithTag(NonPagedPool,
                                sizeof(struct _URB_CONTROL_GET_STATUS_REQUEST),
                                USB_HUB_TAG);

    if (!Urb)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_GET_STATUS_REQUEST));

    Urb->Hdr.Length = sizeof(struct _URB_CONTROL_GET_STATUS_REQUEST);
    Urb->Hdr.Function = Function;

    Urb->TransferBuffer = &UsbStatus;
    Urb->TransferBufferLength = sizeof(UsbStatus);
    Urb->Index = RequestIndex;

    NtStatus = USBH_FdoSyncSubmitUrb(DeviceObject, (PURB)Urb);

    *OutStatus = UsbStatus;

    ExFreePool(Urb);

    return NtStatus;
}

NTSTATUS
NTAPI
USBH_SyncGetPortStatus(IN PUSBHUB_FDO_EXTENSION HubExtension,
                       IN USHORT Port,
                       IN PUSBHUB_PORT_STATUS PortStatus,
                       IN ULONG Length)
{
    BM_REQUEST_TYPE RequestType;

    DPRINT("USBH_SyncGetPortStatus: Port - %x\n", Port);

    RequestType.B = 0;//0xA3
    RequestType._BM.Recipient = BMREQUEST_TO_OTHER;
    RequestType._BM.Type = BMREQUEST_CLASS;
    RequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;

    return USBH_Transact(HubExtension,
                         PortStatus,
                         Length,
                         0, // to host
                         URB_FUNCTION_CLASS_OTHER,
                         RequestType,
                         USB_REQUEST_GET_STATUS,
                         0,
                         Port);
}


NTSTATUS
NTAPI
USBH_SyncClearPortStatus(IN PUSBHUB_FDO_EXTENSION HubExtension,
                         IN USHORT Port,
                         IN USHORT RequestValue)
{
    BM_REQUEST_TYPE RequestType;

    DPRINT("USBH_SyncClearPortStatus: Port - %x, RequestValue - %x\n",
           Port,
           RequestValue);

    RequestType.B = 0;//0x23
    RequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
    RequestType._BM.Type = BMREQUEST_CLASS;
    RequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;

    return USBH_Transact(HubExtension,
                         NULL,
                         0,
                         1, // to device
                         URB_FUNCTION_CLASS_OTHER,
                         RequestType,
                         USB_REQUEST_CLEAR_FEATURE,
                         RequestValue,
                         Port);
}

NTSTATUS
NTAPI
USBH_SyncPowerOnPort(IN PUSBHUB_FDO_EXTENSION HubExtension,
                     IN USHORT Port,
                     IN BOOLEAN IsWait)
{
    PUSBHUB_PORT_DATA PortData;
    PUSB_HUB_DESCRIPTOR HubDescriptor;
    NTSTATUS Status;
    BM_REQUEST_TYPE RequestType;

    DPRINT("USBH_SyncPowerOnPort: Port - %x, IsWait - %x\n", Port, IsWait);

    PortData = &HubExtension->PortData[Port - 1];

    HubDescriptor = HubExtension->HubDescriptor;

    if (PortData->PortStatus.UsbPortStatus.ConnectStatus)
    {
        return STATUS_SUCCESS;
    }

    RequestType.B = 0;
    RequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
    RequestType._BM.Type = BMREQUEST_CLASS;
    RequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;

    Status = USBH_Transact(HubExtension,
                           NULL,
                           0,
                           1,
                           URB_FUNCTION_CLASS_OTHER,
                           RequestType,
                           USB_REQUEST_SET_FEATURE,
                           USBHUB_FEATURE_PORT_POWER,
                           Port);

    if (NT_SUCCESS(Status))
    {
        if (IsWait)
        {
            USBH_Wait(2 * HubDescriptor->bPowerOnToPowerGood);
        }

        PortData->PortStatus.UsbPortStatus.ConnectStatus = 1;
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_SyncPowerOnPorts(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PUSB_HUB_DESCRIPTOR HubDescriptor;
    ULONG NumberOfPorts;
    ULONG Port;
    NTSTATUS Status;

    DPRINT("USBH_SyncPowerOnPorts: ... \n");

    HubDescriptor = HubExtension->HubDescriptor;
    NumberOfPorts = HubDescriptor->bNumberOfPorts;

    Port = 1;

    if (HubDescriptor->bNumberOfPorts)
    {
        do
        {
            Status = USBH_SyncPowerOnPort(HubExtension, Port, 0);

            if (!NT_SUCCESS(Status))
            {
                DPRINT1("USBH_SyncPowerOnPorts: USBH_SyncPowerOnPort() failed - %p\n",
                        Status);
                break;
            }

            ++Port;
        }
        while (Port <= NumberOfPorts);
    }

    USBH_Wait(2 * HubDescriptor->bPowerOnToPowerGood);

    return Status;
}

BOOLEAN
NTAPI
USBH_HubIsBusPowered(IN PDEVICE_OBJECT DeviceObject,
                     IN PUSB_CONFIGURATION_DESCRIPTOR HubConfigDescriptor)
{
    BOOLEAN Result;
    USHORT UsbStatus;
    NTSTATUS Status;

    DPRINT("USBH_HubIsBusPowered: ... \n");

    Status = USBH_SyncGetStatus(DeviceObject,
                                &UsbStatus,
                                URB_FUNCTION_GET_STATUS_FROM_DEVICE,
                                0);

    if (!NT_SUCCESS(Status))
    {
        Result = (HubConfigDescriptor->bmAttributes & 0xC0) == 0x80;
    }
    else
    {
        Result = ~UsbStatus & 1; //SelfPowered bit from status word
    }

    return Result;
}

NTSTATUS
NTAPI
USBH_ChangeIndication(IN PDEVICE_OBJECT DeviceObject,
                      IN PIRP Irp,
                      IN PVOID Context)
{
    DPRINT1("USBH_ChangeIndication: UNIMPLEMENTED. FIXME. \n");
    DbgBreakPoint();
    return 0;
}

NTSTATUS
NTAPI
USBH_SubmitStatusChangeTransfer(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PIRP Irp;
    NTSTATUS Status;
    struct _URB_BULK_OR_INTERRUPT_TRANSFER * Urb;
    PIO_STACK_LOCATION IoStack;

    DPRINT("USBH_SubmitStatusChangeTransfer ... \n");

    if (HubExtension->HubFlags & USBHUB_FDO_FLAG_NOT_D0_STATE)
    {
        return STATUS_INVALID_DEVICE_STATE;
    }

    Irp = HubExtension->SCEIrp;

    if (!Irp)
    {
        return STATUS_INVALID_DEVICE_STATE;
    }

    Urb = (struct _URB_BULK_OR_INTERRUPT_TRANSFER *)&HubExtension->SCEWorkerUrb;

    Urb->Hdr.Length = sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER);
    Urb->Hdr.Function = URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
    Urb->Hdr.UsbdDeviceHandle = NULL;

    Urb->PipeHandle = HubExtension->PipeInfo.PipeHandle;
    Urb->TransferFlags = USBD_SHORT_TRANSFER_OK;
    Urb->TransferBuffer = HubExtension->SCEBitmap;
    Urb->TransferBufferLength = HubExtension->SCEBitmapLength;
    Urb->TransferBufferMDL = NULL;
    Urb->UrbLink = NULL;

    IoInitializeIrp(Irp,
                    IoSizeOfIrp(HubExtension->LowerDevice->StackSize),
                    HubExtension->LowerDevice->StackSize);

    IoStack = IoGetNextIrpStackLocation(Irp);

    IoStack->MajorFunction = IRP_MJ_INTERNAL_DEVICE_CONTROL;
    IoStack->Parameters.Others.Argument1 = &HubExtension->SCEWorkerUrb;
    IoStack->Parameters.DeviceIoControl.IoControlCode = IOCTL_INTERNAL_USB_SUBMIT_URB;

    IoSetCompletionRoutine(Irp,
                          USBH_ChangeIndication,
                          HubExtension,
                          TRUE,
                          TRUE,
                          TRUE);

    KeResetEvent(&HubExtension->StatusChangeEvent);

    Status = IoCallDriver(HubExtension->LowerDevice, Irp);

    return Status;
}

NTSTATUS
NTAPI
USBD_CreateDeviceEx(IN PUSBHUB_FDO_EXTENSION HubExtension,
                    IN PUSB_DEVICE_HANDLE * OutDeviceHandle,
                    IN USB_PORT_STATUS UsbPortStatus,
                    IN USHORT Port)
{
    PUSB_DEVICE_HANDLE HubDeviceHandle;
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;

    DPRINT("USBD_CreateDeviceEx: Port - %x, UsbPortStatus - 0x%04X\n",
           Port,
           UsbPortStatus.AsUSHORT);

    CreateUsbDevice = HubExtension->BusInterface.CreateUsbDevice;

    if (!CreateUsbDevice)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    HubDeviceHandle = USBH_SyncGetDeviceHandle(HubExtension->LowerDevice);

    return CreateUsbDevice(HubExtension->BusInterface.BusContext,
                           OutDeviceHandle,
                           HubDeviceHandle,
                           UsbPortStatus.AsUSHORT,
                           Port);
}

NTSTATUS
NTAPI
USBD_RemoveDeviceEx(IN PUSBHUB_FDO_EXTENSION HubExtension,
                    IN PUSB_DEVICE_HANDLE DeviceHandle,
                    IN ULONG Flags)
{
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;

    DPRINT("USBD_RemoveDeviceEx: DeviceHandle - %p, Flags - x\n",
           DeviceHandle,
           Flags);

    RemoveUsbDevice = HubExtension->BusInterface.RemoveUsbDevice;

    if (!RemoveUsbDevice)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    return RemoveUsbDevice(HubExtension->BusInterface.BusContext,
                           DeviceHandle,
                           Flags);
}

NTSTATUS
NTAPI
USBD_InitializeDeviceEx(IN PUSBHUB_FDO_EXTENSION HubExtension,
                        IN PUSB_DEVICE_HANDLE DeviceHandle,
                        IN PUCHAR DeviceDescriptorBuffer,
                        IN ULONG DeviceDescriptorBufferLength,
                        IN PUCHAR ConfigDescriptorBuffer,
                        IN ULONG ConfigDescriptorBufferLength)
{
    NTSTATUS Status;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;

    DPRINT("USBD_InitializeDeviceEx: ... \n");

    InitializeUsbDevice = HubExtension->BusInterface.InitializeUsbDevice;
    GetUsbDescriptors = HubExtension->BusInterface.GetUsbDescriptors;

    if (!InitializeUsbDevice || !GetUsbDescriptors)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    Status = InitializeUsbDevice(HubExtension->BusInterface.BusContext,
                                 DeviceHandle);

    if (!NT_SUCCESS(Status))
    {
        return Status;
    }

    return GetUsbDescriptors(HubExtension->BusInterface.BusContext,
                             DeviceHandle,
                             DeviceDescriptorBuffer,
                             &DeviceDescriptorBufferLength,
                             ConfigDescriptorBuffer,
                             &ConfigDescriptorBufferLength);
}

NTSTATUS
NTAPI
USBH_AllocateWorkItem(PUSBHUB_FDO_EXTENSION HubExtension,
                      PUSBHUB_IO_WORK_ITEM * OutHubIoWorkItem,
                      PVOID WorkerRoutine,
                      SIZE_T BufferLength,
                      PVOID * OutHubWorkItemBuffer,
                      WORK_QUEUE_TYPE Type)
{
    PUSBHUB_IO_WORK_ITEM HubIoWorkItem;
    PIO_WORKITEM WorkItem;
    PVOID WorkItemBuffer;

    DPRINT("USBH_AllocateWorkItem: ... \n");

    if (!(HubExtension->HubFlags & USBHUB_FDO_FLAG_WITEM_INIT))
    {
        return STATUS_INVALID_PARAMETER;
    }

    HubIoWorkItem = ExAllocatePoolWithTag(NonPagedPool,
                                          sizeof(USBHUB_IO_WORK_ITEM),
                                          USB_HUB_TAG);

    if (!HubIoWorkItem)
    {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory(HubIoWorkItem, sizeof(USBHUB_IO_WORK_ITEM));

    WorkItem = IoAllocateWorkItem(HubExtension->Common.SelfDevice);

    HubIoWorkItem->HubWorkItem = WorkItem;

    if (!WorkItem)
    {
        ExFreePool(HubIoWorkItem);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    if (BufferLength && OutHubWorkItemBuffer)
    {
        WorkItemBuffer = ExAllocatePoolWithTag(NonPagedPool,
                                               BufferLength,
                                               USB_HUB_TAG);

        HubIoWorkItem->HubWorkItemBuffer = WorkItemBuffer;

        if (!WorkItemBuffer)
        {
            IoFreeWorkItem(HubIoWorkItem->HubWorkItem);
            ExFreePool(HubIoWorkItem);

            return STATUS_INSUFFICIENT_RESOURCES;
        }
    }
    else
    {
        HubIoWorkItem->HubWorkItemBuffer = NULL;
    }

    HubIoWorkItem->HubWorkItemType = Type;
    HubIoWorkItem->HubExtension = HubExtension;
    HubIoWorkItem->HubWorkerRoutine = WorkerRoutine;

    if (OutHubIoWorkItem)
    {
        *OutHubIoWorkItem = HubIoWorkItem;
    }

    if (OutHubWorkItemBuffer)
    {
        *OutHubWorkItemBuffer = HubIoWorkItem->HubWorkItemBuffer;
    }

    return STATUS_SUCCESS;
}

VOID
NTAPI
USBH_Worker(IN PDEVICE_OBJECT DeviceObject,
            IN PVOID Context)
{
    PUSBHUB_IO_WORK_ITEM HubIoWorkItem;
    PUSBHUB_FDO_EXTENSION HubExtension;
    KIRQL OldIrql;
    PIO_WORKITEM WorkItem;

    DPRINT("UsbhIoWorker: ... \n");

    HubIoWorkItem = (PUSBHUB_IO_WORK_ITEM)Context;

    InterlockedDecrement(&HubIoWorkItem->HubWorkerQueued);

    HubExtension = HubIoWorkItem->HubExtension;
    WorkItem = HubIoWorkItem->HubWorkItem;

    HubIoWorkItem->HubWorkerRoutine(HubIoWorkItem->HubExtension,
                                    HubIoWorkItem->HubWorkItemBuffer);

    KeAcquireSpinLock(&HubExtension->WorkItemSpinLock, &OldIrql);
    RemoveEntryList(&HubIoWorkItem->HubWorkItemLink);
    KeReleaseSpinLock(&HubExtension->WorkItemSpinLock, OldIrql);

    if (HubIoWorkItem->HubWorkItemBuffer)
    {
        ExFreePool(HubIoWorkItem->HubWorkItemBuffer);
    }

    ExFreePool(HubIoWorkItem);

    if (!InterlockedDecrement(&HubExtension->PendingRequestCount))
    {
        KeSetEvent(&HubExtension->PendingRequestEvent,
                   EVENT_INCREMENT,
                   FALSE);
    }

    IoFreeWorkItem(WorkItem);
}

VOID
NTAPI
USBH_QueueWorkItem(IN PUSBHUB_FDO_EXTENSION HubExtension,
                   IN PUSBHUB_IO_WORK_ITEM HubIoWorkItem)
{
    DPRINT("UsbhQueueIoWorkItem: ... \n");

    InterlockedIncrement(&HubExtension->PendingRequestCount);
    InterlockedIncrement(&HubIoWorkItem->HubWorkerQueued);

    ExInterlockedInsertTailList(&HubExtension->WorkItemList,
                                &HubIoWorkItem->HubWorkItemLink,
                                &HubExtension->WorkItemSpinLock);

    IoQueueWorkItem(HubIoWorkItem->HubWorkItem,
                    USBH_Worker,
                    HubIoWorkItem->HubWorkItemType,
                    HubIoWorkItem);
}

VOID
NTAPI
USBH_FreeWorkItem(IN PUSBHUB_IO_WORK_ITEM HubIoWorkItem)
{
    PIO_WORKITEM WorkItem;

    DPRINT("UsbhFreeIoWorkItem: ... \n");

    WorkItem = HubIoWorkItem->HubWorkItem;

    if (HubIoWorkItem->HubWorkItemBuffer)
    {
        ExFreePool(HubIoWorkItem->HubWorkItemBuffer);
    }

    ExFreePool(HubIoWorkItem);

    IoFreeWorkItem(WorkItem);
}

VOID
NTAPI
USBHUB_RootHubCallBack(IN PVOID Context)
{
    PUSBHUB_FDO_EXTENSION HubExtension;

    DPRINT("USBHUB_RootHubCallBack: ... \n");

    HubExtension = (PUSBHUB_FDO_EXTENSION)Context;

    if (HubExtension->SCEIrp)
    {
        HubExtension->HubFlags |= (USBHUB_FDO_FLAG_DO_ENUMERATION |
                                   USBHUB_FDO_FLAG_NOT_ENUMERATED);

        USBH_SubmitStatusChangeTransfer(HubExtension);

        IoInvalidateDeviceRelations(HubExtension->LowerPDO, BusRelations);
    }
    else
    {
        HubExtension->HubFlags |= USBHUB_FDO_FLAG_DO_ENUMERATION;
    }

    KeSetEvent(&HubExtension->RootHubNotificationEvent,
               EVENT_INCREMENT,
               FALSE);
}

NTSTATUS
NTAPI
USBD_RegisterRootHubCallBack(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;

    DPRINT("USBD_RegisterRootHubCallBack: ... \n");

    RootHubInitNotification = HubExtension->BusInterface.RootHubInitNotification;

    if (!RootHubInitNotification)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    KeResetEvent(&HubExtension->RootHubNotificationEvent);

    return RootHubInitNotification(HubExtension->BusInterface.BusContext,
                                   HubExtension,
                                   USBHUB_RootHubCallBack);
}

NTSTATUS
NTAPI
USBD_UnRegisterRootHubCallBack(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;
    NTSTATUS Status;

    DPRINT("USBD_UnRegisterRootHubCallBack ... \n");

    RootHubInitNotification = HubExtension->BusInterface.RootHubInitNotification;

    if (!RootHubInitNotification)
    {
        return STATUS_NOT_IMPLEMENTED;
    }

    Status = RootHubInitNotification(HubExtension->BusInterface.BusContext,
                                     NULL,
                                     NULL);

    if (!NT_SUCCESS(Status))
    {
        KeWaitForSingleObject(&HubExtension->RootHubNotificationEvent,
                              Executive,
                              KernelMode,
                              FALSE,
                              NULL);
    }

    return Status;
}

VOID
NTAPI
USBH_CheckHubIdle(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    DPRINT("USBH_CheckIdleWorker: ... \n");
}

VOID
NTAPI
USBH_CheckIdleWorker(IN PUSBHUB_FDO_EXTENSION HubExtension,
                     IN PVOID Context)
{
    DPRINT("USBH_CheckIdleWorker: ... \n");
    USBH_CheckHubIdle(HubExtension);
}

VOID
NTAPI
USBH_CheckIdleDeferred(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    PUSBHUB_IO_WORK_ITEM HubIoWorkItem;
    NTSTATUS Status;

    DPRINT("USBH_CheckIdleDeferred: ... \n");

    Status = USBH_AllocateWorkItem(HubExtension,
                                   &HubIoWorkItem,
                                   USBH_CheckIdleWorker,
                                   0,
                                   NULL,
                                   DelayedWorkQueue);

    if (NT_SUCCESS(Status))
    {
        USBH_QueueWorkItem(HubExtension, HubIoWorkItem);
    }
}

NTSTATUS
NTAPI
USBH_PdoDispatch(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
                 IN PIRP Irp)
{
    DPRINT("USBH_PdoDispatch: PortExtension - %p, Irp - %p\n",
           PortExtension,
           Irp);

    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBH_FdoDispatch(IN PUSBHUB_FDO_EXTENSION HubExtension,
                 IN PIRP Irp)
{
    PIO_STACK_LOCATION IoStack;
    UCHAR MajorFunction;
    NTSTATUS Status;

    DPRINT("USBH_FdoDispatch: HubExtension - %p, Irp - %p\n",
           HubExtension,
           Irp);

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    MajorFunction = IoStack->MajorFunction;

    switch (MajorFunction)
    {
        case IRP_MJ_CREATE:
        case IRP_MJ_CLOSE:
            USBH_CompleteIrp(Irp, STATUS_SUCCESS);
            break;

        case IRP_MJ_DEVICE_CONTROL:
            Status = USBH_DeviceControl(HubExtension, Irp);
            break;

        case IRP_MJ_PNP:
            Status = USBH_FdoPnP(HubExtension, Irp, IoStack->MinorFunction);
            break;

        case IRP_MJ_POWER:
            Status = USBH_FdoPower(HubExtension, Irp, IoStack->MinorFunction);
            break;

        case IRP_MJ_SYSTEM_CONTROL:
            DPRINT1("USBH_FdoDispatch: USBH_SystemControl() UNIMPLEMENTED. FIXME\n");
            //Status USBH_SystemControl(HubExtension, Irp);
            break;

        case IRP_MJ_INTERNAL_DEVICE_CONTROL:
        default:
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_AddDevice(IN PDRIVER_OBJECT DriverObject,
               IN PDEVICE_OBJECT LowerPDO)
{
    PDEVICE_OBJECT DeviceObject;
    NTSTATUS Status;
    PUSBHUB_FDO_EXTENSION HubExtension;
    PDEVICE_OBJECT LowerDevice;

    DPRINT("USBH_AddDevice: DriverObject - %p, LowerPDO - %p\n",
           DriverObject,
           LowerPDO);

    DeviceObject = NULL;

    Status = IoCreateDevice(DriverObject,
                            sizeof(USBHUB_FDO_EXTENSION),
                            NULL,
                            0x8600,
                            FILE_AUTOGENERATED_DEVICE_NAME,
                            FALSE,
                            &DeviceObject);

    if (!NT_SUCCESS(Status))
    {
        DPRINT1("USBH_AddDevice: IoCreateDevice() fail\n");

        if (DeviceObject)
        {
            IoDeleteDevice(DeviceObject);
        }

        return Status;
    }

    DPRINT("USBH_AddDevice: DeviceObject - %p\n", DeviceObject);

    HubExtension = (PUSBHUB_FDO_EXTENSION)DeviceObject->DeviceExtension;
    RtlZeroMemory(HubExtension, sizeof(USBHUB_FDO_EXTENSION));

    HubExtension->Common.ExtensionType = USBH_EXTENSION_TYPE_HUB;

    LowerDevice = IoAttachDeviceToDeviceStack(DeviceObject, LowerPDO);

    if (!LowerDevice)
    {
        DPRINT1("USBH_AddDevice: IoAttachDeviceToDeviceStack() fail\n");

        if (DeviceObject)
        {
            IoDeleteDevice(DeviceObject);
        }

        return STATUS_UNSUCCESSFUL;
    }

    DPRINT("USBH_AddDevice: LowerDevice  - %p\n", LowerDevice);

    HubExtension->Common.SelfDevice = DeviceObject;

    HubExtension->LowerPDO = LowerPDO;
    HubExtension->LowerDevice = LowerDevice;

    KeInitializeSemaphore(&HubExtension->IdleSemaphore, 1, 1);

    DeviceObject->Flags |= DO_POWER_PAGABLE;
    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    DPRINT("USBH_AddDevice: call IoWMIRegistrationControl() UNIMPLEMENTED. FIXME. \n");

    return Status;
}

VOID
NTAPI
USBH_DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DPRINT("USBH_DriverUnload: UNIMPLEMENTED\n");
}

NTSTATUS
NTAPI
USBH_HubDispatch(IN PDEVICE_OBJECT DeviceObject,
                 IN PIRP Irp)
{
    PCOMMON_DEVICE_EXTENSION DeviceExtension;
    ULONG ExtensionType;
    NTSTATUS Status;

    DPRINT("USBH_HubDispatch: DeviceObject - %p, Irp - %p\n",
           DeviceObject,
           Irp);

    DeviceExtension = (PCOMMON_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
    ExtensionType = DeviceExtension->ExtensionType;

    if (ExtensionType == USBH_EXTENSION_TYPE_HUB)
    {
        Status = USBH_FdoDispatch((PUSBHUB_FDO_EXTENSION)DeviceExtension, Irp);
    }
    else if (ExtensionType == USBH_EXTENSION_TYPE_PORT)
    {
        Status = USBH_PdoDispatch((PUSBHUB_PORT_PDO_EXTENSION)DeviceExtension, Irp);
    }
    else
    {
        DPRINT1("USBH_HubDispatch: Unknown ExtensionType - %x\n", ExtensionType);
        DbgBreakPoint();
    }

    return Status;
}

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    DPRINT("USBHUB: DriverEntry - %wZ\n", RegistryPath);

    DriverObject->DriverExtension->AddDevice = USBH_AddDevice;
    DriverObject->DriverUnload = USBH_DriverUnload;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = USBH_HubDispatch;

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = USBH_HubDispatch;

    DriverObject->MajorFunction[IRP_MJ_PNP] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_POWER] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = USBH_HubDispatch;

    return STATUS_SUCCESS;
}

