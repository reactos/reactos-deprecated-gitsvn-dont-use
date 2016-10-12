#include "usbport.h"

//#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_URB
#include "usbdebug.h"

NTSTATUS
NTAPI
USBPORT_SendSetupPacket(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                        IN PDEVICE_OBJECT FdoDevice,
                        IN PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket,
                        IN PVOID Buffer,
                        IN ULONG Length,
                        IN OUT PULONG TransferedLen,
                        IN OUT PUSBD_STATUS pUSBDStatus)
{
    PURB Urb;
    PMDL Mdl;
    USBD_STATUS USBDStatus;
    KEVENT Event;
    NTSTATUS Status;

    DPRINT("USBPORT_SendSetupPacket: DeviceHandle - %p, FdoDevice - %p, SetupPacket - %p, Buffer - %p, Length - %x, TransferedLen - %x, pUSBDStatus - %x\n",
           DeviceHandle,
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
        InterlockedIncrement(&DeviceHandle->DeviceHandleLock);

        RtlZeroMemory(Urb, sizeof(struct _URB_CONTROL_TRANSFER));

        RtlCopyMemory(Urb->UrbControlTransfer.SetupPacket,
                      SetupPacket,
                      sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

        Urb->UrbHeader.Length = sizeof(struct _URB_CONTROL_TRANSFER);
        Urb->UrbHeader.Function = URB_FUNCTION_CONTROL_TRANSFER;
        Urb->UrbHeader.UsbdDeviceHandle = (PVOID)DeviceHandle;
        Urb->UrbHeader.UsbdFlags = 0;

        Urb->UrbControlTransfer.PipeHandle = &DeviceHandle->PipeHandle;
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
                Urb->UrbHeader.UsbdFlags |= USBD_FLAG_ALLOCATED_MDL;
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
                InterlockedIncrement(&DeviceHandle->DeviceHandleLock);

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

        InterlockedDecrement(&DeviceHandle->DeviceHandleLock);
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
NTAPI
USBPORT_SetEndpointState(IN PUSBPORT_ENDPOINT Endpoint,
                         IN ULONG State)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;

    DPRINT("USBPORT_SetEndpointState \n");

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&Endpoint->StateChangeSpinLock, &Endpoint->EndpointStateOldIrql);

    if (!(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
    {
        if (Endpoint->Flags & ENDPOINT_FLAG_NUKE)
        {
            Endpoint->StateLast = State;
            Endpoint->StateNext = State;

            KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);
            USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 1);
            return;
        }

        KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FdoExtension->MiniPortInterface->Packet.SetEndpointState(FdoExtension->MiniPortExt,
                                                                 (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                                                 State);
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

        Endpoint->StateNext = State;

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        Endpoint->FrameNumber = FdoExtension->MiniPortInterface->Packet.Get32BitFrameNumber(FdoExtension->MiniPortExt);
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);

        ExInterlockedInsertTailList(&FdoExtension->EpStateChangeList,
                                    &Endpoint->StateChangeLink,
                                    &FdoExtension->EpStateChangeSpinLock);

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FdoExtension->MiniPortInterface->Packet.InterruptNextSOF(FdoExtension->MiniPortExt);
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
    }
    else
    {
        Endpoint->StateLast = State;
        Endpoint->StateNext = State;

        if (State == USBPORT_ENDPOINT_CLOSED)
        {
            KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);
            USBPORT_InvalidateEndpointHandler(FdoDevice, Endpoint, 1);
            return;
        }

        KeReleaseSpinLock(&Endpoint->StateChangeSpinLock, Endpoint->EndpointStateOldIrql);
    }
}

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

VOID
NTAPI
MiniportCloseEndpoint(IN PDEVICE_OBJECT FdoDevice,
                      IN PUSBPORT_ENDPOINT Endpoint)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    BOOLEAN IsDoDisablePeriodic;
    ULONG TransferType;
    KIRQL OldIrql;

    DPRINT("MiniportCloseEndpoint: Endpoint - %p\n", Endpoint);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);

    if (Endpoint->Flags & ENDPOINT_FLAG_OPENED)
    {
        TransferType = Endpoint->EndpointProperties.TransferType;

        if (TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT ||
            TransferType == USBPORT_TRANSFER_TYPE_ISOCHRONOUS)
        {
            --FdoExtension->PeriodicEndpoints;
        }

        IsDoDisablePeriodic = FdoExtension->PeriodicEndpoints == 0;

        FdoExtension->MiniPortInterface->Packet.CloseEndpoint(FdoExtension->MiniPortExt,
                                                              (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                                              IsDoDisablePeriodic);

        Endpoint->Flags &= ~ENDPOINT_FLAG_OPENED;
        Endpoint->Flags |= ENDPOINT_FLAG_CLOSED;
    }

    KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
}

