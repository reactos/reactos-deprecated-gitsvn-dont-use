#include "usbhub.h"

//#define NDEBUG
#include <debug.h>

VOID
NTAPI
USBPORT_DumpingDeviceDescriptor(IN PUSB_DEVICE_DESCRIPTOR DeviceDescriptor)
{
    if (!DeviceDescriptor)
    {
        return;
    }

    DPRINT("Dumping Device Descriptor - %p\n", DeviceDescriptor);
    DPRINT("bLength             - %x\n", DeviceDescriptor->bLength);
    DPRINT("bDescriptorType     - %x\n", DeviceDescriptor->bDescriptorType);
    DPRINT("bcdUSB              - %x\n", DeviceDescriptor->bcdUSB);
    DPRINT("bDeviceClass        - %x\n", DeviceDescriptor->bDeviceClass);
    DPRINT("bDeviceSubClass     - %x\n", DeviceDescriptor->bDeviceSubClass);
    DPRINT("bDeviceProtocol     - %x\n", DeviceDescriptor->bDeviceProtocol);
    DPRINT("bMaxPacketSize0     - %x\n", DeviceDescriptor->bMaxPacketSize0);
    DPRINT("idVendor            - %x\n", DeviceDescriptor->idVendor);
    DPRINT("idProduct           - %x\n", DeviceDescriptor->idProduct);
    DPRINT("bcdDevice           - %x\n", DeviceDescriptor->bcdDevice);
    DPRINT("iManufacturer       - %x\n", DeviceDescriptor->iManufacturer);
    DPRINT("iProduct            - %x\n", DeviceDescriptor->iProduct);
    DPRINT("iSerialNumber       - %x\n", DeviceDescriptor->iSerialNumber);
    DPRINT("bNumConfigurations  - %x\n", DeviceDescriptor->bNumConfigurations);
}

VOID
NTAPI
USBPORT_DumpingConfiguration(IN PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor)
{
    PUSB_INTERFACE_DESCRIPTOR iDescriptor;
    PUSB_ENDPOINT_DESCRIPTOR Descriptor;
    ULONG ix;

    if (!ConfigDescriptor)
    {
        return;
    }

    DPRINT("Dumping ConfigDescriptor - %p\n", ConfigDescriptor);
    DPRINT("bLength             - %x\n", ConfigDescriptor->bLength);
    DPRINT("bDescriptorType     - %x\n", ConfigDescriptor->bDescriptorType);
    DPRINT("wTotalLength        - %x\n", ConfigDescriptor->wTotalLength);
    DPRINT("bNumInterfaces      - %x\n", ConfigDescriptor->bNumInterfaces);
    DPRINT("bConfigurationValue - %x\n", ConfigDescriptor->bConfigurationValue);
    DPRINT("iConfiguration      - %x\n", ConfigDescriptor->iConfiguration);
    DPRINT("bmAttributes        - %x\n", ConfigDescriptor->bmAttributes);
    DPRINT("MaxPower            - %x\n", ConfigDescriptor->MaxPower);

    iDescriptor = (PUSB_INTERFACE_DESCRIPTOR)((ULONG_PTR)ConfigDescriptor +
                                                         ConfigDescriptor->bLength);

    if (!iDescriptor)
    {
        return;
    }

    DPRINT("Dumping iDescriptor - %p\n", iDescriptor);
    DPRINT("bLength             - %x\n", iDescriptor->bLength);
    DPRINT("bDescriptorType     - %x\n", iDescriptor->bDescriptorType);
    DPRINT("bInterfaceNumber    - %x\n", iDescriptor->bInterfaceNumber);
    DPRINT("bAlternateSetting   - %x\n", iDescriptor->bAlternateSetting);
    DPRINT("bNumEndpoints       - %x\n", iDescriptor->bNumEndpoints);
    DPRINT("bInterfaceClass     - %x\n", iDescriptor->bInterfaceClass);
    DPRINT("bInterfaceSubClass  - %x\n", iDescriptor->bInterfaceSubClass);
    DPRINT("bInterfaceProtocol  - %x\n", iDescriptor->bInterfaceProtocol);
    DPRINT("iInterface          - %x\n", iDescriptor->iInterface);

    Descriptor = (PUSB_ENDPOINT_DESCRIPTOR)((ULONG_PTR)iDescriptor +
                                                       iDescriptor->bLength);

    if (!Descriptor)
    {
        return;
    }

    for (ix = 0; ix < iDescriptor->bNumEndpoints; ix++)
    {
        DPRINT("Dumping Descriptor  - %p\n", Descriptor);
        DPRINT("bLength             - %x\n", Descriptor->bLength);
        DPRINT("bDescriptorType     - %x\n", Descriptor->bDescriptorType);
        DPRINT("bEndpointAddress    - %x\n", Descriptor->bEndpointAddress);
        DPRINT("bmAttributes        - %x\n", Descriptor->bmAttributes);
        DPRINT("wMaxPacketSize      - %x\n", Descriptor->wMaxPacketSize);
        DPRINT("bInterval           - %x\n", Descriptor->bInterval);

        Descriptor += 1;
    }
}

VOID
NTAPI
USBPORT_DumpingIDs(IN PVOID Id)
{
    PWSTR Ptr;
    ULONG Length;
    ULONG TotalLength = 0;

    Ptr = (PWSTR)Id;
    DPRINT("USBPORT_DumpingIDs:\n");

    while (*Ptr)
    {
        DPRINT("  %S\n", Ptr);
        Length = (ULONG)wcslen(Ptr) + 1;

        Ptr += Length;
        TotalLength += Length;
    }

    DPRINT("TotalLength: %hu\n", TotalLength);
    DPRINT("\n");
}
