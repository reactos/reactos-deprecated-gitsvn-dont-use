#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_HubSetD0(IN PUSBHUB_FDO_EXTENSION HubExtension)
{
    DPRINT1("USBH_HubSetD0: UNIMPLEMENTED. FIXME. \n");
    DbgBreakPoint();
    return 0;
}

VOID
NTAPI
USBH_CompletePortWakeIrpsWorker(IN PUSBHUB_FDO_EXTENSION HubExtension,
                                IN PVOID Context)
{
    DPRINT1("USBH_CompletePortWakeIrpsWorker: UNIMPLEMENTED. FIXME. \n");
    DbgBreakPoint();
}

NTSTATUS
NTAPI
USBH_FdoWWIrpIoCompletion(IN PDEVICE_OBJECT DeviceObject,
                          IN PIRP Irp,
                          IN PVOID Context)
{
    PUSBHUB_FDO_EXTENSION HubExtension;
    NTSTATUS Status;
    KIRQL OldIrql;
    POWER_STATE PowerState;
    LONG WakeIrp;

    DPRINT("USBH_FdoWWIrpIoCompletion: DeviceObject - %p, Irp - %p\n",
            DeviceObject,
            Irp);

    HubExtension = (PUSBHUB_FDO_EXTENSION)Context;

    Status = Irp->IoStatus.Status;

    IoAcquireCancelSpinLock(&OldIrql);

    HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_PENDING_WAKE_IRP;

    WakeIrp = InterlockedExchange((PLONG)&HubExtension->PendingWakeIrp,
                                  0);

    if (!InterlockedDecrement(&HubExtension->PendingRequestCount))
    {
        KeSetEvent(&HubExtension->PendingRequestEvent,
                   EVENT_INCREMENT,
                   FALSE);
    }

    IoReleaseCancelSpinLock(OldIrql);

    DPRINT("USBH_FdoWWIrpIoCompletion: Status - %p\n", Status);

    if (!NT_SUCCESS(Status))
    {
        DPRINT1("USBH_FdoWWIrpIoCompletion: DbgBreakPoint() \n");
        DbgBreakPoint();
    }
    else
    {
        PowerState.DeviceState = PowerDeviceD0;

        HubExtension->HubFlags |= USBHUB_FDO_FLAG_WAKEUP_START;
        InterlockedIncrement(&HubExtension->PendingRequestCount);

        Status = STATUS_SUCCESS;

        PoRequestPowerIrp(HubExtension->LowerPDO,
                          IRP_MN_SET_POWER,
                          PowerState,
                          USBH_FdoPoRequestD0Completion,
                          (PVOID)HubExtension,
                          NULL);
    }

    if (!WakeIrp)
    {
        if (!InterlockedExchange(&HubExtension->FdoWaitWakeLock, 1))
        {
            Status = STATUS_MORE_PROCESSING_REQUIRED;
        }
    }

    DPRINT("USBH_FdoWWIrpIoCompletion: Status - %p\n", Status);

    IoMarkIrpPending(Irp);

    if (Status != STATUS_MORE_PROCESSING_REQUIRED)
    {
        PoStartNextPowerIrp(Irp);
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_PowerIrpCompletion(IN PDEVICE_OBJECT DeviceObject,
                        IN PIRP Irp,
                        IN PVOID Context)
{
    PUSBHUB_FDO_EXTENSION HubExtension;
    PIO_STACK_LOCATION IoStack;
    DEVICE_POWER_STATE OldDeviceState;
    NTSTATUS Status;
    POWER_STATE PowerState;

    DPRINT("USBH_PowerIrpCompletion: DeviceObject - %p, Irp - %p\n",
           DeviceObject,
           Irp);

    HubExtension = (PUSBHUB_FDO_EXTENSION)Context;

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    PowerState = IoStack->Parameters.Power.State;

    Status = Irp->IoStatus.Status;
    DPRINT("USBH_PowerIrpCompletion: Status - %p\n", Status);


    if (Irp->PendingReturned)
    {
        IoMarkIrpPending(Irp);
    }

    if (!NT_SUCCESS(Status))
    {
        if (PowerState.DeviceState == PowerDeviceD0)
        {
            PoStartNextPowerIrp(Irp);
            HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_SET_D0_STATE;
        }
    }
    else if (PowerState.DeviceState == PowerDeviceD0)
    {
        HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_SET_D0_STATE;

        OldDeviceState = HubExtension->CurrentPowerState.DeviceState;
        HubExtension->CurrentPowerState.DeviceState = PowerDeviceD0;

        DPRINT("USBH_PowerIrpCompletion: OldDeviceState - %x\n", OldDeviceState);

        if (HubExtension->HubFlags & USBHUB_FDO_FLAG_HIBERNATE_STATE)
        {
            DPRINT1("USBH_PowerIrpCompletion: USBHUB_FDO_FLAG_HIBERNATE_STATE. FIXME. \n");
            DbgBreakPoint();
        }

        HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_HIBERNATE_STATE;

        if (OldDeviceState == PowerDeviceD3)
        {
            DPRINT1("USBH_PowerIrpCompletion: PowerDeviceD3. FIXME. \n");
            DbgBreakPoint();
        }

        if (!(HubExtension->HubFlags & USBHUB_FDO_FLAG_DEVICE_STOPPED) &&
            HubExtension->HubFlags & USBHUB_FDO_FLAG_DO_ENUMERATION)
        {
            USBH_SubmitStatusChangeTransfer(HubExtension);
        }

        DPRINT("USBH_PowerIrpCompletion: Status - %p\n", Status);

        if (Status != STATUS_MORE_PROCESSING_REQUIRED)
        {
            PoStartNextPowerIrp(Irp);
            return Status;
        }
    }

    return Status;
}

VOID
NTAPI
USBH_FdoDeferPoRequestCompletion(IN PDEVICE_OBJECT DeviceObject,
                                 IN UCHAR MinorFunction,
                                 IN POWER_STATE PowerState,
                                 IN PVOID Context,
                                 IN PIO_STATUS_BLOCK IoStatus)
{
    DPRINT1("USBH_FdoDeferPoRequestCompletion: UNIMPLEMENTED. FIXME. \n");
    DbgBreakPoint();
}

NTSTATUS
NTAPI
USBH_FdoPower(IN PUSBHUB_FDO_EXTENSION HubExtension,
              IN PIRP Irp,
              IN UCHAR Minor)
{
    NTSTATUS Status;
    PIO_STACK_LOCATION IoStack;
    POWER_STATE PowerState;
    POWER_STATE DevicePwrState;
    BOOLEAN IsAllPortsD3;
    PUSBHUB_PORT_DATA PortData;
    PDEVICE_OBJECT PdoDevice;
    PUSBHUB_PORT_PDO_EXTENSION PortExtension;
    ULONG Port;

    DPRINT("USBH_FdoPower: HubExtension - %p, Irp - %p, Minor - %x\n",
           HubExtension,
           Irp,
           Minor);

    switch (Minor)
    {
      case IRP_MN_WAIT_WAKE:
          DPRINT("USBH_FdoPower: IRP_MN_WAIT_WAKE\n");
          IoCopyCurrentIrpStackLocationToNext(Irp);

          IoSetCompletionRoutine(Irp,
                                 USBH_FdoWWIrpIoCompletion,
                                 HubExtension,
                                 TRUE,
                                 TRUE,
                                 TRUE);

          PoStartNextPowerIrp(Irp);
          IoMarkIrpPending(Irp);

          PoCallDriver(HubExtension->LowerDevice, Irp);

          return STATUS_PENDING;

      case IRP_MN_POWER_SEQUENCE:
          DPRINT("USBH_FdoPower: IRP_MN_POWER_SEQUENCE\n");
          break;

      case IRP_MN_SET_POWER:
          DPRINT("USBH_FdoPower: IRP_MN_SET_POWER\n");

          IoStack = IoGetCurrentIrpStackLocation(Irp);
          DPRINT("USBH_FdoPower: IRP_MN_SET_POWER/DevicePowerState\n");
          PowerState = IoStack->Parameters.Power.State;

          if (IoStack->Parameters.Power.Type == DevicePowerState)
          {
              DPRINT("USBH_FdoPower: PowerState - %x\n", PowerState);

              if (HubExtension->CurrentPowerState.DeviceState == PowerState.DeviceState)
              {
                    IoCopyCurrentIrpStackLocationToNext(Irp);
                    PoStartNextPowerIrp(Irp);
                    IoMarkIrpPending(Irp);

                    PoCallDriver(HubExtension->LowerDevice, Irp);

                    return STATUS_PENDING;
              }

              switch (PowerState.DeviceState)
              {
                  case PowerDeviceD0:
                      if (!(HubExtension->HubFlags & USBHUB_FDO_FLAG_SET_D0_STATE))
                      {
                          HubExtension->HubFlags &= ~(USBHUB_FDO_FLAG_NOT_D0_STATE |
                                                      USBHUB_FDO_FLAG_DEVICE_STOPPING);

                          HubExtension->HubFlags |= USBHUB_FDO_FLAG_SET_D0_STATE;

                          IoCopyCurrentIrpStackLocationToNext(Irp);

                          IoSetCompletionRoutine(Irp,
                                                 USBH_PowerIrpCompletion,
                                                 HubExtension,
                                                 TRUE,
                                                 TRUE,
                                                 TRUE);
                      }
                      else
                      {
                          IoCopyCurrentIrpStackLocationToNext(Irp);
                          PoStartNextPowerIrp(Irp);
                      }

                      IoMarkIrpPending(Irp);
                      PoCallDriver(HubExtension->LowerDevice, Irp);
                      return STATUS_PENDING;

                  case PowerDeviceD1:
                  case PowerDeviceD2:
                  case PowerDeviceD3:
                      if (HubExtension->ResetRequestCount)
                      {
                          IoCancelIrp(HubExtension->ResetPortIrp);

                          KeWaitForSingleObject(&HubExtension->ResetEvent,
                                                Suspended,
                                                KernelMode,
                                                FALSE,
                                                NULL);
                      }

                      if (!(HubExtension->HubFlags & USBHUB_FDO_FLAG_DEVICE_STOPPED))
                      {
                          HubExtension->HubFlags |= (USBHUB_FDO_FLAG_NOT_D0_STATE |
                                                     USBHUB_FDO_FLAG_DEVICE_STOPPING);

                          IoCancelIrp(HubExtension->SCEIrp);

                          KeWaitForSingleObject(&HubExtension->StatusChangeEvent,
                                                Suspended,
                                                KernelMode,
                                                FALSE,
                                                NULL);
                      }

                      HubExtension->CurrentPowerState.DeviceState = PowerState.DeviceState;

                      if (HubExtension->HubFlags & USBHUB_FDO_FLAG_DO_SUSPENSE &&
                          USBH_CheckIdleAbort(HubExtension, TRUE, TRUE) == TRUE)
                      {
                          HubExtension->HubFlags &= ~(USBHUB_FDO_FLAG_NOT_D0_STATE |
                                                      USBHUB_FDO_FLAG_DEVICE_STOPPING);

                          HubExtension->CurrentPowerState.DeviceState = PowerDeviceD0;

                          USBH_SubmitStatusChangeTransfer(HubExtension);

                          PoStartNextPowerIrp(Irp);

                          Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
                          IoCompleteRequest(Irp, IO_NO_INCREMENT);

                          HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_DO_SUSPENSE;

                          KeReleaseSemaphore(&HubExtension->IdleSemaphore,
                                             LOW_REALTIME_PRIORITY,
                                             1,
                                             FALSE);

                          return STATUS_UNSUCCESSFUL;
                      }

                      IoCopyCurrentIrpStackLocationToNext(Irp);

                      IoSetCompletionRoutine(Irp,
                                             USBH_PowerIrpCompletion,
                                             HubExtension,
                                             TRUE,
                                             TRUE,
                                             TRUE);

                      PoStartNextPowerIrp(Irp);

                      IoMarkIrpPending(Irp);
                      PoCallDriver(HubExtension->LowerDevice, Irp);

                      if (HubExtension->HubFlags & USBHUB_FDO_FLAG_DO_SUSPENSE)
                      {
                          HubExtension->HubFlags &= ~USBHUB_FDO_FLAG_DO_SUSPENSE;

                          KeReleaseSemaphore(&HubExtension->IdleSemaphore,
                                             LOW_REALTIME_PRIORITY,
                                             1,
                                             FALSE);
                      }

                      return STATUS_PENDING;

                  default:
                      DPRINT1("USBH_FdoPower: Unsupported PowerState.DeviceState\n");
                      DbgBreakPoint();
                      break;
              }
          }
          else
          {
              if (PowerState.SystemState != PowerSystemWorking)
              {
                  USBH_GetRootHubExtension(HubExtension)->SystemPowerState.SystemState =
                                                          PowerState.SystemState;
              }

              if (PowerState.SystemState == PowerSystemHibernate)
              {
                  HubExtension->HubFlags |= USBHUB_FDO_FLAG_HIBERNATE_STATE;
              }

              PortData = HubExtension->PortData;

              IsAllPortsD3 = 1;

              if (PortData)
              {
                  if (HubExtension->HubDescriptor &&
                      HubExtension->HubDescriptor->bNumberOfPorts)
                  {
                      Port = 0;

                      while (TRUE)
                      {
                          PdoDevice = PortData[Port].DeviceObject;

                          if (PdoDevice)
                          {
                              PortExtension = (PUSBHUB_PORT_PDO_EXTENSION)PdoDevice->DeviceExtension;

                              if (PortExtension->CurrentPowerState.DeviceState != PowerDeviceD3)
                              {
                                  break;
                              }
                          }

                          ++Port;

                          if (Port >= HubExtension->HubDescriptor->bNumberOfPorts)
                          {
                              goto Next;
                          }
                      }

                      IsAllPortsD3 = FALSE;
                  }
              }

          Next:

              if (PowerState.SystemState == PowerSystemWorking)
              {
                  DevicePwrState.DeviceState = PowerDeviceD0;
              }
              else if (HubExtension->HubFlags & USBHUB_FDO_FLAG_PENDING_WAKE_IRP ||
                      !IsAllPortsD3)
              {
                  DevicePwrState.DeviceState = HubExtension->DeviceState[PowerState.SystemState];

                  if (DevicePwrState.DeviceState == PowerDeviceUnspecified)
                  {
                      goto Exit;
                  }
              }
              else
              {
                  DevicePwrState.DeviceState = PowerDeviceD3;
              }

              if (DevicePwrState.DeviceState != HubExtension->CurrentPowerState.DeviceState &&
                  HubExtension->HubFlags & USBHUB_FDO_FLAG_DEVICE_STARTED)
              {
                  HubExtension->PowerIrp = Irp;

                  IoMarkIrpPending(Irp);

                  if (PoRequestPowerIrp(HubExtension->LowerPDO,
                                        IRP_MN_SET_POWER,
                                        DevicePwrState,
                                        USBH_FdoDeferPoRequestCompletion,
                                        (PVOID)HubExtension,
                                        0) == STATUS_PENDING)
                  {
                      return STATUS_PENDING;
                  }

                  IoCopyCurrentIrpStackLocationToNext(Irp);
                  PoStartNextPowerIrp(Irp);
                  PoCallDriver(HubExtension->LowerDevice, Irp);

                  return STATUS_PENDING;
              }

          Exit:

              HubExtension->SystemPowerState.SystemState = PowerState.SystemState;

              if (PowerState.SystemState == PowerSystemWorking)
              {
                  USBH_CheckIdleDeferred(HubExtension);
              }

              IoCopyCurrentIrpStackLocationToNext(Irp);
              PoStartNextPowerIrp(Irp);

              return PoCallDriver(HubExtension->LowerDevice, Irp);
          }

          break;

      case IRP_MN_QUERY_POWER:
          DPRINT("USBH_FdoPower: IRP_MN_QUERY_POWER\n");
          break;

      default:
          DPRINT1("USBH_FdoPower: unknown IRP_MN_POWER!\n");
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
