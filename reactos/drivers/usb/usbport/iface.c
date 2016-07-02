#include "usbport.h"

//#define NDEBUG
#include <debug.h>

VOID
USB_BUSIFFN
USBI_InterfaceReference(IN PVOID BusContext)
{
    DPRINT("USBI_InterfaceReference \n");
}

VOID
USB_BUSIFFN
USBI_InterfaceDereference(IN PVOID BusContext)
{
    DPRINT("USBI_InterfaceDereference \n");
}

/* USB port driver Interface functions */

NTSTATUS
USB_BUSIFFN
USBHI_CreateUsbDevice(IN PVOID BusContext,
                      IN PUSB_DEVICE_HANDLE *NewDevice,
                      IN PUSB_DEVICE_HANDLE HubDeviceHandle,
                      IN USHORT PortStatus,
                      IN USHORT PortNumber)
{
    DPRINT("USBHI_CreateUsbDevice\n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_InitializeUsbDevice(IN PVOID BusContext,
                          OUT PUSB_DEVICE_HANDLE DeviceHandle)
{
    DPRINT("USBHI_InitializeUsbDevice \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_GetUsbDescriptors(IN PVOID BusContext,
                        IN PUSB_DEVICE_HANDLE DeviceHandle,
                        IN PUCHAR DeviceDescBuffer,
                        IN PULONG DeviceDescBufferLen,
                        IN PUCHAR ConfigDescBuffer,
                        IN PULONG ConfigDescBufferLen)
{
    DPRINT("USBHI_GetUsbDescriptors \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_RemoveUsbDevice(IN PVOID BusContext,
                      OUT PUSB_DEVICE_HANDLE DeviceHandle,
                      IN ULONG Flags)
{
    DPRINT("USBHI_RemoveUsbDevice \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_RestoreUsbDevice(IN PVOID BusContext,
                       OUT PUSB_DEVICE_HANDLE OldDeviceHandle,
                       OUT PUSB_DEVICE_HANDLE NewDeviceHandle)
{
    DPRINT("USBHI_RestoreUsbDevice \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_QueryDeviceInformation(IN PVOID BusContext,
                             IN PUSB_DEVICE_HANDLE DeviceHandle,
                             OUT PVOID DeviceInfoBuffer,
                             IN ULONG DeviceInfoBufferLen,
                             OUT PULONG LenDataReturned)
{
    DPRINT("USBHI_QueryDeviceInformation \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_GetControllerInformation(IN PVOID BusContext,
                               OUT PVOID ControllerInfoBuffer,
                               IN ULONG ControllerInfoBufferLen,
                               OUT PULONG LenDataReturned)
{
    DPRINT("USBHI_GetControllerInformation \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_ControllerSelectiveSuspend(IN PVOID BusContext,
                                 IN BOOLEAN Enable)
{
    DPRINT("USBHI_ControllerSelectiveSuspend \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_GetExtendedHubInformation(IN PVOID BusContext,
                                IN PDEVICE_OBJECT HubPhysicalDeviceObject,
                                OUT PVOID HubInfoBuffer,
                                IN ULONG HubInfoLen,
                                OUT PULONG LenDataReturned)
{
    DPRINT("USBHI_GetExtendedHubInformation \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_GetRootHubSymbolicName(IN PVOID BusContext,
                             OUT PVOID HubInfoBuffer,
                             IN ULONG HubInfoBufferLen,
                             OUT PULONG HubNameActualLen)
{
    DPRINT("USBHI_GetRootHubSymbolicName \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

PVOID
USB_BUSIFFN
USBHI_GetDeviceBusContext(IN PVOID BusContext,
                          IN PVOID DeviceHandle)
{
    DPRINT("USBHI_GetDeviceBusContext \n");
    ASSERT(FALSE);
    return NULL;
}

NTSTATUS
USB_BUSIFFN
USBHI_Initialize20Hub(IN PVOID BusContext,
                      IN PUSB_DEVICE_HANDLE HubDeviceHandle,
                      IN ULONG TtCount)
{
    DPRINT("USBHI_Initialize20Hub \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_RootHubInitNotification(IN PVOID BusContext,
                              IN PVOID CallbackContext,
                              IN PRH_INIT_CALLBACK CallbackFunction)
{
    DPRINT("USBHI_RootHubInitNotification \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

VOID
USB_BUSIFFN
USBHI_FlushTransfers(IN PVOID BusContext,
                     OUT PUSB_DEVICE_HANDLE DeviceHandle)
{
    DPRINT("USBHI_FlushTransfers \n");
    ASSERT(FALSE);
}

VOID
USB_BUSIFFN
USBHI_SetDeviceHandleData(IN PVOID BusContext,
                          IN PVOID DeviceHandle,
                          IN PDEVICE_OBJECT UsbDevicePdo)
{
    DPRINT("USBHI_SetDeviceHandleData \n");
    ASSERT(FALSE);
}

/* USB bus driver Interface functions */

VOID
USB_BUSIFFN
USBDI_GetUSBDIVersion(IN PVOID BusContext,
                      OUT PUSBD_VERSION_INFORMATION VersionInfo,
                      OUT PULONG HcdCapabilities)
{
    DPRINT("USBDI_GetUSBDIVersion \n");
    ASSERT(FALSE);
}

NTSTATUS
USB_BUSIFFN
USBDI_QueryBusTime(IN PVOID BusContext,
                   OUT PULONG CurrentFrame)
{
    DPRINT("USBDI_QueryBusTime \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBDI_SubmitIsoOutUrb(IN PVOID BusContext,
                      IN PURB Urb)
{
    DPRINT("USBDI_SubmitIsoOutUrb \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBDI_QueryBusInformation(IN PVOID BusContext,
                          IN ULONG Level,
                          OUT PVOID BusInfoBuffer,
                          OUT PULONG BusInfoBufferLen,
                          OUT PULONG BusInfoActualLen)
{
    DPRINT("USBDI_QueryBusInformation \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

BOOLEAN
USB_BUSIFFN
USBDI_IsDeviceHighSpeed(IN PVOID BusContext)
{
    DPRINT("USBDI_IsDeviceHighSpeed \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBDI_EnumLogEntry(IN PVOID BusContext,
                   IN ULONG DriverTag,
                   IN ULONG EnumTag,
                   IN ULONG P1,
                   IN ULONG P2)
{
    DPRINT("USBDI_EnumLogEntry \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
USBPORT_PdoQueryInterface(IN PDEVICE_OBJECT FdoDevice,
                          IN PDEVICE_OBJECT PdoDevice,
                          IN PIRP Irp)
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
