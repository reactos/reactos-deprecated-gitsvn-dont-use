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
                      IN OUT PUSB_DEVICE_HANDLE *UsbdDeviceHandle,
                      IN PUSB_DEVICE_HANDLE UsbdHubDeviceHandle,
                      IN USHORT PortStatus,
                      IN USHORT PortNumber)
{
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    PUSB_DEVICE_HANDLE deviceHandle = NULL;
    NTSTATUS Status;

    DPRINT("USBHI_CreateUsbDevice: ... \n");

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    Status = USBPORT_CreateDevice(&deviceHandle,
                                  PdoExtension->FdoDevice,
                                  (PUSBPORT_DEVICE_HANDLE)UsbdHubDeviceHandle,
                                  PortStatus,
                                  PortNumber);

    *UsbdDeviceHandle = deviceHandle;

    return Status;
}

NTSTATUS
USB_BUSIFFN
USBHI_InitializeUsbDevice(IN PVOID BusContext,
                          OUT PUSB_DEVICE_HANDLE UsbdDeviceHandle)
{
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;

    DPRINT("USBHI_InitializeUsbDevice \n");

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;
    return USBPORT_InitializeDevice((PUSBPORT_DEVICE_HANDLE)UsbdDeviceHandle, PdoExtension->FdoDevice);
}

VOID
NTAPI
USBPORT_DumpingDeviceDescriptor(IN PUSB_DEVICE_DESCRIPTOR DeviceDescriptor)
{
    if (!DeviceDescriptor)
    {
        return;
    }

    DPRINT1("Dumping Device Descriptor - %p\n", DeviceDescriptor);
    DPRINT1("bLength             - %x\n", DeviceDescriptor->bLength);
    DPRINT1("bDescriptorType     - %x\n", DeviceDescriptor->bDescriptorType);
    DPRINT1("bcdUSB              - %x\n", DeviceDescriptor->bcdUSB);
    DPRINT1("bDeviceClass        - %x\n", DeviceDescriptor->bDeviceClass);
    DPRINT1("bDeviceSubClass     - %x\n", DeviceDescriptor->bDeviceSubClass);
    DPRINT1("bDeviceProtocol     - %x\n", DeviceDescriptor->bDeviceProtocol);
    DPRINT1("bMaxPacketSize0     - %x\n", DeviceDescriptor->bMaxPacketSize0);
    DPRINT1("idVendor            - %x\n", DeviceDescriptor->idVendor);
    DPRINT1("idProduct           - %x\n", DeviceDescriptor->idProduct);
    DPRINT1("bcdDevice           - %x\n", DeviceDescriptor->bcdDevice);
    DPRINT1("iManufacturer       - %x\n", DeviceDescriptor->iManufacturer);
    DPRINT1("iProduct            - %x\n", DeviceDescriptor->iProduct);
    DPRINT1("iSerialNumber       - %x\n", DeviceDescriptor->iSerialNumber);
    DPRINT1("bNumConfigurations  - %x\n", DeviceDescriptor->bNumConfigurations);

}

NTSTATUS
USB_BUSIFFN
USBHI_GetUsbDescriptors(IN PVOID BusContext,
                        IN PUSB_DEVICE_HANDLE UsbdDeviceHandle,
                        IN PUCHAR DeviceDescBuffer,
                        IN PULONG DeviceDescBufferLen,
                        IN PUCHAR ConfigDescBuffer,
                        IN PULONG ConfigDescBufferLen)
{
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    NTSTATUS Status;

    DPRINT("USBHI_GetUsbDescriptors ...\n");

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    if (DeviceDescBuffer && *DeviceDescBufferLen)
    {
        if (*DeviceDescBufferLen > sizeof(USB_DEVICE_DESCRIPTOR))
            *DeviceDescBufferLen = sizeof(USB_DEVICE_DESCRIPTOR);

        RtlCopyMemory(DeviceDescBuffer,
                      &(((PUSBPORT_DEVICE_HANDLE)UsbdDeviceHandle)->DeviceDescriptor),
                      *DeviceDescBufferLen);
    }

    Status = USBPORT_GetUsbDescriptor((PUSBPORT_DEVICE_HANDLE)UsbdDeviceHandle,
                                      PdoExtension->FdoDevice,
                                      USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                      ConfigDescBuffer,
                                      ConfigDescBufferLen);

    USBPORT_DumpingDeviceDescriptor((PUSB_DEVICE_DESCRIPTOR)DeviceDescBuffer);

    return Status;
}

