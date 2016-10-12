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

#define USBPORT_RECIPIENT_ROOT_HUB  BMREQUEST_TO_DEVICE
#define USBPORT_RECIPIENT_ROOT_PORT BMREQUEST_TO_OTHER 

#define USBPORT_TRANSFER_TYPE_ISOCHRONOUS 0
#define USBPORT_TRANSFER_TYPE_CONTROL     1
#define USBPORT_TRANSFER_TYPE_BULK        2
#define USBPORT_TRANSFER_TYPE_INTERRUPT   3

#define INVALIDATE_CONTROLLER_RESET           1
#define INVALIDATE_CONTROLLER_SURPRISE_REMOVE 2
#define INVALIDATE_CONTROLLER_SOFT_INTERRUPT  3

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

#define USBPORT_FLAG_DEVICE_STARTED    0x00000002
#define USBPORT_FLAG_WORKER_THREAD_ON  0x00000008
#define USBPORT_FLAG_HC_SUSPEND        0x00000100
#define USBPORT_FLAG_INTERRUPT_ENABLED 0x00000400
#define USBPORT_FLAG_RH_INIT_CALLBACK  0x80000000

/* Timer Flags */

#define USBPORT_TMFLAG_HC_SUSPENDED       0x00000002
#define USBPORT_TMFLAG_RH_SUSPENDED       0x00000008
#define USBPORT_TMFLAG_TIMER_STARTED      0x00000010
#define USBPORT_TMFLAG_WAKE               0x00000020
#define USBPORT_TMFLAG_IDLE_QUEUEITEM_ON  0x00000040

/* Miniport Flags */

#define USBPORT_MPFLAG_INTERRUPTS_ENABLED  0x00000001 
#define USBPORT_MPFLAG_SUSPENDED           0x00000002 

/* Device handle Flags (USBPORT_DEVICE_HANDLE) */

#define DEVICE_HANDLE_FLAG_ROOTHUB     0x00000002

/* Endpoint Flags (USBPORT_ENDPOINT) */

#define ENDPOINT_FLAG_DMA_TYPE      0x00000001
#define ENDPOINT_FLAG_ROOTHUB_EP0   0x00000002
#define ENDPOINT_FLAG_NUKE          0x00000008
#define ENDPOINT_FLAG_QUEUENE_EMPTY 0x00000010
#define ENDPOINT_FLAG_IDLE          0x00000100
#define ENDPOINT_FLAG_OPENED        0x00000200
#define ENDPOINT_FLAG_CLOSED        0x00000400

/* UsbdFlags Flags (URB) */

#define USBD_FLAG_ALLOCATED_MDL      0x00000002
#define USBD_FLAG_ALLOCATED_TRANSFER 0x00000020

/* Pipe handle Flags (USBPORT_PIPE_HANDLE) */

#define PIPEHANDLE_FLAG_CLOSED 0x00000001

/* Transfer Flags (USBPORT_TRANSFER) */

#define TRANSFER_FLAG_CANCELED   0x00000001
#define TRANSFER_FLAG_DMA_MAPPED 0x00000002
#define TRANSFER_FLAG_SUBMITED   0x00000008
#define TRANSFER_FLAG_ABORTED    0x00000010

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
  ULONG Flags;
  USHORT DeviceAddress;
  USHORT PortNumber;
  USBPORT_PIPE_HANDLE PipeHandle;
  ULONG DeviceSpeed;
  BOOL IsRootHub;
  LIST_ENTRY PipeHandleList;
  PUSBPORT_CONFIGURATION_HANDLE ConfigHandle;
  struct _USBPORT_DEVICE_HANDLE *HubDeviceHandle;
  USB_DEVICE_DESCRIPTOR DeviceDescriptor; // 0x12
  LIST_ENTRY DeviceHandleLink;
  LONG DeviceHandleLock;
} USBPORT_DEVICE_HANDLE, *PUSBPORT_DEVICE_HANDLE;

