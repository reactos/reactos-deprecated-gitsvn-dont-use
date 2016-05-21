#ifndef USBMPORT_H__
#define USBMPORT_H__

typedef struct _USBPORT_RESOURCES {
  ULONG TypesResources; // 1 | 2 | 4  (Port | Interrupt | Memory)
  ULONG InterruptVector;
  KIRQL InterruptLevel;
  BOOLEAN ShareVector;
  KINTERRUPT_MODE InterruptMode;
  KAFFINITY InterruptAffinity;
  PVOID ResourceBase;
  SIZE_T IoSpaceLength;
  PVOID StartVA;
  PVOID StartPA;
} USBPORT_RESOURCES, *PUSBPORT_RESOURCES;

typedef struct _USBPORT_REGISTRATION_PACKET {
  ULONG Version; // Type: 1 - OHCI, 2 - UHCI, 3 - EHCI
  ULONG MiniPortHwResourcesSize;
} USBPORT_REGISTRATION_PACKET, *PUSBPORT_REGISTRATION_PACKET;

typedef struct _USBPORT_MINIPORT_INTERFACE {
  PDRIVER_OBJECT DriverObject;
  LIST_ENTRY DriverList;
  PDRIVER_UNLOAD DriverUnload;
  USBPORT_REGISTRATION_PACKET Packet;
} USBPORT_MINIPORT_INTERFACE, *PUSBPORT_MINIPORT_INTERFACE;

#endif /* USBMPORT_H__ */
