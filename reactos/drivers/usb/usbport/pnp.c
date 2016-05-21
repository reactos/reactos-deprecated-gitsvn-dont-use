#include "usbport.h"

//#define NDEBUG
#include <debug.h>

static
NTSTATUS
NTAPI
USBPORT_ParseResources(PDEVICE_OBJECT FdoDevice,
                       PIRP Irp,
                       PUSBPORT_RESOURCES UsbPortResources)
{
    PCM_RESOURCE_LIST AllocatedResourcesTranslated;
    PCM_PARTIAL_RESOURCE_LIST ResourceList;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PartialDescriptor;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR PortDescriptor = NULL;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR MemoryDescriptor = NULL;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR InterruptDescriptor = NULL;
    ULONG ix = 0;
    NTSTATUS Status = STATUS_SUCCESS;

    DPRINT("USBPORT_ParseResources: ... \n");

    AllocatedResourcesTranslated = IoGetCurrentIrpStackLocation(Irp)->Parameters.StartDevice.AllocatedResourcesTranslated;

    if (AllocatedResourcesTranslated)
    {
        RtlZeroMemory(UsbPortResources, sizeof(USBPORT_RESOURCES));

        ResourceList = &AllocatedResourcesTranslated->List[0].PartialResourceList;

        if (ResourceList->Count > 0)
        {
            PartialDescriptor = &ResourceList->PartialDescriptors[0];

            do
            {
                if (PartialDescriptor->Type == CmResourceTypePort) // 1
                {
                    if (!PortDescriptor)
                        PortDescriptor = PartialDescriptor;
                }
                else if (PartialDescriptor->Type == CmResourceTypeInterrupt) // 2
                {
                    if (!InterruptDescriptor)
                        InterruptDescriptor = PartialDescriptor;
                }
                else if (PartialDescriptor->Type == CmResourceTypeMemory) // 3
                {
                    if (!MemoryDescriptor)
                        MemoryDescriptor = PartialDescriptor;
                }
                ++ix;
                PartialDescriptor += 1;
            }
            while (ix < ResourceList->Count);
        }

        if (PortDescriptor)
        {
            if (PortDescriptor->Flags & CM_RESOURCE_PORT_IO)
            {
                UsbPortResources->ResourceBase = (PVOID)PortDescriptor->u.Port.Start.LowPart;
            }
            else
            {
                UsbPortResources->ResourceBase = MmMapIoSpace(PortDescriptor->u.Port.Start,
                                                              PortDescriptor->u.Port.Length,
                                                              0);
            }

            UsbPortResources->IoSpaceLength = PortDescriptor->u.Port.Length;

            if (UsbPortResources->ResourceBase)
            {
                UsbPortResources->TypesResources |= 1; // FIXME const
            }
            else
            {
                Status = STATUS_NONE_MAPPED;
            }
        }
    }
    else
    {
        Status = STATUS_NONE_MAPPED;
    }

    return Status;
}

