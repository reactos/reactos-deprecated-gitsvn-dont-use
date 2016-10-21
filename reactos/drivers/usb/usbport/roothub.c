#include "usbport.h"

#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_CORE
#include "usbdebug.h"

RHSTATUS
NTAPI
USBPORT_MPStatusToRHStatus(IN MPSTATUS MPStatus)
{
    RHSTATUS RHStatus = 0;

    //DPRINT("USBPORT_MPStatusToRHStatus: MPStatus - %x\n", MPStatus);

    if (MPStatus)
    {
        RHStatus = (MPStatus != 1);
        ++RHStatus;
    }

    return RHStatus;
}

ULONG
NTAPI
USBPORT_SetBit(ULONG_PTR Address,
               UCHAR Index)
{
    ULONG_PTR AddressBitMap;

    DPRINT("USBPORT_SetBit: Address - %p, Index - %p\n", Address, Index);

    AddressBitMap = Address + 4 * (Index >> 5);
    *(ULONG_PTR *)AddressBitMap |= 1 << (Index & 0x1F);

    return AddressBitMap;
}

BOOLEAN
NTAPI
USBPORT_RootHubClassCommand(IN PDEVICE_OBJECT FdoDevice,
                            IN PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket,
                            IN PVOID Buffer,
                            IN PULONG BufferLength)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_REGISTRATION_PACKET Packet;
    USHORT Port;
    ULONG Result = 2;
    KIRQL OldIrql;

    DPRINT("USBPORT_RootHubClassCommand: USB command - %x, *BufferLength - %x\n",
           SetupPacket->bRequest,
           *BufferLength);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;
    Packet = &FdoExtension->MiniPortInterface->Packet;

    Port = SetupPacket->wIndex.W;

    switch (SetupPacket->bRequest)
    {
        case USB_REQUEST_GET_STATUS:
        {
            if (!Buffer)
                return 1;

            *(PULONG)Buffer = 0;

            if (SetupPacket->bmRequestType._BM.Recipient == BMREQUEST_TO_OTHER)
            {
                KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
                Result = FdoExtension->MiniPortInterface->Packet.RH_GetPortStatus(FdoExtension->MiniPortExt,
                                                                                  SetupPacket->wIndex.W,
                                                                                  Buffer);
            }
            else
            {
                KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
                Result = FdoExtension->MiniPortInterface->Packet.RH_GetHubStatus(FdoExtension->MiniPortExt,
                                                                                 Buffer);
            }

            KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

            if (Result)
                return 1;
            else
                return 0;
        }

        case USB_REQUEST_CLEAR_FEATURE:
            switch (SetupPacket->wValue.W)
            {
                case FEATURE_PORT_CONNECTION: // 0
                    if ((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_HUB) // 0 FEATURE_C_HUB_LOCAL_POWER
                    {
                        Result = 0;
                    }
                    else
                    {
                        Result = 2;
                    }
                    break;

                case FEATURE_PORT_ENABLE: // 1
                    if ((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_HUB) // 1 FEATURE_C_HUB_OVER_CURRENT
                    {
                        Result = Packet->RH_ClearFeaturePortOvercurrentChange(FdoExtension->MiniPortExt,
                                                                              Port);
                    }
                    else
                    {
                        ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                        Result = Packet->RH_ClearFeaturePortEnable(FdoExtension->MiniPortExt, Port);
                    }
                    break;

                case FEATURE_PORT_SUSPEND: // 2
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);

                    Result = Packet->RH_ClearFeaturePortSuspend(FdoExtension->MiniPortExt,
                                                                Port);
                    break;

                case FEATURE_PORT_OVER_CURRENT: // 3
                    Result = 2;
                    break;

                case FEATURE_PORT_RESET: // 4
                    Result = 2;
                    break;

                case FEATURE_PORT_POWER: // 8
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);

                    Result = Packet->RH_ClearFeaturePortPower(FdoExtension->MiniPortExt,
                                                              Port);
                    break;

                case FEATURE_PORT_LOW_SPEED: // 9
                    Result = 2;
                    break;

                case FEATURE_C_PORT_CONNECTION: // 16
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                    Result = Packet->RH_ClearFeaturePortConnectChange(FdoExtension->MiniPortExt,
                                                                      Port);
                    break;

                case FEATURE_C_PORT_ENABLE: // 17
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                    Result = Packet->RH_ClearFeaturePortEnableChange(FdoExtension->MiniPortExt,
                                                                     Port);
                    break;

                case FEATURE_C_PORT_SUSPEND: // 18
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                    Result = Packet->RH_ClearFeaturePortSuspendChange(FdoExtension->MiniPortExt,
                                                                      Port);
                    break;

                case FEATURE_C_PORT_OVER_CURRENT: // 19
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                    Result = Packet->RH_ClearFeaturePortOvercurrentChange(FdoExtension->MiniPortExt,
                                                                          Port);
                    break;

                case FEATURE_C_PORT_RESET: // 20
                    ASSERT((SetupPacket->bmRequestType._BM.Recipient) == USBPORT_RECIPIENT_ROOT_PORT);
                    Result = Packet->RH_ClearFeaturePortResetChange(FdoExtension->MiniPortExt,
                                                                    Port);
                    break;

                default:
                    Result = 2;
                    break;
            }
            break;

        case USB_REQUEST_SET_FEATURE:
            switch (SetupPacket->wValue.W)
            {
                case FEATURE_PORT_ENABLE: // 1
                    Result = Packet->RH_SetFeaturePortEnable(FdoExtension->MiniPortExt,
                                                             Port);
                    break;

                case FEATURE_PORT_SUSPEND: // 2
                    Result = Packet->RH_SetFeaturePortSuspend(FdoExtension->MiniPortExt,
                                                              Port);
                    break;

                case FEATURE_PORT_RESET: // 4

                    Result = Packet->RH_SetFeaturePortReset(FdoExtension->MiniPortExt,
                                                            Port);
                    break;

                case FEATURE_PORT_POWER: // 8
                    if (Packet->MiniPortFlags & USB_MINIPORT_FLAGS_USB2)
                    {
                        DPRINT1("USBPORT_RootHubClassCommand: Usb2PortPower UNIMPLEMENTED. FIXME.\n");

                        Result = Packet->RH_SetFeaturePortPower(FdoExtension->MiniPortExt,
                                                                Port);

                        //Result = USBPORT_RH_SetFeatureUsb2PortPower(FdoDevice, Port);
                    }
                    else
                    {
                        Result = Packet->RH_SetFeaturePortPower(FdoExtension->MiniPortExt,
                                                                Port);
                    }
                    break;

                default:
                    DPRINT1("USBPORT_RootHubClassCommand: Unknown feature - %x\n", SetupPacket->wValue.W);
                    break;
            }
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

RHSTATUS
NTAPI
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
    MPSTATUS MPStatus;
    RHSTATUS RHStatus = 2;
    KIRQL OldIrql;

    DPRINT("USBPORT_RootHubStandardCommand: USB command - %x, TransferLength - %p\n",
           SetupPacket->bRequest,
           TransferLength);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;

    switch (SetupPacket->bRequest)
    {
        case USB_REQUEST_GET_DESCRIPTOR: //0x06
            if (SetupPacket->wValue.LowByte ||
                !(SetupPacket->bmRequestType._BM.Dir))
            {
                return RHStatus;
            }

            switch (SetupPacket->wValue.HiByte)
            {
                case USB_DEVICE_DESCRIPTOR_TYPE:
                    Descriptor = &PdoExtension->RootHubDescriptors->DeviceDescriptor;
                    DescriptorLength = sizeof(USB_DEVICE_DESCRIPTOR);
                    break;

                case USB_CONFIGURATION_DESCRIPTOR_TYPE:
                    Descriptor = &PdoExtension->RootHubDescriptors->ConfigDescriptor;
                    DescriptorLength = sizeof(USB_CONFIGURATION_DESCRIPTOR) +
                                       sizeof(USB_INTERFACE_DESCRIPTOR) +
                                       sizeof(USB_ENDPOINT_DESCRIPTOR);
                    break;

                default:
                    DPRINT1("USBPORT_RootHubStandardCommand: Not supported Descriptor Type - %x\n",
                            SetupPacket->wValue.HiByte);
                    return RHStatus;
            }

            if (!Descriptor)
            {
                return RHStatus;
            }

            if (*TransferLength >= DescriptorLength)
                Length = DescriptorLength;
            else
                Length = *TransferLength;

            RtlCopyMemory(Buffer, Descriptor, Length);
            *TransferLength = Length;

            RHStatus = 0;
            break;

        case USB_REQUEST_GET_STATUS: //0x00
            if (!SetupPacket->wValue.W &&
                 SetupPacket->wLength == 2 &&
                 !SetupPacket->wIndex.W &&
                 SetupPacket->bmRequestType._BM.Dir)
            {
                KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
                MPStatus = FdoExtension->MiniPortInterface->Packet.RH_GetStatus(FdoExtension->MiniPortExt,
                                                                                Buffer);
                KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

                *TransferLength = 2;
                RHStatus = USBPORT_MPStatusToRHStatus(MPStatus);
            }

            break;

        case USB_REQUEST_GET_CONFIGURATION: //0x08
            if (SetupPacket->wValue.W ||
                SetupPacket->wIndex.W ||
                SetupPacket->wLength != 1 ||
                !(SetupPacket->bmRequestType._BM.Dir))
            {
                return RHStatus;
            }

            Length = 0;

            if (*TransferLength >= 1)
            {
                Length = 1;
                RtlCopyMemory(Buffer, &PdoExtension->ConfigurationValue, Length);
            }

            *TransferLength = Length;

            RHStatus = 0;
            break;

        case USB_REQUEST_SET_CONFIGURATION: //0x09
            if (!SetupPacket->wIndex.W &&
                !SetupPacket->wLength &&
                !(SetupPacket->bmRequestType._BM.Dir == BMREQUEST_DEVICE_TO_HOST))
            {
                if (SetupPacket->wValue.W == PdoExtension->RootHubDescriptors->ConfigDescriptor.bConfigurationValue ||
                    SetupPacket->wValue.W == 0)
                {
                  PdoExtension->ConfigurationValue = SetupPacket->wValue.LowByte;
                  RHStatus = 0;
                }
            }

            break;

        case USB_REQUEST_SET_ADDRESS: //0x05
            if (!SetupPacket->wIndex.W &&
                !SetupPacket->wLength &&
                !(SetupPacket->bmRequestType._BM.Dir))
            {
                PdoExtension->DeviceHandle.DeviceAddress = SetupPacket->wValue.LowByte;
                RHStatus = 0;
                break;
            }

            break;

        default:
            DPRINT1("USBPORT_RootHubStandardCommand: Not supported USB request - %x\n",
                    SetupPacket->bRequest);
            //USB_REQUEST_CLEAR_FEATURE                 0x01
            //USB_REQUEST_SET_FEATURE                   0x03
            //USB_REQUEST_SET_DESCRIPTOR                0x07
            //USB_REQUEST_GET_INTERFACE                 0x0A
            //USB_REQUEST_SET_INTERFACE                 0x0B
            //USB_REQUEST_SYNC_FRAME                    0x0C
            break;
    }

    return RHStatus;
}

