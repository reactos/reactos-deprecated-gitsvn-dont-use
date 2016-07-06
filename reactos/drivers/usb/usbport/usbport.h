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

/* Flags */
#define USBD_FLAG_ALLOCATED_MDL 0x00000002

extern KSPIN_LOCK USBPORT_SpinLock;
extern LIST_ENTRY USBPORT_MiniPortDrivers;

typedef struct _USBPORT_COMMON_BUFFER_HEADER {
  ULONG Length;
  ULONG_PTR BaseVA;
  PHYSICAL_ADDRESS LogicalAddress;
  SIZE_T BufferLength;
  ULONG_PTR VirtualAddress;
  ULONG_PTR PhysicalAddress;
} USBPORT_COMMON_BUFFER_HEADER, *PUSBPORT_COMMON_BUFFER_HEADER;

typedef struct _USBPORT_PIPE_HANDLE {
  USB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
  UCHAR Padded;
} USBPORT_PIPE_HANDLE, *PUSBPORT_PIPE_HANDLE;

typedef struct _USBPORT_DEVICE_HANDLE { 
  USHORT DeviceAddress;
  USHORT PortNumber;
  USBPORT_PIPE_HANDLE PipeHandle;
} USBPORT_DEVICE_HANDLE, *PUSBPORT_DEVICE_HANDLE;

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
} USBPORT_DEVICE_EXTENSION, *PUSBPORT_DEVICE_EXTENSION;

typedef struct _USBPORT_RHDEVICE_EXTENSION {
  USBPORT_COMMON_DEVICE_EXTENSION CommonExtension;
  ULONG Flags;
  PDEVICE_OBJECT FdoDevice;
  ULONG PdoNameNumber;
  UNICODE_STRING RhSymbolicLinkName;
  BOOL IsInterfaceEnabled;
  USBPORT_DEVICE_HANDLE DeviceHandle;
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

NTSTATUS
NTAPI
USBPORT_CreateRhDevice(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice);

#endif /* USBPORT_H__ */