NTSTATUS
USB_BUSIFFN
USBHI_RemoveUsbDevice(IN PVOID BusContext,
                      IN OUT PUSB_DEVICE_HANDLE UsbdDeviceHandle,
                      IN ULONG Flags)
{
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;

    DPRINT("USBHI_RemoveUsbDevice: UsbdDeviceHandle - %p, Flags - %x\n",
           UsbdDeviceHandle,
           Flags);

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    return USBPORT_RemoveDevice(PdoExtension->FdoDevice, (PUSBPORT_DEVICE_HANDLE)UsbdDeviceHandle, Flags);
}

NTSTATUS
USB_BUSIFFN
USBHI_RestoreUsbDevice(IN PVOID BusContext,
                       OUT PUSB_DEVICE_HANDLE OldUsbdDeviceHandle,
                       OUT PUSB_DEVICE_HANDLE NewUsbdDeviceHandle)
{
    DPRINT("USBHI_RestoreUsbDevice \n");
    ASSERT(FALSE);
    return STATUS_SUCCESS;
}

NTSTATUS
USB_BUSIFFN
USBHI_QueryDeviceInformation(IN PVOID BusContext,
                             IN PUSB_DEVICE_HANDLE UsbdDeviceHandle,
                             OUT PVOID DeviceInfoBuffer,
                             IN ULONG DeviceInfoBufferLen,
                             OUT PULONG LenDataReturned)
{
    DPRINT1("USBHI_QueryDeviceInformation FIXME\n");
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
                                IN OUT PVOID HubInformationBuffer,
                                IN ULONG HubInfoLen,
                                IN OUT PULONG LenDataReturned)
{
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;
    ULONG NumPorts;
    ULONG ix;
    PUSB_EXTHUB_INFORMATION_0 HubInfoBuffer;

    DPRINT("USBHI_GetExtendedHubInformation: ... \n");

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    HubInfoBuffer = (PUSB_EXTHUB_INFORMATION_0)HubInformationBuffer;

    if (HubPhysicalDeviceObject != PdoDevice)
    {
        *LenDataReturned = 0;
        return STATUS_NOT_SUPPORTED;
    }

    if (HubInfoLen < sizeof(USB_EXTHUB_INFORMATION_0))
    {
        *LenDataReturned = 0;
        return STATUS_BUFFER_TOO_SMALL;
    }

    NumPorts = PdoExtension->RootHubDescriptors->Descriptor.bNumberOfPorts;
    HubInfoBuffer->NumberOfPorts = NumPorts;

    if (NumPorts == 0)
    {
        *LenDataReturned = sizeof(USB_EXTHUB_INFORMATION_0);
        return STATUS_SUCCESS;
    }

    for (ix = 1; ix < HubInfoBuffer->NumberOfPorts; ++ix)
    {
        HubInfoBuffer->Port[ix].PhysicalPortNumber = ix + 1;
        HubInfoBuffer->Port[ix].PortLabelNumber = ix + 1;
        HubInfoBuffer->Port[ix].VidOverride = 0;
        HubInfoBuffer->Port[ix].PidOverride = 0;
        HubInfoBuffer->Port[ix].PortAttributes = 0; // USB_PORTATTR_SHARED_USB2; // FIXME
    }

    *LenDataReturned = sizeof(USB_EXTHUB_INFORMATION_0);

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
                      IN PUSB_DEVICE_HANDLE UsbdHubDeviceHandle,
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
    PDEVICE_OBJECT PdoDevice;
    PUSBPORT_RHDEVICE_EXTENSION PdoExtension;

    DPRINT("USBHI_RootHubInitNotification \n");

    PdoDevice = (PDEVICE_OBJECT)BusContext;
    PdoExtension = (PUSBPORT_RHDEVICE_EXTENSION)PdoDevice->DeviceExtension;

    PdoExtension->RootHubInitContext = CallbackContext;
    PdoExtension->RootHubInitCallback = CallbackFunction;

    return STATUS_SUCCESS;
}

VOID
USB_BUSIFFN
USBHI_FlushTransfers(IN PVOID BusContext,
                     OUT PUSB_DEVICE_HANDLE UsbdDeviceHandle)
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
    DPRINT("USBHI_SetDeviceHandleData FIXME\n");
    // ASSERT(FALSE);
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
    return FALSE;
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
    // ASSERT(FALSE);
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
