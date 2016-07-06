#include "usbport.h"

//#define NDEBUG
#include <debug.h>

LIST_ENTRY USBPORT_MiniPortDrivers = {NULL, NULL};
KSPIN_LOCK USBPORT_SpinLock = 0;
BOOLEAN USBPORT_Initialized = FALSE;

NTSTATUS
USBPORT_USBDStatusToNtStatus(IN PURB Urb,
                             IN USBD_STATUS USBDStatus)
{
    NTSTATUS Status;

    DPRINT("USBPORT_USBDStatusToNtStatus: Urb - %p, USBDStatus - %p\n",
           Urb,
           USBDStatus);

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

        default:
            if (USBD_ERROR(Status))
                Status = STATUS_UNSUCCESSFUL;
            break;
    }

    return USBD_STATUS_SUCCESS;
}

PUSBPORT_COMMON_BUFFER_HEADER
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

PUSBPORT_MINIPORT_INTERFACE
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
                                              DriverList);

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

    FdoExtension->MiniPortExt = (PVOID)((ULONG_PTR)FdoExtension +
                                        sizeof(USBPORT_DEVICE_EXTENSION));

    FdoExtension->MiniPortInterface = MiniPortInterface;
    FdoExtension->FdoNameNumber = DeviceNumber;

    DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;

    return Status;
}

VOID
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

    ASSERT(FALSE); //FIXME
    // ...
    //MiniPortInterface->DriverUnload(DriverObject); // Call MiniPort _HCI_Unload
    // ...
}

VOID
USBPORT_QueueTransferUrb(PURB Urb)
{
    DPRINT("USBPORT_QueueTransferUrb: Urb - %p\n", Urb);
}

NTSTATUS
ValidateTransferParameters(IN PURB Urb)
{
    struct _URB_CONTROL_TRANSFER *UrbTransfer;
    PMDL Mdl;

    DPRINT("ValidateTransferParameters: Urb - %p\n", Urb);

    UrbTransfer = &Urb->UrbControlTransfer;

    if (UrbTransfer->TransferBuffer == NULL &&
        UrbTransfer->TransferBufferMDL == NULL &&
        UrbTransfer->TransferBufferLength > 0)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if ((UrbTransfer->TransferBuffer > 0 || UrbTransfer->TransferBufferMDL > 0) &&
        UrbTransfer->TransferBufferLength == 0)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (UrbTransfer->TransferBuffer != NULL &&
        UrbTransfer->TransferBufferMDL == NULL &&
        UrbTransfer->TransferBufferLength != 0)
    {
        DPRINT("ValidateTransferParameters: TransferBuffer       - %p\n",
               UrbTransfer->TransferBuffer);

        DPRINT("ValidateTransferParameters: TransferBufferMDL    - %p\n",
               UrbTransfer->TransferBufferMDL);

        DPRINT("ValidateTransferParameters: TransferBufferLength - %p\n",
               UrbTransfer->TransferBufferLength);

        Mdl = IoAllocateMdl(UrbTransfer->TransferBuffer,
                            UrbTransfer->TransferBufferLength,
                            FALSE,
                            FALSE,
                            NULL);

        if (!Mdl)
        {
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        MmBuildMdlForNonPagedPool(Mdl);

        UrbTransfer->TransferBufferMDL = Mdl;
        UrbTransfer->hca.Reserved8[1] = (PVOID)USBD_FLAG_ALLOCATED_MDL;

        DPRINT("ValidateTransferParameters: Mdl - %p\n", Mdl);
    }

    return STATUS_SUCCESS;
}

USBD_STATUS
USBPORT_AllocateTransfer(PDEVICE_OBJECT FdoDevice,
                         PURB Urb,
                         PUSBPORT_DEVICE_HANDLE UsbdDeviceHandle,
                         PIRP Irp,
                         PRKEVENT Event)
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

    DPRINT("USBPORT_AllocateTransfer: FdoDevice - %p, Urb - %p, UsbdDeviceHandle - %p, Irp - %p, Event - %p\n",
           FdoDevice,
           Urb,
           UsbdDeviceHandle,
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

        Urb->UrbControlTransfer.hca.Reserved8[1] = (PVOID)USBD_FLAG_ALLOCATED_TRANSFER;

        USBDStatus = USBD_STATUS_SUCCESS;
    }
    else
    {
        USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
    }

    Urb->UrbControlTransfer.hca.Reserved8[0] = Transfer;

    DPRINT("USBPORT_AllocateTransfer: return USBDStatus - %x\n", USBDStatus);
    return USBDStatus;
}

