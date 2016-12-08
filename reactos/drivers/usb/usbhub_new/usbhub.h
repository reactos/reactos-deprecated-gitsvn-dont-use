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

#define USBHUB_FDO_FLAG_DEVICE_STARTED    (1 << 0)    // 0x00000001
#define USBHUB_FDO_FLAG_DEVICE_STOPPING   (1 << 2)    // 0x00000004
#define USBHUB_FDO_FLAG_DEVICE_FAILED     (1 << 3)    // 0x00000008
#define USBHUB_FDO_FLAG_REMOTE_WAKEUP     (1 << 4)    // 0x00000010

typedef struct _COMMON_DEVICE_EXTENSION {
  ULONG ExtensionType;
  PDEVICE_OBJECT SelfDevice;
} COMMON_DEVICE_EXTENSION, *PCOMMON_DEVICE_EXTENSION;

typedef struct _USBHUB_FDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
  PDEVICE_OBJECT LowerPDO;
  PDEVICE_OBJECT LowerDevice;
  PDEVICE_OBJECT RootHubPdo;
  PDEVICE_OBJECT RootHubPdo2;
  KEVENT LowerDeviceEvent;
  ULONG HubFlags;
} USBHUB_FDO_EXTENSION, *PUSBHUB_FDO_EXTENSION;

typedef struct _USBHUB_PORT_PDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
} USBHUB_PORT_PDO_EXTENSION, *PUSBHUB_PORT_PDO_EXTENSION;

/* ioctl.c */

NTSTATUS
NTAPI
USBH_DeviceControl(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PIRP Irp);

/* pnp.c */

NTSTATUS
NTAPI
USBH_FdoPnP(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PIRP Irp,
  IN UCHAR Minor);

/* power.c */

NTSTATUS
NTAPI
USBH_FdoPower(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PIRP Irp,
  IN UCHAR Minor);

/* usbhub.c */

VOID
NTAPI
USBH_CompleteIrp(
  IN PIRP Irp,
  IN NTSTATUS CompleteStatus);

NTSTATUS
NTAPI
USBH_PassIrp(
  IN PDEVICE_OBJECT DeviceObject,
  IN PIRP Irp);

NTSTATUS
NTAPI
USBH_WriteFailReasonID(
  IN PDEVICE_OBJECT DeviceObject,
  IN ULONG Data);

NTSTATUS
NTAPI
USBH_SyncGetRootHubPdo(
  IN PDEVICE_OBJECT DeviceObject,
  IN OUT PDEVICE_OBJECT * OutPdo1,
  IN OUT PDEVICE_OBJECT * OutPdo2);

NTSTATUS
NTAPI
USBH_SyncGetHubCount(
  IN PDEVICE_OBJECT DeviceObject,
  IN OUT PULONG OutHubCount);

NTSTATUS
NTAPI
DriverEntry(
  IN PDRIVER_OBJECT DriverObject,
  IN PUNICODE_STRING RegistryPath);

#endif /* _USBHUB_H_ */
