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
USBPORT_HcWake(IN PDEVICE_OBJECT FdoDevice,
               IN PIRP Irp)
{
    DPRINT("USBPORT_HcWake: ... \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_DevicePowerState(IN PDEVICE_OBJECT FdoDevice,
                         IN PIRP Irp)
{
    DPRINT("USBPORT_DevicePowerState: ... \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_SystemPowerState(IN PDEVICE_OBJECT FdoDevice,
                         IN PIRP Irp)
{
    DPRINT("USBPORT_SystemPowerState: ... \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_FdoPower(IN PDEVICE_OBJECT FdoDevice,
                 IN PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status;

    DPRINT("USBPORT_FdoPower: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);

    switch (IoStack->MinorFunction)
    {
      case IRP_MN_WAIT_WAKE:
          DPRINT("USBPORT_FdoPower: IRP_MN_WAIT_WAKE\n");
          Status = USBPORT_HcWake(FdoDevice, Irp);
          return Status;

      case IRP_MN_POWER_SEQUENCE:
          DPRINT("USBPORT_FdoPower: IRP_MN_POWER_SEQUENCE\n");
          break;

      case IRP_MN_SET_POWER:
          DPRINT("USBPORT_FdoPower: IRP_MN_SET_POWER\n");
          if (IoStack->Parameters.Power.Type == DevicePowerState)
          {
              Status = USBPORT_DevicePowerState(FdoDevice, Irp);
          }
          else
          {
              Status = USBPORT_SystemPowerState(FdoDevice, Irp);
          }
      
          if (Status != STATUS_PENDING)
              break;

          return Status;

      case IRP_MN_QUERY_POWER:
          DPRINT("USBPORT_FdoPower: IRP_MN_QUERY_POWER\n");
          Irp->IoStatus.Status = STATUS_SUCCESS;
          break;

      default:
          DPRINT1("USBPORT_FdoPower: unknown IRP_MN_POWER!\n");
          break;
    }

    IoCopyCurrentIrpStackLocationToNext(Irp);
    PoStartNextPowerIrp(Irp);
    return PoCallDriver(FdoExtension->CommonExtension.LowerDevice, Irp);
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
