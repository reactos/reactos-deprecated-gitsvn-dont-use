#include "usbport.h"

#define NDEBUG
#include <debug.h>

LIST_ENTRY USBPORT_MiniPortDrivers = {NULL, NULL};
KSPIN_LOCK USBPORT_SpinLock = 0;
BOOLEAN USBPORT_Initialized = FALSE;

NTSTATUS
USBPORT_USBDStatusToNtStatus(IN PURB Urb,
                             IN USBD_STATUS USBDStatus)
{
    NTSTATUS Status;

    DPRINT("USBPORT_USBDStatusToNtStatus: Urb - %p, USBDStatus - %x\n",
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

NTSTATUS
USBPORT_Wait(IN ULONG Milliseconds)
{
    LARGE_INTEGER Interval = {{0, 0}};

    DPRINT("USBPORT_Wait: Milliseconds - %x\n", Milliseconds);
    Interval.QuadPart -= 10000 * Milliseconds + (KeQueryTimeIncrement() - 1);
    return KeDelayExecutionThread(KernelMode, FALSE, &Interval);
}

VOID
NTAPI
USBPORT_WorkerThread(IN PVOID StartContext)
{
    PDEVICE_OBJECT FdoDevice;
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    LARGE_INTEGER OldTime = {{0, 0}};
    LARGE_INTEGER NewTime = {{0, 0}};
    PRH_INIT_CALLBACK RootHubInitCallback;
    PVOID RootHubInitContext;
    PUSBPORT_ENDPOINT Endpoint;
    PLIST_ENTRY workerList;
    KIRQL OldIrql;

    DPRINT("USBPORT_WorkerThread ... \n");

    FdoDevice = (PDEVICE_OBJECT)StartContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    FdoExtension->WorkerThread = KeGetCurrentThread();

    do
    {
        KeQuerySystemTime(&OldTime);

        KeWaitForSingleObject(&FdoExtension->WorkerThreadEvent,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);

        KeQuerySystemTime(&NewTime);

        KeResetEvent(&FdoExtension->WorkerThreadEvent);

        if (FdoExtension->MiniPortInterruptEnable & 1)
        {
            if (FdoExtension->Flags & USBPORT_FLAG_RH_INIT_CALLBACK)
            {
                PdoDevice = FdoExtension->RootHubPdo;

                if (PdoDevice)
                {
                    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

                    RootHubInitContext = PdoExtension->RootHubInitContext;
                    RootHubInitCallback = PdoExtension->RootHubInitCallback;

                    PdoExtension->RootHubInitCallback = NULL;
                    PdoExtension->RootHubInitContext = NULL;

                    if (RootHubInitCallback)
                        RootHubInitCallback(RootHubInitContext);
                }

                FdoExtension->Flags &= ~USBPORT_FLAG_RH_INIT_CALLBACK;
            }
        }

        while (TRUE)
        {
            KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);
            KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);

            workerList = &FdoExtension->WorkerList;

            if (IsListEmpty(workerList))
                break;

            Endpoint = CONTAINING_RECORD(workerList->Flink,
                                         USBPORT_ENDPOINT,
                                         WorkerLink);

            RemoveHeadList(workerList);
            Endpoint->WorkerLink.Blink = NULL;
            Endpoint->WorkerLink.Flink = NULL;

            KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

            USBPORT_EndpointWorker(Endpoint, FALSE);

            KeLowerIrql(OldIrql);
        }

        KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);
        KeLowerIrql(OldIrql);
    }
    while (!(FdoExtension->Flags & USBPORT_FLAG_WORKER_THREAD_ON));

    PsTerminateSystemThread(0);
}

