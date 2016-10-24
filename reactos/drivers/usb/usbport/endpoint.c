#include "usbport.h"

#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_CORE
#include "usbdebug.h"

MPSTATUS
NTAPI
MiniportOpenEndpoint(IN PDEVICE_OBJECT FdoDevice,
                     IN PUSBPORT_ENDPOINT Endpoint)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    KIRQL OldIrql;
    ULONG TransferType;
    MPSTATUS MpStatus;

    DPRINT("MiniportOpenEndpoint: Endpoint - %p\n", Endpoint);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);

    Endpoint->Flags &= ~ENDPOINT_FLAG_CLOSED;

    MpStatus = FdoExtension->MiniPortInterface->Packet.OpenEndpoint(FdoExtension->MiniPortExt,
                                                                    &Endpoint->EndpointProperties,
                                                                    (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)));

    if (!MpStatus)
    {
        TransferType = Endpoint->EndpointProperties.TransferType;

        if (TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT ||
            TransferType == USBPORT_TRANSFER_TYPE_ISOCHRONOUS)
        {
            ++FdoExtension->PeriodicEndpoints;
        }

        Endpoint->Flags |= ENDPOINT_FLAG_OPENED;
    }

    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
    return MpStatus;
}

NTSTATUS
NTAPI
USBPORT_OpenPipe(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                 IN PDEVICE_OBJECT FdoDevice,
                 IN PUSBPORT_PIPE_HANDLE PipeHandle,
                 IN OUT PUSBD_STATUS UsbdStatus)
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
    MPSTATUS MpStatus;
    USBD_STATUS USBDStatus;
    NTSTATUS Status;
    KIRQL OldIrql;

    DPRINT("USBPORT_OpenPipe: DeviceHandle - %p, FdoDevice - %p, PipeHandle - %p, UsbdStatus - %p\n",
           DeviceHandle,
           FdoDevice,
           PipeHandle,
           UsbdStatus);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    EndpointSize = sizeof(USBPORT_ENDPOINT) +
                   FdoExtension->MiniPortInterface->Packet.MiniPortEndpointSize;

    if (FdoExtension->MiniPortInterface->Packet.MiniPortFlags & USB_MINIPORT_FLAGS_USB2)
    {
        DPRINT1("USBPORT_OpenPipe: FIXME USB2 EndpointSize\n");
    }

    if (PipeHandle->EndpointDescriptor.wMaxPacketSize == 0)
    {
        USBPORT_AddPipeHandle(DeviceHandle, PipeHandle);
  
        PipeHandle->Flags = (PipeHandle->Flags & ~PIPE_HANDLE_FLAG_CLOSED) | 2;
        PipeHandle->Endpoint = (PUSBPORT_ENDPOINT)-1;

        return STATUS_SUCCESS;
    }

    Endpoint = ExAllocatePoolWithTag(NonPagedPool, EndpointSize, USB_PORT_TAG);

    if (!Endpoint)
    {
        DPRINT1("USBPORT_OpenPipe: Not allocated Endpoint!\n");
        Status = STATUS_INSUFFICIENT_RESOURCES;
        return Status;
    }

    RtlZeroMemory(Endpoint, EndpointSize);

    Endpoint->Flags = 0;
    Endpoint->FdoDevice = FdoDevice;
    Endpoint->DeviceHandle = (PUSBPORT_DEVICE_HANDLE)DeviceHandle;
    Endpoint->LockCounter = -1;

    KeInitializeSpinLock(&Endpoint->EndpointSpinLock);
    KeInitializeSpinLock(&Endpoint->StateChangeSpinLock);

    InitializeListHead(&Endpoint->PendingTransferList);
    InitializeListHead(&Endpoint->TransferList);
    InitializeListHead(&Endpoint->CancelList);
    InitializeListHead(&Endpoint->AbortList);

    EndpointProperties = &Endpoint->EndpointProperties;
    EndpointDescriptor = &PipeHandle->EndpointDescriptor;

    EndpointProperties->DeviceAddress = DeviceHandle->DeviceAddress;
    EndpointProperties->DeviceSpeed = DeviceHandle->DeviceSpeed;
    EndpointProperties->Period = 0; // HighSpeedInterval
    EndpointProperties->EndpointAddress = EndpointDescriptor->bEndpointAddress;

    EndpointProperties->MaxPacketSize = EndpointDescriptor->wMaxPacketSize &
                                        0x7FF; // FIXME for Highspeed

    switch (EndpointDescriptor->bmAttributes & USB_ENDPOINT_TYPE_MASK)
    {
        case USB_ENDPOINT_TYPE_CONTROL: // 0x00
            EndpointProperties->TransferType = USBPORT_TRANSFER_TYPE_CONTROL;
            if (EndpointProperties->EndpointAddress == 0)
                EndpointProperties->MaxTransferSize = 0x400; // OUT Ep0
            else
                EndpointProperties->MaxTransferSize = 0x10000;

            break;

        case USB_ENDPOINT_TYPE_ISOCHRONOUS: // 0x01
            EndpointProperties->TransferType = USBPORT_TRANSFER_TYPE_ISOCHRONOUS;
            DPRINT1("USBPORT_OpenPipe: USB_ENDPOINT_TYPE_ISOCHRONOUS UNIMPLEMENTED. FIXME. \n");
            break;

        case USB_ENDPOINT_TYPE_BULK: // 0x02
            EndpointProperties->TransferType = USBPORT_TRANSFER_TYPE_BULK;
            EndpointProperties->MaxTransferSize = 0x10000;
            break;

        case USB_ENDPOINT_TYPE_INTERRUPT: // 0x03
            EndpointProperties->TransferType = USBPORT_TRANSFER_TYPE_INTERRUPT;
            EndpointProperties->MaxTransferSize = 0x400;
            break;
    }

    if (EndpointProperties->TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT)
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

        if (EndpointProperties->TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT)
            PdoExtension->Endpoint = Endpoint;

        USBDStatus = USBD_STATUS_SUCCESS;
    }
    else
    {
        Endpoint->EndpointWorker = 1; // USBPORT_DmaEndpointWorker;

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FdoExtension->MiniPortInterface->Packet.QueryEndpointRequirements(FdoExtension->MiniPortExt,
                                                                          &Endpoint->EndpointProperties,
                                                                          (PULONG)&TransferParams);
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

        if ((EndpointProperties->TransferType == USBPORT_TRANSFER_TYPE_BULK) ||
            (EndpointProperties->TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT))
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

            MpStatus = MiniportOpenEndpoint(FdoDevice, Endpoint);

            Endpoint->Flags |= ENDPOINT_FLAG_DMA_TYPE;
            Endpoint->Flags |= ENDPOINT_FLAG_QUEUENE_EMPTY;

            if (MpStatus == 0)
            {
                KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);

                Endpoint->StateLast = USBPORT_ENDPOINT_PAUSED;
                Endpoint->StateNext = USBPORT_ENDPOINT_PAUSED;

                USBPORT_SetEndpointState(Endpoint, USBPORT_ENDPOINT_ACTIVE);

                KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);

                while (TRUE)
                {
                    if (Endpoint->StateLast == USBPORT_ENDPOINT_ACTIVE)
                        break;

                    USBPORT_Wait(FdoDevice, 1); // 1 msec.
                }
            }
        }
        else
        {
            MpStatus = 1;
            Endpoint->HeaderBuffer = NULL;
        }

        if (MpStatus)
            USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
        else
            USBDStatus = USBD_STATUS_SUCCESS;
    }

    if (UsbdStatus)
        *UsbdStatus = USBDStatus;

    Status = USBPORT_USBDStatusToNtStatus(NULL, USBDStatus);

    if (NT_SUCCESS(Status))
    {
        USBPORT_AddPipeHandle(DeviceHandle, PipeHandle);

        ExInterlockedInsertTailList(&FdoExtension->EndpointList,
                                    &Endpoint->EndpointLink,
                                    &FdoExtension->EndpointListSpinLock);

        PipeHandle->Endpoint = Endpoint;
        PipeHandle->Flags &= ~PIPE_HANDLE_FLAG_CLOSED;

        return Status;
    }
    else
    {
        if (Endpoint)
            ExFreePool(Endpoint);

        DPRINT1("USBPORT_OpenPipe: Status - %p\n", Status);
        return Status;
    }
}

