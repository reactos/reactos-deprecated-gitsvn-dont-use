#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_SelectConfigOrInterfaceComplete(IN PDEVICE_OBJECT DeviceObject,
                                     IN PIRP Irp,
                                     IN PVOID Context)
{
    PUSBHUB_PORT_PDO_EXTENSION PortExtension;
    PUSBHUB_FDO_EXTENSION HubExtension;
    PVOID TimeoutContext; // PUSBHUB_BANDWIDTH_TIMEOUT_CONTEXT
    PUSBHUB_PORT_DATA PortData = NULL;
    NTSTATUS Status;
    KIRQL OldIrql;

    DPRINT("USBH_SelectConfigOrInterfaceComplete ... \n");

    PortData;

    if (Irp->PendingReturned)
    {
         IoMarkIrpPending(Irp);
    }

    PortExtension = (PUSBHUB_PORT_PDO_EXTENSION)Context;
    HubExtension = PortExtension->HubExtension;

    if (HubExtension)
    {
        PortData = &HubExtension->PortData[PortExtension->PortNumber - 1];
    }

    Status = Irp->IoStatus.Status;

    if (Irp->IoStatus.Status == STATUS_SUCCESS)
    {
        KeAcquireSpinLock(&PortExtension->PortTimeoutSpinLock, &OldIrql);

        TimeoutContext = PortExtension->BndwTimeoutContext;

        if (TimeoutContext)
        {
            DPRINT1("USBH_SelectConfigOrInterfaceComplete: TimeoutContext != NULL. FIXME. \n");
            DbgBreakPoint();
        }

        KeReleaseSpinLock(&PortExtension->PortTimeoutSpinLock, OldIrql);

        PortExtension->PortPdoFlags &= ~(USBHUB_PDO_FLAG_PORT_RESTORE_FAIL |
                                         USBHUB_PDO_FLAG_ALLOC_BNDW_FAILED);

        if (PortData && PortData->ConnectionStatus != DeviceHubNestedTooDeeply)
        {
            PortData->ConnectionStatus = DeviceConnected;
        }
    }
    else
    {
        DPRINT1("USBH_SelectConfigOrInterfaceComplete: Status != STATUS_SUCCESS. FIXME. \n");
        DbgBreakPoint();
    }

    return Status;
}