BOOLEAN
NTAPI
USBPORT_DeleteEndpoint(IN PDEVICE_OBJECT FdoDevice,
                       IN PUSBPORT_ENDPOINT Endpoint)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    BOOLEAN Result;
    KIRQL OldIrql;

    DPRINT("USBPORT_DeleteEndpoint: Endpoint - %p\n", Endpoint);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if ((Endpoint->WorkerLink.Flink && Endpoint->WorkerLink.Blink) ||
        Endpoint->LockCounter != -1)
    {
        KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

        ExInterlockedInsertTailList(&FdoExtension->EndpointClosedList,
                                    &Endpoint->CloseLink,
                                    &FdoExtension->EndpointClosedSpinLock);

        KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);

        Result = FALSE;
    }
    else
    {
        KeAcquireSpinLock(&FdoExtension->EndpointListSpinLock, &OldIrql);

        RemoveEntryList(&Endpoint->EndpointLink);
        Endpoint->EndpointLink.Flink = NULL;
        Endpoint->EndpointLink.Blink = NULL;

        KeReleaseSpinLock(&FdoExtension->EndpointListSpinLock, OldIrql);

        MiniportCloseEndpoint(FdoDevice, Endpoint);

        if (Endpoint->HeaderBuffer)
        {
            USBPORT_FreeCommonBuffer(FdoDevice, Endpoint->HeaderBuffer);
        }

        ExFreePool(Endpoint);

        Result = TRUE;
    }

    return Result;
}

VOID
NTAPI
USBPORT_FlushClosedEndpointList(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    KIRQL OldIrql;
    PLIST_ENTRY ClosedList;
    PUSBPORT_ENDPOINT Endpoint;

    DPRINT("USBPORT_FlushClosedEndpointList: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->EndpointClosedSpinLock, &OldIrql);
    ClosedList = &FdoExtension->EndpointClosedList;

    while (TRUE)
    {
        if (IsListEmpty(ClosedList))
            break;

        Endpoint = CONTAINING_RECORD(ClosedList->Flink,
                                     USBPORT_ENDPOINT,
                                     CloseLink);

        RemoveHeadList(ClosedList);
        Endpoint->CloseLink.Flink = NULL;
        Endpoint->CloseLink.Blink = NULL;

        KeReleaseSpinLock(&FdoExtension->EndpointClosedSpinLock, OldIrql);

        USBPORT_DeleteEndpoint(FdoDevice, Endpoint);

        KeAcquireSpinLock(&FdoExtension->EndpointClosedSpinLock, &OldIrql);
    }

    KeReleaseSpinLock(&FdoExtension->EndpointClosedSpinLock, OldIrql);
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

        DPRINT1("USBPORT_OpenPipe: Status - %p\n", Status);
        return Status;
    }
}

VOID
NTAPI
USBPORT_ClosePipe(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                  IN PDEVICE_OBJECT FdoDevice,
                  IN PUSBPORT_PIPE_HANDLE PipeHandle)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSBPORT_ENDPOINT Endpoint;
    BOOLEAN IsReady;

    DPRINT("USBPORT_ClosePipe \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (!(PipeHandle->Flags & PIPEHANDLE_FLAG_CLOSED))
    {
        RemoveEntryList(&PipeHandle->PipeLink);

        PipeHandle->PipeLink.Flink = NULL;
        PipeHandle->PipeLink.Blink = NULL;

        PipeHandle->Flags |= PIPEHANDLE_FLAG_CLOSED;

        Endpoint = PipeHandle->Endpoint;

        if ((Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0) &&
            (Endpoint->EndpointProperties.TransferType == USBPORT_TRANSFER_TYPE_INTERRUPT))
        {
            PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)FdoExtension->RootHubPdo->DeviceExtension;
            PdoExtension->Endpoint = NULL;
        }

        while (TRUE)
        {
            IsReady = TRUE;

            if (!IsListEmpty(&Endpoint->PendingTransferList))
                IsReady = FALSE;

            if (!IsListEmpty(&Endpoint->TransferList))
                IsReady = FALSE;

            if (Endpoint->StateLast != Endpoint->StateNext)
                IsReady = FALSE;

            if (InterlockedIncrement(&Endpoint->LockCounter))
                IsReady = FALSE;

            InterlockedDecrement(&Endpoint->LockCounter);

            if (IsReady == TRUE)
                break;

            USBPORT_Wait(FdoDevice, 1);
        }

        Endpoint->DeviceHandle = NULL;

        USBPORT_SetEndpointState(Endpoint,
                                 USBPORT_ENDPOINT_CLOSED);

        USBPORT_SignalWorkerThread(FdoDevice);
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
NTAPI
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
        DPRINT1("USBPORT_OpenInterface: InterfaceInfo->AlternateSetting && IsSetInterface !\n");
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
                DPRINT1("USBPORT_OpenInterface: USBDStatus - %p\n", USBDStatus);
            }

            if (IsAllocated)
                ExFreePool(InterfaceHandle);
        }
    }

    return USBDStatus;
}

