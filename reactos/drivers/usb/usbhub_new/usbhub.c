#include "usbhub.h"

#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBH_AddDevice(
  IN PDRIVER_OBJECT DriverObject,
  IN PDEVICE_OBJECT LowerPDO)
{
    DPRINT("USBH_HubDispatch: DriverObject - %p, LowerPDO - %p\n", DriverObject, LowerPDO);
    return STATUS_SUCCESS;
}

VOID
NTAPI
USBH_DriverUnload(
  IN PDRIVER_OBJECT DriverObject)
{
    DPRINT("USBH_DriverUnload: UNIMPLEMENTED\n");
}

NTSTATUS
NTAPI
USBH_HubDispatch(IN PDEVICE_OBJECT DeviceObject,
                 IN PIRP Irp)
{
    DPRINT("USBH_HubDispatch: DeviceObject - %p, Irp - %p\n", DeviceObject, Irp);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    DPRINT("USBHUB: DriverEntry - %wZ\n", RegistryPath);

    DriverObject->DriverExtension->AddDevice = USBH_AddDevice;
    DriverObject->DriverUnload = USBH_DriverUnload;

    DriverObject->MajorFunction[IRP_MJ_CREATE] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = USBH_HubDispatch;

    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = USBH_HubDispatch;

    DriverObject->MajorFunction[IRP_MJ_PNP] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_POWER] = USBH_HubDispatch;
    DriverObject->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = USBH_HubDispatch;

    return STATUS_SUCCESS;
}

