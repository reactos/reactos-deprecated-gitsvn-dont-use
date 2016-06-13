#include "usbport.h"

//#define NDEBUG
#include <debug.h>

LIST_ENTRY USBPORT_MiniPortDrivers = {NULL, NULL};
KSPIN_LOCK USBPORT_SpinLock = 0;
BOOLEAN USBPORT_Initialized = FALSE;

PUSBPORT_COMMON_BUFFER_HEADER
NTAPI
USBPORT_AllocateCommonBuffer(PDEVICE_OBJECT FdoDevice,
                             SIZE_T BufferLength)
{
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer = 0;
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PDMA_ADAPTER DmaAdapter= FdoExtention->DmaAdapter;
    SIZE_T HeaderSize = sizeof(USBPORT_COMMON_BUFFER_HEADER);
    ULONG Length = 0;
    ULONG LengthPadded;
    PHYSICAL_ADDRESS LogicalAddress;
    ULONG_PTR BaseVA;
    ULONG_PTR StartBufferVA;
    ULONG_PTR StartBufferPA;

    DPRINT("USBPORT_HalAllocateCommonBuffer: FdoDevice - %p, BufferLength - %p\n",
           FdoDevice,
           BufferLength);

    if (BufferLength == 0)
        goto Exit;

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

static
PUSBPORT_MINIPORT_INTERFACE
NTAPI
USBPORT_FindMiniPort(PDRIVER_OBJECT DriverObject)
{
    KIRQL OldIrql; 
    PLIST_ENTRY List;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface = NULL;

    DPRINT("USBPORT_FindMiniport: ... \n");

    KeAcquireSpinLock(&USBPORT_SpinLock, &OldIrql);

    for (List = USBPORT_MiniPortDrivers.Flink;
         List != &USBPORT_MiniPortDrivers;
         List = List->Flink)
    {
        MiniPortInterface = CONTAINING_RECORD(List,
                                              USBPORT_MINIPORT_INTERFACE,
                                              DriverList);

        if (MiniPortInterface->DriverObject == DriverObject)
        {
            DPRINT("USBPORT_FindMiniport: find MiniPortInterface - %p\n",
                   MiniPortInterface);
            break;
        }
    }

    KeReleaseSpinLock(&USBPORT_SpinLock, OldIrql);

    return MiniPortInterface;
}

static
NTSTATUS
NTAPI
USBPORT_AddDevice(PDRIVER_OBJECT DriverObject,
                  PDEVICE_OBJECT PhysicalDeviceObject)
{
    NTSTATUS Status;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;
    ULONG DeviceNumber = 0;
    WCHAR CharDeviceName[64];
    UNICODE_STRING DeviceName;
    PDEVICE_OBJECT DeviceObject;
    PUSBPORT_DEVICE_EXTENSION FdoExtention;

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

    FdoExtention = (PUSBPORT_DEVICE_EXTENSION)DeviceObject->DeviceExtension;

    RtlZeroMemory(FdoExtention, sizeof(USBPORT_DEVICE_EXTENSION));

    FdoExtention->CommonExtension.SelfDevice = DeviceObject;
    FdoExtention->CommonExtension.LowerPdoDevice = PhysicalDeviceObject;
    FdoExtention->CommonExtension.IsPDO = FALSE;
    FdoExtention->CommonExtension.LowerDevice = IoAttachDeviceToDeviceStack(DeviceObject,
                                                                            PhysicalDeviceObject);

    FdoExtention->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtention +
                                        sizeof(USBPORT_DEVICE_EXTENSION));

    FdoExtention->MiniPortInterface = MiniPortInterface;
    FdoExtention->FdoNameNumber = DeviceNumber;

    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    return Status;
}

VOID
NTAPI
USBPORT_Unload(PDRIVER_OBJECT DriverObject)
{
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;

    DPRINT("USBPORT_Unload: FIXME!\n");

    MiniPortInterface = USBPORT_FindMiniPort(DriverObject);
    if (!MiniPortInterface)
    {
        DPRINT("USBPORT_Unload: CRITICAL ERROR!!! USBPORT_FindMiniPort not found MiniPortInterface\n");
        ASSERT(FALSE);
    }

    ASSERT(FALSE); //FIXME
    // ...
    //MiniPortInterface->DriverUnload(DriverObject); // Call MiniPort _HCI_Unload
    // ...
}

