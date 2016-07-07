#include "usbport.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBPORT_OpenPipe(PUSBPORT_DEVICE_HANDLE DeviceHandle,
                 PDEVICE_OBJECT FdoDevice,
                 PUSBPORT_PIPE_HANDLE PipeHandle,
                 PUSBD_STATUS UsbdStatus)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    ULONG EndpointSize;
    PUSBPORT_ENDPOINT Endpoint;
    PUSBPORT_ENDPOINT_PROPERTIES EndpointProperties;
    PUSB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
    UCHAR Direction;
    NTSTATUS Status=0;

    DPRINT("USBPORT_OpenPipe: DeviceHandle - %p, FdoDevice - %p, PipeHandle - %p, UsbdStatus - %p\n",
           DeviceHandle,
           FdoDevice,
           PipeHandle,
           UsbdStatus);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    EndpointSize = sizeof(USBPORT_ENDPOINT) +
                   FdoExtension->MiniPortInterface->Packet.MiniPortEndpointSize;

    Endpoint = ExAllocatePoolWithTag(NonPagedPool, EndpointSize, USB_PORT_TAG);

    if (!Endpoint)
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
        return Status;
    }

    RtlZeroMemory(Endpoint, EndpointSize);

    Endpoint->FdoDevice = FdoDevice;
    Endpoint->DeviceHandle = (PUSBPORT_DEVICE_HANDLE)DeviceHandle;
    Endpoint->LockCounter = -1;

    InitializeListHead(&Endpoint->PendingTransferList);
    InitializeListHead(&Endpoint->TransferList);

    EndpointProperties = &Endpoint->EndpointProperties;
    EndpointDescriptor = &PipeHandle->EndpointDescriptor;

    EndpointProperties->DeviceAddress = DeviceHandle->DeviceAddress;
    EndpointProperties->DeviceSpeed = DeviceHandle->DeviceSpeed;
    EndpointProperties->Period = 0; // HighSpeedInterval
    EndpointProperties->EndpointAddress = EndpointDescriptor->bEndpointAddress;

    EndpointProperties->MaxPacketSize = EndpointDescriptor->wMaxPacketSize &
                                        0x7FF; // FIXME for Highspeed

    EndpointProperties->TransferType = EndpointDescriptor->bmAttributes &
                                       USB_ENDPOINT_TYPE_MASK;

    switch ( EndpointDescriptor->bmAttributes & USB_ENDPOINT_TYPE_MASK )
    {
        case USB_ENDPOINT_TYPE_CONTROL: // 0x00

            if (EndpointProperties->EndpointAddress == 0)
                EndpointProperties->MaxTransferSize = 0x400; // OUT Ep0
            else
                EndpointProperties->MaxTransferSize = 0x10000;

            break;

        case USB_ENDPOINT_TYPE_ISOCHRONOUS: // 0x01
            ASSERT(FALSE);
            break;

        case USB_ENDPOINT_TYPE_BULK: // 0x02
            EndpointProperties->MaxTransferSize = 0x10000;
            break;

        case USB_ENDPOINT_TYPE_INTERRUPT: // 0x03
            EndpointProperties->MaxTransferSize = 0x400;
            break;
    }

    if (EndpointProperties->TransferType == USB_ENDPOINT_TYPE_INTERRUPT)
    {
        ASSERT(FALSE);
    }

    //EndpointProperties->Direction = ~EndpointDescriptor->bEndpointAddress >> 7;
    Direction = ~EndpointDescriptor->bEndpointAddress;
    Direction >>= 7;
    EndpointProperties->Direction = Direction;

    if (DeviceHandle->IsRootHub)
    {
        Endpoint->EndpointWorker = 0; // USBPORT_RootHubEndpointWorker;

        Endpoint->Flags |= ENDPOINT_FLAG_ROOTHUB_EP0;
        Endpoint->StateLast = USBPORT_ENDPOINT_ACTIVE;
        Endpoint->StateNext = USBPORT_ENDPOINT_ACTIVE;

        PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;

        if (EndpointProperties->TransferType == USB_ENDPOINT_TYPE_INTERRUPT)
            PdoExtension->Endpoint = Endpoint;

        Status = STATUS_SUCCESS;
    }
    else
    {
        ASSERT(FALSE);
    }

    if (NT_SUCCESS(Status))
    {
        InsertTailList(&DeviceHandle->PipeHandleList, &PipeHandle->PipeLink);

        ExInterlockedInsertTailList(&FdoExtension->EndpointList,
                                    &Endpoint->EndpointLink,
                                    &FdoExtension->EndpointListSpinLock);

        PipeHandle->Endpoint = Endpoint;
        PipeHandle->Flags &= ~PIPEHANDLE_FLAG_CLOSED;

        return Status;
    }
    else
    {
        if (Endpoint)
            ExFreePool(Endpoint);

        ASSERT(FALSE);
    }

    return Status;
}

