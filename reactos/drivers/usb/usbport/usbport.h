#ifndef USBPORT_H__
#define USBPORT_H__

#include <ntddk.h>
#include <windef.h>
#include <stdio.h>
#include <wdm.h>
#include <wdmguid.h>
#include <usb.h>
#include <hubbusif.h>
#include <usbbusif.h>
#include <usbdlib.h>
#include "..\usbmport.h"

#ifdef USBD_TRANSFER_DIRECTION
#undef USBD_TRANSFER_DIRECTION
#define USBD_TRANSFER_DIRECTION 0x00000001
#endif

#define USB_PORT_TAG 'pbsu'
#define URB_FUNCTION_MAX 0x31

/* Hub Class Feature Selectors (Recipient - Port) */
#define FEATURE_PORT_CONNECTION     0
#define FEATURE_PORT_ENABLE         1
#define FEATURE_PORT_SUSPEND        2
#define FEATURE_PORT_OVER_CURRENT   3
#define FEATURE_PORT_RESET          4
#define FEATURE_PORT_POWER          8
#define FEATURE_PORT_LOW_SPEED      9
#define FEATURE_C_PORT_CONNECTION   16
#define FEATURE_C_PORT_ENABLE       17
#define FEATURE_C_PORT_SUSPEND      18
#define FEATURE_C_PORT_OVER_CURRENT 19
#define FEATURE_C_PORT_RESET        20

/* Hub Class Feature Selectors (Recipient - Hub) */
#define FEATURE_C_HUB_LOCAL_POWER  0 
#define FEATURE_C_HUB_OVER_CURRENT 1

/* Endpoint states */

#define USBPORT_ENDPOINT_PAUSED 2
#define USBPORT_ENDPOINT_ACTIVE 3
#define USBPORT_ENDPOINT_CLOSED 4

/* Flags */

#define USBPORT_FLAG_WORKER_THREAD_ON 0x00000008
#define USBPORT_FLAG_RH_INIT_CALLBACK 0x80000000

#define ENDPOINT_FLAG_DMA_TYPE      0x00000001
#define ENDPOINT_FLAG_ROOTHUB_EP0   0x00000002
#define ENDPOINT_FLAG_QUEUENE_EMPTY 0x00000010
#define ENDPOINT_FLAG_OPENED        0x00000200
#define ENDPOINT_FLAG_CLOSED        0x00000400

#define USBD_FLAG_ALLOCATED_MDL      0x00000002
#define USBD_FLAG_ALLOCATED_TRANSFER 0x00000020

#define PIPEHANDLE_FLAG_CLOSED 0x00000001

#define TRANSFER_FLAG_DMA_MAPPED 0x00000002

extern KSPIN_LOCK USBPORT_SpinLock;
extern LIST_ENTRY USBPORT_MiniPortDrivers;

typedef USBD_STATUS* PUSBD_STATUS;

typedef struct _USBPORT_COMMON_BUFFER_HEADER {
  ULONG Length;
  ULONG_PTR BaseVA;
  PHYSICAL_ADDRESS LogicalAddress;
  SIZE_T BufferLength;
  ULONG_PTR VirtualAddress;
  ULONG_PTR PhysicalAddress;
} USBPORT_COMMON_BUFFER_HEADER, *PUSBPORT_COMMON_BUFFER_HEADER;

typedef struct _USBPORT_ENDPOINT *PUSBPORT_ENDPOINT;

typedef struct _USBPORT_PIPE_HANDLE {
  ULONG Flags;
  ULONG PipeFlags;
  USB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
  UCHAR Padded;
  PUSBPORT_ENDPOINT Endpoint;
  LIST_ENTRY PipeLink;
} USBPORT_PIPE_HANDLE, *PUSBPORT_PIPE_HANDLE;

typedef struct _USBPORT_CONFIGURATION_HANDLE {
  PUSB_CONFIGURATION_DESCRIPTOR ConfigurationDescriptor; // 00
  LIST_ENTRY InterfaceHandleList; // 04
  //USB_CONFIGURATION_DESCRIPTOR CfgDescriptor; // 12 Body
} USBPORT_CONFIGURATION_HANDLE, *PUSBPORT_CONFIGURATION_HANDLE;

