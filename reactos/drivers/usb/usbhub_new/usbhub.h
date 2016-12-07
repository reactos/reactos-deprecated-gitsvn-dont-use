#ifndef _USBHUB_H_
#define _USBHUB_H_

#include <ntddk.h>
#include <windef.h>
#include <stdio.h>
#include <wdm.h>
#include <wmistr.h>
#include <wmilib.h>
#include <wdmguid.h>
#include <hubbusif.h>
#include <usbbusif.h>
#include <usbdlib.h>

#define USB_HUB_TAG 'BUHU'

#define USBH_EXTENSION_TYPE_HUB        0x01
#define USBH_EXTENSION_TYPE_PORT       0x02

typedef struct _COMMON_DEVICE_EXTENSION {
  ULONG ExtensionType;
  PDEVICE_OBJECT SelfDevice;
} COMMON_DEVICE_EXTENSION, *PCOMMON_DEVICE_EXTENSION;

typedef struct _USBHUB_FDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
  PDEVICE_OBJECT LowerPDO;
  PDEVICE_OBJECT LowerDevice;
} USBHUB_FDO_EXTENSION, *PUSBHUB_FDO_EXTENSION;

typedef struct _USBHUB_PORT_PDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
} USBHUB_PORT_PDO_EXTENSION, *PUSBHUB_PORT_PDO_EXTENSION;

/* ioctl.c */

/* pnp.c */

/* power.c */

/* usbhub.c */

VOID
NTAPI
USBH_CompleteIrp(
  IN PIRP Irp,
  IN NTSTATUS CompleteStatus);

NTSTATUS
NTAPI
DriverEntry(
  IN PDRIVER_OBJECT DriverObject,
  IN PUNICODE_STRING RegistryPath);

#endif /* _USBHUB_H_ */
