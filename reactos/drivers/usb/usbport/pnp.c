#include "usbport.h"

//#define NDEBUG
#include <debug.h>

BOOLEAN
NTAPI
USBPORT_InterruptService(PKINTERRUPT Interrupt,
                         PVOID ServiceContext)
{
    BOOLEAN Result = TRUE;

    DPRINT("USBPORT_InterruptService: Interrupt - %p, ServiceContext - %p\n",
           Interrupt,
           ServiceContext);

    //KeInsertQueueDpc();

    DPRINT("USBPORT_InterruptService: return - %x\n", Result);
    return Result;
}

VOID
NTAPI
USBPORT_IsrDpc(PRKDPC Dpc,
               PVOID DeferredContext,
               PVOID SystemArgument1,
               PVOID SystemArgument2)
{
    DPRINT("USBPORT_IsrDpc: Dpc - %p, DeferredContext - %p, SystemArgument1 - %p, SystemArgument2 - %p\n",
           Dpc,
           DeferredContext,
           SystemArgument1,
           SystemArgument2);

    DPRINT("USBPORT_IsrDpc: exit\n");
}

static
NTSTATUS
NTAPI
USBPORT_QueryPciBusInterface(PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PBUS_INTERFACE_STANDARD BusInterface;
    PIO_STACK_LOCATION IoStack;
    IO_STATUS_BLOCK IoStatusBlock;
    PDEVICE_OBJECT HighestDevice;
    KEVENT Event;
    PIRP Irp;
    NTSTATUS Status;

    DPRINT("USBPORT_QueryPciBusInterface: ...  \n");

    BusInterface = &FdoExtention->BusInterface;
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

static
NTSTATUS
NTAPI
USBPORT_StartDevice(PDEVICE_OBJECT FdoDevice,
                    PUSBPORT_RESOURCES UsbPortResources)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    NTSTATUS Status;
    PCI_COMMON_CONFIG PciConfig;
    ULONG BytesRead;
    DEVICE_DESCRIPTION DeviceDescription;
    PDMA_ADAPTER DmaAdapter = 0;
    ULONG MiniPortStatus;
    PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;

    DPRINT("USBPORT_StartDevice: FdoDevice - %p, UsbPortResources - %p\n",
           FdoDevice,
           UsbPortResources);

    Status = USBPORT_QueryPciBusInterface(FdoDevice);

    BytesRead = (*FdoExtention->BusInterface.GetBusData)(FdoExtention->BusInterface.Context,
                                                         PCI_WHICHSPACE_CONFIG,
                                                         &PciConfig,
                                                         0,
                                                         PCI_COMMON_HDR_LENGTH);

    if (BytesRead != PCI_COMMON_HDR_LENGTH)
    {
        DPRINT1("USBPORT_StartDevice: Failed to get pci config information!\n");
        goto ExitWithError;
    }

    FdoExtention->VendorID = PciConfig.VendorID;
    FdoExtention->DeviceID = PciConfig.DeviceID;
    FdoExtention->RevisionID = PciConfig.RevisionID;
    FdoExtention->ProgIf = PciConfig.ProgIf;
    FdoExtention->SubClass = PciConfig.SubClass;
    FdoExtention->BaseClass = PciConfig.BaseClass;

    RtlZeroMemory(&DeviceDescription, sizeof(DeviceDescription));

    DeviceDescription.Version = DEVICE_DESCRIPTION_VERSION;
    DeviceDescription.Master = TRUE;
    DeviceDescription.ScatterGather = TRUE;
    DeviceDescription.Dma32BitAddresses = TRUE;
    DeviceDescription.InterfaceType = PCIBus;
    DeviceDescription.DmaWidth = Width32Bits;
    DeviceDescription.DmaSpeed = Compatible;
    DeviceDescription.MaximumLength = MAXULONG;

    DmaAdapter = IoGetDmaAdapter(FdoExtention->CommonExtension.LowerPdoDevice,
                                 &DeviceDescription,
                                 &FdoExtention->NumberMapRegs);

    FdoExtention->DmaAdapter = DmaAdapter;

    if (!DmaAdapter)
    {
        DPRINT1("USBPORT_StartDevice: Failed to get DmaAdapter!\n");
        Status = STATUS_INSUFFICIENT_RESOURCES;
        goto ExitWithError;
    }

    KeInitializeDpc(&FdoExtention->IsrDpc, USBPORT_IsrDpc, FdoDevice);

    Status = IoConnectInterrupt(&FdoExtention->InterruptObject,
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

    if (FdoExtention->MiniPortInterface->Packet.MiniPortExtensionSize)
    {
        RtlZeroMemory(FdoExtention->MiniPortExt,
                      FdoExtention->MiniPortInterface->Packet.MiniPortExtensionSize);
    }

    if (FdoExtention->MiniPortInterface->Packet.MiniPortResourcesSize)
    {
        HeaderBuffer = USBPORT_AllocateCommonBuffer(FdoDevice,
                                                    FdoExtention->MiniPortInterface->Packet.MiniPortResourcesSize);

        if (!HeaderBuffer)
        {
            DPRINT1("USBPORT_StartDevice: Failed to AllocateCommonBuffer!\n");
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto ExitWithError;
        }

        UsbPortResources->StartVA = (PVOID)HeaderBuffer->VirtualAddress;
        UsbPortResources->StartPA = (PVOID)HeaderBuffer->PhysicalAddress;

        FdoExtention->MiniPortCommonBuffer = HeaderBuffer;
    }
    else
    {
        FdoExtention->MiniPortCommonBuffer = 0;
    }

    MiniPortStatus = FdoExtention->MiniPortInterface->Packet.StartController(FdoExtention->MiniPortExt,
                                                                             UsbPortResources);

    if (MiniPortStatus)
    {
        DPRINT1("USBPORT_StartDevice: Failed to Start MiniPort. MiniPortStatus - %x\n",
                MiniPortStatus);

        IoDisconnectInterrupt(FdoExtention->InterruptObject);
        goto ExitWithError;
    }
    else
    {
        FdoExtention->MiniPortInterface->Packet.EnableInterrupts(FdoExtention->MiniPortExt);
    }

    if (NT_SUCCESS(Status))
        goto Exit;

ExitWithError:
    DPRINT("USBPORT_StartDevice: ExitWithError Status - %p\n", Status);
    //USBPORT_StopDevice();

Exit:
    DPRINT("USBPORT_StartDevice: Exit Status - %p\n", Status);
    return Status;
}

static
NTSTATUS
NTAPI
USBPORT_ParseResources(PDEVICE_OBJECT FdoDevice,
                       PIRP Irp,
                       PUSBPORT_RESOURCES UsbPortResources)
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

static
NTSTATUS
NTAPI
USBPORT_FdoStartCompletion(PDEVICE_OBJECT DeviceObject,
                           PIRP Irp,
                           PRKEVENT Event)
{
    KeSetEvent(Event, EVENT_INCREMENT, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}

static
NTSTATUS
NTAPI
USBPORT_CreatePdo(PDEVICE_OBJECT FdoDevice,
                  PDEVICE_OBJECT *RootHubPdo)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtention;
    UNICODE_STRING DeviceName;
    ULONG DeviceNumber = 0;
    PDEVICE_OBJECT DeviceObject = NULL;
    WCHAR CharDeviceName[64];
    NTSTATUS Status = 0;

    DPRINT("USBPORT_CreatePdo: FdoDevice - %p, RootHubPdo - %p\n",
           FdoDevice,
           RootHubPdo);

    FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    do
    {
        swprintf(CharDeviceName, L"\\Device\\USBPDO-%d", DeviceNumber);
        RtlInitUnicodeString(&DeviceName, CharDeviceName);
        DPRINT("USBPORT_CreatePdo: DeviceName - %wZ\n", &DeviceName);

        Status = IoCreateDevice(FdoExtention->MiniPortInterface->DriverObject,
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
        *RootHubPdo = 0;
        DPRINT1("USBPORT_CreatePdo: Filed create HubPdo!\n");
        return Status;
    }

    if (DeviceObject)
    {
        PdoExtention = (PUSBPORT_RHDEVICE_EXTENSION)DeviceObject->DeviceExtension;

        RtlZeroMemory(PdoExtention, sizeof(USBPORT_RHDEVICE_EXTENSION));

        PdoExtention->CommonExtension.SelfDevice = DeviceObject;
        PdoExtention->CommonExtension.IsPDO = TRUE;

        PdoExtention->FdoDevice = FdoDevice;
        PdoExtention->PdoNameNumber = DeviceNumber;

        DeviceObject->StackSize = FdoDevice->StackSize;

        DeviceObject->Flags |= DO_POWER_PAGABLE;
        DeviceObject->Flags &= ~DO_DEVICE_INITIALIZING;
    }
    else
    {
        Status = STATUS_UNSUCCESSFUL;
    }

    if (!NT_SUCCESS(Status))
        *RootHubPdo = 0;
    else
        *RootHubPdo = DeviceObject;

    DPRINT("USBPORT_CreatePdo: HubPdo - %p\n", DeviceObject);
    return Status;
}

NTSTATUS
NTAPI
USBPORT_FdoPnP(PDEVICE_OBJECT FdoDevice,
               PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PUSBPORT_RESOURCES UsbPortResources = (PUSBPORT_RESOURCES)&FdoExtention->UsbPortResources;
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    UCHAR Minor = IoStack->MinorFunction;
    KEVENT Event;
    NTSTATUS Status = STATUS_SUCCESS;
    DEVICE_RELATION_TYPE RelationType = IoStack->Parameters.QueryDeviceRelations.Type;
    PDEVICE_RELATIONS DeviceRelations;

    DPRINT("USBPORT_FdoPnP: Minor - %d\n", Minor);

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

            Status = IoCallDriver(FdoExtention->CommonExtension.LowerDevice,
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
            DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            DPRINT("IRP_MN_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_STOP_DEVICE: // 4
            DPRINT("IRP_MN_STOP_DEVICE\n");
            ASSERT(FALSE);
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
                    IoCompleteRequest(Irp, 0);
                    return Status;
                }

                DeviceRelations->Count = 0;
                DeviceRelations->Objects[0] = NULL;

                if (!FdoExtention->RootHubPdo)
                {
                    Status = USBPORT_CreatePdo(FdoDevice,
                                               &FdoExtention->RootHubPdo);

                    if (!NT_SUCCESS(Status))
                    {
                        ExFreePool(DeviceRelations);
                        goto ForwardIrp;
                    }
                }

                DeviceRelations->Count = 1;
                DeviceRelations->Objects[0] = FdoExtention->RootHubPdo;
                ObReferenceObject(FdoExtention->RootHubPdo);
                Irp->IoStatus.Information = (ULONG_PTR)DeviceRelations;

            }
            else
            {
                if (RelationType == RemovalRelations)
                {
                    DPRINT("USBPORT_FdoPnP: FIXME IRP_MN_QUERY_DEVICE_RELATIONS/RemovalRelations\n");
                }

                ASSERT(FALSE);
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
            DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            ASSERT(FALSE);
            goto ForwardIrp;
            break;

        default:
            DPRINT("unknown IRP_MN_???\n");
ForwardIrp:
            // forward irp to next device object
            IoSkipCurrentIrpStackLocation(Irp);
            return IoCallDriver(FdoExtention->CommonExtension.LowerDevice, Irp);
            break;
    }

    return Status;
}

static
ULONG_PTR
NTAPI
GetDeviceHwIds(PDEVICE_OBJECT FdoDevice,
               USHORT VendorID,
               USHORT DeviceID,
               UCHAR RevisionID)
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

static
NTSTATUS
NTAPI
USBPORT_RegisterDeviceInterface(PDEVICE_OBJECT PdoDevice,
                                BOOLEAN Enable)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtention = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    PUNICODE_STRING SymbolicLinkName;
    NTSTATUS Status;

    DPRINT("USBPORT_RegisterDeviceInterface ... \n");

    SymbolicLinkName = &PdoExtention->RhSymbolicLinkName;

    if (Enable)
    {
        Status = IoRegisterDeviceInterface(PdoDevice,
                                           &GUID_DEVINTERFACE_USB_HUB,
                                           0,
                                           SymbolicLinkName);

        if (NT_SUCCESS(Status))
        {
            Status = IoSetDeviceInterfaceState(SymbolicLinkName, TRUE);

            if (NT_SUCCESS(Status))
                PdoExtention->IsInterfaceEnabled = 1;
        }
    }
    else if (PdoExtention->IsInterfaceEnabled)
    {
        // Disable device interface
        Status = IoSetDeviceInterfaceState(SymbolicLinkName, FALSE);

        if (NT_SUCCESS(Status))
            RtlFreeUnicodeString(SymbolicLinkName);

        PdoExtention->IsInterfaceEnabled = 0; // Disabled interface
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_PdoPnP(PDEVICE_OBJECT PdoDevice,
               PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtention = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    PDEVICE_OBJECT FdoDevice = PdoExtention->FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    //ULONG_PTR Information = Irp->IoStatus.Information;
    UCHAR Minor = IoStack->MinorFunction;
    NTSTATUS Status;
    WCHAR Buffer[300];
    ULONG Length;
    LPWSTR DeviceName;
    ULONG_PTR Id;
    PPNP_BUS_INFORMATION BusInformation;
    PDEVICE_CAPABILITIES DeviceCapabilities;
    ULONG Index = 0;

    DPRINT("USBPORT_PdoPnP: Minor - %d\n", Minor);

    Status = Irp->IoStatus.Status;

    switch (Minor)
    {
        case IRP_MN_START_DEVICE: // 0
            DPRINT("IRP_MN_START_DEVICE\n");

            Status = USBPORT_CreateRhDevice(FdoDevice, PdoDevice);
            if (NT_SUCCESS(Status))
            {
                Status = USBPORT_RegisterDeviceInterface(PdoDevice, TRUE);
                if (NT_SUCCESS(Status))
                {
                    ;// TODO Flags |= DeviceIsStarted;
                }
            }

            break;

        case IRP_MN_QUERY_REMOVE_DEVICE: // 1
            DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            DPRINT("IRP_MN_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            break;

        case IRP_MN_STOP_DEVICE: // 4
            DPRINT("IRP_MN_STOP_DEVICE\n");
            ASSERT(FALSE);
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
            DPRINT("IRP_MN_QUERY_DEVICE_RELATIONS\n");
            Irp->IoStatus.Status = Status;
            break;

        case IRP_MN_QUERY_INTERFACE: // 8
            DPRINT("IRP_MN_QUERY_INTERFACE\n");
            Status = 0; // USBPORT_PdoQueryInterface(FdoDevice, PdoDevice, Irp);
            break;

        case IRP_MN_QUERY_CAPABILITIES: // 9
            DPRINT("IRP_MN_QUERY_CAPABILITIES\n");

            DeviceCapabilities = (PDEVICE_CAPABILITIES)IoStack->Parameters.DeviceCapabilities.Capabilities;

            DeviceCapabilities->LockSupported = FALSE;
            DeviceCapabilities->EjectSupported = FALSE;
            DeviceCapabilities->Removable = FALSE;
            DeviceCapabilities->DockDevice = FALSE;
            DeviceCapabilities->UniqueID = FALSE;
            DeviceCapabilities->SilentInstall = FALSE;
            DeviceCapabilities->RawDeviceOK = FALSE;
            DeviceCapabilities->SurpriseRemovalOK = FALSE;
            DeviceCapabilities->Address = 0;
            DeviceCapabilities->UINumber = 0;
            DeviceCapabilities->DeviceD2 = 1;

            /* FIXME */
            DeviceCapabilities->HardwareDisabled = FALSE;
            DeviceCapabilities->NoDisplayInUI = FALSE;
            DeviceCapabilities->DeviceState[0] = PowerDeviceD0;

            for (Index = 1; Index < PowerSystemMaximum; Index++)
                DeviceCapabilities->DeviceState[Index] = PowerDeviceD3;

            DeviceCapabilities->DeviceWake = PowerDeviceUnspecified;
            DeviceCapabilities->D1Latency = 0;
            DeviceCapabilities->D2Latency = 0;
            DeviceCapabilities->D3Latency = 0;

            Status = STATUS_SUCCESS;
            break;

        case IRP_MN_QUERY_RESOURCES: // 10
            DPRINT("IRP_MN_QUERY_RESOURCES\n");
            //ASSERT(FALSE);
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
            DPRINT("IRP_MN_QUERY_ID/Type %x\n", IoStack->Parameters.QueryId.IdType);

            if (IoStack->Parameters.QueryId.IdType == BusQueryDeviceID)
            {
                DPRINT("IRP_MN_QUERY_ID/BusQueryDeviceID\n");
                swprintf(Buffer, L"USB\\ROOT_HUB");
                Length = (wcslen(Buffer) + 1);

                DeviceName = (LPWSTR)ExAllocatePoolWithTag(PagedPool,
                                                           Length * sizeof(WCHAR),
                                                           USB_PORT_TAG);
                if (!DeviceName)
                {
                    Status = STATUS_INSUFFICIENT_RESOURCES;
                    break;
                }

                wcscpy(DeviceName, Buffer);

                Irp->IoStatus.Information = (ULONG_PTR)DeviceName;
                Status = STATUS_SUCCESS;
                break;
            }

            if (IoStack->Parameters.QueryId.IdType == BusQueryHardwareIDs)
            {
                DPRINT("IRP_MN_QUERY_ID/BusQueryHardwareIDs\n");

                Id = GetDeviceHwIds(FdoDevice,
                                    FdoExtention->VendorID,
                                    FdoExtention->DeviceID,
                                    FdoExtention->RevisionID);

                Irp->IoStatus.Information = Id;
                Status = STATUS_SUCCESS;
            }

            break;

        case IRP_MN_QUERY_PNP_DEVICE_STATE: // 20
            DPRINT("IRP_MN_QUERY_PNP_DEVICE_STATE\n");
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
            DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            ASSERT(FALSE);
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
