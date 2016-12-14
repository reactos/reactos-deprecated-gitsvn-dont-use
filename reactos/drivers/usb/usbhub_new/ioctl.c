#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_PdoUrbFilter(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension, 
                  IN PIRP Irp)
{
    DPRINT1("USBH_PdoUrbFilter: UNIMPLEMENTED. FIXME. \n");
    DbgBreakPoint();
    return 0;
}

NTSTATUS
NTAPI
USBH_PdoIoctlSubmitUrb(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
                       IN PIRP Irp)
{
    PIO_STACK_LOCATION IoStack;
    PUSBHUB_FDO_EXTENSION HubExtension;
    PURB Urb;
    NTSTATUS Status;

    DPRINT("USBH_PdoIoctlSubmitUrb ... \n");

    HubExtension = (PUSBHUB_FDO_EXTENSION)PortExtension->HubExtension;
    IoStack = Irp->Tail.Overlay.CurrentStackLocation;

    Urb = (PURB)IoStack->Parameters.Others.Argument1;

    if (PortExtension->DeviceHandle == NULL)
    {
        Urb->UrbHeader.UsbdDeviceHandle = (PVOID)-1;
        Status = USBH_PassIrp(Irp, HubExtension->RootHubPdo2);
    }
    else
    {
        Urb->UrbHeader.UsbdDeviceHandle = PortExtension->DeviceHandle;
        Status = USBH_PdoUrbFilter(PortExtension, Irp);
    }

    USBH_CompleteIrp(Irp, Status);

    return Status;
}

NTSTATUS
NTAPI
USBH_DeviceControl(IN PUSBHUB_FDO_EXTENSION HubExtension,
                   IN PIRP Irp)
{
    DPRINT("USBH_DeviceControl: HubExtension - %p, Irp - %p\n",
           HubExtension,
           Irp);

    return 0;
}

NTSTATUS
NTAPI
USBH_PdoInternalControl(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
                        IN PIRP Irp)
{
    PUSBHUB_FDO_EXTENSION HubExtension;
    NTSTATUS Status = STATUS_NOT_SUPPORTED;
    ULONG ControlCode;
    PIO_STACK_LOCATION IoStack;

    DPRINT("USBH_PdoInternalControl: PortExtension - %p, Irp - %p\n",
           PortExtension,
           Irp);

    HubExtension = PortExtension->HubExtension;

    if (PortExtension->PortPdoFlags & USBHUB_PDO_FLAG_NOT_CONNECTED)
    {
        Status = STATUS_DEVICE_NOT_CONNECTED;
        goto Exit;
    }

    if (PortExtension->CurrentPowerState.DeviceState != PowerDeviceD0)
    {
        Status = STATUS_DEVICE_POWERED_OFF;
        goto Exit;
    }

    IoStack = Irp->Tail.Overlay.CurrentStackLocation;
    ControlCode = IoStack->Parameters.DeviceIoControl.IoControlCode;
    DPRINT("USBH_PdoInternalControl: ControlCode - %p\n", ControlCode);

    if (ControlCode == IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO)
    {
        HubExtension = PortExtension->RootHubExtension;
        DPRINT("USBH_PdoInternalControl: HubExtension - %p\n", HubExtension);
    }

    if (!HubExtension)
    {
        Status = STATUS_DEVICE_BUSY;
        goto Exit;
    }

    switch (ControlCode)
    {
        case IOCTL_INTERNAL_USB_SUBMIT_URB:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_SUBMIT_URB. \n");
            return USBH_PdoIoctlSubmitUrb(PortExtension, Irp);

        case IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_PORT_STATUS:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_PORT_STATUS. \n");
            break;

        case IOCTL_INTERNAL_USB_RESET_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_RESET_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_ENABLE_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_ENABLE_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_CYCLE_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_CYCLE_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_HUB_COUNT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_HUB_COUNT. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_HUB_NAME:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_HUB_NAME. \n");
            break;

        case IOCTL_GET_HCD_DRIVERKEY_NAME:
            DPRINT("USBH_PdoInternalControl: IOCTL_GET_HCD_DRIVERKEY_NAME. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_BUS_INFO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_BUS_INFO. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_PARENT_HUB_INFO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_PARENT_HUB_INFO. \n");
            break;

        default:
            DPRINT("USBH_PdoInternalControl: IOCTL_ ???\n");
            break;
    }

Exit:
    USBH_CompleteIrp(Irp, Status);
    return Status;
}