static
NTSTATUS
NTAPI
USBPORT_FdoStartCompletion(PDEVICE_OBJECT DeviceObject,
                           PIRP Irp,
                           PRKEVENT Event)
{
    KeSetEvent(Event, EVENT_INCREMENT, FALSE);
    return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS
NTAPI
USBPORT_FdoPnP(PDEVICE_OBJECT FdoDevice,
               PIRP Irp)
{
    PUSBPORT_DEVICE_EXTENSION FdoExtention = (PUSBPORT_DEVICE_EXTENSION)FdoDevice->DeviceExtension;
    PUSBPORT_RESOURCES UsbPortResources = (PUSBPORT_RESOURCES)&FdoExtention->UsbPortResources;
    PIO_STACK_LOCATION IoStack = IoGetCurrentIrpStackLocation(Irp);
    UCHAR Minor = IoStack->MinorFunction;
    KEVENT Event;
    NTSTATUS Status = STATUS_SUCCESS;

    DPRINT("USBPORT_FdoPnP: Minor - %d\n", Minor);

    switch (Minor)
    {
        case IRP_MN_START_DEVICE: // 0
            DPRINT("IRP_MN_START_DEVICE\n");

            KeInitializeEvent(&Event, NotificationEvent, FALSE);

            IoCopyCurrentIrpStackLocationToNext(Irp);

            IoSetCompletionRoutine(Irp,
                                   (PIO_COMPLETION_ROUTINE)USBPORT_FdoStartCompletion,
                                   &Event,
                                   1,
                                   1,
                                   1);

            Status = IoCallDriver(FdoExtention->CommonExtension.LowerDevice,
                                  Irp);

            if (Status == STATUS_PENDING)
            {
                KeWaitForSingleObject(&Event,
                                      Suspended,
                                      KernelMode,
                                      FALSE,
                                      NULL);

                Status = Irp->IoStatus.Status;
            }

            if (NT_SUCCESS(Status))
            {
                Status = USBPORT_ParseResources(FdoDevice,
                                                Irp,
                                                UsbPortResources);

                if (NT_SUCCESS(Status))
                {
                    Status = 0; // USBPORT_StartDevice(FdoDevice, Irp);
                }
            }

            Irp->IoStatus.Status = Status;
            IoCompleteRequest(Irp, IO_NO_INCREMENT);
            return Status;

        case IRP_MN_QUERY_REMOVE_DEVICE: // 1
            DPRINT("IRP_MN_QUERY_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_REMOVE_DEVICE: // 2
            DPRINT("IRP_MN_REMOVE_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_CANCEL_REMOVE_DEVICE: // 3
            DPRINT("IRP_MN_CANCEL_REMOVE_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_STOP_DEVICE: // 4
            DPRINT("IRP_MN_STOP_DEVICE\n");
            ASSERT(FALSE);
            break;

        case IRP_MN_QUERY_STOP_DEVICE: // 5
            DPRINT("IRP_MN_QUERY_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_CANCEL_STOP_DEVICE: // 6
            DPRINT("IRP_MN_CANCEL_STOP_DEVICE\n");
            Irp->IoStatus.Status = STATUS_SUCCESS;
            goto ForwardIrp;

        case IRP_MN_QUERY_DEVICE_RELATIONS: // 7
            DPRINT("IRP_MN_QUERY_DEVICE_RELATIONS\n");
            ASSERT(FALSE);
            Irp->IoStatus.Status = Status;
            goto ForwardIrp;

        case IRP_MN_QUERY_INTERFACE: // 8
            DPRINT("IRP_MN_QUERY_INTERFACE\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_CAPABILITIES:           // 9
            DPRINT("IRP_MN_QUERY_CAPABILITIES\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_RESOURCES:              // 10
            DPRINT("IRP_MN_QUERY_RESOURCES\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_RESOURCE_REQUIREMENTS:  // 11
            DPRINT("IRP_MN_QUERY_RESOURCE_REQUIREMENTS\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_DEVICE_TEXT:            // 12
            DPRINT("IRP_MN_QUERY_DEVICE_TEXT\n");
            goto ForwardIrp;

        case IRP_MN_FILTER_RESOURCE_REQUIREMENTS: // 13
            DPRINT("IRP_MN_FILTER_RESOURCE_REQUIREMENTS\n");
            goto ForwardIrp;

        case IRP_MN_READ_CONFIG: // 15
            DPRINT("IRP_MN_READ_CONFIG\n");
            goto ForwardIrp;

        case IRP_MN_WRITE_CONFIG: // 16
            DPRINT("IRP_MN_WRITE_CONFIG\n");
            goto ForwardIrp;

        case IRP_MN_EJECT: // 17
            DPRINT("IRP_MN_EJECT\n");
            goto ForwardIrp;

        case IRP_MN_SET_LOCK: // 18
            DPRINT("IRP_MN_SET_LOCK\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_ID: // 19
            DPRINT("IRP_MN_QUERY_ID\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_PNP_DEVICE_STATE: // 20
            DPRINT("IRP_MN_QUERY_PNP_DEVICE_STATE\n");
            goto ForwardIrp;

        case IRP_MN_QUERY_BUS_INFORMATION: // 21
            DPRINT("IRP_MN_QUERY_BUS_INFORMATION\n");
            goto ForwardIrp;

        case IRP_MN_DEVICE_USAGE_NOTIFICATION: // 22
            DPRINT("IRP_MN_DEVICE_USAGE_NOTIFICATION\n");
            goto ForwardIrp;

        case IRP_MN_SURPRISE_REMOVAL: // 23
            DPRINT("IRP_MN_SURPRISE_REMOVAL\n");
            ASSERT(FALSE);
            goto ForwardIrp;
            break;

        default:
            DPRINT("unknown IRP_MN_???\n");
ForwardIrp:
            // forward irp to next device object
            IoSkipCurrentIrpStackLocation(Irp);
            return IoCallDriver(FdoExtention->CommonExtension.LowerDevice, Irp);
            break;
    }

    return Status;
}