NTSTATUS
NTAPI
USBPORT_CreateWorkerThread(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    NTSTATUS Status;

    DPRINT("USBPORT_CreateWorkerThread ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    FdoExtension->Flags &= ~USBPORT_FLAG_WORKER_THREAD_ON;

    KeInitializeEvent(&FdoExtension->WorkerThreadEvent,
                      NotificationEvent,
                      FALSE);

    Status = PsCreateSystemThread(&FdoExtension->WorkerThreadHandle,
                                  THREAD_ALL_ACCESS,
                                  NULL,
                                  NULL,
                                  NULL,
                                  USBPORT_WorkerThread,
                                  (PVOID)FdoDevice);

    return Status;
}

VOID
NTAPI
USBPORT_TimerDpc(IN PRKDPC Dpc,
                 IN PVOID DeferredContext,
                 IN PVOID SystemArgument1,
                 IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    LARGE_INTEGER DueTime = {{0, 0}};
    ULONG TimerFlags;
    ULONG TimeIncrement;

    DPRINT("USBPORT_TimerDpc: Dpc - %p, DeferredContext - %p, SystemArgument1 - %p, SystemArgument2 - %p\n",
           Dpc,
           DeferredContext,
           SystemArgument1,
           SystemArgument2);

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    PdoDevice = FdoExtension->RootHubPdo;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    TimerFlags = FdoExtension->TimerFlags;

    if (PdoDevice &&
        (PdoExtension->RootHubInitCallback != NULL) &&
        !(FdoExtension->Flags & USBPORT_FLAG_RH_INIT_CALLBACK))
    {
        FdoExtension->Flags |= USBPORT_FLAG_RH_INIT_CALLBACK;
        KeSetEvent(&FdoExtension->WorkerThreadEvent, 1, FALSE);
    }

    if (TimerFlags & 1)
    {
        TimeIncrement = KeQueryTimeIncrement();
        DueTime.QuadPart -= 10000 * FdoExtension->TimerValue + (TimeIncrement - 1);

        KeSetTimer(&FdoExtension->TimerObject,
                   DueTime,
                   &FdoExtension->TimerDpc);
    }

    DPRINT("USBPORT_TimerDpc: exit\n");
}

BOOLEAN
NTAPI
USBPORT_StartTimer(IN PDEVICE_OBJECT FdoDevice,
                   IN ULONG Time)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    LARGE_INTEGER DueTime = {{0, 0}};
    ULONG TimeIncrement;
    BOOLEAN Result;

    DPRINT("USBPORT_StartTimer: FdoDevice - %p, Time - %x\n",
           FdoDevice,
           Time);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    TimeIncrement = KeQueryTimeIncrement();

    FdoExtension->TimerFlags |= 1;
    FdoExtension->TimerValue = Time;

    KeInitializeTimer(&FdoExtension->TimerObject);
    KeInitializeDpc(&FdoExtension->TimerDpc, USBPORT_TimerDpc, FdoDevice);

    DueTime.QuadPart -= 10000 * Time + (TimeIncrement - 1);

    Result = KeSetTimer(&FdoExtension->TimerObject,
                        DueTime,
                        &FdoExtension->TimerDpc);

    return Result;
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

VOID
NTAPI
USBPORT_FreeCommonBuffer(IN PDEVICE_OBJECT FdoDevice,
                         IN PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDMA_ADAPTER DmaAdapter;

    DPRINT("USBPORT_FreeCommonBuffer: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DmaAdapter = FdoExtension->DmaAdapter;

    DmaAdapter->DmaOperations->FreeCommonBuffer(FdoExtension->DmaAdapter,
                                                HeaderBuffer->Length,
                                                HeaderBuffer->LogicalAddress,
                                                (PVOID)HeaderBuffer->VirtualAddress,
                                                TRUE);
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

    InitializeListHead(&FdoExtension->EndpointList);
    InitializeListHead(&FdoExtension->DoneTransferList);
    InitializeListHead(&FdoExtension->WorkerList);
    InitializeListHead(&FdoExtension->EpStateChangeList);
    InitializeListHead(&FdoExtension->MapTransferList);

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

ULONG
NTAPI
USBPORT_MiniportCompleteTransfer(IN PVOID MiniPortExtension,
                                 IN PVOID MiniPortEndpoint,
                                 IN PVOID TransferParameters,
                                 IN USBD_STATUS USBDStatus,
                                 IN ULONG TransferLength)
{
    PUSBPORT_TRANSFER Transfer;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_MiniportCompleteTransfer: USBDStatus - %p, TransferLength - %x\n",
           USBDStatus,
           TransferLength);

    Transfer = CONTAINING_RECORD(TransferParameters,
                                 USBPORT_TRANSFER,
                                 TransferParameters);

    FdoDevice = Transfer->Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Transfer->CompletedTransferLen = TransferLength;

    RemoveEntryList(&Transfer->TransferLink);

    Transfer->USBDStatus = USBDStatus;

    ExInterlockedInsertTailList(&FdoExtension->DoneTransferList,
                                &Transfer->TransferLink,
                                &FdoExtension->DoneTransferSpinLock);

    return KeInsertQueueDpc(&FdoExtension->TransferFlushDpc, NULL, NULL);
}

ULONG
NTAPI
USBPORT_GetMappedVirtualAddress(IN ULONG_PTR PhysicalAddress,
                                IN PVOID MiniPortExtension,
                                IN PVOID MiniPortEndpoint)
{
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
    PUSBPORT_ENDPOINT Endpoint;
    ULONG Offset; 
    ULONG_PTR VirtualAddress;

    DPRINT("USBPORT_GetMappedVirtualAddress ... \n");

    Endpoint = (PUSBPORT_ENDPOINT)((ULONG_PTR)MiniPortEndpoint -
                                   sizeof(USBPORT_ENDPOINT));

    if (!Endpoint)
    {
        ASSERT(FALSE);
    }

    HeaderBuffer = Endpoint->HeaderBuffer;

    Offset = PhysicalAddress - HeaderBuffer->PhysicalAddress;
    VirtualAddress = HeaderBuffer->VirtualAddress + Offset;

    return VirtualAddress;
}

VOID
NTAPI
USBPORT_InvalidateEndpoint(IN PVOID Context)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;

    DPRINT("USBPORT_InvalidateEndpoint: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)((ULONG_PTR)Context - 
                                               sizeof(USBPORT_DEVICE_EXTENSION));

    KeSetEvent(&FdoExtension->WorkerThreadEvent, 1, FALSE);
}

VOID
USBPORT_CompleteTransfer(IN PURB Urb,
                         IN USBD_STATUS TransferStatus)
{
    struct _URB_CONTROL_TRANSFER *UrbTransfer;
    PUSBPORT_TRANSFER Transfer;
    NTSTATUS Status;
    PIRP Irp;
    KIRQL OldIrql;
    PRKEVENT Event;
    BOOLEAN WriteToDevice;
    BOOLEAN IsFlushSuccess;
    PMDL Mdl;
    ULONG_PTR CurrentVa;
    SIZE_T TransferLength;
    PUSBPORT_ENDPOINT Endpoint;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_CompleteTransfer: Urb - %p, TransferStatus - %p\n",
           Urb,
           TransferStatus);

    UrbTransfer = &Urb->UrbControlTransfer;
    Transfer = (PUSBPORT_TRANSFER)UrbTransfer->hca.Reserved8[0];

    Transfer->USBDStatus = TransferStatus;
    Status = USBPORT_USBDStatusToNtStatus(Urb, TransferStatus);

    UrbTransfer->TransferBufferLength = Transfer->CompletedTransferLen;

    if (Transfer->Flags & TRANSFER_FLAG_DMA_MAPPED)
    {
        Endpoint = Transfer->Endpoint;
        FdoDevice = Endpoint->FdoDevice;
        FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

        WriteToDevice = Transfer->Direction == 2;
        Mdl = UrbTransfer->TransferBufferMDL;
        CurrentVa = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);
        TransferLength = UrbTransfer->TransferBufferLength;

        IsFlushSuccess = FdoExtension->DmaAdapter->DmaOperations->FlushAdapterBuffers(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                                      Mdl, // IN PMDL Mdl,
                                                                                      Transfer->MapRegisterBase, // IN PVOID MapRegisterBase,
                                                                                      (PVOID)CurrentVa, // IN PVOID CurrentVa,
                                                                                      TransferLength, // IN ULONG Length,
                                                                                      WriteToDevice); // IN BOOLEAN WriteToDevice

        if (!IsFlushSuccess)
        {
            DPRINT("USBPORT_CompleteTransfer: no FlushAdapterBuffers !!!\n");
            ASSERT(FALSE);
        }

        KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

        FdoExtension->DmaAdapter->DmaOperations->FreeMapRegisters(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                  Transfer->MapRegisterBase, // PVOID MapRegisterBase,
                                                                  Transfer->NumberOfMapRegisters); // ULONG NumberOfMapRegisters

        KeLowerIrql(OldIrql);
    }

    if (Urb->UrbHeader.UsbdFlags & USBD_FLAG_ALLOCATED_MDL)
    {
        IoFreeMdl(Transfer->TransferBufferMDL);
        Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_MDL;
    }

    Urb->UrbControlTransfer.hca.Reserved8[0] = NULL;
    Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_TRANSFER;

    Irp = Transfer->Irp;

    if (Irp)
    {
        Irp->IoStatus.Status = Status;
        Irp->IoStatus.Information = 0;

        KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        KeLowerIrql(OldIrql);
    }

    Event = Transfer->Event;

    if (Event)
        KeSetEvent(Event, 1, FALSE);

    ExFreePool(Transfer);

    DPRINT("USBPORT_CompleteTransfer: exit\n");
}