typedef struct _USBPORT_ENDPOINT {
  ULONG Flags;
  PDEVICE_OBJECT FdoDevice;
  PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer;
  PUSBPORT_DEVICE_HANDLE DeviceHandle;
  USBPORT_ENDPOINT_PROPERTIES EndpointProperties;
  ULONG EndpointWorker;
  ULONG FrameNumber;
  /* Locks */
  KSPIN_LOCK EndpointSpinLock;
  KIRQL EndpointOldIrql;
  KIRQL EndpointStateOldIrql;
  UCHAR Padded[2];
  LONG LockCounter;
  LONG FlushPendingLock;
  /* State */
  ULONG StateLast;
  ULONG StateNext;
  LIST_ENTRY StateChangeLink;
  KSPIN_LOCK StateChangeSpinLock;
  /* Transfer lists */
  LIST_ENTRY PendingTransferList;
  LIST_ENTRY TransferList;
  LIST_ENTRY CancelList;
  LIST_ENTRY AbortList;
  /* Links */
  LIST_ENTRY EndpointLink;
  LIST_ENTRY WorkerLink;
  LIST_ENTRY CloseLink;
  LIST_ENTRY DispatchLink;
  LIST_ENTRY FlushLink;
  LIST_ENTRY FlushControllerLink;
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
  ULONG NumberOfMapRegisters;
  PVOID MapRegisterBase;
  // SgList should be LAST field
  USBPORT_SCATTER_GATHER_LIST SgList; // Non IsoTransfer
} USBPORT_TRANSFER, *PUSBPORT_TRANSFER;

typedef struct _USBPORT_IRP_TABLE {
  struct _USBPORT_IRP_TABLE * LinkNextTable; 
  PIRP irp[0X200];
} USBPORT_IRP_TABLE, *PUSBPORT_IRP_TABLE;

typedef struct _USBPORT_COMMON_DEVICE_EXTENSION {
  PDEVICE_OBJECT SelfDevice;
  PDEVICE_OBJECT LowerPdoDevice; // PhysicalDeviceObject
  PDEVICE_OBJECT LowerDevice; // TopOfStackDeviceObject
  ULONG IsPDO;
  UNICODE_STRING SymbolicLinkName;
  BOOL IsInterfaceEnabled;
  DEVICE_POWER_STATE DevicePowerState;
} USBPORT_COMMON_DEVICE_EXTENSION, *PUSBPORT_COMMON_DEVICE_EXTENSION;

typedef struct _USBPORT_DEVICE_EXTENSION {
  USBPORT_COMMON_DEVICE_EXTENSION CommonExtension;
  ULONG Flags;
  PDEVICE_OBJECT RootHubPdo; // RootHubDeviceObject
  ULONG FdoNameNumber;
  /* Miniport */
  ULONG MiniPortFlags;
  PVOID MiniPortExt;
  PUSBPORT_MINIPORT_INTERFACE MiniPortInterface;
  USBPORT_RESOURCES UsbPortResources;
  PUSBPORT_COMMON_BUFFER_HEADER MiniPortCommonBuffer;
  KSPIN_LOCK MiniportSpinLock;
  /* Bus Interface */
  BUS_INTERFACE_STANDARD BusInterface;
  USHORT VendorID;
  USHORT DeviceID;
  UCHAR RevisionID;
  UCHAR ProgIf;
  UCHAR SubClass;
  UCHAR BaseClass;
  /* Dma Adapter */
  PDMA_ADAPTER DmaAdapter;
  ULONG NumberMapRegs;
  /* Interrupt */
  PKINTERRUPT InterruptObject;
  KDPC IsrDpc;
  LONG IsrDpcCounter;
  LONG IsrDpcHandlerCounter;
  KSPIN_LOCK MiniportInterruptsSpinLock;
  KTIMER TimerSoftInterrupt;
  KDPC SoftInterruptDpc;
  /* Endpoints */
  ULONG PeriodicEndpoints;
  LIST_ENTRY EndpointList;
  KSPIN_LOCK EndpointListSpinLock;
  LIST_ENTRY EpStateChangeList;
  KSPIN_LOCK EpStateChangeSpinLock;
  LIST_ENTRY EndpointClosedList;
  KSPIN_LOCK EndpointClosedSpinLock;
  LIST_ENTRY WorkerList;
  /* Transfers */
  LIST_ENTRY MapTransferList;
  LIST_ENTRY DoneTransferList;
  KSPIN_LOCK DoneTransferSpinLock;
  KDPC TransferFlushDpc;
  KSPIN_LOCK FlushTransferSpinLock;
  /* Timer */
  ULONG TimerValue; // Timer period (500) msec. default
  ULONG TimerFlags;
  KTIMER TimerObject;
  KDPC TimerDpc;
  KSPIN_LOCK TimerFlagsSpinLock;
  /* Worker Thread */
  PRKTHREAD WorkerThread;
  HANDLE WorkerThreadHandle;
  KEVENT WorkerThreadEvent;
  KSPIN_LOCK WorkerThreadEventSpinLock;
  /* Usb Devices */
  ULONG UsbAddressBitMap[4];
  LIST_ENTRY DeviceHandleList;
  KSPIN_LOCK DeviceHandleSpinLock;
  /* Device Capabilities */
  DEVICE_CAPABILITIES Capabilities;
  ULONG BusNumber;
  ULONG PciDeviceNumber;
  ULONG PciFunctionNumber;
  ULONG BusBandwidth;
  /* Idle */
  LARGE_INTEGER IdleTime;
  IO_CSQ IdleIoCsq;
  KSPIN_LOCK IdleIoCsqSpinLock;
  LIST_ENTRY IdleIrpList;
  LONG IdleLockCounter;
  /* Bad Requests */
  IO_CSQ BadRequestIoCsq;
  KSPIN_LOCK BadRequestIoCsqSpinLock;
  LIST_ENTRY BadRequestList;
  LONG BadRequestLockCounter;
  /* Irp Queues */
  PUSBPORT_IRP_TABLE PendingIrpTable;
  PUSBPORT_IRP_TABLE ActiveIrpTable;
  /* Power */
  LONG SetPowerLockCounter;
  KSPIN_LOCK PowerWakeSpinLock;
  KDPC WorkerRequestDpc;
  ULONG Padded[64]; // Miniport extension should be aligned on 0x100
} USBPORT_DEVICE_EXTENSION, *PUSBPORT_DEVICE_EXTENSION;

