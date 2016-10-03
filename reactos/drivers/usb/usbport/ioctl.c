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
    DPRINT1("USBPORT_FdoDeviceControl: UNIMPLEMENTED. FIXME. \n");
    return 0;
}

NTSTATUS
NTAPI
USBPORT_FdoScsi(IN PDEVICE_OBJECT FdoDevice,
                IN PIRP Irp)
{
    DPRINT1("USBPORT_FdoScsi: UNIMPLEMENTED. FIXME. \n");
    return 0;
}