VOID
NTAPI
USBPORT_CloseConfiguration(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                           IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_CONFIGURATION_HANDLE ConfigHandle;
    PLIST_ENTRY iHandleList;
    PUSBPORT_INTERFACE_HANDLE iHandle;
    ULONG NumEndpoints;
    PUSBPORT_PIPE_HANDLE PipeHandle;

    DPRINT("USBPORT_CloseConfiguration: ... \n");

    ConfigHandle = DeviceHandle->ConfigHandle;

    if (ConfigHandle)
    {
        iHandleList = &ConfigHandle->InterfaceHandleList;

        while (!IsListEmpty(iHandleList))
        {
            iHandle = CONTAINING_RECORD(iHandleList->Flink,
                                        USBPORT_INTERFACE_HANDLE,
                                        InterfaceLink);

            DPRINT("USBPORT_CloseConfiguration: iHandle - %p\n", iHandle);

            RemoveHeadList(iHandleList);

            NumEndpoints = iHandle->InterfaceDescriptor.bNumEndpoints;

            if (NumEndpoints > 0)
            {
                PipeHandle = &iHandle->PipeHandle[0];

                do
                {
                    USBPORT_ClosePipe(DeviceHandle, FdoDevice, PipeHandle);
                    PipeHandle += 1;
                    --NumEndpoints;
                }
                while (NumEndpoints > 0);
            }

            ExFreePool(iHandle);
        }

        ExFreePool(ConfigHandle);
        DeviceHandle->ConfigHandle = NULL;
    }
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
NTAPI
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
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_HandleSelectConfiguration: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;
    ConfigDescriptor = Urb->UrbSelectConfiguration.ConfigurationDescriptor;

    DPRINT("USBPORT_SelectConfiguration: ConfigDescriptor %x\n",
           ConfigDescriptor);

    if (!ConfigDescriptor)
    {
        DPRINT1("USBPORT_SelectConfiguration: ConfigDescriptor == NULL!\n");
        goto Exit;
    }

    USBPORT_DumpingConfiguration(ConfigDescriptor);

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
        DPRINT1("USBPORT_SelectConfiguration: Status %x\n", Status);
    }

    KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    return Status;
}

VOID
NTAPI
USBPORT_AddDeviceHandle(IN PDEVICE_OBJECT FdoDevice,
                        IN PUSBPORT_DEVICE_HANDLE DeviceHandle)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_AddDeviceHandle: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)(FdoDevice->DeviceExtension);

    InsertTailList(&FdoExtension->DeviceHandleList,
                   &DeviceHandle->DeviceHandleLink);
}

VOID
NTAPI
USBPORT_RemoveDeviceHandle(IN PDEVICE_OBJECT FdoDevice,
                           IN PUSBPORT_DEVICE_HANDLE DeviceHandle)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;

    DPRINT("USBPORT_RemoveDeviceHandle \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->DeviceHandleSpinLock, &OldIrql);
    RemoveEntryList(&DeviceHandle->DeviceHandleLink);
    KeReleaseSpinLock(&FdoExtension->DeviceHandleSpinLock, OldIrql);
}