NTSTATUS
NTAPI
USBH_PdoUrbFilter(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension, 
                  IN PIRP Irp)
{
    PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor;
    PUSBHUB_FDO_EXTENSION HubExtension;
    PDEVICE_OBJECT DeviceObject;
    PIO_STACK_LOCATION IoStack;
    PURB Urb;
    USHORT Function;
    ULONG MaxPower;
    USBD_STATUS UrbStatus;
    BOOLEAN IsValidConfig;

    HubExtension = PortExtension->HubExtension;
    DeviceObject = PortExtension->Common.SelfDevice;

    IoStack = IoGetCurrentIrpStackLocation(Irp);
    Urb = (PURB)IoStack->Parameters.Others.Argument1;

    DPRINT("USBH_PdoUrbFilter: Device - %p, Irp - %p, Urb - %p\n",
           DeviceObject,
           Irp,
           Urb);

    if (PortExtension->PortPdoFlags & (USBHUB_PDO_FLAG_PORT_RESTORE_FAIL |
                                       USBHUB_PDO_FLAG_PORT_RESSETING))
    {
        Urb->UrbHeader.Status = USBD_STATUS_INVALID_PARAMETER;
        USBH_CompleteIrp(Irp, STATUS_INVALID_PARAMETER);
        return STATUS_INVALID_PARAMETER;
    }

    Function = Urb->UrbHeader.Function;

    if (Function == URB_FUNCTION_SELECT_CONFIGURATION)
    {
        ConfigDescriptor = Urb->UrbSelectConfiguration.ConfigurationDescriptor;

        if (ConfigDescriptor)
        {
            IsValidConfig = TRUE;

            if (ConfigDescriptor->bDescriptorType != USB_CONFIGURATION_DESCRIPTOR_TYPE)
            {
                IsValidConfig = FALSE;
                UrbStatus = USBD_STATUS_INVALID_CONFIGURATION_DESCRIPTOR;
            }

            if (ConfigDescriptor->bLength < sizeof(USB_CONFIGURATION_DESCRIPTOR))
            {
                IsValidConfig = FALSE;
                UrbStatus = USBD_STATUS_INVALID_CONFIGURATION_DESCRIPTOR;
            }

            if (!IsValidConfig)
            {
                Urb->UrbHeader.Status = UrbStatus;
                USBH_CompleteIrp(Irp, STATUS_INVALID_PARAMETER);
                return STATUS_INVALID_PARAMETER;
            }

            MaxPower = 2 * ConfigDescriptor->MaxPower;
            PortExtension->MaxPower = MaxPower;

            if (HubExtension->MaxPower < MaxPower)
            {
                DPRINT1("USBH_PdoUrbFilter: USBH_InvalidatePortDeviceState() UNIMPLEMENTED. FIXME. \n");

                DbgBreakPoint();
                PortExtension->PortPdoFlags |= USBHUB_PDO_FLAG_INSUFFICIENT_PWR;
                //USBH_InvalidatePortDeviceState()
                USBH_CompleteIrp(Irp, STATUS_INVALID_PARAMETER);
                return STATUS_INVALID_PARAMETER;
            }
        }
    }

    if (Function == URB_FUNCTION_SELECT_CONFIGURATION ||
        Function == URB_FUNCTION_SELECT_INTERFACE)
    {
        IoCopyCurrentIrpStackLocationToNext(Irp);

        IoSetCompletionRoutine(Irp,
                               USBH_SelectConfigOrInterfaceComplete,
                               PortExtension,
                               TRUE,
                               TRUE,
                               TRUE);

        return IoCallDriver(HubExtension->RootHubPdo2, Irp);
    }

    if (Function == URB_FUNCTION_ABORT_PIPE ||
        Function == URB_FUNCTION_TAKE_FRAME_LENGTH_CONTROL ||
        Function == URB_FUNCTION_RELEASE_FRAME_LENGTH_CONTROL ||
        Function == URB_FUNCTION_GET_FRAME_LENGTH ||
        Function == URB_FUNCTION_SET_FRAME_LENGTH ||
        Function == URB_FUNCTION_GET_CURRENT_FRAME_NUMBER)
    {
        return USBH_PassIrp(HubExtension->RootHubPdo2, Irp);
    }

    if (Function == URB_FUNCTION_CONTROL_TRANSFER ||
        Function == URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER ||
        Function == URB_FUNCTION_ISOCH_TRANSFER)
    {
        if (PortExtension->PortPdoFlags & USBHUB_PDO_FLAG_DELETE_PENDING)
        {
            Urb->UrbHeader.Status = USBD_STATUS_INVALID_PARAMETER;
            USBH_CompleteIrp(Irp, STATUS_DELETE_PENDING);
            return STATUS_DELETE_PENDING;
        }

        return USBH_PassIrp(HubExtension->RootHubPdo2, Irp);
    }

    if (Function != URB_FUNCTION_GET_MS_FEATURE_DESCRIPTOR)
    {
        return USBH_PassIrp(HubExtension->RootHubPdo2, Irp);
    }
    else
    {
        DPRINT1("USBH_PdoUrbFilter: URB_FUNCTION_GET_MS_FEATURE_DESCRIPTOR UNIMPLEMENTED. FIXME. \n");
        DbgBreakPoint();
        USBH_CompleteIrp(Irp, STATUS_NOT_IMPLEMENTED);
        return STATUS_NOT_IMPLEMENTED;
    }
}

