#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

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