RHSTATUS
NTAPI
USBPORT_RootHubEndpoint0(IN PUSBPORT_TRANSFER Transfer)
{
    PDEVICE_OBJECT FdoDevice;
    SIZE_T TransferLength;
    PVOID Buffer;
    PURB Urb;
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    UCHAR Type;
    RHSTATUS RHStatus;

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
        RHStatus = USBPORT_RootHubStandardCommand(FdoDevice,
                                                  SetupPacket,
                                                  Buffer,
                                                  &TransferLength);
    }
    else if (Type == BMREQUEST_CLASS)
    {
        RHStatus = USBPORT_RootHubClassCommand(FdoDevice,
                                               SetupPacket,
                                               Buffer,
                                               &TransferLength);
    }
    else
    {
        return 2;
    }

    if (RHStatus == 0)
        Transfer->CompletedTransferLen = TransferLength;

    return RHStatus;
}

RHSTATUS
NTAPI
USBPORT_RootHubSCE(IN PUSBPORT_TRANSFER Transfer)
{
    PUSBPORT_ENDPOINT Endpoint;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    ULONG TransferLength;
    ULONG PortStatus = 0;
    ULONG HubStatus = 0;
    ULONG_PTR Buffer;
    PURB Urb;
    RHSTATUS RHStatus = 1;
    PUSB_HUB_DESCRIPTOR HubDescriptor;
    UCHAR NumberOfPorts;
    UCHAR ix;

    DPRINT("USBPORT_RootHubSCE: Transfer - %p\n", Transfer);

    Endpoint = Transfer->Endpoint;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)Endpoint->FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;
    HubDescriptor = (PUSB_HUB_DESCRIPTOR)&PdoExtension->RootHubDescriptors->Descriptor;
    NumberOfPorts = HubDescriptor->bNumberOfPorts;

    if (FdoExtension->Flags & 0x20000)
        return 1;

    Urb = Transfer->Urb;
    TransferLength = Transfer->TransferParameters.TransferBufferLength;

    if (TransferLength)
        Buffer = (ULONG_PTR)Urb->UrbControlTransfer.TransferBufferMDL->MappedSystemVa;
    else
        Buffer = 0;

    if (Buffer && TransferLength >= (NumberOfPorts / 8 + 1))
    {
        RtlZeroMemory((PVOID)Buffer, TransferLength);

        if (NumberOfPorts)
        {
            ix = 0;

            while (ix < 256)
            {
                DPRINT("USBPORT_RootHubSCE: ix - %p\n", ix);
                if (FdoExtension->MiniPortInterface->Packet.RH_GetPortStatus(FdoExtension->MiniPortExt,
                                                                             ix + 1,
                                                                             &PortStatus))
                    return 2;

                if (PortStatus & 0x001F0000)
                {
                    USBPORT_SetBit(Buffer, ix + 1);
                    RHStatus = 0;
                }

                ++ix;

                if (ix >= NumberOfPorts)
                    break;
            }
        }

        if (!FdoExtension->MiniPortInterface->Packet.RH_GetHubStatus(FdoExtension->MiniPortExt,
                                                                     &HubStatus))
        {
            if (HubStatus & 0x30000)
            {
                USBPORT_SetBit(Buffer, 0);
                RHStatus = 0;
            }

            if (RHStatus == 0)
            {
                Urb->UrbControlTransfer.TransferBufferLength = TransferLength;
                return 0;
            }

            if (RHStatus == 1)
            {
                FdoExtension->MiniPortInterface->Packet.RH_EnableIrq(FdoExtension->MiniPortExt);
            }

            return RHStatus;
        }
    }

    return 2;
}

