#include "usbport.h"

//#define NDEBUG
#include <debug.h>

VOID
NTAPI
USBPORT_SuspendController(IN PDEVICE_OBJECT FdoDevice)
{
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    KIRQL OldIrql;

    DPRINT("USBPORT_SuspendController \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    FdoExtension->TimerFlags |= USBPORT_TMFLAG_RH_SUSPENDED;

    USBPORT_FlushController(FdoDevice);

    if (!(FdoExtension->Flags & USBPORT_FLAG_HC_SUSPEND))
    {
        KeAcquireSpinLock(&FdoExtension->TimerFlagsSpinLock, &OldIrql);
        FdoExtension->TimerFlags |= USBPORT_TMFLAG_HC_SUSPENDED;
        KeReleaseSpinLock(&FdoExtension->TimerFlagsSpinLock, OldIrql);

        if (FdoExtension->MiniPortFlags & USBPORT_MPFLAG_INTERRUPTS_ENABLED)
        {
            FdoExtension->MiniPortFlags |= USBPORT_MPFLAG_SUSPENDED;

            USBPORT_Wait(FdoDevice, 10);
            FdoExtension->MiniPortInterface->Packet.SuspendController(FdoExtension->MiniPortExt);
        }

        KeAcquireSpinLock(&FdoExtension->MiniportSpinLock, &OldIrql);
        FdoExtension->Flags |= USBPORT_FLAG_HC_SUSPEND;
        KeReleaseSpinLock(&FdoExtension->MiniportSpinLock, OldIrql);
    }
}

NTSTATUS
NTAPI
USBPORT_ResumeController(IN PDEVICE_OBJECT FdoDevice)
{
    NTSTATUS Status = STATUS_UNSUCCESSFUL;
    PUSBPORT_DEVICE_EXTENSION  FdoExtension;
    KIRQL OldIrql;
    MPSTATUS MpStatus;

    DPRINT("USBPORT_ResumeController: ... \n");

    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    if (FdoExtension->Flags & USBPORT_FLAG_HC_SUSPEND)
    {
        KeAcquireSpinLock(&FdoExtension->TimerFlagsSpinLock, &OldIrql);
        FdoExtension->TimerFlags &= ~(USBPORT_TMFLAG_HC_SUSPENDED | USBPORT_TMFLAG_RH_SUSPENDED);
        KeReleaseSpinLock(&FdoExtension->TimerFlagsSpinLock, OldIrql);

        if (FdoExtension->MiniPortFlags & USBPORT_MPFLAG_SUSPENDED)
        {
            FdoExtension->MiniPortFlags &= ~USBPORT_MPFLAG_SUSPENDED;

            if (FdoExtension->MiniPortInterface->Packet.ResumeController(FdoExtension->MiniPortExt))
            {
                KeAcquireSpinLock(&FdoExtension->TimerFlagsSpinLock, &OldIrql);
                FdoExtension->TimerFlags |= (USBPORT_TMFLAG_HC_SUSPENDED | 0x00000004);
                KeReleaseSpinLock(&FdoExtension->TimerFlagsSpinLock, OldIrql);

                USBPORT_MiniportInterrupts(FdoDevice, FALSE);

                FdoExtension->MiniPortInterface->Packet.StopController(FdoExtension->MiniPortExt, 1);

                USBPORT_NukeAllEndpoints(FdoDevice);

                RtlZeroMemory(FdoExtension->MiniPortExt, FdoExtension->MiniPortInterface->Packet.MiniPortExtensionSize);
                RtlZeroMemory(FdoExtension->UsbPortResources.StartVA, FdoExtension->MiniPortInterface->Packet.MiniPortResourcesSize);

                FdoExtension->UsbPortResources.LegacySupport &= ~0xFF00;
                FdoExtension->UsbPortResources.LegacySupport |= 0x0100;// = 1;

                MpStatus = FdoExtension->MiniPortInterface->Packet.StartController(FdoExtension->MiniPortExt, &FdoExtension->UsbPortResources);

                FdoExtension->UsbPortResources.LegacySupport &= ~0xFF00;// = 0;

                if (!MpStatus)
                {
                    USBPORT_MiniportInterrupts(FdoDevice, TRUE);
                }

                KeAcquireSpinLock(&FdoExtension->TimerFlagsSpinLock, &OldIrql);
                FdoExtension->TimerFlags &= ~(USBPORT_TMFLAG_HC_SUSPENDED | 0x00000004 | USBPORT_TMFLAG_RH_SUSPENDED);
                KeReleaseSpinLock(&FdoExtension->TimerFlagsSpinLock, OldIrql);
            }

            Status = USBPORT_Wait(FdoDevice, 100);
        }

        FdoExtension->Flags &= ~USBPORT_FLAG_HC_SUSPEND;
    }

    return Status;
}

NTSTATUS
NTAPI
USBPORT_PdoDevicePowerState(IN PDEVICE_OBJECT PdoDevice,
                            IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PIO_STACK_LOCATION IoStack;
    NTSTATUS Status = STATUS_SUCCESS;
    POWER_STATE State;

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);

    State = IoStack->Parameters.Power.State;

    DPRINT1("USBPORT_PdoDevicePowerState: Irp - %p, State - %x\n", Irp, State.DeviceState);

    if (State.DeviceState == PowerDeviceD0)
    {
        DPRINT1("USBPORT_PdoDevicePowerState: PowerDeviceD0.\n");

        if (FdoExtension->CommonExtension.DevicePowerState == PowerDeviceD0)
        {
            while ((FdoExtension->Flags & 0x00000020) || FdoExtension->SetPowerLockCounter)
            {
                USBPORT_Wait(FdoDevice, 10);
            }

            USBPORT_ResumeController(FdoDevice);

            PdoExtension->CommonExtension.DevicePowerState = PowerDeviceD0;

            USBPORT_CompletePdoWaitWake(FdoDevice);
            USBPORT_CompletePendingIdleIrp(PdoDevice);
        }
        else
        {
            DPRINT1("USBPORT_PdoDevicePowerState: FdoExtension->Flags - %p\n", FdoExtension->Flags);
            DbgBreakPoint();
            Status = STATUS_UNSUCCESSFUL;
        }
    }
    else if (State.DeviceState  == PowerDeviceD1 ||
             State.DeviceState  == PowerDeviceD2 ||
             State.DeviceState  == PowerDeviceD3)
    {
        FdoExtension->TimerFlags |= USBPORT_TMFLAG_WAKE;
        USBPORT_SuspendController(FdoDevice);
        PdoExtension->CommonExtension.DevicePowerState = State.DeviceState;
    }

    return Status;
}

