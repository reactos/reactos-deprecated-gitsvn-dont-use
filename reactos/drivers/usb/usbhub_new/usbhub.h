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
#define USBH_EXTENSION_TYPE_PARENT     0x04
#define USBH_EXTENSION_TYPE_FUNCTION   0x08

#define USBHUB_FDO_FLAG_DEVICE_STARTED    (1 << 0)    // 0x00000001
#define USBHUB_FDO_FLAG_DEVICE_STOPPING   (1 << 2)    // 0x00000004
#define USBHUB_FDO_FLAG_DEVICE_FAILED     (1 << 3)    // 0x00000008
#define USBHUB_FDO_FLAG_REMOTE_WAKEUP     (1 << 4)    // 0x00000010
#define USBHUB_FDO_FLAG_USB20_HUB         (1 << 15)   // 0x00008000
#define USBHUB_FDO_FLAG_MULTIPLE_TTS      (1 << 18)   // 0x00040000 // High-speed Operating Hub with Multiple TTs

/* Hub Class Feature Selectors */

#define USBHUB_FEATURE_USBHUB_FEATURE_C_HUB_LOCAL_POWER  0
#define USBHUB_FEATURE_C_HUB_OVER_CURRENT                1

#define USBHUB_FEATURE_PORT_CONNECTION     0
#define USBHUB_FEATURE_PORT_ENABLE         1
#define USBHUB_FEATURE_PORT_SUSPEND        2
#define USBHUB_FEATURE_PORT_OVER_CURRENT   3
#define USBHUB_FEATURE_PORT_RESET          4
#define USBHUB_FEATURE_PORT_POWER          8
#define USBHUB_FEATURE_PORT_LOW_SPEED      9
#define USBHUB_FEATURE_C_PORT_CONNECTION   16
#define USBHUB_FEATURE_C_PORT_ENABLE       17
#define USBHUB_FEATURE_C_PORT_SUSPEND      18
#define USBHUB_FEATURE_C_PORT_OVER_CURRENT 19
#define USBHUB_FEATURE_C_PORT_RESET        20
#define USBHUB_FEATURE_PORT_TEST           21
#define USBHUB_FEATURE_PORT_INDICATOR      22

typedef union _USB_PORT_STATUS {
  struct {
    USHORT ConnectStatus          : 1; // Current Connect Status
    USHORT EnableStatus           : 1; // Port Enabled/Disabled
    USHORT SuspendStatus          : 1;
    USHORT OverCurrent            : 1;
    USHORT ResetStatus            : 1;
    USHORT Reserved1              : 3;
    USHORT PowerStatus            : 1;
    USHORT LsDeviceAttached       : 1; // Low-Speed Device Attached
    USHORT HsDeviceAttached       : 1; // High-speed Device Attached
    USHORT TestMode               : 1; // Port Test Mode
    USHORT IndicatorControl       : 1; // Port Indicator Control
    USHORT Reserved2              : 3;
  };
  USHORT AsUSHORT;
} USB_PORT_STATUS;

typedef union _USB_PORT_STATUS_CHANGE {
  struct {
    USHORT ConnectStatusChange    : 1;
    USHORT EnableStatusChange     : 1;
    USHORT SuspendStatusChange    : 1;
    USHORT OverCurrentChange      : 1;
    USHORT ResetStatusChange      : 1;
    USHORT Reserved3              : 3;
    USHORT PowerStatusChange      : 1;
    USHORT LsDeviceAttachedChange : 1;
    USHORT HsDeviceAttachedChange : 1;
    USHORT TestModeChange         : 1;
    USHORT IndicatorControlChange : 1;
    USHORT Reserved4              : 3;
  };
  USHORT AsUSHORT;
} USB_PORT_STATUS_CHANGE;

typedef union _USBHUB_PORT_STATUS {
struct {
    USB_PORT_STATUS UsbPortStatus;
    USB_PORT_STATUS_CHANGE UsbPortStatusChange;
  };
  ULONG AsULONG;
} USBHUB_PORT_STATUS, *PUSBHUB_PORT_STATUS;

