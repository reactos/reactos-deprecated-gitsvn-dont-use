#include "usbport.h"

//#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_CORE
#define NDEBUG_USBPORT_INTERRUPT
#include "usbdebug.h"

NTSTATUS
NTAPI
USBPORT_FdoStartCompletion(IN PDEVICE_OBJECT DeviceObject,
                           IN PIRP Irp,
                           PRKEVENT Event)
{
    KeSetEvent(Event, EVENT_INCREMENT, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}

BOOLEAN
NTAPI
USBPORT_InterruptService(IN PKINTERRUPT Interrupt,
                         IN PVOID ServiceContext)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN Result = 0;

    FdoDevice = (PDEVICE_OBJECT)ServiceContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    DPRINT_INT("USBPORT_InterruptService: FdoExtension->Flags - %p\n",
           FdoExtension->Flags);

    if (FdoExtension->Flags & USBPORT_FLAG_INTERRUPT_ENABLED)
    {
        if (FdoExtension->MiniPortFlags & 1)
        {
            Result = FdoExtension->MiniPortInterface->Packet.InterruptService(FdoExtension->MiniPortExt);

            if (Result)
            {
                KeInsertQueueDpc(&FdoExtension->IsrDpc, NULL, NULL);
            }
        }
    }
    else
    {
        Result = 0;
    }

    DPRINT_INT("USBPORT_InterruptService: return - %x\n", Result);

    return Result;
}

VOID
NTAPI
USBPORT_IsrDpcHandler(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_ENDPOINT Endpoint;
    PLIST_ENTRY List;
    PLIST_ENTRY EndpointList;
    PLIST_ENTRY DoneTransferList;
    LIST_ENTRY DispatchList;
    PUSBPORT_TRANSFER Transfer;
    PURB Urb;

    DPRINT_CORE("USBPORT_IsrDpcHandler: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    InitializeListHead(&DispatchList);
    InterlockedIncrement(&FdoExtension->IsrDpcHandlerCounter);

    for (List = ExInterlockedRemoveHeadList(&FdoExtension->EpStateChangeList,
                                            &FdoExtension->EpStateChangeSpinLock);
         List != NULL;
         List = ExInterlockedRemoveHeadList(&FdoExtension->EpStateChangeList,
                                             &FdoExtension->EpStateChangeSpinLock))
    {
        Endpoint = CONTAINING_RECORD(List, USBPORT_ENDPOINT, StateChangeLink);
        Endpoint->StateLast = Endpoint->StateNext;

        if (!Endpoint->WorkerLink.Flink || !Endpoint->WorkerLink.Blink)
        {
            InsertTailList(&FdoExtension->WorkerList, &Endpoint->WorkerLink);
        }

        KeSetEvent(&FdoExtension->WorkerThreadEvent, 1, FALSE);
    }

    DoneTransferList = &FdoExtension->DoneTransferList;

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);
    EndpointList = FdoExtension->EndpointList.Flink;

    if (!IsListEmpty(&FdoExtension->EndpointList))
    {
        while (EndpointList && EndpointList != &FdoExtension->EndpointList)
        {
            Endpoint = CONTAINING_RECORD(EndpointList,
                                         USBPORT_ENDPOINT,
                                         EndpointLink);

            if (Endpoint->StateLast == USBPORT_ENDPOINT_ACTIVE &&
                !InterlockedIncrement(&Endpoint->LockCounter) &&
                !(Endpoint->Flags & ENDPOINT_FLAG_ROOTHUB_EP0))
            {
                InsertTailList(&DispatchList, &Endpoint->DispatchLink);
            }
            else
            {
                InterlockedDecrement(&Endpoint->LockCounter);
            }

            EndpointList = Endpoint->EndpointLink.Flink;
        }
    }

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

    while (!IsListEmpty(&DispatchList))
    {
        Endpoint = CONTAINING_RECORD(DispatchList.Flink,
                                     USBPORT_ENDPOINT,
                                     DispatchLink);

        RemoveEntryList(DispatchList.Flink);
        Endpoint->DispatchLink.Flink = NULL;
        Endpoint->DispatchLink.Blink = NULL;

        USBPORT_EndpointWorker(Endpoint, TRUE);
        USBPORT_FlushPendingTransfers(Endpoint);
    }

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->EndpointListSpinLock);
    if (!IsListEmpty(&FdoExtension->WorkerList))
    {
        KeSetEvent(&FdoExtension->WorkerThreadEvent, 1, FALSE);
    }
    KeReleaseSpinLockFromDpcLevel(&FdoExtension->EndpointListSpinLock);

    while (TRUE)
    {
        if (IsListEmpty(DoneTransferList))
            break;

        Transfer = CONTAINING_RECORD(DoneTransferList->Flink,
                                     USBPORT_TRANSFER,
                                     TransferLink);

        RemoveHeadList(DoneTransferList);

        Urb = Transfer->Urb;
        Urb->UrbHeader.Status = Transfer->USBDStatus;
        USBPORT_CompleteTransfer(Urb, Urb->UrbHeader.Status);
    }

    InterlockedDecrement(&FdoExtension->IsrDpcHandlerCounter);
}

