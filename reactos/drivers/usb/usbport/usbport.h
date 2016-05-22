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

extern KSPIN_LOCK USBPORT_SpinLock;
extern LIST_ENTRY USBPORT_MiniPortDrivers;

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
} USBPORT_DEVICE_EXTENSION, *PUSBPORT_DEVICE_EXTENSION;

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(
  PDRIVER_OBJECT DriverObject,
  ULONG Version,
  PUSBPORT_REGISTRATION_PACKET RegistrationPacket);

ULONG
NTAPI
USBPORT_GetHciMn(VOID);

NTSTATUS
NTAPI
USBPORT_Dispatch(
  PDEVICE_OBJECT DeviceObject,
  PIRP Irp);

NTSTATUS
NTAPI
USBPORT_FdoPnP(
  PDEVICE_OBJECT FdoDevice,
  PIRP Irp);

VOID
NTAPI
USBPORT_IsrDpc(
  PRKDPC Dpc,
  PVOID DeferredContext,
  PVOID SystemArgument1,
  PVOID SystemArgument2);

#endif /* USBPORT_H__ */
