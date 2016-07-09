#include "usbport.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBPORT_SendSetupPacket(PUSBPORT_DEVICE_HANDLE UsbdDeviceHandle,
                        PDEVICE_OBJECT FdoDevice,
                        PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket,
                        PVOID Buffer,
                        ULONG Length,
                        PULONG TransferedLen,
                        PUSBD_STATUS pUSBDStatus)
{
    PURB Urb;
    PMDL Mdl;
    USBD_STATUS USBDStatus;
    KEVENT Event;
    NTSTATUS Status;

    DPRINT("USBPORT_SendSetupPacket: UsbdDeviceHandle - %p, FdoDevice - %p, SetupPacket - %p, Buffer - %p, Length - %x, TransferedLen - %x, pUSBDStatus - %x\n",
           UsbdDeviceHandle,
           FdoDevice,
           SetupPacket,
           Buffer,
           Length,
           TransferedLen,
           pUSBDStatus);

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    Urb = (PURB)ExAllocatePoolWithTag(NonPagedPool,
                                      sizeof(struct _URB_CONTROL_TRANSFER),
                                      USB_PORT_TAG);

    if (Urb)
    {
        RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_TRANSFER));

        RtlCopyMemory(Urb->UrbControlTransfer.SetupPacket,
                      SetupPacket,
                      sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

        Urb->UrbHeader.Length = sizeof(struct _URB_CONTROL_TRANSFER);
        Urb->UrbHeader.Function = URB_FUNCTION_CONTROL_TRANSFER;
        Urb->UrbHeader.UsbdDeviceHandle = (PVOID)UsbdDeviceHandle;
        Urb->UrbHeader.UsbdFlags = 0;

        Urb->UrbControlTransfer.PipeHandle = &UsbdDeviceHandle->PipeHandle;
        Urb->UrbControlTransfer.TransferBufferLength = Length;
        Urb->UrbControlTransfer.TransferBuffer = Buffer;
        Urb->UrbControlTransfer.TransferBufferMDL = NULL;

        Urb->UrbControlTransfer.TransferFlags = USBD_SHORT_TRANSFER_OK |
                                                USBD_TRANSFER_DIRECTION;

        if (SetupPacket->bmRequestType._BM.Dir != BMREQUEST_DEVICE_TO_HOST)
        {
            Urb->UrbControlTransfer.TransferFlags &= ~USBD_TRANSFER_DIRECTION_IN;
        }

        Status = STATUS_SUCCESS;

        if (Length)
        {
            Mdl = IoAllocateMdl(Buffer, Length, FALSE, FALSE, NULL);

            Urb->UrbControlTransfer.TransferBufferMDL = Mdl;

            if (Mdl)
            {
                Urb->UrbControlTransfer.hca.Reserved8[1] = (PVOID)USBD_FLAG_ALLOCATED_MDL;
                MmBuildMdlForNonPagedPool(Mdl);
            }
            else
            {
                Status = USBPORT_USBDStatusToNtStatus(NULL,
                                                      USBD_STATUS_INSUFFICIENT_RESOURCES);
            }
        }

        if (NT_SUCCESS(Status))
        {
            USBDStatus = USBPORT_AllocateTransfer(FdoDevice,
                                                  Urb,
                                                  NULL,
                                                  NULL,
                                                  &Event);

            if (USBD_SUCCESS(USBDStatus))
            {
                USBPORT_QueueTransferUrb(Urb);

                KeWaitForSingleObject(&Event,
                                      Suspended,
                                      KernelMode,
                                      FALSE,
                                      NULL);

                USBDStatus = Urb->UrbHeader.Status;
            }

            Status = USBPORT_USBDStatusToNtStatus(Urb, USBDStatus);

            if (TransferedLen)
                *TransferedLen = Urb->UrbControlTransfer.TransferBufferLength;

            if (pUSBDStatus)
                *pUSBDStatus = USBDStatus;
        }

        ExFreePool(Urb);
    }
    else
    {
        if (pUSBDStatus)
            *pUSBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;

        Status = USBPORT_USBDStatusToNtStatus(NULL,
                                              USBD_STATUS_INSUFFICIENT_RESOURCES);
    }

    DPRINT("USBPORT_SendSetupPacket: Status - %x\n", Status);
    return Status;
}

