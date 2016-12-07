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

    return 0;
}
