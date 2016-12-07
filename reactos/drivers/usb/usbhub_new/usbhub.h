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

/* ioctl.c */
/* pnp.c */
/* power.c */

/* usbhub.c */
NTSTATUS
NTAPI
DriverEntry(
  IN PDRIVER_OBJECT DriverObject,
  IN PUNICODE_STRING RegistryPath);

#endif /* _USBHUB_H_ */