NTSTATUS
NTAPI
USBPORT_ReopenPipe(IN PDEVICE_OBJECT FdoDevice,
                   IN PUSBPORT_ENDPOINT Endpoint)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
    ULONG EndpointRequirements[2] = {0};
    PUSBPORT_REGISTRATION_PACKET Packet;
    KIRQL MiniportOldIrql;
    KIRQL OldIrql;
    NTSTATUS Status;

    DPRINT("USBPORT_ReopenPipe ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    Packet = &FdoExtension->MiniPortInterface->Packet;

    while (TRUE)
    {
        if (!InterlockedIncrement(&Endpoint->LockCounter))
            break;
    }

    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &MiniportOldIrql);
    Packet->SetEndpointState(FdoExtension->MiniPortExt,
                             (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                             USBPORT_ENDPOINT_CLOSED);
    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, MiniportOldIrql);

    USBPORT_Wait(FdoDevice, 2);

    MiniportCloseEndpoint(FdoDevice, Endpoint);

    RtlZeroMemory((PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                  Packet->MiniPortEndpointSize);

    if (Endpoint->HeaderBuffer)
    {
        USBPORT_FreeCommonBuffer(FdoDevice, Endpoint->HeaderBuffer);
        Endpoint->HeaderBuffer = 0;
    }

    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &MiniportOldIrql);
    Packet->QueryEndpointRequirements(FdoExtension->MiniPortExt,
                                      &Endpoint->EndpointProperties,
                                      (PULONG)&EndpointRequirements);
    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, MiniportOldIrql);

    if (EndpointRequirements[0])
    {
        HeaderBuffer = USBPORT_AllocateCommonBuffer(FdoDevice,
                                                    EndpointRequirements[0]);
    }
    else
    {
        HeaderBuffer = NULL;
    }

    if (HeaderBuffer || EndpointRequirements[0] == 0)
    {
        Endpoint->HeaderBuffer = HeaderBuffer;
        Status = STATUS_SUCCESS;
    }
    else
    {
        Endpoint->HeaderBuffer = 0;
        Status = STATUS_INSUFFICIENT_RESOURCES;
    }

    if (Endpoint->HeaderBuffer && HeaderBuffer)
    {
        Endpoint->EndpointProperties.BufferVA = HeaderBuffer->VirtualAddress;
        Endpoint->EndpointProperties.BufferPA = HeaderBuffer->PhysicalAddress;
        Endpoint->EndpointProperties.BufferLength = HeaderBuffer->BufferLength;
    }

    if (NT_SUCCESS(Status))
    {
        MiniportOpenEndpoint(FdoDevice, Endpoint);

        KeAcquireSpinLock(&Endpoint->EndpointSpinLock, &Endpoint->EndpointOldIrql);
        KeAcquireSpinLock(&Endpoint->StateChangeSpinLock, &OldIrql);

        if (Endpoint->StateLast == USBPORT_ENDPOINT_ACTIVE)
        {
            KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &MiniportOldIrql);
            Packet->SetEndpointState(FdoExtension->MiniPortExt,
                                     (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                     USBPORT_ENDPOINT_ACTIVE);
            KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, MiniportOldIrql);
        }

        KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, OldIrql);
        KeReleaseSpinLock(&Endpoint->EndpointSpinLock, Endpoint->EndpointOldIrql);
    }

    InterlockedDecrement(&Endpoint->LockCounter);

    return Status;
}

