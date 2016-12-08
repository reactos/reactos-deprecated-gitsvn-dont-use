#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

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