VOID
NTAPI
USBPORT_TransferFlushDpc(IN PRKDPC Dpc,
                         IN PVOID DeferredContext,
                         IN PVOID SystemArgument1,
                         IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY DoneTransferList;
    PUSBPORT_TRANSFER Transfer;
    PURB Urb;

    DPRINT("USBPORT_TransferFlushDpc: ... \n");

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    DoneTransferList = &FdoExtension->DoneTransferList;

    while (TRUE)
    {
        if (IsListEmpty(DoneTransferList))
            break;

        Transfer = CONTAINING_RECORD(DoneTransferList->Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(DoneTransferList);

        Urb = Transfer->Urb;
        USBPORT_USBDStatusToNtStatus(Transfer->Urb, Transfer->USBDStatus);

        USBPORT_CompleteTransfer(Urb, Urb->UrbHeader.Status);
    }
}

VOID
NTAPI
USBPORT_DmaEndpointWorker(PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_TRANSFER Transfer;
    ULONG OldState;
    ULONG NewState;
    ULONG CurrentState;
    PLIST_ENTRY List;
    ULONG Result;

    DPRINT("USBPORT_DmaEndpointWorker ... \n");

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    OldState = Endpoint->StateLast;

    if (OldState == USBPORT_ENDPOINT_ACTIVE)
    {
        CurrentState = Endpoint->StateLast;
        ASSERT(CurrentState == USBPORT_ENDPOINT_ACTIVE);

        List = Endpoint->TransferList.Flink;
        if (!IsListEmpty(&Endpoint->TransferList))
        {
            while (List && (List != &Endpoint->TransferList))
            {
                Transfer = CONTAINING_RECORD(List,
                                             USBPORT_TRANSFER,
                                             TransferLink);

                if (!(Transfer->Flags & TRANSFER_FLAG_SUBMITED))
                {
                    Result = FdoExtension->MiniPortInterface->Packet.SubmitTransfer(FdoExtension->MiniPortExt,
                                                                                    (PVOID)((ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT)),
                                                                                    &Transfer->TransferParameters,
                                                                                    Transfer->MiniportTransfer,
                                                                                    &Transfer->SgList);

                    if (Result)
                        goto Exit;

                    Transfer->Flags |= TRANSFER_FLAG_SUBMITED;
                    CurrentState = USBPORT_ENDPOINT_ACTIVE;
                }

                List = Transfer->TransferLink.Flink;
            }
        }

Exit:
        NewState = CurrentState;
    }

    if (NewState != OldState)
    {
        USBPORT_SetEndpointState(Endpoint, NewState);
    }

    DPRINT("USBPORT_DmaEndpointWorker exit \n");
}

VOID
USBPORT_EndpointWorker(IN PUSBPORT_ENDPOINT Endpoint,
                       IN BOOLEAN Flag)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;

    DPRINT("USBPORT_EndpointWorker: Endpoint - %p, Flag - %x\n",
           Endpoint,
           Flag);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (Flag == FALSE)
    {
        if (InterlockedIncrement(&Endpoint->LockCounter))
        {
            InterlockedDecrement(&Endpoint->LockCounter);
            DPRINT("USBPORT_EndpointWorker: LockCounter > 0\n");
            return;
        }
    }

    if (!(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
    {
        FdoExtension->MiniPortInterface->Packet.PollEndpoint(FdoExtension->MiniPortExt,
                                                             (ULONG_PTR)Endpoint + sizeof(USBPORT_ENDPOINT));
    }

    if (!IsListEmpty(&Endpoint->PendingTransferList) ||
        !IsListEmpty(&Endpoint->TransferList))
    {
        if (Endpoint->StateLast == Endpoint->StateNext)
        {
            if (Endpoint->EndpointWorker)
            {
                USBPORT_DmaEndpointWorker(Endpoint);
            }
            else
            {
                USBPORT_RootHubEndpointWorker(Endpoint);
            }

            InterlockedDecrement(&Endpoint->LockCounter);
            return;
        }
    }

    InterlockedDecrement(&Endpoint->LockCounter);
}

IO_ALLOCATION_ACTION
NTAPI
USBPORT_MapTransfer(IN PDEVICE_OBJECT FdoDevice,
                    IN PIRP Irp,
                    IN PVOID MapRegisterBase,
                    IN PVOID Context)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDMA_ADAPTER DmaAdapter;
    PUSBPORT_TRANSFER Transfer;
    PURB Urb;
    PUSBPORT_ENDPOINT Endpoint;
    PMDL Mdl;
    ULONG_PTR CurrentVa;
    PVOID MappedSystemVa;
    PUSBPORT_SCATTER_GATHER_LIST sgList;
    SIZE_T CurrentLength;
    ULONG ix;
    BOOLEAN WriteToDevice;
    PHYSICAL_ADDRESS PhAddr;
    PHYSICAL_ADDRESS PhAddress = {{0, 0}};
    SIZE_T TransferLength;
    SIZE_T SgCurrentLength;
    SIZE_T ElementLength;

    DPRINT("USBPORT_MapTransfer: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    DmaAdapter = FdoExtension->DmaAdapter;

    Transfer = (PUSBPORT_TRANSFER)Context;

    Urb = Transfer->Urb;
    Endpoint = Transfer->Endpoint;
    TransferLength = Transfer->TransferParameters.TransferBufferLength;

    Mdl = Urb->UrbControlTransfer.TransferBufferMDL;
    CurrentVa = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);
    Transfer->SgList.CurrentVa = CurrentVa;

    Mdl->MdlFlags |= MDL_MAPPING_CAN_FAIL;

    if (Mdl->MdlFlags & (MDL_SOURCE_IS_NONPAGED_POOL | MDL_MAPPED_TO_SYSTEM_VA))
        MappedSystemVa = Mdl->MappedSystemVa;
    else
        MappedSystemVa = MmMapLockedPages(Mdl, KernelMode);

    Transfer->SgList.MappedSystemVa = MappedSystemVa;

    Mdl->MdlFlags & ~MDL_MAPPING_CAN_FAIL;

    sgList = &Transfer->SgList;
    sgList->Flags = 0;

    Transfer->MapRegisterBase = MapRegisterBase;

    ix = 0;
    CurrentLength = 0;

    do
    {
        WriteToDevice = Transfer->Direction == 2;
        ASSERT(Transfer->Direction != 0);

        PhAddress = DmaAdapter->DmaOperations->MapTransfer(DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                           Mdl, // IN PMDL Mdl,
                                                           MapRegisterBase, // IN PVOID MapRegisterBase,
                                                           (PVOID)CurrentVa, // IN PVOID CurrentVa,
                                                           (PULONG)&TransferLength, // IN OUT PULONG Length,
                                                           WriteToDevice); // IN BOOLEAN WriteToDevice

        DPRINT("USBPORT_MapTransfer: PhAddress.LowPart - %p, PhAddress.HighPart - %x, TransferLength - %x\n",
               PhAddress.LowPart,
               PhAddress.HighPart,
               TransferLength);

        PhAddress.HighPart = 0;
        SgCurrentLength = TransferLength;

        do
        {
            ElementLength = 0x1000 - (PhAddress.LowPart & 0xFFF);

            if (ElementLength > SgCurrentLength)
                ElementLength = SgCurrentLength;

            DPRINT("USBPORT_MapTransfer: PhAddress.LowPart - %p, HighPart - %x, ElementLength - %x\n",
                   PhAddress.LowPart,
                   PhAddress.HighPart,
                   ElementLength);

            sgList->SgElement[ix].SgPhysicalAddress = PhAddress;
            sgList->SgElement[ix].SgTransferLength = ElementLength;
            sgList->SgElement[ix].SgOffset = CurrentLength + (TransferLength - SgCurrentLength);

            PhAddress.LowPart += ElementLength;
            SgCurrentLength -= ElementLength;

            ++ix;
        }
        while (SgCurrentLength);

        if ((PhAddr.LowPart == PhAddress.LowPart) &&
            (PhAddr.HighPart == PhAddress.HighPart))
        {
            ASSERT(FALSE);
        }

        PhAddr = PhAddress;

        CurrentLength += TransferLength;
        CurrentVa += TransferLength;

        TransferLength = Transfer->TransferParameters.TransferBufferLength - CurrentLength;
    }
    while (CurrentLength != Transfer->TransferParameters.TransferBufferLength);

    Transfer->SgList.SgElementCount = ix;
    Transfer->Flags |= TRANSFER_FLAG_DMA_MAPPED;

    ASSERT(Transfer->TransferParameters.TransferBufferLength <=
           Endpoint->EndpointProperties.MaxTransferSize);

    InsertTailList(&Endpoint->TransferList, &Transfer->TransferLink);

    return DeallocateObjectKeepRegisters;
}

VOID
USBPORT_FlushMapTransfers(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PLIST_ENTRY MapTransferList;
    PUSBPORT_TRANSFER Transfer;
    ULONG NumMapRegisters;
    PMDL Mdl;
    SIZE_T TransferBufferLength;
    ULONG_PTR VirtualAddr;
    KIRQL OldIrql;
    NTSTATUS Status;

    DPRINT("USBPORT_FlushMapTransfers: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    KeRaiseIrql(DISPATCH_LEVEL, &OldIrql);

    while (TRUE)
    {
        MapTransferList = &FdoExtension->MapTransferList;

        if (IsListEmpty(&FdoExtension->MapTransferList))
        {
            KeLowerIrql(OldIrql);
            return;
        }

        Transfer = CONTAINING_RECORD(MapTransferList->Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(MapTransferList);

        Mdl = Transfer->Urb->UrbControlTransfer.TransferBufferMDL;
        TransferBufferLength = Transfer->TransferParameters.TransferBufferLength;
        VirtualAddr = (ULONG_PTR)MmGetMdlVirtualAddress(Mdl);

        NumMapRegisters = ADDRESS_AND_SIZE_TO_SPAN_PAGES(VirtualAddr,
                                                         TransferBufferLength);

        Transfer->NumberOfMapRegisters = NumMapRegisters;

        Status = FdoExtension->DmaAdapter->DmaOperations->AllocateAdapterChannel(FdoExtension->DmaAdapter, // IN PDMA_ADAPTER DmaAdapter,
                                                                                 FdoDevice, // IN PDEVICE_OBJECT DeviceObject,
                                                                                 NumMapRegisters, // IN ULONG NumberOfMapRegisters,
                                                                                 (PDRIVER_CONTROL)USBPORT_MapTransfer, // IN PDRIVER_CONTROL ExecutionRoutine,
                                                                                 Transfer); // IN PVOID Context

        if (!NT_SUCCESS(Status))
            ASSERT(FALSE);
    }

    KeLowerIrql(OldIrql);
}

VOID
USBPORT_FlushPendingTransfers(IN PUSBPORT_ENDPOINT Endpoint)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN IsMapTransfer;
    BOOLEAN IsEnd = FALSE;
    PLIST_ENTRY List;
    PUSBPORT_TRANSFER Transfer;
    KIRQL PrevIrql;

    DPRINT("USBPORT_FlushPendingTransfers: Endpoint - %p\n", Endpoint);

    FdoDevice = Endpoint->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    while (TRUE)
    {
        IsMapTransfer = 0;

        if (IsListEmpty(&Endpoint->PendingTransferList) ||
            Endpoint->PendingTransferList.Flink == 0)
        {
            IsEnd = TRUE;
            goto Worker;
        }

        List = Endpoint->PendingTransferList.Flink;
        Transfer = CONTAINING_RECORD(List, USBPORT_TRANSFER, TransferLink);

        RemoveEntryList(&Transfer->TransferLink);
        Transfer->TransferLink.Flink = NULL;
        Transfer->TransferLink.Blink = NULL;

        if (Transfer->TransferParameters.TransferBufferLength == 0 ||
            !(Endpoint->Flags & ENDPOINT_FLAG_DMA_TYPE))
        {
            InsertTailList(&Endpoint->TransferList, &Transfer->TransferLink);
            IsMapTransfer = 0;
        }
        else
        {
            InsertTailList(&FdoExtension->MapTransferList,
                           &Transfer->TransferLink);

            IsMapTransfer = 1;
        }

        if (IsMapTransfer)
        {
            USBPORT_FlushMapTransfers(FdoDevice);

            if (IsEnd)
                return;
        }

Worker:
        KeRaiseIrql(DISPATCH_LEVEL, &PrevIrql);
        ASSERT(FALSE);
        KeLowerIrql(PrevIrql);

        if (IsEnd)
            return;
    }
}

VOID
USBPORT_QueueTransferUrb(IN PURB Urb)
{
    PUSBPORT_TRANSFER Transfer;
    PUSBPORT_ENDPOINT Endpoint;
    PIRP Irp;

    DPRINT("USBPORT_QueueTransferUrb: Urb - %p\n", Urb);

    if (Urb->UrbControlTransfer.TransferFlags & USBD_DEFAULT_PIPE_TRANSFER)
        Urb->UrbHeader.Function = URB_FUNCTION_CONTROL_TRANSFER;

    Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
    Endpoint = Transfer->Endpoint;

    Endpoint->Flags &= ~ENDPOINT_FLAG_QUEUENE_EMPTY;

    Transfer->TransferParameters.TransferBufferLength = Urb->UrbControlTransfer.TransferBufferLength;
    Transfer->TransferParameters.TransferFlags = Urb->UrbControlTransfer.TransferFlags;

    Transfer->TransferBufferMDL = Urb->UrbControlTransfer.TransferBufferMDL;
    Transfer->Direction = ((Urb->UrbControlTransfer.TransferFlags & 1) == 0) + 1;

    if (Endpoint->EndpointProperties.TransferType == USB_ENDPOINT_TYPE_CONTROL)
    {
        RtlCopyMemory(&Transfer->TransferParameters.SetupPacket,
                      Urb->UrbControlTransfer.SetupPacket,
                      sizeof(USB_DEFAULT_PIPE_SETUP_PACKET));
    }

    DPRINT("... URB TransferBufferLength - %x\n",
           Urb->UrbControlTransfer.TransferBufferLength);

    Urb->UrbControlTransfer.TransferBufferLength = 0;

    Irp = Transfer->Irp;

    if (Irp)
        Irp->IoStatus.Status = STATUS_PENDING;

    InsertTailList(&Endpoint->PendingTransferList, &Transfer->TransferLink);
    Urb->UrbHeader.Status = USBD_STATUS_PENDING;

    ASSERT(FALSE);

    DPRINT("... URB TransferBufferLength - %x\n",
           Urb->UrbControlTransfer.TransferBufferLength);

    if (Urb->UrbControlTransfer.TransferBufferLength)
    {
        ULONG TransferBuffer;

        TransferBuffer = (ULONG)Urb->UrbControlTransfer.TransferBuffer;
        DPRINT("URB TransferBuffer - %p\n", TransferBuffer);
        DPRINT("*TransferBuffer    - %p\n", *(PULONG)TransferBuffer);
        DPRINT("*TransferBuffer+1  - %p\n", *(PULONG)(TransferBuffer+4));
        DPRINT("*TransferBuffer+2  - %p\n", *(PULONG)(TransferBuffer+8));
        DPRINT("*TransferBuffer+3  - %p\n", *(PULONG)(TransferBuffer+12));
    }
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
        Urb->UrbHeader.UsbdFlags |= USBD_FLAG_ALLOCATED_MDL;

        DPRINT("ValidateTransferParameters: Mdl - %p\n", Mdl);
    }

    return STATUS_SUCCESS;
}

USBD_STATUS
USBPORT_AllocateTransfer(IN PDEVICE_OBJECT FdoDevice,
                         IN PURB Urb,
                         IN PUSBPORT_DEVICE_HANDLE UsbdDeviceHandle,
                         IN PIRP Irp,
                         IN PRKEVENT Event)
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

        Urb->UrbControlTransfer.hca.Reserved8[0] = Transfer;
        Urb->UrbHeader.UsbdFlags |= USBD_FLAG_ALLOCATED_TRANSFER;
        USBDStatus = USBD_STATUS_SUCCESS;
    }
    else
    {
        USBDStatus = USBD_STATUS_INSUFFICIENT_RESOURCES;
    }

    DPRINT("USBPORT_AllocateTransfer: return USBDStatus - %x\n", USBDStatus);
    return USBDStatus;
}

