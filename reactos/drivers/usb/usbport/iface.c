#include "usbport.h"

//#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
USBPORT_PdoQueryInterface(PDEVICE_OBJECT FdoDevice,
                          PDEVICE_OBJECT PdoDevice,
                          PIRP Irp)
{
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    PUSB_BUS_INTERFACE_HUB_V5 InterfaceHub;
    PUSB_BUS_INTERFACE_USBDI_V2 InterfaceDI;
    UNICODE_STRING GuidBuffer;
    NTSTATUS Status;

    DPRINT("USBPORT_PdoQueryInterface: ... \n");

    if (IsEqualGUIDAligned((REFGUID)(IoStack->Parameters.QueryInterface.InterfaceType),
                           &USB_BUS_INTERFACE_HUB_GUID))
    {
        // Get request parameters
        InterfaceHub = (PUSB_BUS_INTERFACE_HUB_V5)IoStack->Parameters.QueryInterface.Interface;
        InterfaceHub->Version = IoStack->Parameters.QueryInterface.Version;

        // Check version
        if (IoStack->Parameters.QueryInterface.Version >= 6)
        {
            DPRINT1("USB_BUS_INTERFACE_HUB_GUID version %x not supported!\n",
                    IoStack->Parameters.QueryInterface.Version);

            return STATUS_NOT_SUPPORTED; // Version not supported
        }

        // Interface version 0
        InterfaceHub->Size = IoStack->Parameters.QueryInterface.Size;
        InterfaceHub->BusContext = (PVOID)PdoDevice;

        InterfaceHub->InterfaceReference = USBI_InterfaceReference;
        InterfaceHub->InterfaceDereference = USBI_InterfaceDereference;

        // Interface version 1
        if (IoStack->Parameters.QueryInterface.Version >= 1)
        {
            InterfaceHub->CreateUsbDevice = USBHI_CreateUsbDevice;
            InterfaceHub->InitializeUsbDevice = USBHI_InitializeUsbDevice;
            InterfaceHub->GetUsbDescriptors = USBHI_GetUsbDescriptors;
            InterfaceHub->RemoveUsbDevice = USBHI_RemoveUsbDevice;
            InterfaceHub->RestoreUsbDevice = USBHI_RestoreUsbDevice;
            InterfaceHub->QueryDeviceInformation = USBHI_QueryDeviceInformation;
        }

        // Interface version 2
        if (IoStack->Parameters.QueryInterface.Version >= 2)
        {
            InterfaceHub->GetControllerInformation = USBHI_GetControllerInformation;
            InterfaceHub->ControllerSelectiveSuspend = USBHI_ControllerSelectiveSuspend;
            InterfaceHub->GetExtendedHubInformation = USBHI_GetExtendedHubInformation;
            InterfaceHub->GetRootHubSymbolicName = USBHI_GetRootHubSymbolicName;
            InterfaceHub->GetDeviceBusContext = USBHI_GetDeviceBusContext;
            InterfaceHub->Initialize20Hub = USBHI_Initialize20Hub;
        }

        // Interface version 3
        if (IoStack->Parameters.QueryInterface.Version >= 3)
            InterfaceHub->RootHubInitNotification = USBHI_RootHubInitNotification;

        // Interface version 4
        if (IoStack->Parameters.QueryInterface.Version >= 4)
            InterfaceHub->FlushTransfers = USBHI_FlushTransfers;

        // Interface version 5
        if (IoStack->Parameters.QueryInterface.Version >= 5)
            InterfaceHub->SetDeviceHandleData = USBHI_SetDeviceHandleData;

        // Request completed
        return STATUS_SUCCESS;
    }
    else if (IsEqualGUIDAligned(IoStack->Parameters.QueryInterface.InterfaceType,
                                &USB_BUS_INTERFACE_USBDI_GUID))
    {
        // Get request parameters
        InterfaceDI = (PUSB_BUS_INTERFACE_USBDI_V2)IoStack->Parameters.QueryInterface.Interface;
        InterfaceDI->Version = IoStack->Parameters.QueryInterface.Version;

        // Check version
        if (IoStack->Parameters.QueryInterface.Version >= 3)
        {
            DPRINT1("USB_BUS_INTERFACE_USBDI_GUID version %x not supported!\n",
                    IoStack->Parameters.QueryInterface.Version);

            return STATUS_NOT_SUPPORTED; // Version not supported
        }

        // Interface version 0
        InterfaceDI->Size = IoStack->Parameters.QueryInterface.Size;
        InterfaceDI->BusContext = (PVOID)PdoDevice;
        InterfaceDI->InterfaceReference = USBI_InterfaceReference;
        InterfaceDI->InterfaceDereference = USBI_InterfaceDereference;
        InterfaceDI->GetUSBDIVersion = USBDI_GetUSBDIVersion;
        InterfaceDI->QueryBusTime = USBDI_QueryBusTime;
        InterfaceDI->SubmitIsoOutUrb = USBDI_SubmitIsoOutUrb;
        InterfaceDI->QueryBusInformation = USBDI_QueryBusInformation;

        // Interface version 1
        if (IoStack->Parameters.QueryInterface.Version >= 1)
            InterfaceDI->IsDeviceHighSpeed = USBDI_IsDeviceHighSpeed;

        // Interface version 2
        if (IoStack->Parameters.QueryInterface.Version >= 2)
            InterfaceDI->EnumLogEntry = USBDI_EnumLogEntry;

        return STATUS_SUCCESS;
    }
    else
    {
        // Convert GUID to string
        Status = RtlStringFromGUID(IoStack->Parameters.QueryInterface.InterfaceType,
                                   &GuidBuffer);

        if (NT_SUCCESS(Status))
        {
            // Print interface
            DPRINT1("HandleQueryInterface UNKNOWN INTERFACE GUID: %wZ Version %x\n",
                    &GuidBuffer,
                    IoStack->Parameters.QueryInterface.Version);
            
            RtlFreeUnicodeString(&GuidBuffer); // Free GUID buffer
        }
    }

    return STATUS_NOT_SUPPORTED;
}