VOID
NTAPI
USBPORT_RootHubEndpointWorker(IN PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_TRANSFER Transfer;
    RHSTATUS RHStatus;
    USBD_STATUS USBDStatus;
    KIRQL OldIrql;

    DPRINT_CORE("USBPORT_RootHubEndpointWorker: Endpoint - %p\n", Endpoint);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
    if (!(FdoExtension->Flags & 0x20300))
    {
        FdoExtension->MiniPortInterface->Packet.CheckController(FdoExtension->MiniPortExt);
    }
    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

    KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);

    Transfer = CONTAINING_RECORD(Endpoint->TransferList.Flink,
                                 USBPORT_TRANSFER,
                                 TransferLink);

    if (IsListEmpty(&Endpoint->TransferList) ||
        Endpoint->TransferList.Flink == NULL ||
        !Transfer)
    {
        if (Endpoint->StateLast == USBPORT_ENDPOINT_CLOSED)
        {
            ExInterlockedInsertTailList(&FdoExtension->EndpointClosedList,
                                        &Endpoint->CloseLink,
                                        &FdoExtension->EndpointClosedSpinLock);
            DPRINT("USBPORT_RootHubEndpointWorker: USBPORT_ENDPOINT_CLOSED Endpoint - %p\n", Endpoint);
       }

        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);
        USBPORT_FlushCancelList(Endpoint);
        return;
    }

    if (Transfer->Flags & (TRANSFER_FLAG_ABORTED | TRANSFER_FLAG_CANCELED))
    {
        RemoveEntryList(&Transfer->TransferLink);
        InsertTailList(&Endpoint->CancelList, &Transfer->TransferLink);

        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);
        USBPORT_FlushCancelList(Endpoint);
        return;
    }

    KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

    if (Endpoint->EndpointProperties.TransferType == USBPORT_TRANSFER_TYPE_CONTROL)
        RHStatus = USBPORT_RootHubEndpoint0(Transfer);
    else
        RHStatus = USBPORT_RootHubSCE(Transfer);

    if (RHStatus != 1)
    {
        if (!RHStatus)
            USBDStatus = USBD_STATUS_SUCCESS;
        else
            USBDStatus = USBD_STATUS_STALL_PID;

        KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);
        USBPORT_QueueDoneTransfer(Transfer, USBDStatus);
        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

        USBPORT_FlushCancelList(Endpoint);
        return;
    }

    USBPORT_FlushCancelList(Endpoint);
}

