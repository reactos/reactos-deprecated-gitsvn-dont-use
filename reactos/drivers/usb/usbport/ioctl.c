#include "usbport.h"

//#define NDEBUG
#include <debug.h>


NTSTATUS
NTAPI
USBPORT_PdoDeviceControl(PDEVICE_OBJECT PdoDevice,
                         PIRP Irp)
{
    DPRINT("USBPORT_PdoDeviceControl: FIXME \n");
    ASSERT(FALSE);
    return 0;
}

NTSTATUS
NTAPI
USBPORT_FdoDeviceControl(PDEVICE_OBJECT FdoDevice,
                         PIRP Irp)
{
    DPRINT("USBPORT_FdoDeviceControl: FIXME \n");
    ASSERT(FALSE);
    return 0;
}

NTSTATUS
NTAPI
USBPORT_FdoScsi(IN PDEVICE_OBJECT FdoDevice,
                IN PIRP Irp)
{
    DPRINT("USBPORT_FdoScsi: FIXME \n");
    ASSERT(FALSE);
    return 0;
}
