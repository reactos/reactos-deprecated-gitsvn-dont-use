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

