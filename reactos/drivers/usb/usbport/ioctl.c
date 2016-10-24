#include "usbport.h"

//#define NDEBUG
#include <debug.h>


NTSTATUS
NTAPI
USBPORT_PdoDeviceControl(PDEVICE_OBJECT PdoDevice,
                         PIRP Irp)
{
    DPRINT1("USBPORT_PdoDeviceControl: UNIMPLEMENTED. FIXME. \n");
    return 0;
}

NTSTATUS
NTAPI
USBPORT_FdoDeviceControl(PDEVICE_OBJECT FdoDevice,
                         PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    ULONG ControlCode;
    NTSTATUS Status = STATUS_INVALID_DEVICE_REQUEST;

    DPRINT("USBPORT_FdoDeviceControl: Irp - %p\n", Irp);

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    ControlCode = IoStack->Parameters.DeviceIoControl.IoControlCode;

    switch (ControlCode)
    {
        case IOCTL_USB_DIAGNOSTIC_MODE_ON: // 220400
            DPRINT("USBPORT_FdoDeviceControl: IOCTL_USB_DIAGNOSTIC_MODE_ON\n");
            FdoExtension->Flags |= USBPORT_FLAG_DIAGNOSTIC_MODE;
            break;

        case IOCTL_USB_DIAGNOSTIC_MODE_OFF: // 0x220404
            DPRINT("USBPORT_FdoDeviceControl: IOCTL_USB_DIAGNOSTIC_MODE_OFF\n");
            FdoExtension->Flags &= ~USBPORT_FLAG_DIAGNOSTIC_MODE;
            break;

        case IOCTL_USB_GET_NODE_INFORMATION: // 0x220408
            DPRINT1("USBPORT_FdoDeviceControl: IOCTL_USB_GET_NODE_INFORMATION UNIMPLEMENTED. FIXME\n");
            break;

        case IOCTL_GET_HCD_DRIVERKEY_NAME: // 0x220424
            DPRINT1("USBPORT_FdoDeviceControl: IOCTL_GET_HCD_DRIVERKEY_NAME UNIMPLEMENTED. FIXME\n");
            break;

        case IOCTL_USB_USER_REQUEST: // 0x220438
            DPRINT1("USBPORT_FdoDeviceControl: IOCTL_USB_USER_REQUEST UNIMPLEMENTED. FIXME\n");
            break;

        default:
            DPRINT1("USBPORT_FdoDeviceControl: Not supported IoControlCode - %x\n",
                    ControlCode);
            Status = STATUS_INVALID_DEVICE_REQUEST;
            break;
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_FdoScsi(IN PDEVICE_OBJECT FdoDevice,
                IN PIRP Irp)
{
    DPRINT1("USBPORT_FdoScsi: UNIMPLEMENTED. FIXME. \n");
    return 0;
}