NTSTATUS
NTAPI
USBH_PdoIoctlSubmitUrb(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
                       IN PIRP Irp)
{
    PIO_STACK_LOCATION IoStack;
    PUSBHUB_FDO_EXTENSION HubExtension;
    PURB Urb;
    NTSTATUS Status;

    DPRINT("USBH_PdoIoctlSubmitUrb ... \n");

    HubExtension = (PUSBHUB_FDO_EXTENSION)PortExtension->HubExtension;
    IoStack = Irp->Tail.Overlay.CurrentStackLocation;

    Urb = (PURB)IoStack->Parameters.Others.Argument1;

    if (PortExtension->DeviceHandle == NULL)
    {
        Urb->UrbHeader.UsbdDeviceHandle = (PVOID)-1;
        Status = USBH_PassIrp(HubExtension->RootHubPdo2, Irp);
    }
    else
    {
        Urb->UrbHeader.UsbdDeviceHandle = PortExtension->DeviceHandle;
        Status = USBH_PdoUrbFilter(PortExtension, Irp);
    }

    return Status;
}

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

NTSTATUS
NTAPI
USBH_PdoInternalControl(IN PUSBHUB_PORT_PDO_EXTENSION PortExtension,
                        IN PIRP Irp)
{
    PUSBHUB_FDO_EXTENSION HubExtension;
    NTSTATUS Status = STATUS_NOT_SUPPORTED;
    ULONG ControlCode;
    PIO_STACK_LOCATION IoStack;

    DPRINT("USBH_PdoInternalControl: PortExtension - %p, Irp - %p\n",
           PortExtension,
           Irp);

    HubExtension = PortExtension->HubExtension;

    if (PortExtension->PortPdoFlags & USBHUB_PDO_FLAG_NOT_CONNECTED)
    {
        Status = STATUS_DEVICE_NOT_CONNECTED;
        goto Exit;
    }

    if (PortExtension->CurrentPowerState.DeviceState != PowerDeviceD0)
    {
        Status = STATUS_DEVICE_POWERED_OFF;
        goto Exit;
    }

    IoStack = Irp->Tail.Overlay.CurrentStackLocation;
    ControlCode = IoStack->Parameters.DeviceIoControl.IoControlCode;
    DPRINT("USBH_PdoInternalControl: ControlCode - %p\n", ControlCode);

    if (ControlCode == IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO)
    {
        HubExtension = PortExtension->RootHubExtension;
        DPRINT("USBH_PdoInternalControl: HubExtension - %p\n", HubExtension);
    }

    if (!HubExtension)
    {
        Status = STATUS_DEVICE_BUSY;
        goto Exit;
    }

    switch (ControlCode)
    {
        case IOCTL_INTERNAL_USB_SUBMIT_URB:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_SUBMIT_URB. \n");
            return USBH_PdoIoctlSubmitUrb(PortExtension, Irp);

        case IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_SUBMIT_IDLE_NOTIFICATION. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_PORT_STATUS:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_PORT_STATUS. \n");
            break;

        case IOCTL_INTERNAL_USB_RESET_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_RESET_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_ENABLE_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_ENABLE_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_CYCLE_PORT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_CYCLE_PORT. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_DEVICE_HANDLE. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_HUB_COUNT:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_HUB_COUNT. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_ROOTHUB_PDO. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_HUB_NAME:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_HUB_NAME. \n");
            break;

        case IOCTL_GET_HCD_DRIVERKEY_NAME:
            DPRINT("USBH_PdoInternalControl: IOCTL_GET_HCD_DRIVERKEY_NAME. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_BUS_INFO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_BUS_INFO. \n");
            break;

        case IOCTL_INTERNAL_USB_GET_PARENT_HUB_INFO:
            DPRINT("USBH_PdoInternalControl: IOCTL_INTERNAL_USB_GET_PARENT_HUB_INFO. \n");
            break;

        default:
            DPRINT("USBH_PdoInternalControl: IOCTL_ ???\n");
            break;
    }

Exit:
    USBH_CompleteIrp(Irp, Status);
    return Status;
}