VOID
USBPORT_SetEndpointState(PUSBPORT_ENDPOINT Endpoint,
                         ULONG State)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_SetEndpointState \n");

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (!(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
    {
        FdoExtension->MiniPortInterface->Packet.SetEndpointState(FdoExtension->MiniPortExt,
                                                                 (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                                                 State);

        Endpoint->StateNext = State;

        ExInterlockedInsertTailList(&FdoExtension->EpStateChangeList,
                                    &Endpoint->StateChangeLink,
                                    &FdoExtension->EpStateChangeSpinLock);

        FdoExtension->MiniPortInterface->Packet.InterruptNextSOF(FdoExtension->MiniPortExt);
    }
}

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
    UCHAR Interval;
    UCHAR Period;
    ULONG TransferParams[2] = {0};
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
    ULONG Result;
    USBD_STATUS USBDStatus;
    NTSTATUS Status;

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
        Interval = EndpointDescriptor->bInterval;
        EndpointProperties->Period = 0x20;

        if (Interval && (Interval < 0x20))
        {
            if ((EndpointProperties->DeviceSpeed != UsbLowSpeed) ||
                (Interval >= 8))
            {
                if (!(Interval & 0x20))
                {
                    Period = EndpointProperties->Period;

                    do
                    {
                        Period >>= 1;
                    }
                    while (!(Period & Interval));

                    EndpointProperties->Period = Period;
                }
            }
            else
            {
                EndpointProperties->Period = 8;
            }
        }
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

        USBDStatus = USBD_STATUS_SUCCESS;
    }
    {
        Endpoint->EndpointWorker = 1; // USBPORT_DmaEndpointWorker;

        FdoExtension->MiniPortInterface->Packet.QueryEndpointRequirements(FdoExtension->MiniPortExt,
                                                                          &Endpoint->EndpointProperties,
                                                                          (PULONG)&TransferParams);

        if ((EndpointProperties->TransferType == USB_ENDPOINT_TYPE_BULK) ||
            (EndpointProperties->TransferType == USB_ENDPOINT_TYPE_INTERRUPT))
        {
            EndpointProperties->MaxTransferSize = TransferParams[1];
        }

        if (TransferParams[0])
        {
            HeaderBuffer = USBPORT_AllocateCommonBuffer(FdoDevice,
                                                        TransferParams[0]);
        }
        else
        {
            HeaderBuffer = NULL;
        }

        if (HeaderBuffer || (TransferParams[0] == 0))
        {
            Endpoint->HeaderBuffer = HeaderBuffer;

            if (HeaderBuffer)
            {
                EndpointProperties->BufferVA = HeaderBuffer->VirtualAddress;
                EndpointProperties->BufferPA = HeaderBuffer->PhysicalAddress;
                EndpointProperties->BufferLength = HeaderBuffer->BufferLength; // BufferLength + LengthPadded;
            }

            Endpoint->Flags &= ~ENDPOINT_FLAG_CLOSED;
            
            Result = FdoExtension->MiniPortInterface->Packet.OpenEndpoint(FdoExtension->MiniPortExt,
                                                                          &Endpoint->EndpointProperties,
                                                                          (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)));

            Endpoint->Flags |= ENDPOINT_FLAG_DMA_TYPE;
            Endpoint->Flags |= ENDPOINT_FLAG_QUEUENE_EMPTY;

            if (Result == 0)
            {
                Endpoint->Flags |= ENDPOINT_FLAG_OPENED;
                Endpoint->StateLast = USBPORT_ENDPOINT_PAUSED;
                Endpoint->StateNext = USBPORT_ENDPOINT_PAUSED;

                USBPORT_SetEndpointState(Endpoint, USBPORT_ENDPOINT_ACTIVE);

                while (TRUE)
                {
                    if (Endpoint->StateLast == USBPORT_ENDPOINT_ACTIVE)
                        break;

                    ASSERT(FALSE); // USBPORT_Wait(1); // 1 msec.
                }
            }
        }
        else
        {
            Result = 1;
            Endpoint->HeaderBuffer = NULL;
        }

        if (Result)
            USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
        else
            USBDStatus = USBD_STATUS_SUCCESS;
    }

    if (UsbdStatus)
        *UsbdStatus = USBDStatus;

    Status = USBPORT_USBDStatusToNtStatus(NULL, USBDStatus);

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

