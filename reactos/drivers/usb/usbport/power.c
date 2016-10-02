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

VOID
NTAPI
USBPORT_AdjustDeviceCapabilities(IN PDEVICE_OBJECT FdoDevice,
                                 IN PDEVICE_OBJECT PdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_CAPABILITIES Capabilities;

    DPRINT("USBPORT_AdjustDeviceCapabilities: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    Capabilities = &PdoExtension->Capabilities;

    RtlCopyMemory(Capabilities, &FdoExtension->Capabilities, sizeof(DEVICE_CAPABILITIES));

    Capabilities->DeviceD1 = FALSE;
    Capabilities->DeviceD2 = TRUE;

    Capabilities->Removable = FALSE;
    Capabilities->UniqueID = FALSE;

    Capabilities->WakeFromD0 = TRUE;
    Capabilities->WakeFromD1 = FALSE;
    Capabilities->WakeFromD2 = TRUE;
    Capabilities->WakeFromD3 = FALSE;

    Capabilities->Address = 0;
    Capabilities->UINumber = 0;

    if (Capabilities->SystemWake == PowerSystemUnspecified)
        Capabilities->SystemWake = PowerSystemWorking;

    Capabilities->DeviceWake = PowerDeviceD2;

    Capabilities->DeviceState[PowerSystemSleeping1] = PowerDeviceD3;
    Capabilities->DeviceState[PowerSystemSleeping2] = PowerDeviceD3;
    Capabilities->DeviceState[PowerSystemSleeping3] = PowerDeviceD3;
    Capabilities->DeviceState[PowerSystemHibernate] = PowerDeviceD3;
}