BOOLEAN
NTAPI
USBPORT_ValidateDeviceHandle(IN PDEVICE_OBJECT FdoDevice,
                             IN PUSBPORT_DEVICE_HANDLE DeviceHandle)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    KIRQL OldIrql;
    PLIST_ENTRY HandleList;
    PUSBPORT_DEVICE_HANDLE CurrentHandle;
    BOOLEAN Result = FALSE;

    DPRINT_URB("USBPORT_ValidateDeviceHandle: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeAcquireSpinLock(&FdoExtension->DeviceHandleSpinLock, &OldIrql);
    if (DeviceHandle)
    {
        HandleList = &FdoExtension->DeviceHandleList;

        if (!IsListEmpty(HandleList))
        {
            HandleList = HandleList->Flink;
        }

        while (HandleList != &FdoExtension->DeviceHandleList)
        {
            CurrentHandle = CONTAINING_RECORD(HandleList,
                                              USBPORT_DEVICE_HANDLE,
                                              DeviceHandleLink);

            if (CurrentHandle == DeviceHandle)
            {
                Result = TRUE;
                break;
            }

            HandleList = HandleList->Flink;
        }
    }
    KeReleaseSpinLock(&FdoExtension->DeviceHandleSpinLock, OldIrql);

    return Result;
}

VOID
NTAPI
USBPORT_AbortTransfers(IN PDEVICE_OBJECT FdoDevice,
                          IN PUSBPORT_DEVICE_HANDLE DeviceHandle)
{
    DPRINT1("USBPORT_AbortTransfers: UNIMPLEMENTED. FIXME. \n");
}

NTSTATUS
NTAPI
USBPORT_CreateDevice(IN OUT PUSB_DEVICE_HANDLE *pUsbdDeviceHandle,
                     IN PDEVICE_OBJECT FdoDevice,
                     IN PUSBPORT_DEVICE_HANDLE HubDeviceHandle,
                     IN USHORT PortStatus,
                     IN USHORT Port)
{
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    PUSBPORT_PIPE_HANDLE PipeHandle;
    BOOLEAN IsOpenedPipe;
    PVOID DeviceDescriptor;
    USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;
    SIZE_T TransferedLen;
    UCHAR MaxPacketSize;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    NTSTATUS Status;

    DPRINT("USBPORT_CreateDevice: PortStatus - %p, Port - %p\n",
           PortStatus,
           Port);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    if (!USBPORT_ValidateDeviceHandle(FdoDevice, HubDeviceHandle))
    {
        KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                           LOW_REALTIME_PRIORITY,
                           1,
                           FALSE);

        DPRINT1("USBPORT_CreateDevice: Not valid hub DeviceHandle\n");
        return STATUS_DEVICE_NOT_CONNECTED;
    }

    KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)ExAllocatePoolWithTag(NonPagedPool,
                                                                 sizeof(USBPORT_DEVICE_HANDLE),
                                                                 USB_PORT_TAG);

    if (!DeviceHandle)
        return STATUS_INSUFFICIENT_RESOURCES;

    RtlZeroMemory(DeviceHandle, sizeof(USBPORT_DEVICE_HANDLE));

    *pUsbdDeviceHandle = NULL;

    DeviceHandle->PortNumber = Port;
    DeviceHandle->HubDeviceHandle = HubDeviceHandle;

    if (PortStatus & USB_PORT_STATUS_LOW_SPEED)
        DeviceHandle->DeviceSpeed = 0;
    else
        DeviceHandle->DeviceSpeed = 1; // Fullspeed

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

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
        KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                           LOW_REALTIME_PRIORITY,
                           1,
                           FALSE);
        ExFreePool(DeviceHandle);
        return Status;
    }

    DeviceDescriptor = ExAllocatePoolWithTag(NonPagedPool,
                                             USB_DEFAULT_MAX_PACKET,
                                             USB_PORT_TAG);

    if (!DeviceDescriptor)
    {
        Status = STATUS_DEVICE_DATA_ERROR;

        if (IsOpenedPipe)
        {
            USBPORT_ClosePipe(DeviceHandle, FdoDevice, PipeHandle);
        }

        KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                           LOW_REALTIME_PRIORITY,
                           1,
                           FALSE);

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
                USBPORT_AddDeviceHandle(FdoDevice, DeviceHandle);
                *pUsbdDeviceHandle = (PUSB_DEVICE_HANDLE)DeviceHandle;

                KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                                   LOW_REALTIME_PRIORITY,
                                   1,
                                   FALSE);

                if (!NT_SUCCESS(Status))
                    ExFreePool(DeviceHandle);

                return Status;
            }
        }
    }

    DPRINT1("USBPORT_CreateDevice: ERROR!!! TransferedLen - %x, Status - %p\n",
            TransferedLen,
            Status);

    return Status;
}