typedef struct _USBHUB_PORT_DATA {
  USBHUB_PORT_STATUS PortStatus;
  PDEVICE_OBJECT DeviceObject;
  USB_CONNECTION_STATUS ConnectionStatus;
  ULONG PortAttributes;
} USBHUB_PORT_DATA, *PUSBHUB_PORT_DATA;

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
  USB_BUS_INTERFACE_HUB_V5 BusInterface;
  USB_BUS_INTERFACE_USBDI_V2 BusInterfaceUSBDI;
  DEVICE_POWER_STATE DeviceState[POWER_SYSTEM_MAXIMUM];
  SYSTEM_POWER_STATE SystemWake;
  DEVICE_POWER_STATE DeviceWake;
  POWER_STATE CurrentPowerState;
  ULONG MaxPower;
  USB_DEVICE_DESCRIPTOR HubDeviceDescriptor;
  USHORT Reserved1;
  PUSB_CONFIGURATION_DESCRIPTOR HubConfigDescriptor;
  PUSB_HUB_DESCRIPTOR HubDescriptor;
  PUSBHUB_PORT_DATA PortData;
  USBD_CONFIGURATION_HANDLE ConfigHandle;
  USBD_PIPE_INFORMATION PipeInfo;
  PIRP SCEIrp;
  PIRP ResetPortIrp;
  PVOID HubBuffer;
  ULONG HubBufferLength;
} USBHUB_FDO_EXTENSION, *PUSBHUB_FDO_EXTENSION;

typedef struct _USBHUB_PORT_PDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
} USBHUB_PORT_PDO_EXTENSION, *PUSBHUB_PORT_PDO_EXTENSION;

typedef struct _USBHUB_URB_TIMEOUT_CONTEXT {
  PIRP Irp;
  KEVENT UrbTimeoutEvent;
  KDPC UrbTimeoutDPC;
  KTIMER UrbTimeoutTimer;
  KSPIN_LOCK UrbTimeoutSpinLock;
  BOOL IsNormalCompleted;
} USBHUB_URB_TIMEOUT_CONTEXT, *PUSBHUB_URB_TIMEOUT_CONTEXT;

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

NTSTATUS
NTAPI
USBH_Wait(
  IN ULONG Milliseconds);

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
USBH_FdoSyncSubmitUrb(
  IN PDEVICE_OBJECT FdoDevice,
  IN PURB Urb);

NTSTATUS
NTAPI
USBH_GetDeviceType(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PUSB_DEVICE_HANDLE DeviceHandle,
  OUT USB_DEVICE_TYPE * OutDeviceType);

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

PUSB_DEVICE_HANDLE
NTAPI
USBH_SyncGetDeviceHandle(
  IN PDEVICE_OBJECT DeviceObject);

NTSTATUS
NTAPI
USBH_GetDeviceDescriptor(
  IN PDEVICE_OBJECT DeviceObject,
  IN PUSB_DEVICE_DESCRIPTOR HubDeviceDescriptor);

NTSTATUS
NTAPI
USBH_GetConfigurationDescriptor(
  IN PDEVICE_OBJECT DeviceObject,
  IN PUSB_CONFIGURATION_DESCRIPTOR * pConfigurationDescriptor);

NTSTATUS
NTAPI
USBH_SyncGetHubDescriptor(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
USBH_SyncClearPortStatus(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN USHORT Port,
  IN USHORT RequestValue);

NTSTATUS
NTAPI
USBH_SyncPowerOnPorts(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

BOOLEAN
NTAPI
USBH_HubIsBusPowered(
  IN PDEVICE_OBJECT DeviceObject,
  IN PUSB_CONFIGURATION_DESCRIPTOR HubConfigDescriptor);

NTSTATUS
NTAPI
USBD_RegisterRootHubCallBack(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
USBD_UnRegisterRootHubCallBack(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
DriverEntry(
  IN PDRIVER_OBJECT DriverObject,
  IN PUNICODE_STRING RegistryPath);

#endif /* _USBHUB_H_ */
