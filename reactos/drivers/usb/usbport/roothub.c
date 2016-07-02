#include "usbport.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
USBPORT_CreateRhDevice(IN PDEVICE_OBJECT FdoDevice,
                       IN PDEVICE_OBJECT PdoDevice)
{
    DPRINT("USBPORT_RootHub_CreateDevice: FIXME! FdoDevice - %p, PdoDevice - %p\n",
           FdoDevice,
           PdoDevice);

    return 0;
}
