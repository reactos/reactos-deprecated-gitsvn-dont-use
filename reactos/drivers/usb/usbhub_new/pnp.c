#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_IrpCompletion(IN PDEVICE_OBJECT DeviceObject,
                   IN PIRP Irp,
                   IN PVOID Context)
{
    PRKEVENT Event;

    DPRINT("USBH_IrpCompletion: Irp - %p\n", Irp);

    Event = (PRKEVENT)Context;
    KeSetEvent(Event, EVENT_INCREMENT, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS
NTAPI
USBH_HubPnPIrpComplete(IN PDEVICE_OBJECT DeviceObject,
                       IN PIRP Irp,
                       IN PVOID Context)
{
    PUSBHUB_FDO_EXTENSION HubExtension;

    DPRINT("USBH_HubPnPIrpComplete: Irp - %p\n", Irp);

     HubExtension = (PUSBHUB_FDO_EXTENSION)Context;

    if (!NT_SUCCESS(Irp->IoStatus.Status))
    {
        DPRINT1("USBH_HubPnPIrpComplete: Irp failed - %p\n", Irp->IoStatus.Status);
        HubExtension->HubFlags |= USBHUB_FDO_FLAG_DEVICE_FAILED;
    }

    Irp->IoStatus.Status = STATUS_MORE_PROCESSING_REQUIRED;

    KeSetEvent(&HubExtension->LowerDeviceEvent, EVENT_INCREMENT, FALSE);

    return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS
NTAPI
USBHUB_GetBusInterface(IN PDEVICE_OBJECT DeviceObject,
                       IN PUSB_BUS_INTERFACE_HUB_V5 BusInterface)
{
    PIRP Irp;
    NTSTATUS Status;
    PIO_STACK_LOCATION IoStack;
    KEVENT Event;

    DPRINT("USBHUB_GetBusInterface: ... \n");

    Irp = IoAllocateIrp(DeviceObject->StackSize, 0);

    if (!Irp)
    {
        DPRINT1("USBHUB_GetBusInterface: IoAllocateIrp() failed\n");
        return STATUS_UNSUCCESSFUL;
    }

    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    IoSetCompletionRoutine(Irp,
                           USBH_IrpCompletion,
                           &Event,
                           TRUE,
                           TRUE,
                           TRUE);

    IoStack = IoGetNextIrpStackLocation(Irp);

    IoStack->MajorFunction = IRP_MJ_PNP;
    IoStack->MinorFunction = IRP_MN_QUERY_INTERFACE;

    IoStack->Parameters.QueryInterface.InterfaceType = &USB_BUS_INTERFACE_HUB_GUID;
    IoStack->Parameters.QueryInterface.Size = sizeof(USB_BUS_INTERFACE_HUB_V5);
    IoStack->Parameters.QueryInterface.Version = USB_BUSIF_HUB_VERSION_5;
    IoStack->Parameters.QueryInterface.Interface = (PINTERFACE)BusInterface;
    IoStack->Parameters.QueryInterface.InterfaceSpecificData = DeviceObject;

    Status = IoCallDriver(DeviceObject, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);

        Status = Irp->IoStatus.Status;
    }

    IoFreeIrp(Irp);

    return Status;
}

NTSTATUS
NTAPI
USBHUB_GetBusInterfaceUSBDI(IN PDEVICE_OBJECT DeviceObject,
                            IN PUSB_BUS_INTERFACE_USBDI_V2 BusInterfaceUSBDI)
{
    PIRP Irp;
    NTSTATUS Status;
    PIO_STACK_LOCATION IoStack;
    KEVENT Event;

    DPRINT("USBHUB_GetBusInterfaceUSBDI: ... \n");

    Irp = IoAllocateIrp(DeviceObject->StackSize, 0);

    if (!Irp)
    {
        DPRINT1("USBHUB_GetBusInterfaceUSBDI: IoAllocateIrp() failed\n");
        return STATUS_UNSUCCESSFUL;
    }

    Irp->IoStatus.Status = STATUS_NOT_SUPPORTED;

    KeInitializeEvent(&Event, NotificationEvent, FALSE);

    IoSetCompletionRoutine(Irp,
                           USBH_IrpCompletion,
                           &Event,
                           TRUE,
                           TRUE,
                           TRUE);

    IoStack = IoGetNextIrpStackLocation(Irp);

    IoStack->MajorFunction = IRP_MJ_PNP;
    IoStack->MinorFunction = IRP_MN_QUERY_INTERFACE;

    IoStack->Parameters.QueryInterface.InterfaceType = &USB_BUS_INTERFACE_USBDI_GUID;
    IoStack->Parameters.QueryInterface.Size = sizeof(USB_BUS_INTERFACE_USBDI_V2);
    IoStack->Parameters.QueryInterface.Version = USB_BUSIF_USBDI_VERSION_2;
    IoStack->Parameters.QueryInterface.Interface = (PINTERFACE)BusInterfaceUSBDI;
    IoStack->Parameters.QueryInterface.InterfaceSpecificData = NULL;

    Status = IoCallDriver(DeviceObject, Irp);

    if (Status == STATUS_PENDING)
    {
        KeWaitForSingleObject(&Event,
                              Suspended,
                              KernelMode,
                              FALSE,
                              NULL);

        Status = Irp->IoStatus.Status;
    }

    IoFreeIrp(Irp);

    return Status;
}

NTSTATUS
NTAPI
USBH_StartHubFdoDevice(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PIRP Irp)
{
    NTSTATUS Status=0;

    DPRINT("USBH_StartHubFdoDevice: ... \n");

    USBH_CompleteIrp(Irp, Status);

    return Status;
}

NTSTATUS
NTAPI
USBH_FdoStartDevice(IN PUSBHUB_FDO_EXTENSION HubExtension,
                    IN PIRP Irp)
{
    NTSTATUS Status;

    DPRINT("USBH_FdoStartDevice: ... \n");

    HubExtension->RootHubPdo = NULL;

    Status = USBH_SyncGetRootHubPdo(HubExtension->LowerDevice,
                                    &HubExtension->RootHubPdo,
                                    &HubExtension->RootHubPdo2);

    if (NT_SUCCESS(Status))
    {
        if (HubExtension->RootHubPdo)
        {
            Status = USBH_StartHubFdoDevice(HubExtension, Irp);
        }
        else
        {
            DPRINT1("USBH_FdoStartDevice: FIXME. start ParentDevice\n");
            DbgBreakPoint();
        }
    }
    else
    {
        DPRINT1("USBH_FdoStartDevice: FIXME. USBH_SyncGetRootHubPdo return - %p\n",
                Status);

        DbgBreakPoint();
        USBH_CompleteIrp(Irp, Status);
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_FdoPnP(IN PUSBHUB_FDO_EXTENSION HubExtension,
            IN PIRP Irp,
            IN UCHAR Minor)
{
    NTSTATUS Status=0;

    DPRINT("USBH_FdoPnP: HubExtension - %p, Irp - %p, Minor - %x\n",
           HubExtension,
           Irp,
           Minor);

    switch (Minor)
    {
        case IRP_MN_START_DEVICE: // 0
            DPRINT("IRP_MN_START_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            Status = USBH_FdoStartDevice(HubExtension, Irp);
            break;

        case IRP_MN_QUERY_REMOVE_DEVICE: // 1
            DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            DPRINT("IRP_MN_REMOVE_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_STOP_DEVICE: // 4
            DPRINT("IRP_MN_STOP_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_STOP_DEVICE: // 5
            DPRINT("IRP_MN_QUERY_STOP_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_CANCEL_STOP_DEVICE: // 6
            DPRINT("IRP_MN_CANCEL_STOP_DEVICE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_DEVICE_RELATIONS: // 7
            DPRINT("IRP_MN_QUERY_DEVICE_RELATIONS\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_INTERFACE: // 8
            DPRINT("IRP_MN_QUERY_INTERFACE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_CAPABILITIES:           // 9
            DPRINT("IRP_MN_QUERY_CAPABILITIES\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_RESOURCES:              // 10
            DPRINT("IRP_MN_QUERY_RESOURCES\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:  // 11
            DPRINT("IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_DEVICE_TEXT:            // 12
            DPRINT("IRP_MN_QUERY_DEVICE_TEXT\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS: // 13
            DPRINT("IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_READ_CONFIG: // 15
            DPRINT("IRP_MN_READ_CONFIG\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_WRITE_CONFIG: // 16
            DPRINT("IRP_MN_WRITE_CONFIG\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_EJECT: // 17
            DPRINT("IRP_MN_EJECT\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_SET_LOCK: // 18
            DPRINT("IRP_MN_SET_LOCK\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_ID: // 19
            DPRINT("IRP_MN_QUERY_ID\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_PNP_DEVICE_STATE: // 20
            DPRINT("IRP_MN_QUERY_PNP_DEVICE_STATE\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_QUERY_BUS_INFORMATION: // 21
            DPRINT("IRP_MN_QUERY_BUS_INFORMATION\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_DEVICE_USAGE_NOTIFICATION: // 22
            DPRINT("IRP_MN_DEVICE_USAGE_NOTIFICATION\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        case IRP_MN_SURPRISE_REMOVAL: // 23
            DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;

        default:
            DPRINT("unknown IRP_MN_???\n");
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
            break;
    }

    return Status;
}