NTSTATUS
USBPORT_HandleGetSetDescriptor(PIRP Irp,
                               PURB Urb)
{
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket = (PUSB_DEFAULT_PIPE_SETUP_PACKET)&Urb->UrbControlDescriptorRequest.Reserved1;

    SetupPacket->wLength = Urb->UrbControlDescriptorRequest.TransferBufferLength;
    SetupPacket->bmRequestType.B = 0; // Clear bmRequestType
    SetupPacket->bmRequestType._BM.Type = BMREQUEST_STANDARD;

    switch (Urb->UrbHeader.Function)
    {
        case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE: // 0x0B
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE\n");
            SetupPacket->bRequest = USB_REQUEST_GET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
            break;

        case URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE: // 0x0C
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE\n");
            SetupPacket->bRequest = USB_REQUEST_SET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
            break;

        case URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT: // 0x24
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT\n");
            SetupPacket->bRequest = USB_REQUEST_GET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_ENDPOINT;
            break;

        case URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT: // 0x25
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT\n");
            SetupPacket->bRequest = USB_REQUEST_SET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_ENDPOINT;
            break;

        case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE: // 0x28
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE\n");
            SetupPacket->bRequest = USB_REQUEST_GET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_INTERFACE;
            break;

        case URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE: // 0x29
            DPRINT("USBPORT_HandleGetSetDescriptor: URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE\n");
            SetupPacket->bRequest = USB_REQUEST_SET_DESCRIPTOR;
            SetupPacket->bmRequestType._BM.Dir = BMREQUEST_HOST_TO_DEVICE;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_INTERFACE;
            break;
    }

    Urb->UrbControlTransfer.TransferFlags |= USBD_SHORT_TRANSFER_OK; // 2

    if (SetupPacket->bmRequestType._BM.Dir)
        Urb->UrbControlTransfer.TransferFlags |= USBD_TRANSFER_DIRECTION_IN; // 1;
    else
        Urb->UrbControlTransfer.TransferFlags &= ~USBD_TRANSFER_DIRECTION_IN; // ~1;

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->bmRequestType.B - %x\n",
           SetupPacket->bmRequestType.B); // 0x80

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->bRequest        - %x\n",
           SetupPacket->bRequest); // USB_REQUEST_GET_DESCRIPTOR

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->wValue.LowByte  - %x\n",
           SetupPacket->wValue.LowByte); // Urb->UrbControlDescriptorRequest.Index;

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->wValue.HiByte   - %x\n",
           SetupPacket->wValue.HiByte); // Urb->UrbControlDescriptorRequest.DescriptorType;

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->wIndex.W        - %x\n",
           SetupPacket->wIndex.W); // Urb->UrbControlDescriptorRequest.LanguageId;

    DPRINT("USBPORT_HandleGetSetDescriptor: SetupPacket->wLength         - %x\n",
           SetupPacket->wLength); // Urb->UrbControlDescriptorRequest.TransferBufferLength;

    USBPORT_QueueTransferUrb(Urb);

    return STATUS_PENDING;
}