ULONG
NTAPI
USBPORT_AllocateUsbAddress(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    ULONG BitMap;
    ULONG Bit;
    ULONG ix = 0;

    DPRINT("USBPORT_AllocateUsbAddress \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    while (TRUE)
    {
        BitMap = 1;
        Bit = 0;

        do
        {
            if (!(FdoExtension->UsbAddressBitMap[ix] & BitMap))
            {
                FdoExtension->UsbAddressBitMap[ix] |= BitMap;
                return 32 * ix + Bit;
            }

            BitMap *= 2;
            ++Bit;
        }
        while (Bit < 32);

        ++ix;

        if (ix < 4)
            continue;

        break;
    }

    return 0;
}

VOID
NTAPI
USBPORT_FreeUsbAddress(IN PDEVICE_OBJECT FdoDevice,
                       IN USHORT DeviceAddress)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    ULONG ix = 0;
    ULONG BitMap;
    ULONG Bit;
    USHORT CurrentAddress;

    DPRINT("USBPORT_FreeUsbAddress: DeviceAddress - %x\n", DeviceAddress);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    while (TRUE)
    {
        BitMap = 1;
        Bit = 0;
        CurrentAddress = 32 * ix;

        do
        {
            if (CurrentAddress == DeviceAddress)
            {
                BitMap = ~BitMap;
                FdoExtension->UsbAddressBitMap[ix] &= BitMap;
                return;
            }

            BitMap *= 2;

            ++Bit;
            ++CurrentAddress;
        }
        while (Bit < 32);

        ++ix;

        if (ix < 4)
            continue;

        break;
    }
}

