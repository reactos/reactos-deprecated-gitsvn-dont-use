#ifndef USBPORT_H__
#define USBPORT_H__

#include <ntddk.h>
#include <windef.h>
#include <stdio.h>
#include <wdm.h>
#include <wdmguid.h>
#include <hubbusif.h>
#include <usbbusif.h>
#include <usbdlib.h>
#include "..\usbmport.h"

#define USB_PORT_TAG 'pbsu'
#define URB_FUNCTION_MAX 0x31

#define USBPORT_ENDPOINT_PAUSED 2
#define USBPORT_ENDPOINT_ACTIVE 3
#define USBPORT_ENDPOINT_CLOSED 4

/* Flags */

#define ENDPOINT_FLAG_DMA_TYPE      0x00000001
#define ENDPOINT_FLAG_ROOTHUB_EP0   0x00000002
#define ENDPOINT_FLAG_QUEUENE_EMPTY 0x00000010
#define ENDPOINT_FLAG_OPENED        0x00000200
#define ENDPOINT_FLAG_CLOSED        0x00000400

#define USBD_FLAG_ALLOCATED_MDL      0x00000002
#define USBD_FLAG_ALLOCATED_TRANSFER 0x00000020

#define PIPEHANDLE_FLAG_CLOSED 0x00000001

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
  USB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
  UCHAR Padded;
  PUSBPORT_ENDPOINT Endpoint;
  LIST_ENTRY PipeLink;
} USBPORT_PIPE_HANDLE, *PUSBPORT_PIPE_HANDLE;

typedef struct _USBPORT_DEVICE_HANDLE { 
  USHORT DeviceAddress;
  USHORT PortNumber;
  USBPORT_PIPE_HANDLE PipeHandle;
  ULONG DeviceSpeed;
  BOOL IsRootHub;
  LIST_ENTRY PipeHandleList;
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
} USBPORT_TRANSFER, *PUSBPORT_TRANSFER;

typedef struct _USBPORT_COMMON_DEVICE_EXTENSION {
  PDEVICE_OBJECT SelfDevice; // SelfDevice
  PDEVICE_OBJECT LowerPdoDevice; // PhysicalDeviceObject
  PDEVICE_OBJECT LowerDevice; // TopOfStackDeviceObject
  ULONG IsPDO;
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
  PUSBPORT_COMMON_BUFFER_HEADER MiniPortCommonBuffer;
  LIST_ENTRY EndpointList;
  KSPIN_LOCK EndpointListSpinLock;
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
  UNICODE_STRING RhSymbolicLinkName;
  BOOL IsInterfaceEnabled;
  USBPORT_DEVICE_HANDLE DeviceHandle;
  PUSBPORT_RH_DESCRIPTORS RootHubDescriptors;
  PUSBPORT_ENDPOINT Endpoint;
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

VOID
NTAPI
USBPORT_IsrDpc(
  IN PRKDPC Dpc,
  IN PVOID DeferredContext,
  IN PVOID SystemArgument1,
  IN PVOID SystemArgument2);

PUSBPORT_COMMON_BUFFER_HEADER
NTAPI
USBPORT_AllocateCommonBuffer(
  IN PDEVICE_OBJECT FdoDevice,
  IN SIZE_T BufferLength);

/* device.c */
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
