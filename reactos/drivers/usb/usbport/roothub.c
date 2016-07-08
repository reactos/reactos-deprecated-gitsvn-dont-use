#include "usbport.h"

//#define NDEBUG
#include <debug.h>

BOOLEAN
USBPORT_RootHubClassCommand(IN PDEVICE_OBJECT FdoDevice,
                            IN PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket,
                            IN PVOID Buffer,
                            IN PULONG BufferLength)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    ULONG Result = 1;

    DPRINT("USBPORT_RootHubClassCommand: USB command - %x, BufferLength - %p\n",
           SetupPacket->bRequest,
           BufferLength);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;

    switch (SetupPacket->bRequest)
    {
        case USB_REQUEST_GET_STATUS:
            ASSERT(FALSE);
            break;

        case USB_REQUEST_CLEAR_FEATURE:
            ASSERT(FALSE);
            break;

        case USB_REQUEST_SET_FEATURE:
            ASSERT(FALSE);
            break;

        case USB_REQUEST_GET_DESCRIPTOR:
            if (Buffer &&
                SetupPacket->bmRequestType._BM.Dir == BMREQUEST_DEVICE_TO_HOST)
            {
                SIZE_T DescriptorLength;

                DescriptorLength = PdoExtension->RootHubDescriptors->Descriptor.bDescriptorLength;
  
                if (*BufferLength < DescriptorLength)
                    DescriptorLength = *BufferLength;
  
                RtlCopyMemory(Buffer,
                              &PdoExtension->RootHubDescriptors->Descriptor,
                              DescriptorLength);
    
                *BufferLength = DescriptorLength;
                Result = 0;
            }
            break;
    }

    return Result;
}

BOOLEAN
USBPORT_RootHubStandardCommand(IN PDEVICE_OBJECT FdoDevice,
                               IN PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket,
                               IN PVOID Buffer,
                               IN OUT PULONG TransferLength)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    SIZE_T Length;
    PVOID Descriptor;
    SIZE_T DescriptorLength;

    DPRINT("USBPORT_RootHubStandardCommand: USB command - %x, TransferLength - %p\n",
           SetupPacket->bRequest,
           TransferLength);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;

    switch (SetupPacket->bRequest)
    {
        case USB_REQUEST_GET_DESCRIPTOR:
            switch (SetupPacket->wValue.HiByte)
            {
                case USB_DEVICE_DESCRIPTOR_TYPE:
                    if (SetupPacket->wValue.LowByte ||
                        !(SetupPacket->bmRequestType._BM.Dir))
                        return 1;

                    Descriptor = &PdoExtension->RootHubDescriptors->DeviceDescriptor;
                    DescriptorLength = sizeof(USB_DEVICE_DESCRIPTOR);
                    break;

                case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                    if (SetupPacket->wValue.LowByte ||
                        !(SetupPacket->bmRequestType._BM.Dir))
                        return 1;

                    Descriptor = &PdoExtension->RootHubDescriptors->ConfigDescriptor;
                    DescriptorLength = sizeof(USB_CONFIGURATION_DESCRIPTOR) +
                                       sizeof(USB_INTERFACE_DESCRIPTOR) +
                                       sizeof(USB_ENDPOINT_DESCRIPTOR);
                    break;

                default:
                    ASSERT(FALSE);
                    break;
            }

            if (*TransferLength >= DescriptorLength)
                Length = DescriptorLength;
            else
                Length = *TransferLength;

            RtlCopyMemory(Buffer, Descriptor, Length);
            *TransferLength = Length;
            break;

        case USB_REQUEST_GET_STATUS:
            *(PULONG)Buffer = 1; // FIXME
            *TransferLength = 2;
            return 0;


        case USB_REQUEST_SET_CONFIGURATION:
            if ((SetupPacket->wValue.W == 0) ||
                (SetupPacket->wValue.W == PdoExtension->RootHubDescriptors->ConfigDescriptor.bConfigurationValue))
            {
                PdoExtension->ConfigurationValue = SetupPacket->wValue.LowByte;
            }

            return 0;

        default:
            ASSERT(FALSE);
            break;
    }

    return 0;
}

ULONG
USBPORT_RootHubEndpoint0(IN PUSBPORT_TRANSFER Transfer)
{
    PDEVICE_OBJECT FdoDevice;
    SIZE_T TransferLength;
    PVOID Buffer;
    PURB Urb;
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    UCHAR Type;
    BOOLEAN Result;

    DPRINT("USBPORT_RootHubEndpoint0: Transfer - %p\n", Transfer);

    TransferLength = Transfer->TransferParameters.TransferBufferLength;
    Urb = Transfer->Urb;
    FdoDevice = Transfer->Endpoint->FdoDevice;

    if (TransferLength > 0)
        Buffer = Urb->UrbControlTransfer.TransferBufferMDL->MappedSystemVa;
    else
        Buffer = NULL;

    SetupPacket = (PUSB_DEFAULT_PIPE_SETUP_PACKET)Urb->UrbControlTransfer.SetupPacket;

    Type = SetupPacket->bmRequestType._BM.Type;

    if (Type == BMREQUEST_STANDARD)
    {
        Result = USBPORT_RootHubStandardCommand(FdoDevice,
                                                SetupPacket,
                                                Buffer,
                                                &TransferLength);
    }
    else if (Type == BMREQUEST_CLASS)
    {
        Result = USBPORT_RootHubClassCommand(FdoDevice,
                                             SetupPacket,
                                             Buffer,
                                             &TransferLength);
    }
    else
    {
        return 1;
    }

    if (Result == 0)
        Transfer->CompletedTransferLen = TransferLength;

    return Result;
}