typedef struct _USBPORT_INTERFACE_HANDLE {
  LIST_ENTRY InterfaceLink; // 00
  UCHAR AlternateSetting; // 08
  UCHAR Pad1[3]; // 09
  USB_INTERFACE_DESCRIPTOR InterfaceDescriptor; // 12
  UCHAR Pad2[3]; // 21
  USBPORT_PIPE_HANDLE PipeHandle[1]; // 24
} USBPORT_INTERFACE_HANDLE, *PUSBPORT_INTERFACE_HANDLE;

typedef struct _USBPORT_DEVICE_HANDLE { 
  USHORT DeviceAddress;
  USHORT PortNumber;
  USBPORT_PIPE_HANDLE PipeHandle;
  ULONG DeviceSpeed;
  BOOL IsRootHub;
  LIST_ENTRY PipeHandleList;
  PUSBPORT_CONFIGURATION_HANDLE ConfigHandle;
} USBPORT_DEVICE_HANDLE, *PUSBPORT_DEVICE_HANDLE;

typedef struct _USBPORT_ENDPOINT {
  ULONG Flags;
  PDEVICE_OBJECT FdoDevice;
  PUSBPORT_DEVICE_HANDLE DeviceHandle;
  USBPORT_ENDPOINT_PROPERTIES EndpointProperties;
  ULONG EndpointWorker;
  ULONG StateLast;
  ULONG StateNext;
  LIST_ENTRY EndpointLink;
  LIST_ENTRY PendingTransferList;
  LIST_ENTRY TransferList;
  LONG LockCounter;
} USBPORT_ENDPOINT, *PUSBPORT_ENDPOINT;

typedef struct _USBPORT_TRANSFER {
  ULONG Flags;
  PIRP Irp;
  PURB Urb;
  PRKEVENT Event;
  PVOID MiniportTransfer;
  SIZE_T PortTransferLength; // Only port part
  SIZE_T FullTransferLength; // Port + miniport
  PUSBPORT_ENDPOINT Endpoint;
  USBPORT_TRANSFER_PARAMETERS TransferParameters;
  PMDL TransferBufferMDL;
  ULONG Direction;
  LIST_ENTRY TransferLink;
  USBD_STATUS USBDStatus;
  ULONG CompletedTransferLen;
} USBPORT_TRANSFER, *PUSBPORT_TRANSFER;

typedef struct _USBPORT_COMMON_DEVICE_EXTENSION {
  PDEVICE_OBJECT SelfDevice; // SelfDevice
  PDEVICE_OBJECT LowerPdoDevice; // PhysicalDeviceObject
  PDEVICE_OBJECT LowerDevice; // TopOfStackDeviceObject
  ULONG IsPDO;
  UNICODE_STRING SymbolicLinkName;
  BOOL IsInterfaceEnabled;
} USBPORT_COMMON_DEVICE_EXTENSION, *PUSBPORT_COMMON_DEVICE_EXTENSION;

typedef struct _USBPORT_DEVICE_EXTENSION {
  USBPORT_COMMON_DEVICE_EXTENSION CommonExtension;
  ULONG Flags;
  PDEVICE_OBJECT RootHubPdo; // RootHubDeviceObject
  PVOID MiniPortExt;
  PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;
  ULONG FdoNameNumber;
  USBPORT_RESOURCES UsbPortResources;
  BUS_INTERFACE_STANDARD BusInterface;
  USHORT VendorID;
  USHORT DeviceID;
  UCHAR RevisionID;
  UCHAR ProgIf;
  UCHAR SubClass;
  UCHAR BaseClass;
  PDMA_ADAPTER DmaAdapter;
  ULONG NumberMapRegs;
  PKINTERRUPT InterruptObject;
  KDPC IsrDpc;
  ULONG MiniPortInterruptEnable;
  PUSBPORT_COMMON_BUFFER_HEADER MiniPortCommonBuffer;
  LIST_ENTRY EndpointList;
  KSPIN_LOCK EndpointListSpinLock;
  LIST_ENTRY DoneTransferList;
  KSPIN_LOCK DoneTransferSpinLock;
  KDPC TransferFlushDpc;
  ULONG TimerValue; // Timer period (500) msec. default
  ULONG TimerFlags;
  KTIMER TimerObject;
  KDPC TimerDpc;
  PRKTHREAD WorkerThread;
  HANDLE WorkerThreadHandle;
  KEVENT WorkerThreadEvent;
} USBPORT_DEVICE_EXTENSION, *PUSBPORT_DEVICE_EXTENSION;

