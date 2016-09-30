#include "usbport.h"

//#define NDEBUG
#include <debug.h>


NTSTATUS
NTAPI
USBPORT_PdoPower(IN PDEVICE_OBJECT PdoDevice,
                 IN PIRP Irp)
{
    DPRINT("USBPORT_PdoPower: FIXME \n");
    ASSERT(FALSE);
    return 0;
}

NTSTATUS
NTAPI
USBPORT_FdoPower(IN PDEVICE_OBJECT FdoDevice,
                 IN PIRP Irp)
{
    DPRINT("USBPORT_FdoPower: FIXME \n");
    ASSERT(FALSE);
    return 0;
}

NTSTATUS
NTAPI
USBPORT_IdleNotification(IN PDEVICE_OBJECT PdoDevice,
                         IN PIRP Irp)
{
    NTSTATUS Status;

    DPRINT("USBPORT_IdleNotification: ... \n");

    ASSERT(FALSE);

    Status = STATUS_NOT_SUPPORTED;
    return Status;
}
