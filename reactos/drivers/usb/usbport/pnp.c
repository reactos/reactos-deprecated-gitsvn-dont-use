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
        goto ExitWithError;

    if (FdoExtention->MiniPortInterface->Packet.MiniPortHwResourcesSize)
    {
        RtlZeroMemory(FdoExtention->MiniPortExt,
                      FdoExtention->MiniPortInterface->Packet.MiniPortHwResourcesSize);
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
            ASSERT(FALSE);
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