NTSTATUS
NTAPI
USBPORT_RootHubCreateDevice(IN PDEVICE_OBJECT FdoDevice,
                            IN PDEVICE_OBJECT PdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    USBPORT_ROOT_HUB_DATA RootHubData;
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
    USBPORT_AddDeviceHandle(FdoDevice, DeviceHandle);

    InitializeListHead(&DeviceHandle->PipeHandleList);

    DeviceHandle->IsRootHub = 1;
    DeviceHandle->DeviceSpeed = 1; // 0-low, 1-full, 2-high
    DeviceHandle->Flags = DEVICE_HANDLE_FLAG_ROOTHUB;

    RtlZeroMemory(&RootHubData, sizeof(RootHubData));

    FdoExtension->MiniPortInterface->Packet.RH_GetRootHubData(FdoExtension->MiniPortExt,
                                                              &RootHubData);

    ASSERT(RootHubData.NumberOfPorts > 1);
    NumMaskByte = ((RootHubData.NumberOfPorts - 1) >> 3) + 1;

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

        RH_HubDescriptor->bDescriptorLength = 7 + 2 * NumMaskByte;
        RH_HubDescriptor->bDescriptorType = 0x29;
        RH_HubDescriptor->bNumberOfPorts = RootHubData.NumberOfPorts;
        RH_HubDescriptor->wHubCharacteristics = RootHubData.HubCharacteristics;
        RH_HubDescriptor->bPowerOnToPowerGood = RootHubData.PowerOnToPowerGood;
        RH_HubDescriptor->bHubControlCurrent = RootHubData.HubControlCurrent;

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

ULONG
NTAPI
USBPORT_InvalidateRootHub(PVOID Context)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_ENDPOINT Endpoint = NULL;

    DPRINT("USBPORT_InvalidateRootHub ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context -
                                               sizeof(USBPORT_DEVICE_EXTENSION));

    FdoDevice = FdoExtension->CommonExtension.SelfDevice;

    if ( FdoExtension->Flags & USBPORT_FLAG_HC_SUSPEND && 
         FdoExtension->Flags & 0x00200000 &&
         FdoExtension->MiniPortFlags & USBPORT_MPFLAG_SUSPENDED &&
         FdoExtension->TimerFlags & USBPORT_TMFLAG_WAKE )
    {
        
        DPRINT1("USBPORT_InvalidateRootHub: USBPORT_HcQueueWakeDpc UNIMPLEMENTED. FIXME. \n");
        DbgBreakPoint();
        //USBPORT_HcQueueWakeDpc(FdoDevice);
        return 0;
    }
    
    FdoExtension->MiniPortInterface->Packet.RH_DisableIrq(FdoExtension->MiniPortExt);

    PdoDevice = FdoExtension->RootHubPdo;

    if (PdoDevice)
    {
        PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
        Endpoint = PdoExtension->Endpoint;
    }

    if (Endpoint)
    {
        USBPORT_InvalidateEndpointHandler(FdoDevice, PdoExtension->Endpoint, 1);
    }

    return 0;
}
