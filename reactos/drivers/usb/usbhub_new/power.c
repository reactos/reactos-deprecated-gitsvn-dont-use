#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_FdoPower(IN PUSBHUB_FDO_EXTENSION HubExtension,
              IN PIRP Irp,
              IN UCHAR Minor)
{
    NTSTATUS Status;

    DPRINT("USBH_FdoPower: HubExtension - %p, Irp - %p, Minor - %x\n",
           HubExtension,
           Irp,
           Minor);

    switch (Minor)
    {
      case IRP_MN_WAIT_WAKE:
          DPRINT("USBPORT_FdoPower: IRP_MN_WAIT_WAKE\n");
          break;

      case IRP_MN_POWER_SEQUENCE:
          DPRINT("USBPORT_FdoPower: IRP_MN_POWER_SEQUENCE\n");
          break;

      case IRP_MN_SET_POWER:
          DPRINT("USBPORT_FdoPower: IRP_MN_SET_POWER\n");
          break;

      case IRP_MN_QUERY_POWER:
          DPRINT("USBPORT_FdoPower: IRP_MN_QUERY_POWER\n");
          break;

      default:
          DPRINT1("USBPORT_FdoPower: unknown IRP_MN_POWER!\n");
          break;
    }

    IoCopyCurrentIrpStackLocationToNext(Irp);
    PoStartNextPowerIrp(Irp);
    Status = PoCallDriver(HubExtension->LowerDevice, Irp);

    return Status;
}

NTSTATUS
NTAPI
USBH_PdoPower(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
              IN PIRP Irp,
              IN UCHAR Minor)
{
    NTSTATUS Status=0;

    DPRINT("USBH_FdoPower: PortExtension - %p, Irp - %p, Minor - %x\n",
           PortExtension,
           Irp,
           Minor);

    switch (Minor)
    {
      case IRP_MN_WAIT_WAKE:
          DPRINT("USBPORT_PdoPower: IRP_MN_WAIT_WAKE\n");
          PoStartNextPowerIrp(Irp);
          break;

      case IRP_MN_POWER_SEQUENCE:
          DPRINT("USBPORT_PdoPower: IRP_MN_POWER_SEQUENCE\n");
          PoStartNextPowerIrp(Irp);
          break;

      case IRP_MN_SET_POWER:
          DPRINT("USBPORT_PdoPower: IRP_MN_SET_POWER\n");
          PoStartNextPowerIrp(Irp);
          break;

      case IRP_MN_QUERY_POWER:
          DPRINT("USBPORT_PdoPower: IRP_MN_QUERY_POWER\n");
          PoStartNextPowerIrp(Irp);
          break;

      default:
          DPRINT1("USBPORT_PdoPower: unknown IRP_MN_POWER!\n");
          PoStartNextPowerIrp(Irp);
          break;
    }

    Irp->IoStatus.Status = Status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return Status;
}
