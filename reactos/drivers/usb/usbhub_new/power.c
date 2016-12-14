#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_FdoPower(IN PUSBHUB_FDO_EXTENSION HubExtension,
              IN PIRP Irp,
              IN UCHAR Minor)
{
    DPRINT("USBH_FdoPower: HubExtension - %p, Irp - %p, Minor - %x\n",
           HubExtension,
           Irp,
           Minor);

    DbgBreakPoint();

    return 0;
}

NTSTATUS
NTAPI
USBH_PdoPower(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
              IN PIRP Irp,
              IN UCHAR Minor)
{
    DPRINT("USBH_FdoPower: PortExtension - %p, Irp - %p, Minor - %x\n",
           PortExtension,
           Irp,
           Minor);

    DbgBreakPoint();

    return 0;
}
