#include "usbport.h"

//#define NDEBUG
#include <debug.h>

ULONG
NTAPI
USBPORT_GetHciMn(VOID)
{
    return 0;
}

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(PDRIVER_OBJECT DriverObject,
                              ULONG Version,
                              PUSBPORT_REGISTRATION_PACKET RegPacket)
{
    NTSTATUS Status;

    DPRINT("USBPORT_RegisterUSBPortDriver: DriverObject - %p, Version - %p, RegPacket - %p\n",
           DriverObject,
           Version,
           RegPacket);

    Status = STATUS_SUCCESS;
    return Status;
}

NTSTATUS
NTAPI
DriverEntry(PDRIVER_OBJECT DriverObject,
            PUNICODE_STRING RegistryPath)
{
    return STATUS_SUCCESS;
}