C_ASSERT(sizeof(USBPORT_DEVICE_EXTENSION) == 0x400);

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
  DEVICE_CAPABILITIES Capabilities;
  PIRP WakeIrp;
} USBPORT_RHDEVICE_EXTENSION, *PUSBPORT_RHDEVICE_EXTENSION;

typedef struct _USBPORT_ROOT_HUB_DATA {
  UCHAR NumberOfPorts;
  UCHAR Rezerved1[3];
  USHORT HubCharacteristics;
  USHORT Rezerved2;
  UCHAR PowerOnToPowerGood;
  UCHAR Rezerved3[3];
  UCHAR HubControlCurrent;
  UCHAR Rezerved4[3];
} USBPORT_ROOT_HUB_DATA, *PUSBPORT_ROOT_HUB_DATA;

C_ASSERT(sizeof(USBPORT_ROOT_HUB_DATA) == 16);

typedef VOID
(NTAPI ASYNC_TIMER_CALLBACK)(
  IN PVOID MiniportExtension,
  IN PVOID CallBackContext);

typedef struct _USBPORT_ASYNC_CALLBACK_DATA {
  ULONG Reserved;
  PDEVICE_OBJECT FdoDevice;
  KTIMER AsyncTimer;
  KDPC AsyncTimerDpc;
  ASYNC_TIMER_CALLBACK *CallbackFunction;
  ULONG CallbackContext;
} USBPORT_ASYNC_CALLBACK_DATA, *PUSBPORT_ASYNC_CALLBACK_DATA;

C_ASSERT(sizeof(USBPORT_ASYNC_CALLBACK_DATA) == 88);

typedef struct _TIMER_WORK_QUEUE_ITEM {
  WORK_QUEUE_ITEM WqItem;
  PDEVICE_OBJECT FdoDevice;
  ULONG Context;
} TIMER_WORK_QUEUE_ITEM, *PTIMER_WORK_QUEUE_ITEM;

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
NTAPI
USBPORT_USBDStatusToNtStatus(
  IN PURB Urb,
  IN USBD_STATUS USBDStatus);

NTSTATUS
NTAPI
USBPORT_Wait(
  IN PVOID Context,
  IN ULONG Milliseconds);

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

VOID
NTAPI
USBPORT_FreeCommonBuffer(
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_COMMON_BUFFER_HEADER HeaderBuffer);

USBD_STATUS
NTAPI
USBPORT_AllocateTransfer(
  IN PDEVICE_OBJECT FdoDevice,
  IN PURB Urb,
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
  IN PIRP Irp,
  IN PRKEVENT Event);

VOID
NTAPI
USBPORT_FlushMapTransfers(
  IN PDEVICE_OBJECT FdoDevice);

VOID
NTAPI
USBPORT_IsrDpc(
  IN PRKDPC Dpc,
  IN PVOID DeferredContext,
  IN PVOID SystemArgument1,
  IN PVOID SystemArgument2);

BOOLEAN
NTAPI
USBPORT_InterruptService(
  IN PKINTERRUPT Interrupt,
  IN PVOID ServiceContext);