NTSTATUS
USBPORT_HandleSelectConfiguration(IN PDEVICE_OBJECT FdoDevice,
                                  IN PIRP Irp,
                                  IN PURB Urb)
{
    PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    PUSBPORT_CONFIGURATION_HANDLE ConfigHandle = NULL;
    PUSBD_INTERFACE_INFORMATION InterfaceInfo;
    ULONG iNumber;
    //ULONG ix;
    USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    NTSTATUS Status = 0;
    USBD_STATUS USBDStatus = 0;

    DPRINT("USBPORT_HandleSelectConfiguration: ... \n");

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;
    ConfigDescriptor = Urb->UrbSelectConfiguration.ConfigurationDescriptor;

    DPRINT("USBPORT_SelectConfiguration: ConfigDescriptor %x\n",
           ConfigDescriptor);

    if (!ConfigDescriptor)
    {
        ASSERT(FALSE);
        goto Exit;
    }

    InterfaceInfo = &Urb->UrbSelectConfiguration.Interface;

    iNumber = 0;

    do
    {
        ++iNumber;
        InterfaceInfo = (PUSBD_INTERFACE_INFORMATION)((ULONG_PTR)InterfaceInfo +
                                                      InterfaceInfo->Length);
    }
    while ((ULONG_PTR)InterfaceInfo < (ULONG_PTR)Urb + Urb->UrbHeader.Length);

    if ((iNumber > 0) && (iNumber == ConfigDescriptor->bNumInterfaces))
    {
        ConfigHandle = (PUSBPORT_CONFIGURATION_HANDLE)ExAllocatePoolWithTag(NonPagedPool,
                                                                            ConfigDescriptor->wTotalLength + sizeof(USBPORT_CONFIGURATION_HANDLE),
                                                                            USB_PORT_TAG);

        if (ConfigHandle)
        {
            RtlZeroMemory(ConfigHandle,
                          ConfigDescriptor->wTotalLength + sizeof(USBPORT_CONFIGURATION_HANDLE));

            InitializeListHead(&ConfigHandle->InterfaceHandleList);

            ConfigHandle->ConfigurationDescriptor = (PUSB_CONFIGURATION_DESCRIPTOR)((ULONG_PTR)ConfigHandle +
                                                                                    sizeof(USBPORT_CONFIGURATION_HANDLE));

            RtlCopyMemory(ConfigHandle->ConfigurationDescriptor,
                          ConfigDescriptor,
                          ConfigDescriptor->wTotalLength);

            RtlZeroMemory(&SetupPacket, sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

            SetupPacket.bmRequestType.B = 0;
            SetupPacket.bRequest = USB_REQUEST_SET_CONFIGURATION;
            SetupPacket.wValue.W = ConfigDescriptor->bConfigurationValue;
            SetupPacket.wIndex.W = 0;
            SetupPacket.wLength = 0;

            ASSERT(FALSE);

            if (USBD_SUCCESS(USBDStatus))
            {
                if (iNumber <= 0)
                {
                    Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                          USBD_STATUS_SUCCESS);

                    goto Exit;
                }

                InterfaceInfo = &Urb->UrbSelectConfiguration.Interface;

                //ix = 0;

                while (TRUE)
                {
                    ASSERT(FALSE);
                }

                Status = USBPORT_USBDStatusToNtStatus(Urb, USBDStatus);
            }
            else
            {
                Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                      USBD_STATUS_SET_CONFIG_FAILED);
            }
        }
        else
        {
            Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                  USBD_STATUS_INSUFFICIENT_RESOURCES);
        }
    }
    else
    {
        Status = USBPORT_USBDStatusToNtStatus(Urb,
                                              USBD_STATUS_INVALID_CONFIGURATION_DESCRIPTOR);
    }

Exit:

    if (NT_SUCCESS(Status))
    {
        Urb->UrbSelectConfiguration.ConfigurationHandle = ConfigHandle;
        DeviceHandle->ConfigHandle = ConfigHandle;
    }
    else
    {
        ASSERT(FALSE);
    }

    return Status;
}