NTSTATUS
USBPORT_HandleDataTransfers(IN PURB Urb)
{
    PUSBPORT_ENDPOINT Endpoint;

    DPRINT("USBPORT_HandleDataTransfers: Urb - %p\n", Urb);

    Endpoint = ((PUSBPORT_PIPE_HANDLE)(Urb->UrbBulkOrInterruptTransfer.PipeHandle))->Endpoint;

    if (Endpoint->EndpointProperties.TransferType != USB_ENDPOINT_TYPE_CONTROL)
    {
        if (Endpoint->EndpointProperties.Direction)
            Urb->UrbBulkOrInterruptTransfer.TransferFlags &= ~USBD_TRANSFER_DIRECTION_IN; // ~1
        else
            Urb->UrbBulkOrInterruptTransfer.TransferFlags |= USBD_TRANSFER_DIRECTION_IN; // 1
    }

    USBPORT_QueueTransferUrb(Urb);

    return STATUS_PENDING;
}

NTSTATUS
USBPORT_HandleGetStatus(IN PIRP Irp,
                        IN PURB Urb)
{
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket = (PUSB_DEFAULT_PIPE_SETUP_PACKET)&Urb->UrbControlDescriptorRequest.Reserved1;
    NTSTATUS Status;

    SetupPacket->bmRequestType.B = 0;
    SetupPacket->bmRequestType._BM.Dir = BMREQUEST_DEVICE_TO_HOST;
    SetupPacket->bRequest = USB_REQUEST_GET_STATUS;
    SetupPacket->wLength = Urb->UrbControlDescriptorRequest.TransferBufferLength;
    SetupPacket->wValue.W = 0;

    switch (Urb->UrbHeader.Function)
    {
        case URB_FUNCTION_GET_STATUS_FROM_DEVICE: // 0x13
            DPRINT("USBPORT_HandleGetStatus: URB_FUNCTION_GET_STATUS_FROM_DEVICE\n");
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
            break;

        case URB_FUNCTION_GET_STATUS_FROM_INTERFACE: // 0x14
            DPRINT("USBPORT_HandleGetStatus: URB_FUNCTION_GET_STATUS_FROM_INTERFACE\n");
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_INTERFACE;
            break;

        case URB_FUNCTION_GET_STATUS_FROM_ENDPOINT: // 0x15
            DPRINT("USBPORT_HandleGetStatus: URB_FUNCTION_GET_STATUS_FROM_ENDPOINT\n");
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_ENDPOINT;
            break;

        case URB_FUNCTION_GET_STATUS_FROM_OTHER: // 0x21
            DPRINT("USBPORT_HandleGetStatus: URB_FUNCTION_GET_STATUS_FROM_OTHER\n");
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_OTHER;
            break;
    }

    if (SetupPacket->wLength == 2)
    {
        Urb->UrbControlTransfer.TransferFlags |= USBD_SHORT_TRANSFER_OK; // 2

        if (SetupPacket->bmRequestType._BM.Dir)
            Urb->UrbControlTransfer.TransferFlags |= USBD_TRANSFER_DIRECTION_IN; // 1;
        else
            Urb->UrbControlTransfer.TransferFlags &= ~USBD_TRANSFER_DIRECTION_IN; // ~1;

        DPRINT("GetStatus: SetupPacket->bmRequestType.B - %x\n",
               SetupPacket->bmRequestType.B);

        DPRINT("GetStatus: SetupPacket->bRequest        - %x\n",
               SetupPacket->bRequest);

        DPRINT("GetStatus: SetupPacket->wIndex.W        - %x\n",
               SetupPacket->wIndex.W);

        DPRINT("GetStatus: SetupPacket->wLength         - %x\n",
               SetupPacket->wLength);

        USBPORT_QueueTransferUrb(Urb);

        Status = STATUS_PENDING;
    }
    else
    {
        Status = USBPORT_USBDStatusToNtStatus(Urb,
                                              USBD_STATUS_INVALID_PARAMETER);

        DPRINT1("USBPORT_HandleGetStatus: Bad wLength\n");
    }

    return Status;
}