VOID
NTAPI
USBPORT_SignalWorkerThread(
  IN PDEVICE_OBJECT FdoDevice);

BOOLEAN
NTAPI
USBPORT_EndpointWorker(
  IN PUSBPORT_ENDPOINT Endpoint,
  IN BOOLEAN Flag);

VOID
NTAPI
USBPORT_CompleteTransfer(
  IN PURB Urb,
  IN USBD_STATUS TransferStatus);

VOID
NTAPI
USBPORT_DpcHandler(
  IN PDEVICE_OBJECT FdoDevice);

VOID
NTAPI
USBPORT_WorkerRequestDpc(
  IN PRKDPC Dpc,
  IN PVOID DeferredContext,
  IN PVOID SystemArgument1,
  IN PVOID SystemArgument2);

VOID
NTAPI
USBPORT_InvalidateEndpointHandler(
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_ENDPOINT Endpoint,
  IN ULONG Type);

BOOLEAN
NTAPI
USBPORT_QueueDoneTransfer(
  IN PUSBPORT_TRANSFER Transfer,
  IN USBD_STATUS USBDStatus);

VOID
NTAPI
USBPORT_MiniportInterrupts(
  IN PDEVICE_OBJECT FdoDevice,
  IN BOOLEAN IsEnable);

VOID
NTAPI
USBPORT_NukeAllEndpoints(
  IN PDEVICE_OBJECT FdoDevice);

/* debug.c */

ULONG
NTAPI
USBPORT_DbgPrint(
  IN PVOID Context,
  IN ULONG Level,
  IN PCH Format,
  IN ULONG Arg1,
  IN ULONG Arg2,
  IN ULONG Arg3,
  IN ULONG Arg4,
  IN ULONG Arg5,
  IN ULONG Arg6);

ULONG
NTAPI
USBPORT_TestDebugBreak(
  IN PVOID Context);

ULONG
NTAPI
USBPORT_AssertFailure(
  PVOID Context,
  PVOID FailedAssertion,
  PVOID FileName,
  ULONG LineNumber,
  PCHAR Message);

VOID
NTAPI
USBPORT_BugCheck(
  IN PVOID Context);

ULONG
NTAPI
USBPORT_LogEntry(
  IN PVOID BusContext,
  IN PVOID DriverTag,
  IN PVOID EnumTag,
  IN ULONG P1,
  IN ULONG P2,
  IN ULONG P3);

VOID
NTAPI
USBPORT_DumpingDeviceDescriptor(
  IN PUSB_DEVICE_DESCRIPTOR DeviceDescriptor);

VOID
NTAPI
USBPORT_DumpingConfiguration(
  IN PUSB_CONFIGURATION_DESCRIPTOR ConfigDescriptor);

VOID
NTAPI
USBPORT_DumpingCapabilities(
  IN PDEVICE_CAPABILITIES Capabilities);

/* device.c */

NTSTATUS
NTAPI
USBPORT_HandleSelectConfiguration(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp,
  IN PURB Urb);

NTSTATUS
NTAPI
USBPORT_OpenPipe(
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_PIPE_HANDLE PipeHandle,
  IN PUSBD_STATUS UsbdStatus);

VOID
NTAPI
USBPORT_AddDeviceHandle(
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle);

VOID
NTAPI
USBPORT_RemoveDeviceHandle(
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle);

BOOLEAN
NTAPI
USBPORT_ValidateDeviceHandle(
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle);

NTSTATUS
NTAPI
USBPORT_CreateDevice(
  IN OUT PUSB_DEVICE_HANDLE *pUsbdDeviceHandle,
  IN PDEVICE_OBJECT FdoDevice,
  IN PUSBPORT_DEVICE_HANDLE HubDeviceHandle,
  IN USHORT PortStatus,
  IN USHORT Port);

NTSTATUS
NTAPI
USBPORT_InitializeDevice(
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
  IN PDEVICE_OBJECT FdoDevice);

NTSTATUS
NTAPI
USBPORT_GetUsbDescriptor(
  IN PUSBPORT_DEVICE_HANDLE DeviceHandle,
  IN PDEVICE_OBJECT FdoDevice,
  IN UCHAR Type,
  IN PUCHAR ConfigDesc,
  IN PULONG ConfigDescSize);

VOID
NTAPI
USBPORT_SetEndpointState(
  IN PUSBPORT_ENDPOINT Endpoint,
  IN ULONG State);

NTSTATUS
NTAPI
USBPORT_HandleSelectInterface(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp,
  IN PURB Urb);

