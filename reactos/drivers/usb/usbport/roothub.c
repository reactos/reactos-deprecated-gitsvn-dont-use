#include "usbport.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBPORT_CreateRhDevice(PDEVICE_OBJECT FdoDevice,
                       PDEVICE_OBJECT PdoDevice)
{
    DPRINT("USBPORT_RootHub_CreateDevice: FIXME! FdoDevice - %p, PdoDevice - %p\n",
           FdoDevice,
           PdoDevice);

    return 0;
}