VOID
USBPORT_RootHubEndpointWorker(IN PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_TRANSFER Transfer;
    ULONG Result;

    DPRINT("USBPORT_RootHubEndpointWorker: Endpoint - %p\n", Endpoint);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Transfer = CONTAINING_RECORD(Endpoint->TransferList.Flink,
                                 USBPORT_TRANSFER,
                                 TransferLink);

    if (IsListEmpty(&Endpoint->TransferList) ||
        Endpoint->TransferList.Flink == NULL ||
        !Transfer)
    {
        if (Endpoint->StateLast == USBPORT_ENDPOINT_CLOSED)
            return;
    }

    if (Endpoint->EndpointProperties.TransferType == USB_ENDPOINT_TYPE_CONTROL)
        Result = USBPORT_RootHubEndpoint0(Transfer);
    else
        ASSERT(FALSE);

    if (Result == 0)
    {
        Transfer->USBDStatus = USBD_STATUS_SUCCESS;

        RemoveEntryList(&Transfer->TransferLink);

        ExInterlockedInsertTailList(&FdoExtension->DoneTransferList,
                                    &Transfer->TransferLink,
                                    &FdoExtension->DoneTransferSpinLock);

        KeInsertQueueDpc(&FdoExtension->TransferFlushDpc, NULL, NULL);
    }
}

