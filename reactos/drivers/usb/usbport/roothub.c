#include "usbport.h"

//#define NDEBUG
#include <debug.h>

ULONG
USBPORT_RootHubEndpoint0(PUSBPORT_TRANSFER Transfer)
{
    SIZE_T TransferLength;
    PVOID Buffer;
    PURB Urb;
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    UCHAR Type;
    ULONG Result;

    DPRINT("USBPORT_RootHubEndpoint0: Transfer - %p\n", Transfer);

    TransferLength = Transfer->TransferParameters.TransferBufferLength;
    Urb = Transfer->Urb;

    if (TransferLength > 0)
        Buffer = Urb->UrbControlTransfer.TransferBufferMDL->MappedSystemVa;
    else
        Buffer = NULL;

    DPRINT("USBPORT_RootHubEndpoint0: Buffer - %p\n", Buffer);

    SetupPacket = (PUSB_DEFAULT_PIPE_SETUP_PACKET)Urb->UrbControlTransfer.SetupPacket;

    Type = SetupPacket->bmRequestType._BM.Type;

    if (Type != BMREQUEST_STANDARD)
    {
        Result = 0;
        ASSERT(FALSE);
    }
    else if (Type != BMREQUEST_CLASS)
    {
        Result = 0;
        ASSERT(FALSE);
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
USBPORT_RootHubEndpointWorker(PUSBPORT_ENDPOINT Endpoint)
{
    PUSBPORT_TRANSFER Transfer;
    ULONG Result;

    DPRINT("USBPORT_RootHubEndpointWorker: Endpoint - %p\n", Endpoint);

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
    }
}

NTSTATUS
USBPORT_RootHubCreateDevice(PDEVICE_OBJECT FdoDevice,
                            PDEVICE_OBJECT PdoDevice)
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