NTSTATUS
USBPORT_PdoScsi(IN PDEVICE_OBJECT PdoDevice,
                IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_DEVICE_HANDLE UsbdDeviceHandle;
    PIO_STACK_LOCATION IoStack;
    ULONG IoCtl;
    NTSTATUS Status;

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);
    IoCtl = IoStack->Parameters.DeviceIoControl.IoControlCode;

    DPRINT("USBPORT_PdoScsi: PdoDevice - %p, Irp - %p, IoCtl - %x\n",
           PdoDevice,
           Irp,
           IoCtl);

    if (IoCtl == IOCTL_INTERNAL_USB_SUBMIT_URB)
    {
        PURB Urb = (PURB)IoStack->Parameters.Others.Argument1;
        USHORT Function = Urb->UrbHeader.Function;

        ASSERT(Urb);
        Status = STATUS_NOT_IMPLEMENTED;

        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_SUBMIT_URB. Urb - %p, UrbFunction - 0x%02X\n",
               Urb,
               Urb->UrbHeader.Function);

        Urb->UrbHeader.Status = 0;
        Urb->UrbHeader.UsbdFlags = 0;

        if (Function > URB_FUNCTION_MAX)
        {
            Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                  USBD_STATUS_INVALID_URB_FUNCTION);

            DPRINT("USBPORT_PdoScsi: !!! Unknown Function %x NOT IMPLEMENTED\n",
                   Function);

            //ASSERT(FALSE);
            return Status;
        }

        UsbdDeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;

        if (!UsbdDeviceHandle)
        {
            DPRINT("USBPORT_PdoScsi: UsbdDeviceHandle == 0\n");
            Urb->UrbHeader.UsbdDeviceHandle = &PdoExtension->DeviceHandle;
            UsbdDeviceHandle = &PdoExtension->DeviceHandle;
        }

        switch (Function)
        {
            case URB_FUNCTION_ISOCH_TRANSFER: // 0x10
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER: // 0x09
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_CONTROL_TRANSFER: // 0x08
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_VENDOR_DEVICE: // 0x17
            case URB_FUNCTION_VENDOR_INTERFACE: // 0x18
            case URB_FUNCTION_VENDOR_ENDPOINT: // 0x19
            case URB_FUNCTION_CLASS_DEVICE: // 0x1A
            case URB_FUNCTION_CLASS_INTERFACE: // 0x1B
            case URB_FUNCTION_CLASS_ENDPOINT: // 0x1C
            case URB_FUNCTION_CLASS_OTHER: // 0x1F
            case URB_FUNCTION_VENDOR_OTHER: // 0x20
                Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
                Urb->UrbControlTransfer.PipeHandle = &UsbdDeviceHandle->PipeHandle;
                ValidateTransferParameters(Urb);
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE: // 0x0B
            case URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE: // 0x0C
            case URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT: // 0x24
            case URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT: // 0x25
            case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE: // 0x28
            case URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE: // 0x29
                Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
                Urb->UrbControlTransfer.PipeHandle = &UsbdDeviceHandle->PipeHandle;
                ValidateTransferParameters(Urb);

                Status = USBPORT_AllocateTransfer(PdoExtension->FdoDevice,
                                                  Urb,
                                                  UsbdDeviceHandle,
                                                  Irp,
                                                  NULL);

                if (!NT_SUCCESS(Status))
                {
                    goto Exit;
                }

                Status = USBPORT_HandleGetSetDescriptor(Irp, Urb);
                break;

            case URB_FUNCTION_GET_STATUS_FROM_DEVICE: // 0x13
            case URB_FUNCTION_GET_STATUS_FROM_INTERFACE: // 0x14
            case URB_FUNCTION_GET_STATUS_FROM_ENDPOINT: // 0x15
            case URB_FUNCTION_GET_STATUS_FROM_OTHER: // 0x21
                Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
                Urb->UrbControlTransfer.PipeHandle = &UsbdDeviceHandle->PipeHandle;
                ValidateTransferParameters(Urb);
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_SELECT_CONFIGURATION: // 0x00
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_SELECT_INTERFACE: // 0x01
                ASSERT(FALSE);
                break;

            default:
                DPRINT("USBPORT_PdoScsi: default Function - %x\n", Function);
                ASSERT(FALSE);
                break;
        }

        if (Status == STATUS_PENDING)
        {
            return Status;
        }
        else
        {
            if (Urb->UrbControlTransfer.hca.Reserved8[1] == (PVOID)USBD_FLAG_ALLOCATED_TRANSFER)
            {
                PUSBPORT_TRANSFER Transfer;

                Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
                Urb->UrbControlTransfer.hca.Reserved8[0] = (PVOID)-1;
                Urb->UrbControlTransfer.hca.Reserved8[1] = (PVOID)-1;
                ExFreePool(Transfer);
            }
        }

        goto Exit;
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

        if ( IoStack->Parameters.Others.Argument1 )
            *(PVOID *)IoStack->Parameters.Others.Argument1 = &PdoExtension->DeviceHandle;

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
USBPORT_Dispatch(IN PDEVICE_OBJECT DeviceObject,
                 IN PIRP Irp)
{
    PUSBPORT_COMMON_DEVICE_EXTENSION DeviceExtension;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status = STATUS_SUCCESS;

    DeviceExtension = (PUSBPORT_COMMON_DEVICE_EXTENSION)DeviceObject->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);

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
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    return STATUS_SUCCESS;
}
