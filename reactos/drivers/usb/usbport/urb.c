#include "usbport.h"

//#define NDEBUG
#include <debug.h>

#define NDEBUG_USBPORT_URB
#include "usbdebug.h"

NTSTATUS
NTAPI
USBPORT_HandleSubmitURB(IN PDEVICE_OBJECT PdoDevice,
                        IN PIRP Irp,
                        IN PURB Urb)
{
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PDEVICE_OBJECT FdoDevice;
    PUSBPORT_DEVICE_EXTENSION FdoExtension;
    USHORT Function;
    PUSBPORT_DEVICE_HANDLE DeviceHandle;
    NTSTATUS Status;

    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    FdoDevice = PdoExtension->FdoDevice;
    FdoExtension = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;

    Function = Urb->UrbHeader.Function;

    ASSERT(Urb);
    Status = STATUS_NOT_IMPLEMENTED;

    Urb->UrbHeader.Status = USBD_STATUS_SUCCESS;
    Urb->UrbHeader.UsbdFlags = 0;

    if (Function > URB_FUNCTION_MAX)
    {
        Status = USBPORT_USBDStatusToNtStatus(Urb,
                                              USBD_STATUS_INVALID_URB_FUNCTION);

        DPRINT1("USBPORT_HandleSubmitURB: Unknown Function %x. UNIMPLEMENTED\n",
               Function);

        return Status;
    }

    if (FdoExtension->TimerFlags & USBPORT_TMFLAG_RH_SUSPENDED)
    {
        DPRINT1("USBPORT_HandleSubmitURB: Bad Request\n");

        Irp->IoStatus.Status = STATUS_PENDING;
        Status = STATUS_PENDING;
        USBPORT_USBDStatusToNtStatus(Urb, USBD_STATUS_DEVICE_GONE);

        IoMarkIrpPending(Irp);
        IoCsqInsertIrp(&FdoExtension->BadRequestIoCsq, Irp, NULL);

        return Status;
    }

    DeviceHandle = (PUSBPORT_DEVICE_HANDLE)Urb->UrbHeader.UsbdDeviceHandle;

    if (!DeviceHandle)
    {
        //DPRINT("USBPORT_HandleSubmitURB: DeviceHandle == 0\n");
        Urb->UrbHeader.UsbdDeviceHandle = &PdoExtension->DeviceHandle;
        DeviceHandle = &PdoExtension->DeviceHandle;
    }

    if (!USBPORT_ValidateDeviceHandle(PdoExtension->FdoDevice,
                                      DeviceHandle))
    {
        DPRINT1("USBPORT_HandleSubmitURB: Not valid device handle\n");

        Irp->IoStatus.Status = STATUS_PENDING;
        IoMarkIrpPending(Irp);
        return STATUS_PENDING;
    }

    InterlockedIncrement(&DeviceHandle->DeviceHandleLock);

    DPRINT_URB("USBPORT_HandleSubmitURB: Function - 0x%02X, DeviceHandle - %p\n",
               Function,
               Urb->UrbHeader.UsbdDeviceHandle);

    switch (Function)
    {
        case URB_FUNCTION_ISOCH_TRANSFER: // 0x10
            DPRINT1("USBPORT_HandleSubmitURB: URB_FUNCTION_ISOCH_TRANSFER UNIMPLEMENTED. FIXME. \n");
            break;

        case URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER: // 0x09
        case URB_FUNCTION_CONTROL_TRANSFER: // 0x08
            Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer

            if ((Urb->UrbControlTransfer.TransferFlags & USBD_DEFAULT_PIPE_TRANSFER) &&
                (Function == URB_FUNCTION_CONTROL_TRANSFER))
                Urb->UrbControlTransfer.PipeHandle = &DeviceHandle->PipeHandle;

            ValidateTransferParameters(Urb);

            Status = USBPORT_AllocateTransfer(PdoExtension->FdoDevice,
                                              Urb,
                                              DeviceHandle,
                                              Irp,
                                              NULL);

            if (!NT_SUCCESS(Status))
            {
                DPRINT1("USBPORT_HandleSubmitURB: Not allocated transfer\n");
                return Status;
            }

            Status = USBPORT_HandleDataTransfers(Urb);
            break;

        case URB_FUNCTION_VENDOR_DEVICE: // 0x17
        case URB_FUNCTION_VENDOR_INTERFACE: // 0x18
        case URB_FUNCTION_VENDOR_ENDPOINT: // 0x19
        case URB_FUNCTION_CLASS_DEVICE: // 0x1A
        case URB_FUNCTION_CLASS_INTERFACE: // 0x1B
        case URB_FUNCTION_CLASS_ENDPOINT: // 0x1C
        case URB_FUNCTION_CLASS_OTHER: // 0x1F
        case URB_FUNCTION_VENDOR_OTHER: // 0x20
            Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
            Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
            Urb->UrbControlTransfer.PipeHandle = &DeviceHandle->PipeHandle;

            ValidateTransferParameters(Urb);

            Status = USBPORT_AllocateTransfer(PdoExtension->FdoDevice,
                                              Urb,
                                              DeviceHandle,
                                              Irp,
                                              NULL);

            if (!NT_SUCCESS(Status))
            {
                DPRINT1("USBPORT_HandleSubmitURB: Not allocated transfer\n");
                return Status;
            }

            Status = USBPORT_HandleVendorOrClass(Irp, Urb);
            break;

        case URB_FUNCTION_GET_DESCRIPTOR_FROM_DEVICE: // 0x0B
        case URB_FUNCTION_SET_DESCRIPTOR_TO_DEVICE: // 0x0C
        case URB_FUNCTION_GET_DESCRIPTOR_FROM_ENDPOINT: // 0x24
        case URB_FUNCTION_SET_DESCRIPTOR_TO_ENDPOINT: // 0x25
        case URB_FUNCTION_GET_DESCRIPTOR_FROM_INTERFACE: // 0x28
        case URB_FUNCTION_SET_DESCRIPTOR_TO_INTERFACE: // 0x29
            Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
            Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
            Urb->UrbControlTransfer.PipeHandle = &DeviceHandle->PipeHandle;

            ValidateTransferParameters(Urb);

            Status = USBPORT_AllocateTransfer(PdoExtension->FdoDevice,
                                              Urb,
                                              DeviceHandle,
                                              Irp,
                                              NULL);

            if (!NT_SUCCESS(Status))
            {
                DPRINT1("USBPORT_HandleSubmitURB: Not allocated transfer\n");
                return Status;
            }

            Status = USBPORT_HandleGetSetDescriptor(Irp, Urb);
            break;

        case URB_FUNCTION_GET_STATUS_FROM_DEVICE: // 0x13
        case URB_FUNCTION_GET_STATUS_FROM_INTERFACE: // 0x14
        case URB_FUNCTION_GET_STATUS_FROM_ENDPOINT: // 0x15
        case URB_FUNCTION_GET_STATUS_FROM_OTHER: // 0x21
            Urb->UrbControlTransfer.hca.Reserved8[0] = NULL; // Transfer
            Urb->UrbControlTransfer.TransferFlags |= USBD_DEFAULT_PIPE_TRANSFER;
            Urb->UrbControlTransfer.PipeHandle = &DeviceHandle->PipeHandle;

            ValidateTransferParameters(Urb);

            Status = USBPORT_AllocateTransfer(PdoExtension->FdoDevice,
                                              Urb,
                                              DeviceHandle,
                                              Irp,
                                              NULL);

            if (!NT_SUCCESS(Status))
            {
                DPRINT1("USBPORT_HandleSubmitURB: Not allocated transfer\n");
                return Status;
            }

            Status = USBPORT_HandleGetStatus(Irp, Urb);
            break;

        case URB_FUNCTION_SELECT_CONFIGURATION: // 0x00
            Status = USBPORT_HandleSelectConfiguration(PdoExtension->FdoDevice,
                                                       Irp,
                                                       Urb);
            break;

        case URB_FUNCTION_SELECT_INTERFACE: // 0x01
            Status = USBPORT_HandleSelectInterface(PdoExtension->FdoDevice,
                                                   Irp,
                                                   Urb);
            break;

        case URB_FUNCTION_SYNC_RESET_PIPE_AND_CLEAR_STALL: // 0x1E
            Status = USBPORT_SyncResetPipeAndClearStall(PdoExtension->FdoDevice,
                                                        Irp,
                                                        Urb);
            break;

        default:
            DPRINT1("USBPORT_HandleSubmitURB: Function - %x UNIMPLEMENTED. FIXME\n", Function);
            //0x02 02 URB_FUNCTION_ABORT_PIPE
            //0x03 03 URB_FUNCTION_TAKE_FRAME_LENGTH_CONTROL
            //0x04 04 URB_FUNCTION_RELEASE_FRAME_LENGTH_CONTROL
            //0x05 05 URB_FUNCTION_GET_FRAME_LENGTH
            //0x06 06 URB_FUNCTION_SET_FRAME_LENGTH
            //0x07 07 URB_FUNCTION_GET_CURRENT_FRAME_NUMBER
            //0x0A 10 URB_FUNCTION_ISOCH_TRANSFER
            //0x0D 13 URB_FUNCTION_SET_FEATURE_TO_DEVICE
            //0x0E 14 URB_FUNCTION_SET_FEATURE_TO_INTERFACE
            //0x0F 15 URB_FUNCTION_SET_FEATURE_TO_ENDPOINT
            //0x11 17 URB_FUNCTION_CLEAR_FEATURE_TO_INTERFACE
            //0x12 18 URB_FUNCTION_CLEAR_FEATURE_TO_ENDPOINT
            //0x16 22 URB_FUNCTION_RESERVED_0X0016
            //0x1D 29 URB_FUNCTION_RESERVE_0X001D
            //0x22 34 URB_FUNCTION_CLEAR_FEATURE_TO_OTHER
            //0x23 35 URB_FUNCTION_SET_FEATURE_TO_OTHER
            //0x26 38 URB_FUNCTION_GET_CONFIGURATION
            //0x27 39 URB_FUNCTION_GET_INTERFACE
            //0x2A 42 URB_FUNCTION_GET_MS_FEATURE_DESCRIPTOR
            //0x2B 43 URB_FUNCTION_RESERVE_0X002B
            //0x2C 44 URB_FUNCTION_RESERVE_0X002C
            //0x2D 45 URB_FUNCTION_RESERVE_0X002D
            //0x2E 46 URB_FUNCTION_RESERVE_0X002E
            //0x2F 47 URB_FUNCTION_RESERVE_0X002F
            //0x30 48 URB_FUNCTION_SYNC_RESET_PIPE
            //0x31 49 URB_FUNCTION_SYNC_CLEAR_STALL
            break;
    }

    if (Status == STATUS_PENDING)
    {
        return Status;
    }

    if (Urb->UrbHeader.UsbdFlags & USBD_FLAG_ALLOCATED_TRANSFER)
    {
        PUSBPORT_TRANSFER Transfer;

        Transfer = (PUSBPORT_TRANSFER)Urb->UrbControlTransfer.hca.Reserved8[0];
        Urb->UrbControlTransfer.hca.Reserved8[0] = NULL;
        Urb->UrbHeader.UsbdFlags |= ~USBD_FLAG_ALLOCATED_TRANSFER;
        ExFreePool(Transfer);
    }

    if (DeviceHandle)
    {
        InterlockedDecrement(&DeviceHandle->DeviceHandleLock);
    }

    return Status;
}