NTSTATUS
USBPORT_HandleVendorOrClass(IN PIRP Irp,
                            IN PURB Urb)
{
    PUSB_DEFAULT_PIPE_SETUP_PACKET SetupPacket = (PUSB_DEFAULT_PIPE_SETUP_PACKET)&Urb->UrbControlDescriptorRequest.Reserved1;

    // Specifies a value, from 4 to 31 inclusive, that becomes part of the request type code in the USB-defined setup packet.
    // This value is defined by USB for a class request or the vendor for a vendor request. 

    SetupPacket->bmRequestType._BM.Dir = Urb->UrbControlTransfer.TransferFlags & 1;
    SetupPacket->wLength = Urb->UrbControlDescriptorRequest.TransferBufferLength;

    Urb->UrbControlTransfer.TransferFlags |= USBD_SHORT_TRANSFER_OK; // 2

    switch (Urb->UrbHeader.Function)
    {
        case URB_FUNCTION_VENDOR_DEVICE: // 0x17
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_VENDOR_DEVICE\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_VENDOR;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
            break;

        case URB_FUNCTION_VENDOR_INTERFACE: // 0x18
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_VENDOR_INTERFACE\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_VENDOR;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_INTERFACE;
            break;

        case URB_FUNCTION_VENDOR_ENDPOINT: // 0x19
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_VENDOR_ENDPOINT\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_VENDOR;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_ENDPOINT;
            break;

        case URB_FUNCTION_CLASS_DEVICE: // 0x1A
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_CLASS_DEVICE\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_CLASS;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_DEVICE;
            break;

        case URB_FUNCTION_CLASS_INTERFACE: // 0x1B
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_CLASS_INTERFACE\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_CLASS;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_INTERFACE;
            break;

        case URB_FUNCTION_CLASS_ENDPOINT: // 0x1C
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_CLASS_ENDPOINT\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_CLASS;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_ENDPOINT;
            break;

        case URB_FUNCTION_CLASS_OTHER: // 0x1F
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_CLASS_OTHER\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_CLASS;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_OTHER;
            break;

        case URB_FUNCTION_VENDOR_OTHER: // 0x20
            DPRINT("USBPORT_HandleVendorOrClass: URB_FUNCTION_VENDOR_OTHER\n");
            SetupPacket->bmRequestType._BM.Type = BMREQUEST_VENDOR;
            SetupPacket->bmRequestType._BM.Recipient = BMREQUEST_TO_OTHER;
            break;
    }

    DPRINT("VendorOrClass: bmRequestType.B - %x\n", SetupPacket->bmRequestType.B);
    DPRINT("VendorOrClass: bRequest        - %x\n", SetupPacket->bRequest);
    DPRINT("VendorOrClass: wValue.LowByte  - %x\n", SetupPacket->wValue.LowByte);
    DPRINT("VendorOrClass: wValue.HiByte   - %x\n", SetupPacket->wValue.HiByte);
    DPRINT("VendorOrClass: wIndex.W        - %x\n", SetupPacket->wIndex.W);
    DPRINT("VendorOrClass: wLength         - %x\n", SetupPacket->wLength);

    USBPORT_QueueTransferUrb(Urb);

    return STATUS_PENDING;
}

