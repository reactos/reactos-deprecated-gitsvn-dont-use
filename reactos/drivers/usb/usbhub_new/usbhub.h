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
#define USBHUB_FDO_FLAG_DEVICE_STOPPED    (1 << 5)    // 0x00000020
#define USBHUB_FDO_FLAG_HUB_BUSY          (1 << 6)    // 0x00000040
#define USBHUB_FDO_FLAG_PENDING_WAKE_IRP  (1 << 7)    // 0x00000080
#define USBHUB_FDO_FLAG_RESET_PORT_LOCK   (1 << 8)    // 0x00000100
#define USBHUB_FDO_FLAG_ESD_RECOVERING    (1 << 9)    // 0x00000200
#define USBHUB_FDO_FLAG_NOT_D0_STATE      (1 << 11)   // 0x00000800
#define USBHUB_FDO_FLAG_HUB_IDLE_ON       (1 << 12)   // 0x00001000
#define USBHUB_FDO_FLAG_USB20_HUB         (1 << 15)   // 0x00008000
#define USBHUB_FDO_FLAG_DEFER_CHECK_IDLE  (1 << 16)   // 0x00010000
#define USBHUB_FDO_FLAG_MULTIPLE_TTS      (1 << 18)   // 0x00040000 // High-speed Operating Hub with Multiple TTs
#define USBHUB_FDO_FLAG_ENUM_POST_RECOVER (1 << 19)   // 0x00080000
#define USBHUB_FDO_FLAG_DO_ENUMERATION    (1 << 20)   // 0x00100000
#define USBHUB_FDO_FLAG_CHECK_IDLE_LOCK   (1 << 21)   // 0x00200000
#define USBHUB_FDO_FLAG_NOT_ENUMERATED    (1 << 23)   // 0x00800000
#define USBHUB_FDO_FLAG_WITEM_INIT        (1 << 27)   // 0x08000000

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

typedef struct _USBHUB_FDO_EXTENSION *PUSBHUB_FDO_EXTENSION;

typedef VOID
(NTAPI * PUSBHUB_WORKER_ROUTINE)(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PVOID Context);

typedef struct _USBHUB_IO_WORK_ITEM {
  ULONG Reserved;
  LIST_ENTRY HubWorkItemLink;
  LONG HubWorkerQueued;
  PIO_WORKITEM HubWorkItem;
  WORK_QUEUE_TYPE HubWorkItemType;
  PUSBHUB_FDO_EXTENSION HubExtension;
  PUSBHUB_WORKER_ROUTINE HubWorkerRoutine;
  PVOID HubWorkItemBuffer;
} USBHUB_IO_WORK_ITEM, *PUSBHUB_IO_WORK_ITEM;

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
  POWER_STATE SystemPowerState;
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
  PVOID SCEBitmap;
  ULONG SCEBitmapLength;
  KEVENT RootHubNotificationEvent;
  struct _URB_CONTROL_VENDOR_OR_CLASS_REQUEST SCEWorkerUrb;
  KEVENT StatusChangeEvent;
  KSEMAPHORE IdleSemaphore;
  KSPIN_LOCK RelationsWorkerSpinLock;
  LIST_ENTRY PdoList;
  LONG PendingRequestCount;
  KEVENT PendingRequestEvent;
  KSEMAPHORE ResetDeviceSemaphore;
  PRKEVENT pResetPortEvent;
  KSEMAPHORE HubPortSemaphore;
  LONG ResetRequestCount;
  KEVENT ResetEvent;
  PIRP PendingIdleIrp;
  PIRP PendingWakeIrp;
  LIST_ENTRY WorkItemList;
  KSPIN_LOCK WorkItemSpinLock;
} USBHUB_FDO_EXTENSION, *PUSBHUB_FDO_EXTENSION;

typedef struct _USBHUB_PORT_PDO_EXTENSION {
  COMMON_DEVICE_EXTENSION Common;
  ULONG PortPdoFlags;
  PUSBHUB_FDO_EXTENSION HubExtension;
  PUSB_DEVICE_HANDLE DeviceHandle;
  USHORT PortNumber;
  USHORT SN_DescriptorLength;
  BOOL IgnoringHwSerial;
  USB_DEVICE_DESCRIPTOR DeviceDescriptor;
  USHORT Reserved1;
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
USBH_SyncGetStringDescriptor(
  IN PDEVICE_OBJECT DeviceObject,
  IN UCHAR Index,
  IN USHORT LanguageId,
  IN PUSB_STRING_DESCRIPTOR Descriptor,
  IN ULONG NumberOfBytes,
  IN PULONG OutLength,
  IN BOOLEAN IsValidateLength);

NTSTATUS
NTAPI
USBH_SyncGetPortStatus(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN USHORT Port,
  IN PUSBHUB_PORT_STATUS PortStatus,
  IN ULONG Length);

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
USBH_SubmitStatusChangeTransfer(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
USBD_CreateDeviceEx(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PUSB_DEVICE_HANDLE * OutDeviceHandle,
  IN USB_PORT_STATUS UsbPortStatus,
  IN USHORT Port);

NTSTATUS
NTAPI
USBD_RemoveDeviceEx(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PUSB_DEVICE_HANDLE DeviceHandle,
  IN ULONG Flags);

NTSTATUS
NTAPI
USBD_InitializeDeviceEx(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  IN PUSB_DEVICE_HANDLE DeviceHandle,
  IN PUCHAR DeviceDescriptorBuffer,
  IN ULONG DeviceDescriptorBufferLength,
  IN PUCHAR ConfigDescriptorBuffer,
  IN ULONG ConfigDescriptorBufferLength);

NTSTATUS
NTAPI
USBH_AllocateWorkItem(
  IN PUSBHUB_FDO_EXTENSION HubExtension,
  OUT PUSBHUB_IO_WORK_ITEM * OutHubIoWorkItem,
  IN PVOID WorkerRoutine,
  IN SIZE_T BufferLength,
  OUT PVOID * OutHubWorkItemBuffer,
  IN WORK_QUEUE_TYPE Type);

NTSTATUS
NTAPI
USBD_RegisterRootHubCallBack(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
USBD_UnRegisterRootHubCallBack(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

VOID
NTAPI
USBH_CheckIdleDeferred(
  IN PUSBHUB_FDO_EXTENSION HubExtension);

NTSTATUS
NTAPI
DriverEntry(
  IN PDRIVER_OBJECT DriverObject,
  IN PUNICODE_STRING RegistryPath);

#endif /* _USBHUB_H_ */