ULONG
NTAPI
USBPORT_GetInterfaceLength(IN PUSB_INTERFACE_DESCRIPTOR iDescriptor,
                           IN ULONG EndDescriptors)
{
    SIZE_T Length;
    PUSB_ENDPOINT_DESCRIPTOR Descriptor;
    ULONG ix;

    DPRINT("USBPORT_GetInterfaceLength ... \n");

    Length = iDescriptor->bLength;
    Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)iDescriptor + Length);

    if (iDescriptor->bNumEndpoints)
    {
        ix = iDescriptor->bNumEndpoints;

        do
        {
            while ((Descriptor->bDescriptorType != USB_ENDPOINT_DESCRIPTOR_TYPE) &&
                   (Descriptor->bLength > 0))
            {
                Length += Descriptor->bLength;
                Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)Descriptor +
                                                        Descriptor->bLength);
            }

            Length += Descriptor->bLength;
            Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)Descriptor +
                                                    Descriptor->bLength);
            --ix;
        }
        while (ix);
    }

    while (((ULONG_PTR)Descriptor < EndDescriptors) &&
           (Descriptor->bDescriptorType != USB_INTERFACE_DESCRIPTOR_TYPE) &&
           (Descriptor->bLength > 0))
    {
        Length += Descriptor->bLength;
        Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)Descriptor +
                                                Descriptor->bLength);
    }

    return Length;
}

PUSB_INTERFACE_DESCRIPTOR
USBPORT_ParseConfigurationDescriptor(IN PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor,
                                     IN UCHAR InterfaceNumber,
                                     IN UCHAR Alternate,
                                     OUT PUCHAR pAlternate)
{
    PUSB_CONFIGURATION_DESCRIPTOR TmpDescriptor;
    PUSB_INTERFACE_DESCRIPTOR iDescriptor;
    PUSB_INTERFACE_DESCRIPTOR OutDescriptor = NULL;
    ULONG_PTR Descriptor = (ULONG_PTR)ConfigDescriptor;
    ULONG EndDescriptors;
    ULONG ix;

    DPRINT("USBPORT_ParseConfigurationDescriptor ... \n");

    if (pAlternate)
        *pAlternate = 0;

    for (TmpDescriptor = (PUSB_CONFIGURATION_DESCRIPTOR)((ULONG_PTR)ConfigDescriptor + ConfigDescriptor->bLength);
         TmpDescriptor->bDescriptorType == USB_CONFIGURATION_DESCRIPTOR_TYPE && TmpDescriptor->bDescriptorType > 0;
         TmpDescriptor = (PUSB_CONFIGURATION_DESCRIPTOR)((ULONG_PTR)TmpDescriptor + TmpDescriptor->bLength));

    iDescriptor = (PUSB_INTERFACE_DESCRIPTOR)TmpDescriptor;

    EndDescriptors = (ULONG_PTR)ConfigDescriptor +
                                ConfigDescriptor->wTotalLength;

    while ((Descriptor < EndDescriptors) &&
           (iDescriptor->bInterfaceNumber != InterfaceNumber))
    {
        Descriptor = (ULONG_PTR)iDescriptor +
                     USBPORT_GetInterfaceLength(iDescriptor, EndDescriptors);

        iDescriptor = (PUSB_INTERFACE_DESCRIPTOR)Descriptor;
    }

    ix = 0;

    if (Descriptor < EndDescriptors)
    {
        do
        {
            if (iDescriptor->bInterfaceNumber != InterfaceNumber)
                break;

            if (iDescriptor->bAlternateSetting == Alternate)
                OutDescriptor = iDescriptor;

            Descriptor = (ULONG_PTR)iDescriptor +
                         USBPORT_GetInterfaceLength(iDescriptor, EndDescriptors);

            iDescriptor = (PUSB_INTERFACE_DESCRIPTOR)Descriptor;

            ++ix;
        }
        while (Descriptor < EndDescriptors);

        if ((ix > 1) && pAlternate)
            *pAlternate = 1;
    }

    return OutDescriptor;
}

