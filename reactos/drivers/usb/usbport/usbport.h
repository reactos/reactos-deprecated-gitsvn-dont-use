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

NTSTATUS
NTAPI
USBPORT_RegisterUSBPortDriver(
  PDRIVER_OBJECT DriverObject,
  ULONG Version,
  PUSBPORT_REGISTRATION_PACKET RegistrationPacket);

ULONG
NTAPI
USBPORT_GetHciMn(VOID);

#endif /* USBPORT_H__ */