NTSTATUS
NTAPI
USBPORT_InitializeDevice(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                         IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_ENDPOINT Endpoint;
    USB_DEFAULT_PIPE_SETUP_PACKET CtrlSetup;
    ULONG TransferedLen;
    USHORT DeviceAddress = 0;
    UCHAR MaxPacketSize;
    NTSTATUS Status;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_InitializeDevice: ... \n");

    ASSERT(DeviceHandle != NULL);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    DeviceAddress = USBPORT_AllocateUsbAddress(FdoDevice);
    ASSERT(DeviceHandle->DeviceAddress == USB_DEFAULT_DEVICE_ADDRESS);

    RtlZeroMemory(&CtrlSetup, sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

    CtrlSetup.bRequest = USB_REQUEST_SET_ADDRESS;
    CtrlSetup.wValue.W = DeviceAddress;

    Status = USBPORT_SendSetupPacket(DeviceHandle,
                                     FdoDevice,
                                     &CtrlSetup,
                                     NULL,
                                     0,
                                     NULL,
                                     NULL);

    DPRINT("USBPORT_InitializeDevice: DeviceAddress - %x. SendSetupPacket Status - %x\n",
           DeviceAddress,
           Status);

    if (!NT_SUCCESS(Status))
        goto ExitError;

    DeviceHandle->DeviceAddress = DeviceAddress;
    Endpoint = DeviceHandle->PipeHandle.Endpoint;

    Endpoint->EndpointProperties.MaxPacketSize = DeviceHandle->DeviceDescriptor.bMaxPacketSize0;
    Endpoint->EndpointProperties.DeviceAddress = DeviceAddress;

    Status = USBPORT_ReopenPipe(FdoDevice, Endpoint);

    if (!NT_SUCCESS(Status))
        goto ExitError;

    USBPORT_Wait(FdoDevice, 10);

    RtlZeroMemory(&CtrlSetup, sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

    CtrlSetup.bRequest = USB_REQUEST_GET_DESCRIPTOR;
    CtrlSetup.wValue.HiByte = USB_DEVICE_DESCRIPTOR_TYPE;
    CtrlSetup.wLength = sizeof(USB_DEVICE_DESCRIPTOR);
    CtrlSetup.bmRequestType.B = 0x80;

    Status = USBPORT_SendSetupPacket(DeviceHandle,
                                     FdoDevice,
                                     &CtrlSetup,
                                     &DeviceHandle->DeviceDescriptor,
                                     sizeof(USB_DEVICE_DESCRIPTOR),
                                     &TransferedLen,
                                     NULL);

    if (NT_SUCCESS(Status))
    {
        ASSERT(TransferedLen == sizeof(USB_DEVICE_DESCRIPTOR));
        ASSERT(DeviceHandle->DeviceDescriptor.bLength >= sizeof(USB_DEVICE_DESCRIPTOR));
        ASSERT(DeviceHandle->DeviceDescriptor.bDescriptorType == 1);

        MaxPacketSize = DeviceHandle->DeviceDescriptor.bMaxPacketSize0;

        ASSERT((MaxPacketSize == 8) ||
               (MaxPacketSize == 16) ||
               (MaxPacketSize == 32) ||
               (MaxPacketSize == 64));
    }
    else
    {
ExitError:
        DPRINT1("USBPORT_InitializeDevice: ExitError. Status - %x\n", Status);
    }

    KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    return Status;
}

NTSTATUS
NTAPI
USBPORT_GetUsbDescriptor(IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
                         IN PDEVICE_OBJECT FdoDevice,
                         IN UCHAR Type,
                         IN PUCHAR ConfigDesc,
                         IN PULONG ConfigDescSize)
{
    USB_DEFAULT_PIPE_SETUP_PACKET SetupPacket;

    DPRINT("USBPORT_GetUsbDescriptor: Type - %x\n");

    RtlZeroMemory(&SetupPacket, sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));

    SetupPacket.bmRequestType.B = 0x80;
    SetupPacket.bRequest = USB_REQUEST_GET_DESCRIPTOR;
    SetupPacket.wValue.HiByte = Type;
    SetupPacket.wLength = (USHORT)*ConfigDescSize;

    return USBPORT_SendSetupPacket(DeviceHandle,
                                   FdoDevice,
                                   &SetupPacket,
                                   ConfigDesc,
                                   *ConfigDescSize,
                                   ConfigDescSize,
                                   NULL);
}

PUSBPORT_INTERFACE_HANDLE
NTAPI
USBPORT_GetInterfaceHandle(IN PUSBPORT_CONFIGURATION_HANDLE ConfigurationHandle,
                           IN UCHAR InterfaceNumber)
{
    PUSBPORT_INTERFACE_HANDLE InterfaceHandle;
    PLIST_ENTRY iHandleList;
    UCHAR InterfaceNum;

    DPRINT("USBPORT_GetInterfaceHandle: ConfigurationHandle - %p, InterfaceNumber - %p\n",
           ConfigurationHandle,
           InterfaceNumber);

    if (!IsListEmpty(&ConfigurationHandle->InterfaceHandleList))
    {
        iHandleList = ConfigurationHandle->InterfaceHandleList.Flink;

        while (iHandleList &&
               (iHandleList != &ConfigurationHandle->InterfaceHandleList))
        {
            InterfaceHandle = CONTAINING_RECORD(iHandleList,
                                                USBPORT_INTERFACE_HANDLE,
                                                InterfaceLink);

            InterfaceNum = InterfaceHandle->InterfaceDescriptor.bInterfaceNumber;

            if (InterfaceNum == InterfaceNumber)
                return InterfaceHandle;

            iHandleList = InterfaceHandle->InterfaceLink.Flink;
        }
    }

    return NULL;
}

NTSTATUS
NTAPI
USBPORT_HandleSelectInterface(IN PDEVICE_OBJECT FdoDevice,
                              IN PIRP Irp,
                              IN PURB Urb)
{
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    PUSBPORT_CONFIGURATION_HANDLE ConfigurationHandle;
    PUSBD_INTERFACE_INFORMATION Interface;
    PUSBPORT_INTERFACE_HANDLE InterfaceHandle;
    PUSBPORT_INTERFACE_HANDLE iHandle;
    PUSBPORT_PIPE_HANDLE PipeHandle;
    USBD_STATUS USBDStatus;
    ULONG NumInterfaces;
    USHORT Length;
    ULONG ix;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_HandleSelectInterface: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    ConfigurationHandle = (PUSBPORT_CONFIGURATION_HANDLE)Urb->UrbSelectInterface.ConfigurationHandle;

    Interface = (PUSBD_INTERFACE_INFORMATION)&Urb->UrbSelectInterface.Interface;

    Length = Interface->Length + sizeof(USBD_PIPE_INFORMATION);

    if (Length != Urb->UrbHeader.Length)
        Urb->UrbHeader.Length = Length;

    USBDStatus = USBPORT_InitInterfaceInfo(Interface, ConfigurationHandle);

    if (USBDStatus)
    {
        Interface->InterfaceHandle = (USBD_INTERFACE_HANDLE)-1;
        return USBPORT_USBDStatusToNtStatus(Urb, USBDStatus);
    }

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;

    InterfaceHandle = USBPORT_GetInterfaceHandle(ConfigurationHandle,
                                                 Interface->InterfaceNumber);

    if (InterfaceHandle)
    {
        RemoveEntryList(&InterfaceHandle->InterfaceLink);


        if (InterfaceHandle->InterfaceDescriptor.bNumEndpoints)
        {
            PipeHandle = &InterfaceHandle->PipeHandle[0];

            ix = 0;

            do
            {
                USBPORT_ClosePipe(DeviceHandle, FdoDevice, PipeHandle);
                NumInterfaces = InterfaceHandle->InterfaceDescriptor.bNumEndpoints;
                ++ix;
                PipeHandle += 1;
            }
            while (ix < NumInterfaces);
        }
    }

    iHandle = 0;

    USBDStatus = USBPORT_OpenInterface(Urb,
                                       DeviceHandle,
                                       FdoDevice,
                                       ConfigurationHandle,
                                       Interface,
                                       &iHandle,
                                       1);

    if (USBDStatus)
    {
        Interface->InterfaceHandle = (USBD_INTERFACE_HANDLE)-1;
    }
    else
    {
        if (InterfaceHandle)
            ExFreePool(InterfaceHandle);

        Interface->InterfaceHandle = (USBD_INTERFACE_HANDLE)iHandle;

        InsertTailList(&ConfigurationHandle->InterfaceHandleList,
                       &iHandle->InterfaceLink);
    }

    KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    return USBPORT_USBDStatusToNtStatus(Urb, USBDStatus);
}

NTSTATUS
NTAPI
USBPORT_RemoveDevice(IN PDEVICE_OBJECT FdoDevice,
                     IN OUT PUSBPORT_DEVICE_HANDLE DeviceHandle,
                     IN ULONG Flags)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_RemoveDevice: DeviceHandle - %p, Flags - %x\n",
           DeviceHandle,
           Flags);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if ((Flags & USBD_KEEP_DEVICE_DATA) || (Flags & USBD_MARK_DEVICE_BUSY))
    {
        return STATUS_SUCCESS;
    }

    KeWaitForSingleObject(&FdoExtension->DeviceSemaphore,
                          Executive,
                          KernelMode,
                          FALSE,
                          NULL);

    if (!USBPORT_ValidateDeviceHandle(FdoDevice, DeviceHandle))
    {
        KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                           LOW_REALTIME_PRIORITY,
                           1,
                           FALSE);

        DPRINT1("USBPORT_RemoveDevice: Not valid device handle\n");
        return STATUS_DEVICE_NOT_CONNECTED;
    }

    USBPORT_RemoveDeviceHandle(FdoDevice, DeviceHandle);
    USBPORT_AbortTransfers(FdoDevice, DeviceHandle);

    DPRINT("USBPORT_RemoveDevice: DeviceHandleLock - %x\n", DeviceHandle->DeviceHandleLock);
    while (InterlockedDecrement(&DeviceHandle->DeviceHandleLock) >= 0)
    {
        InterlockedIncrement(&DeviceHandle->DeviceHandleLock);
        USBPORT_Wait(FdoDevice, 100);
    }
    DPRINT("USBPORT_RemoveDevice: DeviceHandleLock ok\n");

    if (DeviceHandle->ConfigHandle)
    {
        USBPORT_CloseConfiguration(DeviceHandle, FdoDevice);
    }

    USBPORT_ClosePipe(DeviceHandle, FdoDevice, &DeviceHandle->PipeHandle);

    if (DeviceHandle->DeviceAddress)
    {
        USBPORT_FreeUsbAddress(FdoDevice, DeviceHandle->DeviceAddress);
    }

    KeReleaseSemaphore(&FdoExtension->DeviceSemaphore,
                       LOW_REALTIME_PRIORITY,
                       1,
                       FALSE);

    if (!(DeviceHandle->Flags & DEVICE_HANDLE_FLAG_ROOTHUB))
    {
        ExFreePool(DeviceHandle);
    }

    return 0;
}