VOID
NTAPI
USBPORT_CancelPendingWakeIrp(IN PDEVICE_OBJECT DeviceObject,
                             IN PIRP Irp)
{
    DPRINT1("USBPORT_CancelPendingWakeIrp: UNIMPLEMENTED. FIXME. \n");
}

NTSTATUS
NTAPI
USBPORT_PdoPower(IN PDEVICE_OBJECT PdoDevice,
                 IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PIO_STACK_LOCATION IoStack;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PDRIVER_CANCEL OldCancelRoutine;
    NTSTATUS Status;
    KIRQL OldIrql;

    DPRINT("USBPORT_PdoPower: Irp - %p\n", Irp);

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    IoStack = IoGetCurrentIrpStackLocation(Irp);

    Status = Irp->IoStatus.Status;

    switch (IoStack->MinorFunction)
    {
      case IRP_MN_WAIT_WAKE:
          DPRINT("USBPORT_PdoPower: IRP_MN_WAIT_WAKE\n");

          if (!(FdoExtension->Flags & USBPORT_FLAG_DEVICE_STARTED))
          {
              Status = STATUS_NOT_SUPPORTED;
              break;
          }

          KeAcquireSpinLock(&FdoExtension->PowerWakeSpinLock, &OldIrql);

          OldCancelRoutine = IoSetCancelRoutine(Irp, USBPORT_CancelPendingWakeIrp);

          if (Irp->Cancel && InterlockedExchange((PLONG)OldCancelRoutine, 0))
          {
              KeReleaseSpinLock(&FdoExtension->PowerWakeSpinLock, OldIrql);

              DPRINT("USBPORT_PdoPower: IRP_MN_WAIT_WAKE - STATUS_CANCELLED\n");
              Status = STATUS_CANCELLED;
              break;
          }

          if (!PdoExtension->WakeIrp)
          {
              DPRINT("USBPORT_PdoPower: IRP_MN_WAIT_WAKE - No WakeIrp\n");

              IoMarkIrpPending(Irp);
              PdoExtension->WakeIrp = Irp;

              KeReleaseSpinLock(&FdoExtension->PowerWakeSpinLock, OldIrql);
              return STATUS_PENDING;
          }

          if (InterlockedExchange((PLONG)OldCancelRoutine, 0))
          {
              DPRINT("USBPORT_PdoPower: IRP_MN_WAIT_WAKE - STATUS_DEVICE_BUSY\n");

              KeReleaseSpinLock(&FdoExtension->PowerWakeSpinLock, OldIrql);
              PoStartNextPowerIrp(Irp);
              Status = STATUS_DEVICE_BUSY;
              break;
          }

          KeReleaseSpinLock(&FdoExtension->PowerWakeSpinLock, OldIrql);
          return Status;

      case IRP_MN_POWER_SEQUENCE:
          DPRINT("USBPORT_PdoPower: IRP_MN_POWER_SEQUENCE\n");
          PoStartNextPowerIrp(Irp);
          break;

      case IRP_MN_SET_POWER:
          DPRINT("USBPORT_PdoPower: IRP_MN_SET_POWER\n");

          if (IoStack->Parameters.Power.Type == DevicePowerState)
          {
              DPRINT("USBPORT_PdoPower: IRP_MN_SET_POWER/DevicePowerState\n");
              Status = USBPORT_PdoDevicePowerState(PdoDevice, Irp);
          }
          else
          {
              DPRINT("USBPORT_PdoPower: IRP_MN_SET_POWER/SystemPowerState \n");

              if (IoStack->Parameters.Power.State.SystemState == PowerSystemWorking)
              {
                  FdoExtension->TimerFlags |= USBPORT_TMFLAG_WAKE;
              }
              else
              {
                  FdoExtension->TimerFlags &= ~USBPORT_TMFLAG_WAKE;
              }

              Status = STATUS_SUCCESS;
          }

          PoStartNextPowerIrp(Irp);
          break;

      case IRP_MN_QUERY_POWER:
          DPRINT("USBPORT_PdoPower: IRP_MN_QUERY_POWER\n");
          Status = STATUS_SUCCESS;
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

NTSTATUS
NTAPI
USBPORT_HcWake(IN PDEVICE_OBJECT FdoDevice,
               IN PIRP Irp)
{
    DPRINT1("USBPORT_HcWake: UNIMPLEMENTED. FIXME. \n");
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_DevicePowerState(IN PDEVICE_OBJECT FdoDevice,
                         IN PIRP Irp)
{
    DPRINT1("USBPORT_DevicePowerState: UNIMPLEMENTED. FIXME. \n");
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_SystemPowerState(IN PDEVICE_OBJECT FdoDevice,
                         IN PIRP Irp)
{
    DPRINT1("USBPORT_SystemPowerState: UNIMPLEMENTED. FIXME. \n");
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

VOID
NTAPI
USBPORT_DoIdleNotificationCallback(IN PVOID Context)
{
    PIO_STACK_LOCATION IoStack;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PIRP NextIrp;
    LARGE_INTEGER CurrentTime = {{0, 0}};
    PTIMER_WORK_QUEUE_ITEM IdleQueueItem;
    PDEVICE_OBJECT PdoDevice;
    PUSB_IDLE_CALLBACK_INFO IdleCallbackInfo;
    KIRQL OldIrql;

    DPRINT("USBPORT_DoIdleNotificationCallback \n");

    IdleQueueItem = (PTIMER_WORK_QUEUE_ITEM)Context;

    FdoDevice = IdleQueueItem->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PdoDevice = FdoExtension->RootHubPdo;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    KeQuerySystemTime(&CurrentTime);

    if ((FdoExtension->IdleTime.QuadPart == 0) ||
        (((CurrentTime.QuadPart - FdoExtension->IdleTime.QuadPart) / 10000) >= 500))
    {
        if (PdoExtension->CommonExtension.DevicePowerState == PowerDeviceD0 &&
            FdoExtension->CommonExtension.DevicePowerState == PowerDeviceD0)
        {
            NextIrp = IoCsqRemoveNextIrp(&FdoExtension->IdleIoCsq, NULL);

            if (NextIrp)
            {
                IoStack = IoGetCurrentIrpStackLocation(NextIrp);
                IdleCallbackInfo = (PUSB_IDLE_CALLBACK_INFO)IoStack->Parameters.DeviceIoControl.Type3InputBuffer;

                if (IdleCallbackInfo && IdleCallbackInfo->IdleCallback)
                {
                    //DbgBreakPoint();
                    IdleCallbackInfo->IdleCallback(IdleCallbackInfo->IdleContext);
                }

                if (NextIrp->Cancel)
                {
                    InterlockedDecrement(&FdoExtension->IdleLockCounter);

                    NextIrp->IoStatus.Status = STATUS_CANCELLED;
                    NextIrp->IoStatus.Information = 0;
                    IoCompleteRequest(NextIrp, IO_NO_INCREMENT);
                }
                else
                {
                    IoCsqInsertIrp(&FdoExtension->IdleIoCsq, NextIrp, NULL);
                }
            }
        }
    }

    KeAcquireSpinLock(&FdoExtension->TimerFlagsSpinLock, &OldIrql);
    FdoExtension->TimerFlags &= ~USBPORT_TMFLAG_IDLE_QUEUEITEM_ON;
    KeReleaseSpinLock(&FdoExtension->TimerFlagsSpinLock, OldIrql);

    ExFreePool(IdleQueueItem);
}

NTSTATUS
NTAPI
USBPORT_IdleNotification(IN PDEVICE_OBJECT PdoDevice,
                         IN PIRP Irp)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    LONG LockCounter;
    NTSTATUS Status = STATUS_PENDING;

    DPRINT("USBPORT_IdleNotification: Irp - %p\n", Irp);

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    LockCounter = InterlockedIncrement(&FdoExtension->IdleLockCounter);

    if (LockCounter != 0)
    {
        if (Status != STATUS_PENDING)
        {
            InterlockedDecrement(&FdoExtension->IdleLockCounter);

            Irp->IoStatus.Status = Status;
            Irp->IoStatus.Information = 0;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);

            return Status;
        }

        Status = STATUS_DEVICE_BUSY;
    }

    if (Status != STATUS_PENDING)
    {
        InterlockedDecrement(&FdoExtension->IdleLockCounter);

        Irp->IoStatus.Status = Status;
        Irp->IoStatus.Information = 0;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);

        return Status;
    }

    Irp->IoStatus.Status = STATUS_PENDING;
    IoMarkIrpPending(Irp);

    KeQuerySystemTime(&FdoExtension->IdleTime);

    IoCsqInsertIrp(&FdoExtension->IdleIoCsq, Irp, 0);

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