static
NTSTATUS
NTAPI
USBPORT_PdoScsi(PDEVICE_OBJECT PdoDevice,
                PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtention = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS Status;

    DPRINT("USBPORT_PdoScsi: PdoDevice - %p, Irp - %p, IoCtl - %x\n",
           PdoDevice,
           Irp,
           IoStack->Parameters.DeviceIoControl.IoControlCode);

    if (IoStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO\n");

        if (IoStack->Parameters.Others.Argument1)
            *(PVOID *)IoStack->Parameters.Others.Argument1 = PdoDevice;

        if (IoStack->Parameters.Others.Argument2)
            *(PVOID *)IoStack->Parameters.Others.Argument2 = PdoDevice;

        Status = STATUS_SUCCESS;
        goto Exit;
    }

    if (IoStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_INTERNAL_USB_GET_HUB_COUNT)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_HUB_COUNT\n");

        if (IoStack->Parameters.Others.Argument1)
            *(PULONG)IoStack->Parameters.Others.Argument1 = *(PULONG)IoStack->Parameters.Others.Argument1 + 1;

        Status = STATUS_SUCCESS;
        goto Exit;
    }

    if (IoStack->Parameters.DeviceIoControl.IoControlCode == IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE)
    {
        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE\n");

        if ( IoStack->Parameters.Others.Argument1 )
            *(PVOID *)IoStack->Parameters.Others.Argument1 = &PdoExtention->DeviceHandle;

        Status = STATUS_SUCCESS;
        goto Exit;
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
USBPORT_Dispatch(PDEVICE_OBJECT DeviceObject,
                 PIRP Irp)
{
    PUSBPORT_COMMON_DEVICE_EXTENSION DeviceExtension = (PUSBPORT_COMMON_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    NTSTATUS Status = STATUS_SUCCESS;

    if (DeviceExtension->IsPDO)
    {
        DPRINT("USBPORT_Dispatch: PDO (ROOTHUB_DEVICE). Major - %d, Minor - %d\n",
               IoStack->MajorFunction,
               IoStack->MinorFunction);
    }
    else
    {
        DPRINT("USBPORT_Dispatch: FDO (USBPORT_DEVICE). Major - %d, Minor - %d\n",
               IoStack->MajorFunction,
               IoStack->MinorFunction);
    }

    switch (IoStack->MajorFunction)
    {
        case IRP_MJ_DEVICE_CONTROL: // 14
            DPRINT("USBPORT_Dispatch: IRP_MJ_DEVICE_CONTROL\n");
            if (DeviceExtension->IsPDO)
                Status = 0; // USBPORT_PdoDeviceControl(DeviceObject, Irp);
            else
                Status = 0; // USBPORT_FdoDeviceControl(DeviceObject, Irp);
            break;

        case IRP_MJ_SCSI: // 15 IRP_MJ_NTERNAL_DEVICE_CONTROL:
            DPRINT("USBPORT_Dispatch: IRP_MJ_SCSI\n");
            if (DeviceExtension->IsPDO)
                Status = USBPORT_PdoScsi(DeviceObject, Irp);
            else
                ASSERT(FALSE);
                //Status = USBPORT_FdoScsi(DeviceObject, Irp);
            break;

        case IRP_MJ_POWER: // 22
            DPRINT("USBPORT_Dispatch: USBPORT_XdoPowerIrp\n");
            if (DeviceExtension->IsPDO)
                Status = 0; // USBPORT_PdoPower(DeviceObject, Irp);
            else
                Status = 0; // USBPORT_FdoPower(DeviceObject, Irp);
            break;

        case IRP_MJ_SYSTEM_CONTROL: // 23
            DPRINT("USBPORT_Dispatch: IRP_MJ_SYSTEM_CONTROL\n"); 
            if (DeviceExtension->IsPDO)
            {
                Irp->IoStatus.Status = Status;
                IoCompleteRequest(Irp, IO_NO_INCREMENT);
            }
            else
            {
                IoSkipCurrentIrpStackLocation(Irp);
                Status = IoCallDriver(DeviceExtension->LowerDevice, Irp);
            }
            break;

        case IRP_MJ_PNP: // 27
            DPRINT("USBPORT_Dispatch: IRP_MJ_PNP\n"); 
            if (DeviceExtension->IsPDO)
                Status = USBPORT_PdoPnP(DeviceObject, Irp);
            else
                Status = USBPORT_FdoPnP(DeviceObject, Irp);
            break;

        case IRP_MJ_CREATE: // 0
        case IRP_MJ_CLOSE: // 2
            DPRINT("USBPORT_Dispatch: IRP_MJ_CREATE | IRP_MJ_CLOSE\n"); 
            Irp->IoStatus.Status = Status;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            break;

        default:
            DPRINT("USBPORT_Dispatch: unhandled IRP_MJ_??? function - %d\n",
                   IoStack->MajorFunction);

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
    return 0;
}

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(PDRIVER_OBJECT DriverObject,
                              ULONG Version,
                              PUSBPORT_REGISTRATION_PACKET RegPacket)
{
    NTSTATUS Status;
    PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;

    DPRINT("USBPORT_RegisterUSBPortDriver: DriverObject - %p, Version - %p, RegPacket - %p\n",
           DriverObject,
           Version,
           RegPacket);

    DPRINT("USBPORT_RegisterUSBPortDriver: sizeof(USBPORT_MINIPORT_INTERFACE) - %x\n",
           sizeof(USBPORT_MINIPORT_INTERFACE));

    if (!USBPORT_Initialized)
    {
        InitializeListHead(&USBPORT_MiniPortDrivers);
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
                                &MiniPortInterface->DriverList,
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

    RtlCopyMemory(&MiniPortInterface->Packet,
                  RegPacket,
                  sizeof(USBPORT_REGISTRATION_PACKET));

    Status = STATUS_SUCCESS;
    return Status;
}

NTSTATUS
NTAPI
DriverEntry(PDRIVER_OBJECT DriverObject,
            PUNICODE_STRING RegistryPath)
{
    return STATUS_SUCCESS;
}