USBD_STATUS
NTAPI
USBPORT_OpenInterface(IN PURB Urb,
                      IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                      IN PDEVICE_OBJECT FdoDevice,
                      IN PUSBPORT_CONFIGURATION_HANDLE ConfigHandle,
                      IN PUSBD_INTERFACE_INFORMATION InterfaceInfo,
                      IN OUT PUSBPORT_INTERFACE_HANDLE *iHandle,
                      IN BOOLEAN IsSetInterface)
{
    PUSB_INTERFACE_DESCRIPTOR InterfaceDescriptor;
    PUSBPORT_INTERFACE_HANDLE InterfaceHandle = NULL;
    PUSBPORT_PIPE_HANDLE PipeHandle;
    PUSB_ENDPOINT_DESCRIPTOR Descriptor;
    PUSBD_PIPE_INFORMATION PipeInfo;
    ULONG NumInterfaces;
    SIZE_T Length;
    SIZE_T HandleLength;
    BOOLEAN IsAllocated = FALSE;
    USHORT MaxPacketSize;
    USHORT wMaxPacketSize;
    ULONG ix;
    USBD_STATUS USBDStatus = USBD_STATUS_SUCCESS;
    NTSTATUS Status;

    DPRINT("USBPORT_OpenInterface: ...\n");

    InterfaceDescriptor = USBPORT_ParseConfigurationDescriptor(ConfigHandle->ConfigurationDescriptor,
                                                               InterfaceInfo->InterfaceNumber,
                                                               InterfaceInfo->AlternateSetting,
                                                               &InterfaceInfo->AlternateSetting);

    NumInterfaces = InterfaceDescriptor->bNumEndpoints;

    Length = sizeof(USBD_INTERFACE_INFORMATION) +
             (NumInterfaces - 1) * sizeof(USBD_PIPE_INFORMATION);

    if (InterfaceInfo->AlternateSetting && IsSetInterface)
    {
        ASSERT(FALSE);
    }

    if (*iHandle)
    {
        InterfaceHandle = *iHandle;
    }
    else
    {
        HandleLength = sizeof(USBPORT_INTERFACE_HANDLE) + 
                       (NumInterfaces - 1) * sizeof(USBPORT_PIPE_HANDLE);

        InterfaceHandle = (PUSBPORT_INTERFACE_HANDLE)ExAllocatePoolWithTag(NonPagedPool,
                                                                           HandleLength,
                                                                           USB_PORT_TAG);

        if (!InterfaceHandle)
        {
            USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
            goto Exit;
        }

        RtlZeroMemory(InterfaceHandle, HandleLength);

        ix = NumInterfaces;

        if (NumInterfaces > 0)
        {
            PipeHandle = &InterfaceHandle->PipeHandle[0];

            do
            {
                PipeHandle->Flags = PIPEHANDLE_FLAG_CLOSED;
                PipeHandle->Endpoint = NULL;

                PipeHandle += 1;
                --ix;
            }
            while (ix);
        }

        IsAllocated = TRUE;
    }

    InterfaceHandle->AlternateSetting = InterfaceInfo->AlternateSetting;

    RtlCopyMemory(&InterfaceHandle->InterfaceDescriptor,
                  InterfaceDescriptor,
                  sizeof(USB_INTERFACE_DESCRIPTOR));

    InterfaceInfo->Class = InterfaceDescriptor->bInterfaceClass;
    InterfaceInfo->SubClass = InterfaceDescriptor->bInterfaceSubClass;
    InterfaceInfo->Protocol = InterfaceDescriptor->bInterfaceProtocol;
    InterfaceInfo->Reserved = 0;
    InterfaceInfo->NumberOfPipes = InterfaceDescriptor->bNumEndpoints;

    Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)InterfaceDescriptor +
                                            InterfaceDescriptor->bLength);

    ix = 0;

    if (NumInterfaces)
    {
        PipeHandle = &InterfaceHandle->PipeHandle[0];

        do
        {
            if (Descriptor->bDescriptorType != USB_ENDPOINT_DESCRIPTOR_TYPE)
            {
                do
                {
                    if (Descriptor->bLength == 0)
                        break;
                    else
                        Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)Descriptor +
                                                                Descriptor->bLength);
                }
                while (Descriptor->bDescriptorType != USB_ENDPOINT_DESCRIPTOR_TYPE);
            }

            if (InterfaceInfo->Pipes[ix].PipeFlags & USBD_PF_CHANGE_MAX_PACKET)
                Descriptor->wMaxPacketSize = InterfaceInfo->Pipes[ix].MaximumPacketSize;

            RtlCopyMemory(&PipeHandle->EndpointDescriptor,
                          Descriptor,
                          sizeof(USB_ENDPOINT_DESCRIPTOR));

            PipeHandle->Flags = PIPEHANDLE_FLAG_CLOSED;
            PipeHandle->PipeFlags = InterfaceInfo->Pipes[ix].PipeFlags;
            PipeHandle->Endpoint = NULL;

            wMaxPacketSize = Descriptor->wMaxPacketSize;

            MaxPacketSize = (wMaxPacketSize & 0x7FF) * (((wMaxPacketSize >> 11) & 3) + 1);

            InterfaceInfo->Pipes[ix].EndpointAddress = Descriptor->bEndpointAddress;
            InterfaceInfo->Pipes[ix].PipeType = Descriptor->bmAttributes & 3;
            InterfaceInfo->Pipes[ix].MaximumPacketSize = MaxPacketSize;
            InterfaceInfo->Pipes[ix].PipeHandle = (USBD_PIPE_HANDLE)-1;
            InterfaceInfo->Pipes[ix].Interval = Descriptor->bInterval;

            ++ix;

            Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)Descriptor +
                                                    Descriptor->bLength);

            PipeHandle += 1;
        }
        while (ix < NumInterfaces);
    }

    if (USBD_SUCCESS(USBDStatus))
    {
        ix = 0;

        if (InterfaceDescriptor->bNumEndpoints)
        {
            PipeInfo = &InterfaceInfo->Pipes[ix];
            PipeHandle = &InterfaceHandle->PipeHandle[0];

            while (TRUE)
            {
                Status = USBPORT_OpenPipe(DeviceHandle,
                                          FdoDevice,
                                          PipeHandle,
                                          &USBDStatus);

                if (!NT_SUCCESS(Status))
                    break;

                PipeInfo->PipeHandle = (USBD_PIPE_HANDLE)PipeHandle;

                PipeHandle += 1;
                PipeInfo += 1;

                ++ix;

                if (ix >= InterfaceDescriptor->bNumEndpoints)
                    goto Exit;
            }

            USBPORT_USBDStatusToNtStatus(Urb, USBDStatus);
        }
    }