NTSTATUS
USBPORT_HandleGetSetDescriptor(IN PIRP Irp,
                               IN PURB Urb)
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

    DPRINT("GetSetDescriptor: SetupPacket->bmRequestType.B - %x\n",
           SetupPacket->bmRequestType.B); // 0x80

    DPRINT("GetSetDescriptor: SetupPacket->bRequest        - %x\n",
           SetupPacket->bRequest); // USB_REQUEST_GET_DESCRIPTOR

    DPRINT("GetSetDescriptor: SetupPacket->wValue.LowByte  - %x\n",
           SetupPacket->wValue.LowByte); // Urb->UrbControlDescriptorRequest.Index;

    DPRINT("GetSetDescriptor: SetupPacket->wValue.HiByte   - %x\n",
           SetupPacket->wValue.HiByte); // Urb->UrbControlDescriptorRequest.DescriptorType;

    DPRINT("GetSetDescriptor: SetupPacket->wIndex.W        - %x\n",
           SetupPacket->wIndex.W); // Urb->UrbControlDescriptorRequest.LanguageId;

    DPRINT("GetSetDescriptor: SetupPacket->wLength         - %x\n",
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
            //DPRINT("USBPORT_PdoScsi: UsbdDeviceHandle == 0\n");
            Urb->UrbHeader.UsbdDeviceHandle = &PdoExtension->DeviceHandle;
            UsbdDeviceHandle = &PdoExtension->DeviceHandle;
        }

        DPRINT("USBPORT_PdoScsi: IOCTL_INTERNAL_USB_SUBMIT_URB. Function - 0x%02X, UsbdDeviceHandle - %p\n",
               Function,
               Urb->UrbHeader.UsbdDeviceHandle);

        switch (Function)
        {
            case URB_FUNCTION_ISOCH_TRANSFER: // 0x10
                DPRINT("USBPORT_PdoScsi: URB_FUNCTION_ISOCH_TRANSFER\n");
                ASSERT(FALSE);
                break;

            case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER: // 0x09
            case URB_FUNCTION_CONTROL_TRANSFER: // 0x08
                Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer

                if ((Urb->UrbControlTransfer.TransferFlags & USBD_DEFAULT_PIPE_TRANSFER) &&
                    (Function == URB_FUNCTION_CONTROL_TRANSFER))
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

                Status = USBPORT_HandleDataTransfers(Urb);
                break;

            case URB_FUNCTION_VENDOR_DEVICE: // 0x17
            case URB_FUNCTION_VENDOR_INTERFACE: // 0x18
            case URB_FUNCTION_VENDOR_ENDPOINT: // 0x19
            case URB_FUNCTION_CLASS_DEVICE: // 0x1A
            case URB_FUNCTION_CLASS_INTERFACE: // 0x1B
            case URB_FUNCTION_CLASS_ENDPOINT: // 0x1C
            case URB_FUNCTION_CLASS_OTHER: // 0x1F
            case URB_FUNCTION_VENDOR_OTHER: // 0x20
                Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
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

                Status = USBPORT_HandleVendorOrClass(Irp, Urb);
                break;

            case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE: // 0x0B
            case URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE: // 0x0C
            case URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT: // 0x24
            case URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT: // 0x25
            case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE: // 0x28
            case URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE: // 0x29
                Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
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
                Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
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

                Status = USBPORT_HandleGetStatus(Irp, Urb);
                break;

            case URB_FUNCTION_SELECT_CONFIGURATION: // 0x00
                Status = USBPORT_HandleSelectConfiguration(PdoExtension->FdoDevice,
                                                           Irp,
                                                           Urb);
                break;

            case URB_FUNCTION_SELECT_INTERFACE: // 0x01
                Status = USBPORT_HandleSelectInterface(PdoExtension->FdoDevice,
                                                       Irp,
                                                       Urb);
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
            if (Urb->UrbHeader.UsbdFlags & USBD_FLAG_ALLOCATED_TRANSFER)
            {
                PUSBPORT_TRANSFER Transfer;

                Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
                Urb->UrbControlTransfer.hca.Reserved8[0] = NULL;
                Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_TRANSFER;
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

    DPRINT("USBPORT_RegisterUSBPortDriver: sizeof(USBPORT_DEVICE_EXTENSION)   - %x\n",
           sizeof(USBPORT_DEVICE_EXTENSION));

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

    RegPacket->UsbPortInvalidateRootHub = USBPORT_InvalidateRootHub;
    RegPacket->UsbPortInvalidateEndpoint = USBPORT_InvalidateEndpoint;
    RegPacket->UsbPortCompleteTransfer = USBPORT_MiniportCompleteTransfer;
    RegPacket->UsbPortGetMappedVirtualAddress = USBPORT_GetMappedVirtualAddress;

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