VOID
NTAPI
USBPORT_IsrDpc(IN PRKDPC Dpc,
               IN PVOID DeferredContext,
               IN PVOID SystemArgument1,
               IN PVOID SystemArgument2)
{
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    BOOLEAN InterruptEnable;

    DPRINT_INT("USBPORT_IsrDpc: ... \n");

    FdoDevice = (PDEVICE_OBJECT)DeferredContext;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (SystemArgument2)
    {
        InterlockedDecrement(&FdoExtension->IsrDpcCounter);
    }

    KeAcquireSpinLockAtDpcLevel(&FdoExtension->MiniportInterruptsSpinLock);
    InterruptEnable = (FdoExtension->Flags & USBPORT_FLAG_INTERRUPT_ENABLED) == USBPORT_FLAG_INTERRUPT_ENABLED;

    FdoExtension->MiniPortInterface->Packet.InterruptDpc(FdoExtension->MiniPortExt,
                                                         InterruptEnable);

    KeReleaseSpinLockFromDpcLevel(&FdoExtension->MiniportInterruptsSpinLock);

    USBPORT_IsrDpcHandler(FdoDevice);

    DPRINT_INT("USBPORT_IsrDpc: exit\n");
}

NTSTATUS
NTAPI
USBPORT_RegisterDeviceInterface(IN PDEVICE_OBJECT PdoDevice,
                                IN PDEVICE_OBJECT DeviceObject,
                                IN CONST GUID *InterfaceClassGuid,
                                IN BOOLEAN Enable)
{
    PUSBPORT_RHDEVICE_EXTENSION DeviceExtension;
    PUNICODE_STRING SymbolicLinkName;
    NTSTATUS Status;

    DPRINT("USBPORT_RegisterDeviceInterface: Enable - %x\n", Enable);

    DeviceExtension = (PUSBPORT_RHDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    SymbolicLinkName = &DeviceExtension->CommonExtension.SymbolicLinkName;

    if (Enable)
    {
        Status = IoRegisterDeviceInterface(PdoDevice,
                                           InterfaceClassGuid,
                                           NULL,
                                           SymbolicLinkName);

        if (NT_SUCCESS(Status))
        {
            Status = IoSetDeviceInterfaceState(SymbolicLinkName, TRUE);

            if (NT_SUCCESS(Status))
            {
                DeviceExtension->CommonExtension.IsInterfaceEnabled = 1;

                DPRINT("USBPORT_RegisterDeviceInterface: LinkName  - %wZ\n",
                       &DeviceExtension->CommonExtension.SymbolicLinkName);
            }
        }
    }
    else if (DeviceExtension->CommonExtension.IsInterfaceEnabled)
    {
        // Disable device interface
        Status = IoSetDeviceInterfaceState(SymbolicLinkName, FALSE);

        if (NT_SUCCESS(Status))
            RtlFreeUnicodeString(SymbolicLinkName);

        DeviceExtension->CommonExtension.IsInterfaceEnabled = 0; // Disabled interface
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_QueryPciBusInterface(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PBUS_INTERFACE_STANDARD BusInterface;
    PIO_STACK_LOCATION IoStack;
    IO_STATUS_BLOCK IoStatusBlock;
    PDEVICE_OBJECT HighestDevice;
    KEVENT Event;
    PIRP Irp;
    NTSTATUS Status;

    DPRINT("USBPORT_QueryPciBusInterface: ...  \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    BusInterface = &FdoExtension->BusInterface;

    RtlZeroMemory(BusInterface, sizeof(BUS_INTERFACE_STANDARD));
    KeInitializeEvent(&Event, SynchronizationEvent, FALSE);
    HighestDevice = IoGetAttachedDeviceReference(FdoDevice);

    Irp = IoBuildSynchronousFsdRequest(IRP_MJ_PNP,
                                       HighestDevice,
                                       NULL,
                                       0,
                                       NULL,
                                       &Event,
                                       &IoStatusBlock);

    if (Irp)
    {
        IoStack = IoGetNextIrpStackLocation(Irp);

        Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
        Irp->IoStatus.Information = 0;

        IoStack->MinorFunction = IRP_MN_QUERY_INTERFACE;

        IoStack->Parameters.QueryInterface.InterfaceType = &GUID_BUS_INTERFACE_STANDARD;
        IoStack->Parameters.QueryInterface.Size = sizeof(BUS_INTERFACE_STANDARD);
        IoStack->Parameters.QueryInterface.Version = 1;
        IoStack->Parameters.QueryInterface.Interface = (PINTERFACE)BusInterface;
        IoStack->Parameters.QueryInterface.InterfaceSpecificData = 0;

        Status = IoCallDriver(HighestDevice, Irp);

        if (Status == STATUS_PENDING)
        {
            KeWaitForSingleObject(&Event, Executive, KernelMode, FALSE, NULL);
            Status = IoStatusBlock.Status;
        }
    }
    else
    {
        Status = STATUS_INSUFFICIENT_RESOURCES;
    }

    ObDereferenceObject(HighestDevice);

    DPRINT("USBPORT_QueryPciBusInterface: return Status - %x\n", Status);

    return Status;
}

NTSTATUS
NTAPI
USBPORT_QueryCapabilities(IN PDEVICE_OBJECT FdoDevice,
                          IN PDEVICE_CAPABILITIES Capabilities)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention;
    PIRP Irp;
    NTSTATUS Status;
    PIO_STACK_LOCATION IoStack;
    KEVENT Event;

    DPRINT("USBPORT_QueryCapabilities: ... \n");

    FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    RtlZeroMemory(Capabilities, sizeof(DEVICE_CAPABILITIES));

    Capabilities->Size     = 64;
    Capabilities->Version  = 1;
    Capabilities->Address  = -1;
    Capabilities->UINumber = -1;

    Irp = IoAllocateIrp(FdoExtention->CommonExtension.LowerDevice->StackSize, FALSE);

    if (!Irp)
    {
        DPRINT1("USBPORT_QueryCapabilities: No resources - IoAllocateIrp!\n");
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

    IoStack = IoGetNextIrpStackLocation(Irp);
    IoStack->MajorFunction = IRP_MJ_PNP;
    IoStack->MinorFunction = IRP_MN_QUERY_CAPABILITIES;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    IoSetCompletionRoutine(Irp,
                          USBPORT_FdoStartCompletion,
                          &Event,
                          TRUE,
                          TRUE,
                          TRUE);

    IoStack->Parameters.DeviceCapabilities.Capabilities = Capabilities;

    Status = IoCallDriver(FdoExtention->CommonExtension.LowerDevice, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event, Suspended, KernelMode, FALSE, NULL);
        Status = Irp->IoStatus.Status;
    }

    if (NT_SUCCESS(Status) && Capabilities)
    {
        USBPORT_DumpingCapabilities(Capabilities);
    }

    IoFreeIrp(Irp);

    return Status;
}

NTSTATUS
NTAPI
USBPORT_StartDevice(IN PDEVICE_OBJECT FdoDevice,
                    IN PUSBPORT_RESOURCES UsbPortResources)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    NTSTATUS Status;
    PCI_COMMON_CONFIG PciConfig;
    ULONG BytesRead;
    DEVICE_DESCRIPTION DeviceDescription;
    PDMA_ADAPTER DmaAdapter = NULL;
    ULONG MiniPortStatus;
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
    ULONG ResultLength;

    DPRINT("USBPORT_StartDevice: FdoDevice - %p, UsbPortResources - %p\n",
           FdoDevice,
           UsbPortResources);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Status = USBPORT_QueryPciBusInterface(FdoDevice);

    BytesRead = (*FdoExtension->BusInterface.GetBusData)(FdoExtension->BusInterface.Context,
                                                         PCI_WHICHSPACE_CONFIG,
                                                         &PciConfig,
                                                         0,
                                                         PCI_COMMON_HDR_LENGTH);

    if (BytesRead != PCI_COMMON_HDR_LENGTH)
    {
        DPRINT1("USBPORT_StartDevice: Failed to get pci config information!\n");
        goto ExitWithError;
    }

    FdoExtension->VendorID = PciConfig.VendorID;
    FdoExtension->DeviceID = PciConfig.DeviceID;
    FdoExtension->RevisionID = PciConfig.RevisionID;
    FdoExtension->ProgIf = PciConfig.ProgIf;
    FdoExtension->SubClass = PciConfig.SubClass;
    FdoExtension->BaseClass = PciConfig.BaseClass;

    RtlZeroMemory(&DeviceDescription, sizeof(DeviceDescription));

    DeviceDescription.Version = DEVICE_DESCRIPTION_VERSION;
    DeviceDescription.Master = TRUE;
    DeviceDescription.ScatterGather = TRUE;
    DeviceDescription.Dma32BitAddresses = TRUE;
    DeviceDescription.InterfaceType = PCIBus;
    DeviceDescription.DmaWidth = Width32Bits;
    DeviceDescription.DmaSpeed = Compatible;
    DeviceDescription.MaximumLength = MAXULONG;

    DmaAdapter = IoGetDmaAdapter(FdoExtension->CommonExtension.LowerPdoDevice,
                                 &DeviceDescription,
                                 &FdoExtension->NumberMapRegs);

    FdoExtension->DmaAdapter = DmaAdapter;

    if (!DmaAdapter)
    {
        DPRINT1("USBPORT_StartDevice: Failed to get DmaAdapter!\n");
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ExitWithError;
    }

    Status = USBPORT_CreateWorkerThread(FdoDevice);
    if (!NT_SUCCESS(Status))
        goto ExitWithError;

    Status = USBPORT_QueryCapabilities(FdoDevice, &FdoExtension->Capabilities);
    if (!NT_SUCCESS(Status))
        goto ExitWithError;

    FdoExtension->PciDeviceNumber = FdoExtension->Capabilities.Address >> 16;
    FdoExtension->PciFunctionNumber = FdoExtension->Capabilities.Address & 0xFFFF;

    Status = IoGetDeviceProperty(FdoExtension->CommonExtension.LowerPdoDevice,
                                 DevicePropertyBusNumber,
                                 sizeof(ULONG),
                                 &FdoExtension->BusNumber,
                                 &ResultLength);

    if (!NT_SUCCESS(Status))
        goto ExitWithError;

    FdoExtension->BusBandwidth = FdoExtension->MiniPortInterface->Packet.MiniPortBusBandwidth;

    KeInitializeSpinLock(&FdoExtension->EndpointListSpinLock);
    KeInitializeSpinLock(&FdoExtension->DoneTransferSpinLock);
    KeInitializeSpinLock(&FdoExtension->EpStateChangeSpinLock);
    KeInitializeSpinLock(&FdoExtension->DeviceHandleSpinLock);
    KeInitializeSpinLock(&FdoExtension->IdleIoCsqSpinLock);
    KeInitializeSpinLock(&FdoExtension->BadRequestIoCsqSpinLock);
    KeInitializeSpinLock(&FdoExtension->FlushTransferSpinLock);
    KeInitializeSpinLock(&FdoExtension->WorkerThreadEventSpinLock);
    KeInitializeSpinLock(&FdoExtension->MiniportSpinLock);
    KeInitializeSpinLock(&FdoExtension->TimerFlagsSpinLock);

    KeInitializeDpc(&FdoExtension->IsrDpc, USBPORT_IsrDpc, FdoDevice);

    KeInitializeDpc(&FdoExtension->TransferFlushDpc,
                    USBPORT_TransferFlushDpc,
                    FdoDevice);

    KeInitializeDpc(&FdoExtension->WorkerRequestDpc,
                    USBPORT_WorkerRequestDpc,
                    FdoDevice);

    IoCsqInitialize(&FdoExtension->IdleIoCsq,
                    USBPORT_InsertIdleIrp,
                    USBPORT_RemoveIdleIrp,
                    USBPORT_PeekNextIdleIrp,
                    USBPORT_AcquireIdleLock,
                    USBPORT_ReleaseIdleLock,
                    USBPORT_CompleteCanceledIdleIrp);

    IoCsqInitialize(&FdoExtension->BadRequestIoCsq,
                    USBPORT_InsertBadRequest,
                    USBPORT_RemoveBadRequest,
                    USBPORT_PeekNextBadRequest,
                    USBPORT_AcquireBadRequestLock,
                    USBPORT_ReleaseBadRequestLock,
                    USBPORT_CompleteCanceledBadRequest);

    FdoExtension->IsrDpcCounter = -1;
    FdoExtension->IsrDpcHandlerCounter = -1;
    FdoExtension->IdleLockCounter = -1;
    FdoExtension->BadRequestLockCounter = -1;
 
    FdoExtension->UsbAddressBitMap[0] = 1;
    FdoExtension->UsbAddressBitMap[1] = 0;
    FdoExtension->UsbAddressBitMap[2] = 0;
    FdoExtension->UsbAddressBitMap[3] = 0;

    FdoExtension->ActiveIrpTable = ExAllocatePoolWithTag(NonPagedPool, sizeof(USBPORT_IRP_TABLE), USB_PORT_TAG);

    if (!FdoExtension->ActiveIrpTable)
    {
        DPRINT1("USBPORT_StartDevice: Allocate ActiveIrpTable failed!\n");
        goto ExitWithError;
    }

    RtlZeroMemory(FdoExtension->ActiveIrpTable, sizeof(USBPORT_IRP_TABLE));

    FdoExtension->PendingIrpTable = ExAllocatePoolWithTag(NonPagedPool, sizeof(USBPORT_IRP_TABLE), USB_PORT_TAG);

    if (!FdoExtension->PendingIrpTable)
    {
        DPRINT1("USBPORT_StartDevice: Allocate PendingIrpTable failed!\n");
        goto ExitWithError;
    }

    RtlZeroMemory(FdoExtension->PendingIrpTable, sizeof(USBPORT_IRP_TABLE));

    Status = IoConnectInterrupt(&FdoExtension->InterruptObject,
                                USBPORT_InterruptService,
                                (PVOID)FdoDevice,
                                0,
                                UsbPortResources->InterruptVector,
                                UsbPortResources->InterruptLevel,
                                UsbPortResources->InterruptLevel,
                                UsbPortResources->InterruptMode,
                                UsbPortResources->ShareVector,
                                UsbPortResources->InterruptAffinity,
                                0);

    if (!NT_SUCCESS(Status))
    {
        DPRINT1("USBPORT_StartDevice: IoConnectInterrupt failed!\n");
        goto ExitWithError;
    }

    if (FdoExtension->MiniPortInterface->Packet.MiniPortExtensionSize)
    {
        RtlZeroMemory(FdoExtension->MiniPortExt,
                      FdoExtension->MiniPortInterface->Packet.MiniPortExtensionSize);
    }

    if (FdoExtension->MiniPortInterface->Packet.MiniPortResourcesSize)
    {
        HeaderBuffer = USBPORT_AllocateCommonBuffer(FdoDevice,
                                                    FdoExtension->MiniPortInterface->Packet.MiniPortResourcesSize);

        if (!HeaderBuffer)
        {
            DPRINT1("USBPORT_StartDevice: Failed to AllocateCommonBuffer!\n");
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto ExitWithError;
        }

        UsbPortResources->StartVA = (PVOID)HeaderBuffer->VirtualAddress;
        UsbPortResources->StartPA = (PVOID)HeaderBuffer->PhysicalAddress;

        FdoExtension->MiniPortCommonBuffer = HeaderBuffer;
    }
    else
    {
        FdoExtension->MiniPortCommonBuffer = 0;
    }

    MiniPortStatus = FdoExtension->MiniPortInterface->Packet.StartController(FdoExtension->MiniPortExt,
                                                                             UsbPortResources);

    if (MiniPortStatus)
    {
        DPRINT1("USBPORT_StartDevice: Failed to Start MiniPort. MiniPortStatus - %x\n",
                MiniPortStatus);

        IoDisconnectInterrupt(FdoExtension->InterruptObject);
        goto ExitWithError;
    }
    else
    {
        FdoExtension->MiniPortInterface->Packet.EnableInterrupts(FdoExtension->MiniPortExt);
        FdoExtension->MiniPortFlags |= 1;
        FdoExtension->Flags |= USBPORT_FLAG_INTERRUPT_ENABLED;
    }

    FdoExtension->TimerValue = 500;
    USBPORT_StartTimer((PVOID)FdoDevice, 500);

    Status = USBPORT_RegisterDeviceInterface(FdoExtension->CommonExtension.LowerPdoDevice,
                                             FdoDevice,
                                             (CONST GUID *)&GUID_DEVINTERFACE_USB_HOST_CONTROLLER,
                                             TRUE);

    if (!NT_SUCCESS(Status))
    {
        DPRINT1("USBPORT_StartDevice: RegisterDeviceInterface failed!\n");
        goto ExitWithError;
    }

    DPRINT1("USBPORT_StartDevice: FIXME USBPORT_CreateLegacySymbolicLink\n");
    //USBPORT_CreateLegacySymbolicLink(FdoDevice);

    FdoExtension->Flags |= USBPORT_FLAG_DEVICE_STARTED;

    DPRINT("USBPORT_StartDevice: Exit Status - %p\n", Status);
    return Status;

ExitWithError:
    DPRINT1("USBPORT_StartDevice: FIXME USBPORT_StopDevice\n");
    //USBPORT_StopDevice(FdoDevice);

    DPRINT1("USBPORT_StartDevice: ExitWithError Status - %p\n", Status);
    return Status;
}

NTSTATUS
NTAPI
USBPORT_ParseResources(IN PDEVICE_OBJECT FdoDevice,
                       IN PIRP Irp,
                       IN PUSBPORT_RESOURCES UsbPortResources)
{
    PCM_RESOURCE_LIST AllocatedResourcesTranslated;
    PCM_PARTIAL_RESOURCE_LIST ResourceList;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptor;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PortDescriptor = NULL;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR MemoryDescriptor = NULL;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR InterruptDescriptor = NULL;
    ULONG ix = 0;
    NTSTATUS Status = STATUS_SUCCESS;

    DPRINT("USBPORT_ParseResources: ... \n");

    AllocatedResourcesTranslated = IoGetCurrentIrpStackLocation(Irp)->Parameters.StartDevice.AllocatedResourcesTranslated;

    if (AllocatedResourcesTranslated)
    {
        RtlZeroMemory(UsbPortResources, sizeof(USBPORT_RESOURCES));

        ResourceList = &AllocatedResourcesTranslated->List[0].PartialResourceList;

        if (ResourceList->Count > 0)
        {
            PartialDescriptor = &ResourceList->PartialDescriptors[0];

            do
            {
                if (PartialDescriptor->Type == CmResourceTypePort) // 1
                {
                    if (!PortDescriptor)
                        PortDescriptor = PartialDescriptor;
                }
                else if (PartialDescriptor->Type == CmResourceTypeInterrupt) // 2
                {
                    if (!InterruptDescriptor)
                        InterruptDescriptor = PartialDescriptor;
                }
                else if (PartialDescriptor->Type == CmResourceTypeMemory) // 3
                {
                    if (!MemoryDescriptor)
                        MemoryDescriptor = PartialDescriptor;
                }
                ++ix;
                PartialDescriptor += 1;
            }
            while (ix < ResourceList->Count);
        }

        if (PortDescriptor)
        {
            if (PortDescriptor->Flags & CM_RESOURCE_PORT_IO)
            {
                UsbPortResources->ResourceBase = (PVOID)PortDescriptor->u.Port.Start.LowPart;
            }
            else
            {
                UsbPortResources->ResourceBase = MmMapIoSpace(PortDescriptor->u.Port.Start,
                                                              PortDescriptor->u.Port.Length,
                                                              0);
            }

            UsbPortResources->IoSpaceLength = PortDescriptor->u.Port.Length;

            if (UsbPortResources->ResourceBase)
            {
                UsbPortResources->TypesResources |= 1; // FIXME const
            }
            else
            {
                Status = STATUS_NONE_MAPPED;
            }
        }

        if (MemoryDescriptor && NT_SUCCESS(Status))
        {
            UsbPortResources->IoSpaceLength = MemoryDescriptor->u.Memory.Length;

            UsbPortResources->ResourceBase = MmMapIoSpace(MemoryDescriptor->u.Memory.Start,
                                                          MemoryDescriptor->u.Memory.Length,
                                                          0);

            if (UsbPortResources->ResourceBase)
            {
                UsbPortResources->TypesResources |= 4;// FIXME const
            }
            else
            {
                Status = STATUS_NONE_MAPPED;
            }
        }

        if (InterruptDescriptor && NT_SUCCESS(Status))
        {
            UsbPortResources->TypesResources |= 2; // FIXME const

            UsbPortResources->InterruptVector = InterruptDescriptor->u.Interrupt.Vector;
            UsbPortResources->InterruptLevel = InterruptDescriptor->u.Interrupt.Level;
            UsbPortResources->InterruptAffinity = InterruptDescriptor->u.Interrupt.Affinity;
            UsbPortResources->ShareVector = InterruptDescriptor->ShareDisposition == CmResourceShareShared;
            UsbPortResources->InterruptMode = InterruptDescriptor->Flags == CM_RESOURCE_INTERRUPT_LATCHED;
        }
    }
    else
    {
        Status = STATUS_NONE_MAPPED;
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_CreatePdo(IN PDEVICE_OBJECT FdoDevice,
                  OUT PDEVICE_OBJECT *RootHubPdo)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    UNICODE_STRING DeviceName;
    ULONG DeviceNumber = 0;
    PDEVICE_OBJECT DeviceObject = NULL;
    WCHAR CharDeviceName[64];
    NTSTATUS Status = STATUS_SUCCESS;

    DPRINT("USBPORT_CreatePdo: FdoDevice - %p, RootHubPdo - %p\n",
           FdoDevice,
           RootHubPdo);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    do
    {
        swprintf(CharDeviceName, L"\\Device\\USBPDO-%d", DeviceNumber);
        RtlInitUnicodeString(&DeviceName, CharDeviceName);
        DPRINT("USBPORT_CreatePdo: DeviceName - %wZ\n", &DeviceName);

        Status = IoCreateDevice(FdoExtension->MiniPortInterface->DriverObject,
                                sizeof(USBPORT_RHDEVICE_EXTENSION),
                                &DeviceName,
                                FILE_DEVICE_BUS_EXTENDER,
                                0,
                                FALSE,
                                &DeviceObject);

        ++DeviceNumber;
    }
    while (Status == STATUS_OBJECT_NAME_COLLISION);

    if (!NT_SUCCESS(Status))
    {
        *RootHubPdo = NULL;
        DPRINT1("USBPORT_CreatePdo: Filed create HubPdo!\n");
        return Status;
    }

    if (DeviceObject)
    {
        PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)DeviceObject->DeviceExtension;

        RtlZeroMemory(PdoExtension, sizeof(USBPORT_RHDEVICE_EXTENSION));

        PdoExtension->CommonExtension.SelfDevice = DeviceObject;
        PdoExtension->CommonExtension.IsPDO = TRUE;

        PdoExtension->FdoDevice = FdoDevice;
        PdoExtension->PdoNameNumber = DeviceNumber;

        USBPORT_AdjustDeviceCapabilities(FdoDevice, DeviceObject);

        DeviceObject->StackSize = FdoDevice->StackSize;

        DeviceObject->Flags |= DO_POWER_PAGABLE;
        DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    }
    else
    {
        Status = STATUS_UNSUCCESSFUL;
    }

    if (!NT_SUCCESS(Status))
        *RootHubPdo = NULL;
    else
        *RootHubPdo = DeviceObject;

    DPRINT("USBPORT_CreatePdo: HubPdo - %p\n", DeviceObject);
    return Status;
}

NTSTATUS
NTAPI
USBPORT_FdoPnP(IN PDEVICE_OBJECT FdoDevice,
               IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RESOURCES UsbPortResources;
    PIO_STACK_LOCATION IoStack;
    UCHAR Minor;
    KEVENT Event;
    NTSTATUS Status = STATUS_SUCCESS;
    DEVICE_RELATION_TYPE RelationType;
    PDEVICE_RELATIONS DeviceRelations;

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    UsbPortResources = (PUSBPORT_RESOURCES)&FdoExtension->UsbPortResources;

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    Minor = IoStack->MinorFunction;

    DPRINT("USBPORT_FdoPnP: Minor - %d\n", Minor);

    RelationType = IoStack->Parameters.QueryDeviceRelations.Type; 

    switch (Minor)
    {
        case IRP_MN_START_DEVICE: // 0
            DPRINT("IRP_MN_START_DEVICE\n");

            KeInitializeEvent(&Event, NotificationEvent, FALSE);

            IoCopyCurrentIrpStackLocationToNext(Irp);

            IoSetCompletionRoutine(Irp,
                                   (PIO_COMPLETION_ROUTINE)USBPORT_FdoStartCompletion,
                                   &Event,
                                   1,
                                   1,
                                   1);

            Status = IoCallDriver(FdoExtension->CommonExtension.LowerDevice,
                                  Irp);

            if (Status == STATUS_PENDING)
            {
                KeWaitForSingleObject(&Event,
                                      Suspended,
                                      KernelMode,
                                      FALSE,
                                      NULL);

                Status = Irp->IoStatus.Status;
            }

            if (NT_SUCCESS(Status))
            {
                Status = USBPORT_ParseResources(FdoDevice,
                                                Irp,
                                                UsbPortResources);

                if (NT_SUCCESS(Status))
                {
                    Status = USBPORT_StartDevice(FdoDevice, UsbPortResources);
                }
            }

            Irp->IoStatus.Status = Status;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            return Status;

        case IRP_MN_QUERY_REMOVE_DEVICE: // 1
            //DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            DPRINT1("USBPORT_FdoPnP: IRP_MN_QUERY_REMOVE_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            //DPRINT("IRP_MN_REMOVE_DEVICE\n");
            DPRINT1("USBPORT_FdoPnP: IRP_MN_REMOVE_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_STOP_DEVICE: // 4
            //DPRINT("IRP_MN_STOP_DEVICE\n");
            DPRINT1("USBPORT_FdoPnP: IRP_MN_STOP_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_QUERY_STOP_DEVICE: // 5
            DPRINT("IRP_MN_QUERY_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_CANCEL_STOP_DEVICE: // 6
            DPRINT("IRP_MN_CANCEL_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_QUERY_DEVICE_RELATIONS: // 7
            DPRINT("IRP_MN_QUERY_DEVICE_RELATIONS\n");
            if (RelationType == BusRelations)
            {
                DeviceRelations = ExAllocatePoolWithTag(PagedPool,
                                                        sizeof(DEVICE_RELATIONS),
                                                        USB_PORT_TAG);

                if (!DeviceRelations)
                {
                    Status = STATUS_INSUFFICIENT_RESOURCES;
                    Irp->IoStatus.Information = 0;
                    Irp->IoStatus.Status = Status;
                    IoCompleteRequest(Irp, IO_NO_INCREMENT);
                    return Status;
                }

                DeviceRelations->Count = 0;
                DeviceRelations->Objects[0] = NULL;

                if (!FdoExtension->RootHubPdo)
                {
                    Status = USBPORT_CreatePdo(FdoDevice,
                                               &FdoExtension->RootHubPdo);

                    if (!NT_SUCCESS(Status))
                    {
                        ExFreePool(DeviceRelations);
                        goto ForwardIrp;
                    }
                }

                DeviceRelations->Count = 1;
                DeviceRelations->Objects[0] = FdoExtension->RootHubPdo;
                ObReferenceObject(FdoExtension->RootHubPdo);
                Irp->IoStatus.Information = (ULONG_PTR)DeviceRelations;

            }
            else
            {
                if (RelationType == RemovalRelations)
                {
                    DPRINT("USBPORT_FdoPnP: FIXME IRP_MN_QUERY_DEVICE_RELATIONS/RemovalRelations\n");
                }

                goto ForwardIrp;
            }

            Irp->IoStatus.Status = Status;
            goto ForwardIrp;

        case IRP_MN_QUERY_INTERFACE: // 8
            DPRINT("IRP_MN_QUERY_INTERFACE\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_CAPABILITIES:           // 9
            DPRINT("IRP_MN_QUERY_CAPABILITIES\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_RESOURCES:              // 10
            DPRINT("IRP_MN_QUERY_RESOURCES\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:  // 11
            DPRINT("IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_DEVICE_TEXT:            // 12
            DPRINT("IRP_MN_QUERY_DEVICE_TEXT\n");
            goto ForwardIrp;

        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS: // 13
            DPRINT("IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n");
            goto ForwardIrp;

        case IRP_MN_READ_CONFIG: // 15
            DPRINT("IRP_MN_READ_CONFIG\n");
            goto ForwardIrp;

        case IRP_MN_WRITE_CONFIG: // 16
            DPRINT("IRP_MN_WRITE_CONFIG\n");
            goto ForwardIrp;

        case IRP_MN_EJECT: // 17
            DPRINT("IRP_MN_EJECT\n");
            goto ForwardIrp;

        case IRP_MN_SET_LOCK: // 18
            DPRINT("IRP_MN_SET_LOCK\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_ID: // 19
            DPRINT("IRP_MN_QUERY_ID\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_PNP_DEVICE_STATE: // 20
            DPRINT("IRP_MN_QUERY_PNP_DEVICE_STATE\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_BUS_INFORMATION: // 21
            DPRINT("IRP_MN_QUERY_BUS_INFORMATION\n");
            goto ForwardIrp;

        case IRP_MN_DEVICE_USAGE_NOTIFICATION: // 22
            DPRINT("IRP_MN_DEVICE_USAGE_NOTIFICATION\n");
            goto ForwardIrp;

        case IRP_MN_SURPRISE_REMOVAL: // 23
            //DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            DPRINT1("USBPORT_FdoPnP: IRP_MN_SURPRISE_REMOVAL UNIMPLEMENTED. FIXME. \n");
            goto ForwardIrp;

        default:
            DPRINT("unknown IRP_MN_???\n");
ForwardIrp:
            // forward irp to next device object
            IoSkipCurrentIrpStackLocation(Irp);
            return IoCallDriver(FdoExtension->CommonExtension.LowerDevice, Irp);
            break;
    }

    return Status;
}

ULONG_PTR
NTAPI
USBPORT_GetDeviceHwIds(IN PDEVICE_OBJECT FdoDevice,
                       IN USHORT VendorID,
                       IN USHORT DeviceID,
                       IN UCHAR RevisionID)
{
    PVOID Id;
    WCHAR Buffer[300];
    ULONG Index = 0;

    Index += swprintf(&Buffer[Index],
                      L"USB\\ROOT_HUB&VID%04x&PID%04x&RE%02x",
                      VendorID,
                      DeviceID,
                      RevisionID) + 1;

    Index += swprintf(&Buffer[Index],
                      L"USB\\ROOT_HUB&VID%04x&PID%04x",
                      VendorID,
                      DeviceID) + 1;

    Index += swprintf(&Buffer[Index], L"USB\\ROOT_HUB") + 1;

    Buffer[Index] = UNICODE_NULL;
    Index++;
    DPRINT("USBPORT_GetIdString: Buffer - %S\n", Buffer);

    Id = (LPWSTR)ExAllocatePoolWithTag(PagedPool,
                                       Index * sizeof(WCHAR),
                                       USB_PORT_TAG);

    if (!Id)
        return 0;

    RtlMoveMemory(Id, Buffer, Index * sizeof(WCHAR)); // copy device name

    return (ULONG_PTR)Id;
}

NTSTATUS
NTAPI
USBPORT_PdoPnP(IN PDEVICE_OBJECT PdoDevice,
               IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    UCHAR Minor;
    NTSTATUS Status;
    PPNP_BUS_INFORMATION BusInformation;
    PDEVICE_CAPABILITIES DeviceCapabilities;
    ULONG Index = 0;
    //ULONG_PTR Information = Irp->IoStatus.Information;

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    Minor = IoStack->MinorFunction;
    Status = Irp->IoStatus.Status;

    DPRINT("USBPORT_PdoPnP: Minor - %d\n", Minor);

    switch (Minor)
    {
        case IRP_MN_START_DEVICE: // 0
            DPRINT("IRP_MN_START_DEVICE\n");

            Status = USBPORT_RootHubCreateDevice(FdoDevice, PdoDevice);
            if (NT_SUCCESS(Status))
            {
                Status = USBPORT_RegisterDeviceInterface(PdoDevice,
                                                         PdoDevice,
                                                         (CONST GUID *)&GUID_DEVINTERFACE_USB_HUB,
                                                         TRUE);

                if (NT_SUCCESS(Status))
                {
                    ;// TODO Flags |= DeviceIsStarted;
                }
            }

            break;

        case IRP_MN_QUERY_REMOVE_DEVICE: // 1
            //DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            DPRINT1("USBPORT_PdoPnP: IRP_MN_QUERY_REMOVE_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            //DPRINT("IRP_MN_REMOVE_DEVICE\n");
            DPRINT1("USBPORT_PdoPnP: IRP_MN_REMOVE_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;

        case IRP_MN_STOP_DEVICE: // 4
            //DPRINT("IRP_MN_STOP_DEVICE\n");
            DPRINT1("USBPORT_PdoPnP: IRP_MN_STOP_DEVICE UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_QUERY_STOP_DEVICE: // 5
            DPRINT("IRP_MN_QUERY_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;

        case IRP_MN_CANCEL_STOP_DEVICE: // 6
            DPRINT("IRP_MN_CANCEL_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;

        case IRP_MN_QUERY_DEVICE_RELATIONS: // 7
        {
            PDEVICE_RELATIONS DeviceRelations;

            DPRINT("IRP_MN_QUERY_DEVICE_RELATIONS\n");
            if (IoStack->Parameters.QueryDeviceRelations.Type != TargetDeviceRelation)
            {
                Status = Irp->IoStatus.Status;
                break;
            }

            DeviceRelations = (PDEVICE_RELATIONS)ExAllocatePoolWithTag(PagedPool,
                                                                       sizeof(DEVICE_RELATIONS),
                                                                       USB_PORT_TAG);

            if (!DeviceRelations)
            {
                Status = STATUS_INSUFFICIENT_RESOURCES;
                Irp->IoStatus.Information = 0;
                break;
            }

            DeviceRelations->Count = 1;
            DeviceRelations->Objects[0] = PdoDevice;

            ObReferenceObject(PdoDevice);

            Status = STATUS_SUCCESS;
            Irp->IoStatus.Information = (ULONG_PTR)DeviceRelations;
            break;
        }

        case IRP_MN_QUERY_INTERFACE: // 8
            DPRINT("IRP_MN_QUERY_INTERFACE\n");
            Status = USBPORT_PdoQueryInterface(FdoDevice, PdoDevice, Irp);
            break;

        case IRP_MN_QUERY_CAPABILITIES: // 9
            DPRINT("IRP_MN_QUERY_CAPABILITIES\n");
            DeviceCapabilities = (PDEVICE_CAPABILITIES)IoStack->Parameters.DeviceCapabilities.Capabilities;
            RtlCopyMemory(DeviceCapabilities, &PdoExtension->Capabilities, sizeof(DEVICE_CAPABILITIES));
            Status = STATUS_SUCCESS;
            break;

        case IRP_MN_QUERY_RESOURCES: // 10
            //DPRINT("IRP_MN_QUERY_RESOURCES\n");
            DPRINT1("USBPORT_PdoPnP: IRP_MN_QUERY_RESOURCES UNIMPLEMENTED. FIXME. \n");
            break;

        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS: // 11
            DPRINT("IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n");
            //ASSERT(FALSE);
            break;

        case IRP_MN_QUERY_DEVICE_TEXT: // 12
            DPRINT("IRP_MN_QUERY_DEVICE_TEXT\n");
            //ASSERT(FALSE);
            break;

        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS: // 13
            DPRINT("IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n");
            //ASSERT(FALSE);
            break;

        case IRP_MN_READ_CONFIG: // 15
            DPRINT("IRP_MN_READ_CONFIG\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_WRITE_CONFIG: // 16
            DPRINT("IRP_MN_WRITE_CONFIG\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_EJECT: // 17
            DPRINT("IRP_MN_EJECT\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_SET_LOCK: // 18
            DPRINT("IRP_MN_SET_LOCK\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_QUERY_ID: // 19
        {
            ULONG IdType;
            LONG Length;
            WCHAR Buffer[64];
            ULONG_PTR Id;

            Status = STATUS_SUCCESS;
            IdType = IoStack->Parameters.QueryId.IdType;

            DPRINT("IRP_MN_QUERY_ID/Type %x\n", IdType);

            if (IdType == BusQueryDeviceID) // 0
            {
                swprintf(Buffer, L"USB\\ROOT_HUB");
                Length = (wcslen(Buffer) + 1);

                Id = (ULONG_PTR)ExAllocatePoolWithTag(PagedPool,
                                                      Length * sizeof(WCHAR),
                                                      USB_PORT_TAG);

                if (!Id)
                {
                    Status = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                wcscpy((PVOID)Id, Buffer);
                Irp->IoStatus.Information = Id;
                break;
            }

            if (IdType == BusQueryHardwareIDs) // 1
            {
                Id = USBPORT_GetDeviceHwIds(FdoDevice,
                                            FdoExtension->VendorID,
                                            FdoExtension->DeviceID,
                                            FdoExtension->RevisionID);

                Irp->IoStatus.Information = Id;
            }

            break;
        }

        case IRP_MN_QUERY_PNP_DEVICE_STATE: // 20
            DPRINT("IRP_MN_QUERY_PNP_DEVICE_STATE\n");
            Status = STATUS_SUCCESS;
            break;

        case IRP_MN_QUERY_BUS_INFORMATION: // 21
            DPRINT("IRP_MN_QUERY_BUS_INFORMATION\n");

            // Allocate buffer for bus information
            BusInformation = (PPNP_BUS_INFORMATION)ExAllocatePool(PagedPool,
                                                                  sizeof(PNP_BUS_INFORMATION));

            if (BusInformation)
            {
                // Copy BUS GUID
                RtlMoveMemory(&BusInformation->BusTypeGuid,
                              &GUID_BUS_TYPE_USB,
                              sizeof(GUID));

                // Set bus type
                BusInformation->LegacyBusType = PNPBus;
                BusInformation->BusNumber = 0;

                Status = STATUS_SUCCESS;
                Irp->IoStatus.Information = (ULONG_PTR)BusInformation;
            }
            else
            {
                // No memory
                Status = STATUS_INSUFFICIENT_RESOURCES;
            }

            break;

        case IRP_MN_DEVICE_USAGE_NOTIFICATION: // 22
            DPRINT("IRP_MN_DEVICE_USAGE_NOTIFICATION\n");
            break;

        case IRP_MN_SURPRISE_REMOVAL: // 23
            //DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            DPRINT1("USBPORT_PdoPnP: IRP_MN_SURPRISE_REMOVAL UNIMPLEMENTED. FIXME. \n");
            break;

        default:
            DPRINT("unknown IRP_MN_???\n");
            Status = Irp->IoStatus.Status;
            break;
    }

    //Irp->IoStatus.Information = Information;
    Irp->IoStatus.Status = Status;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return Status;
}