NTSTATUS
USBPORT_RootHubCreateDevice(IN PDEVICE_OBJECT FdoDevice,
                            IN PDEVICE_OBJECT PdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    USB_HUB_DESCRIPTOR RootHubData;
    ULONG NumMaskByte;
    ULONG LenghtDescriptors;
    PUSBPORT_RH_DESCRIPTORS Descriptors;
    PUSB_DEVICE_DESCRIPTOR RH_DeviceDescriptor;
    PUSB_CONFIGURATION_DESCRIPTOR RH_ConfigurationDescriptor;
    PUSB_INTERFACE_DESCRIPTOR RH_InterfaceDescriptor;
    PUSB_ENDPOINT_DESCRIPTOR RH_EndPointDescriptor;
    PUSB_HUB_DESCRIPTOR RH_HubDescriptor;
    ULONG ix;
    PUSB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
    NTSTATUS Status;

    DPRINT("USBPORT_RootHubCreateDevice: FdoDevice - %p, PdoDevice - %p\n",
           FdoDevice,
           PdoDevice);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    DeviceHandle = &PdoExtension->DeviceHandle;

    InitializeListHead(&DeviceHandle->PipeHandleList);

    DeviceHandle->IsRootHub = 1;
    DeviceHandle->DeviceSpeed = 1; // 0-low, 1-full, 2-high

    FdoExtension->MiniPortInterface->Packet.RH_GetRootHubData(FdoExtension->MiniPortExt,
                                                              (PULONG)&RootHubData);

    NumMaskByte = ((RootHubData.bNumberOfPorts - 1) >> 3) + 1;

    LenghtDescriptors = sizeof(USB_DEVICE_DESCRIPTOR) +
                        sizeof(USB_CONFIGURATION_DESCRIPTOR) +
                        sizeof(USB_INTERFACE_DESCRIPTOR) +
                        sizeof(USB_ENDPOINT_DESCRIPTOR) +
                        (7 + 2 * NumMaskByte);

    Descriptors = ExAllocatePoolWithTag(NonPagedPool,
                                        LenghtDescriptors,
                                        USB_PORT_TAG);

    if (Descriptors)
    {
        RtlZeroMemory(Descriptors, LenghtDescriptors);

        PdoExtension->RootHubDescriptors = Descriptors;

        RH_DeviceDescriptor = &PdoExtension->RootHubDescriptors->DeviceDescriptor;

        RH_DeviceDescriptor->bLength = sizeof(USB_DEVICE_DESCRIPTOR);
        RH_DeviceDescriptor->bDescriptorType = USB_DEVICE_DESCRIPTOR_TYPE;
        RH_DeviceDescriptor->bcdUSB = 0x100;
        RH_DeviceDescriptor->bDeviceClass = USB_DEVICE_CLASS_HUB;
        RH_DeviceDescriptor->bDeviceSubClass = 0x01;
        RH_DeviceDescriptor->bDeviceProtocol = 0x00;
        RH_DeviceDescriptor->bMaxPacketSize0 = 0x08;
        RH_DeviceDescriptor->idVendor = FdoExtension->VendorID;
        RH_DeviceDescriptor->idProduct = FdoExtension->DeviceID;
        RH_DeviceDescriptor->bcdDevice = FdoExtension->RevisionID;
        RH_DeviceDescriptor->iManufacturer = 0x00;
        RH_DeviceDescriptor->iProduct = 0x00;
        RH_DeviceDescriptor->iSerialNumber = 0x00;
        RH_DeviceDescriptor->bNumConfigurations = 0x01;

        RH_ConfigurationDescriptor = &PdoExtension->RootHubDescriptors->ConfigDescriptor;

        RH_ConfigurationDescriptor->bLength = sizeof(USB_CONFIGURATION_DESCRIPTOR);
        RH_ConfigurationDescriptor->bDescriptorType = USB_CONFIGURATION_DESCRIPTOR_TYPE;

        RH_ConfigurationDescriptor->wTotalLength = sizeof(USB_CONFIGURATION_DESCRIPTOR) +
                                                   sizeof(USB_INTERFACE_DESCRIPTOR) +
                                                   sizeof(USB_ENDPOINT_DESCRIPTOR);

        RH_ConfigurationDescriptor->bNumInterfaces = 0x01;
        RH_ConfigurationDescriptor->bConfigurationValue = 0x01;
        RH_ConfigurationDescriptor->iConfiguration = 0x00;
        RH_ConfigurationDescriptor->bmAttributes = 0x40;
        RH_ConfigurationDescriptor->MaxPower = 0x00;

        RH_InterfaceDescriptor = &PdoExtension->RootHubDescriptors->InterfaceDescriptor;

        RH_InterfaceDescriptor->bLength = sizeof(USB_INTERFACE_DESCRIPTOR);
        RH_InterfaceDescriptor->bDescriptorType = USB_INTERFACE_DESCRIPTOR_TYPE;
        RH_InterfaceDescriptor->bInterfaceNumber = 0x00;
        RH_InterfaceDescriptor->bAlternateSetting = 0x00;
        RH_InterfaceDescriptor->bNumEndpoints = 0x01;
        RH_InterfaceDescriptor->bInterfaceClass = USB_DEVICE_CLASS_HUB;
        RH_InterfaceDescriptor->bInterfaceSubClass = 0x01;
        RH_InterfaceDescriptor->bInterfaceProtocol = 0x00;
        RH_InterfaceDescriptor->iInterface = 0x00;

        RH_EndPointDescriptor = &PdoExtension->RootHubDescriptors->EndPointDescriptor;

        RH_EndPointDescriptor->bLength = sizeof(USB_ENDPOINT_DESCRIPTOR);
        RH_EndPointDescriptor->bDescriptorType = USB_ENDPOINT_DESCRIPTOR_TYPE;
        RH_EndPointDescriptor->bEndpointAddress = 0x81;
        RH_EndPointDescriptor->bmAttributes = USB_ENDPOINT_TYPE_INTERRUPT; // SCE endpoint
        RH_EndPointDescriptor->wMaxPacketSize = 0x0008;
        RH_EndPointDescriptor->bInterval = 0x0C; // 12 msec

        RH_HubDescriptor = &PdoExtension->RootHubDescriptors->Descriptor;

        RH_HubDescriptor->bDescriptorLength = 7 + 2 * NumMaskByte;;
        RH_HubDescriptor->bDescriptorType = 0x29;
        RH_HubDescriptor->bNumberOfPorts = RootHubData.bNumberOfPorts;
        RH_HubDescriptor->wHubCharacteristics = RootHubData.wHubCharacteristics;
        RH_HubDescriptor->bPowerOnToPowerGood = RootHubData.bPowerOnToPowerGood;
        RH_HubDescriptor->bHubControlCurrent = RootHubData.bHubControlCurrent;

        if (NumMaskByte)
        {
            ix = 0;
            do
            {
                RH_HubDescriptor->bRemoveAndPowerMask[ix] = 0;
                RH_HubDescriptor->bRemoveAndPowerMask[ix + 1] = -1;
                ix += 2;
            }
            while (ix < NumMaskByte);
        }

        EndpointDescriptor = &DeviceHandle->PipeHandle.EndpointDescriptor;

        EndpointDescriptor->bLength = sizeof(USB_ENDPOINT_DESCRIPTOR);
        EndpointDescriptor->bDescriptorType = USB_ENDPOINT_DESCRIPTOR_TYPE;
        EndpointDescriptor->bEndpointAddress = 0x00;
        EndpointDescriptor->bmAttributes = 0x00;
        EndpointDescriptor->wMaxPacketSize = 0x0040;
        EndpointDescriptor->bInterval = 0x00;

        Status = USBPORT_OpenPipe(DeviceHandle,
                                  FdoDevice,
                                  &DeviceHandle->PipeHandle,
                                  NULL);
    }
    else
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
    }

    return Status;
}