NTSTATUS
NTAPI
USBPORT_RemoveDevice(
  IN PDEVICE_OBJECT FdoDevice,
  IN OUT PUSBPORT_DEVICE_HANDLE DeviceHandle,
  IN ULONG Flags);

VOID
NTAPI
USBPORT_FlushClosedEndpointList(
  IN PDEVICE_OBJECT FdoDevice);

/* iface.c */

NTSTATUS
NTAPI
USBPORT_PdoQueryInterface(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice,
  IN PIRP Irp);

/* ioctl.c */

NTSTATUS
NTAPI
USBPORT_PdoDeviceControl(
  PDEVICE_OBJECT PdoDevice,
  PIRP Irp);

NTSTATUS
NTAPI
USBPORT_FdoDeviceControl(
  PDEVICE_OBJECT FdoDevice,
  PIRP Irp);

NTSTATUS
NTAPI
USBPORT_FdoScsi(
  IN PDEVICE_OBJECT FdoDevice,
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

/* power.c */

NTSTATUS
NTAPI
USBPORT_PdoPower(
  IN PDEVICE_OBJECT PdoDevice,
  IN PIRP Irp);

NTSTATUS
NTAPI
USBPORT_FdoPower(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp);

NTSTATUS
NTAPI
USBPORT_IdleNotification(
  IN PDEVICE_OBJECT PdoDevice,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_AdjustDeviceCapabilities(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice);

/* queue.c */

VOID
NTAPI
USBPORT_InsertIdleIrp(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_RemoveIdleIrp(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

PIRP
NTAPI
USBPORT_PeekNextIdleIrp(
  IN PIO_CSQ Csq,
  IN PIRP Irp,
  IN PVOID PeekContext);

VOID
NTAPI
USBPORT_AcquireIdleLock(
  IN PIO_CSQ Csq,
  IN PKIRQL Irql);

VOID
NTAPI
USBPORT_ReleaseIdleLock(
  IN PIO_CSQ Csq,
  IN KIRQL Irql);

VOID
NTAPI
USBPORT_CompleteCanceledIdleIrp(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_InsertBadRequest(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_RemoveBadRequest(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

PIRP
NTAPI
USBPORT_PeekNextBadRequest(
  IN PIO_CSQ Csq,
  IN PIRP Irp,
  IN PVOID PeekContext);

VOID
NTAPI
USBPORT_AcquireBadRequestLock(
  IN PIO_CSQ Csq,
  IN PKIRQL Irql);

VOID
NTAPI
USBPORT_ReleaseBadRequestLock(
  IN PIO_CSQ Csq,
  IN KIRQL Irql);

VOID
NTAPI
USBPORT_CompleteCanceledBadRequest(
  IN PIO_CSQ Csq,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_InsertIrpInTable(
  IN PUSBPORT_IRP_TABLE IrpTable,
  IN PIRP Irp);

PIRP
NTAPI
USBPORT_RemovePendingTransferIrp(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp);

PIRP
NTAPI
USBPORT_RemoveActiveTransferIrp(
  IN PDEVICE_OBJECT FdoDevice,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_FindUrbInIrpTable(
  IN PUSBPORT_IRP_TABLE IrpTable,
  IN PURB Urb,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_CancelActiveTransferIrp(
  IN PDEVICE_OBJECT DeviceObject,
  IN PIRP Irp);

VOID
NTAPI
USBPORT_FlushAbortList(
  IN PUSBPORT_ENDPOINT Endpoint);

VOID
NTAPI
USBPORT_FlushCancelList(
  IN PUSBPORT_ENDPOINT Endpoint);

VOID
NTAPI
USBPORT_QueueTransferUrb(
  IN PURB Urb);

VOID
NTAPI
USBPORT_FlushAllEndpoints(
  IN PDEVICE_OBJECT FdoDevice);

VOID
NTAPI
USBPORT_FlushPendingTransfers(
  IN PUSBPORT_ENDPOINT Endpoint);

/* roothub.c */

VOID
NTAPI
USBPORT_RootHubEndpointWorker(
  PUSBPORT_ENDPOINT Endpoint);

NTSTATUS
NTAPI
USBPORT_RootHubCreateDevice(
  IN PDEVICE_OBJECT FdoDevice,
  IN PDEVICE_OBJECT PdoDevice);

ULONG
NTAPI
USBPORT_InvalidateRootHub(
  PVOID Context);

#endif /* USBPORT_H__ */