typedef struct _USBPORT_RH_DESCRIPTORS {
  USB_DEVICE_DESCRIPTOR DeviceDescriptor; // 18
  USB_CONFIGURATION_DESCRIPTOR ConfigDescriptor; // 9
  USB_INTERFACE_DESCRIPTOR InterfaceDescriptor; // 9
  USB_ENDPOINT_DESCRIPTOR EndPointDescriptor; // 7
  USB_HUB_DESCRIPTOR Descriptor; // 7 + 2[1..32] (7 + 2..64)
} USBPORT_RH_DESCRIPTORS, *PUSBPORT_RH_DESCRIPTORS;

typedef struct _USBPORT_RHDEVICE_EXTENSION {
  USBPORT_COMMON_DEVICE_EXTENSION CommonExtension;
  ULONG Flags;
  PDEVICE_OBJECT FdoDevice;
  ULONG PdoNameNumber;
  USBPORT_DEVICE_HANDLE DeviceHandle;
  PUSBPORT_RH_DESCRIPTORS RootHubDescriptors;
  PUSBPORT_ENDPOINT Endpoint;
  ULONG ConfigurationValue;
  PRH_INIT_CALLBACK RootHubInitCallback;
  PVOID RootHubInitContext;
} USBPORT_RHDEVICE_EXTENSION, *PUSBPORT_RHDEVICE_EXTENSION;

/* usbport.c */

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(
  IN PDRIVER_OBJECT DriverObject,
  IN ULONG Version,
  IN PUSBPORT_REGISTRATION_PACKET RegistrationPacket);

ULONG
NTAPI
USBPORT_GetHciMn(VOID);

NTSTATUS
USBPORT_USBDStatusToNtStatus(
  IN PURB Urb,
  IN USBD_STATUS USBDStatus);

VOID
NTAPI
USBPORT_IsrDpc(
  IN PRKDPC Dpc,
  IN PVOID DeferredContext,
  IN PVOID SystemArgument1,
  IN PVOID SystemArgument2);

VOID
NTAPI
USBPORT_TransferFlushDpc(
  IN PRKDPC Dpc,
  IN PVOID DeferredContext,
  IN PVOID SystemArgument1,
  IN PVOID SystemArgument2);

NTSTATUS
NTAPI
USBPORT_CreateWorkerThread(
  IN PDEVICE_OBJECT FdoDevice);

BOOLEAN
NTAPI
USBPORT_StartTimer(
  IN PDEVICE_OBJECT FdoDevice,
  IN ULONG Time);

PUSBPORT_COMMON_BUFFER_HEADER
NTAPI
USBPORT_AllocateCommonBuffer(
  IN PDEVICE_OBJECT FdoDevice,
  IN SIZE_T BufferLength);

USBD_STATUS
USBPORT_AllocateTransfer(
  IN PDEVICE_OBJECT FdoDevice,
  IN PURB Urb,
  IN PUSBPORT_DEVICE_HANDLE UsbdDeviceHandle,
  IN PIRP Irp,
  IN PRKEVENT Event);

VOID
USBPORT_QueueTransferUrb(IN PURB Urb);

/* device.c */

NTSTATUS
USBPORT_HandleSelectConfiguration(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp,
  IN PURB Urb);

NTSTATUS
NTAPI
USBPORT_OpenPipe(
  PUSBPORT_DEVICE_HANDLE DeviceHandle,
  PDEVICE_OBJECT FdoDevice,
  PUSBPORT_PIPE_HANDLE PipeHandle,
  PUSBD_STATUS UsbdStatus);

/* iface.c */

NTSTATUS
NTAPI
USBPORT_PdoQueryInterface(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice,
  IN PIRP Irp);

/* pnp.c */

NTSTATUS
NTAPI
USBPORT_FdoPnP(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp);

NTSTATUS
NTAPI
USBPORT_PdoPnP(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp);

/* roothub.c */

VOID
USBPORT_RootHubEndpointWorker(
  PUSBPORT_ENDPOINT Endpoint);

NTSTATUS
NTAPI
USBPORT_RootHubCreateDevice(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice);

#endif /* USBPORT_H__ */
