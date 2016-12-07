#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

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
            Status = USBH_PassIrp(HubExtension->LowerDevice, Irp);
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