Exit:

    if (USBD_SUCCESS(USBDStatus))
    {
        InterfaceInfo->InterfaceHandle = (USBD_INTERFACE_HANDLE)InterfaceHandle;
        *iHandle = InterfaceHandle;
        InterfaceInfo->Length = Length;
    }
    else
    {
        if (InterfaceHandle)
        {
            if (NumInterfaces)
            {
                ASSERT(FALSE);
            }

            if (IsAllocated)
                ExFreePool(InterfaceHandle);
        }
    }

    return USBDStatus;
}

NTSTATUS
NTAPI
USBPORT_InitInterfaceInfo(IN PUSBD_INTERFACE_INFORMATION InterfaceInfo,
                          IN PUSBPORT_CONFIGURATION_HANDLE ConfigHandle)
{
    PUSB_INTERFACE_DESCRIPTOR Descriptor;
    PUSBD_PIPE_INFORMATION Pipe;
    USHORT Length;
    ULONG PipeFlags;
    ULONG NumberOfPipes;
    USBD_STATUS USBDStatus = USBD_STATUS_SUCCESS;

    DPRINT("USBPORT_InitInterfaceInfo: InterfaceInfo - %p, ConfigHandle - %p\n",
           InterfaceInfo,
           ConfigHandle);

    Descriptor = USBPORT_ParseConfigurationDescriptor(ConfigHandle->ConfigurationDescriptor,
                                                      InterfaceInfo->InterfaceNumber,
                                                      InterfaceInfo->AlternateSetting,
                                                      &InterfaceInfo->AlternateSetting);

    Length = sizeof(USBD_INTERFACE_INFORMATION) +
             sizeof(USBD_PIPE_INFORMATION);

    if (Descriptor)
    {
        NumberOfPipes = Descriptor->bNumEndpoints;
        Length = sizeof(USBD_INTERFACE_INFORMATION) + (NumberOfPipes - 1) * sizeof(USBD_PIPE_INFORMATION);

        if (InterfaceInfo->Length >= Length)
        {
            InterfaceInfo->Class = 0;
            InterfaceInfo->SubClass = 0;
            InterfaceInfo->Protocol = 0;
            InterfaceInfo->Reserved = 0;
            InterfaceInfo->InterfaceHandle = 0;
            InterfaceInfo->NumberOfPipes = NumberOfPipes;

            if (NumberOfPipes > 0)
            {
                Pipe = InterfaceInfo->Pipes;

                do
                {
                    Pipe->EndpointAddress = 0;
                    Pipe->Interval = 0;
                    Pipe->PipeType = 0;
                    Pipe->PipeHandle = 0;

                    PipeFlags = Pipe->PipeFlags;

                    if (PipeFlags & ~USBD_PF_VALID_MASK)
                        USBDStatus = USBD_STATUS_INVALID_PIPE_FLAGS;

                    if (!(PipeFlags & USBD_PF_CHANGE_MAX_PACKET))
                        Pipe->MaximumPacketSize = 0;

                    Pipe += 1;
                    --NumberOfPipes;
                }
                while (NumberOfPipes > 0);
            }
        }
        else
        {
            USBDStatus = USBD_STATUS_BUFFER_TOO_SMALL;
        }
    }
    else
    {
        USBDStatus = USBD_STATUS_INTERFACE_NOT_FOUND;
    }

    InterfaceInfo->Length = Length;
    return USBDStatus;
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
    PUSBPORT_INTERFACE_HANDLE InterfaceHandle;
    ULONG iNumber;
    ULONG ix;
    USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    NTSTATUS Status;
    USBD_STATUS USBDStatus;

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

            USBPORT_SendSetupPacket(DeviceHandle,
                                    FdoDevice,
                                    &SetupPacket,
                                    NULL,
                                    0,
                                    NULL,
                                    &USBDStatus);

            if (USBD_SUCCESS(USBDStatus))
            {
                if (iNumber <= 0)
                {
                    Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                          USBD_STATUS_SUCCESS);

                    goto Exit;
                }

                InterfaceInfo = &Urb->UrbSelectConfiguration.Interface;

                ix = 0;

                while (TRUE)
                {
                    USBDStatus = USBPORT_InitInterfaceInfo(InterfaceInfo,
                                                           ConfigHandle);

                    InterfaceHandle = 0;

                    if (USBD_SUCCESS(USBDStatus))
                        USBDStatus = USBPORT_OpenInterface(Urb,
                                                           DeviceHandle,
                                                           FdoDevice,
                                                           ConfigHandle,
                                                           InterfaceInfo,
                                                           &InterfaceHandle,
                                                           TRUE);

                    if (InterfaceHandle)
                    {
                        InsertTailList(&ConfigHandle->InterfaceHandleList,
                                       &InterfaceHandle->InterfaceLink);
                    }

                    if (USBD_ERROR(USBDStatus))
                        break;

                    ++ix;

                    if (ix >= iNumber)
                    {
                        Status = USBPORT_USBDStatusToNtStatus(Urb,
                                                              USBD_STATUS_SUCCESS);
                        goto Exit;
                    }

                    InterfaceInfo = (PUSBD_INTERFACE_INFORMATION)((ULONG_PTR)InterfaceInfo +
                                                                  InterfaceInfo->Length);
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

NTSTATUS
NTAPI
USBPORT_CreateDevice(PUSB_DEVICE_HANDLE *pHandle,
                     PDEVICE_OBJECT FdoDevice,
                     PUSBPORT_DEVICE_HANDLE HubDeviceHandle,
                     USHORT PortStatus,
                     USHORT Port)
{
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    PUSBPORT_PIPE_HANDLE PipeHandle;
    BOOLEAN IsOpenedPipe;
    PVOID DeviceDescriptor;
    USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    SIZE_T TransferedLen;
    UCHAR MaxPacketSize;

    NTSTATUS Status;

    DPRINT("USBPORT_CreateDevice: PortStatus - %p, Port - %p\n",
           PortStatus,
           Port);

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)ExAllocatePoolWithTag(NonPagedPool,
                                                                 sizeof(USBPORT_DEVICE_HANDLE),
                                                                 USB_PORT_TAG);

    if (!DeviceHandle)
        return STATUS_INSUFFICIENT_RESOURCES;

    RtlZeroMemory(DeviceHandle, sizeof(USBPORT_DEVICE_HANDLE));

    *pHandle = NULL;

    DeviceHandle->PortNumber = Port;
    DeviceHandle->HubDeviceHandle = HubDeviceHandle;

    if (PortStatus & USB_PORT_STATUS_LOW_SPEED)
        DeviceHandle->DeviceSpeed = 0;
    else
        DeviceHandle->DeviceSpeed = 1; // Fullspeed

    PipeHandle = &DeviceHandle->PipeHandle;

    PipeHandle->EndpointDescriptor.bLength = 7;
    PipeHandle->EndpointDescriptor.bDescriptorType = 5;
    PipeHandle->EndpointDescriptor.bEndpointAddress = 0;
    PipeHandle->EndpointDescriptor.bmAttributes = 0;
    PipeHandle->EndpointDescriptor.bInterval = 0;

    PipeHandle->Flags = PIPEHANDLE_FLAG_CLOSED;
    PipeHandle->PipeFlags = 0;

    if (DeviceHandle->DeviceSpeed == 0)
        PipeHandle->EndpointDescriptor.wMaxPacketSize = 8;
    else
        PipeHandle->EndpointDescriptor.wMaxPacketSize = USB_DEFAULT_MAX_PACKET;

    InitializeListHead(&DeviceHandle->PipeHandleList);

    Status = USBPORT_OpenPipe(DeviceHandle, FdoDevice, PipeHandle, NULL);

    if (NT_SUCCESS(Status))
    {
        IsOpenedPipe = TRUE;
    }
    else
    {
        ExFreePool(DeviceHandle);
        return Status;
    }

    DeviceDescriptor = ExAllocatePoolWithTag(NonPagedPool,
                                             64,
                                             USB_PORT_TAG);

    if (!DeviceDescriptor)
    {
        Status = STATUS_DEVICE_DATA_ERROR;

        if (IsOpenedPipe)
            ASSERT(FALSE); //USBPORT_ClosePipe();

        ExFreePool(DeviceHandle);
        return Status;
    }

    RtlZeroMemory(DeviceDescriptor, USB_DEFAULT_MAX_PACKET);
    RtlZeroMemory(&SetupPacket, sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

    SetupPacket.bmRequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;
    SetupPacket.bRequest = USB_REQUEST_GET_DESCRIPTOR;
    SetupPacket.wValue.HiByte = USB_DEVICE_DESCRIPTOR_TYPE;
    SetupPacket.wLength = USB_DEFAULT_MAX_PACKET;

    TransferedLen = 0;

    Status = USBPORT_SendSetupPacket(DeviceHandle,
                                     FdoDevice,
                                     &SetupPacket,
                                     DeviceDescriptor,
                                     USB_DEFAULT_MAX_PACKET,
                                     &TransferedLen,
                                     NULL);

    RtlCopyMemory(&DeviceHandle->DeviceDescriptor,
                  DeviceDescriptor,
                  sizeof(USB_DEVICE_DESCRIPTOR));

    ExFreePool(DeviceDescriptor);

    if ((TransferedLen == 8) && (!NT_SUCCESS(Status)))
        Status = STATUS_SUCCESS;

    if (NT_SUCCESS(Status) && (TransferedLen >= 8))
    {
        if ((DeviceHandle->DeviceDescriptor.bLength >= sizeof(USB_DEVICE_DESCRIPTOR)) &&
            (DeviceHandle->DeviceDescriptor.bDescriptorType == 1))
        {
            MaxPacketSize = DeviceHandle->DeviceDescriptor.bMaxPacketSize0;

            if (MaxPacketSize == 8 ||
                MaxPacketSize == 16 ||
                MaxPacketSize == 32 ||
                MaxPacketSize == 64)
            {
                *pHandle = DeviceHandle;

                if (!NT_SUCCESS(Status))
                    ExFreePool(DeviceHandle);

                return Status;
            }
        }
    }

    ASSERT(FALSE);

    return Status;
}
