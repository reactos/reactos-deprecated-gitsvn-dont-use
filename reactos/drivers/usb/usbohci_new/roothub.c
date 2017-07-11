#include "usbohci.h"

//#define NDEBUG
#include <debug.h>

VOID
NTAPI
OHCI_RH_GetRootHubData(IN PVOID ohciExtension,
                       IN PVOID rootHubData)
{
    DPRINT("OHCI_RH_GetRootHubData: UNIMPLEMENTED. FIXME\n");
}

MPSTATUS
NTAPI
OHCI_RH_GetStatus(IN PVOID ohciExtension,
                  IN PUSHORT Status)
{
    DPRINT("OHCI_RH_GetStatus: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_GetPortStatus(IN PVOID ohciExtension,
                      IN USHORT Port,
                      IN PULONG PortStatus)
{
    DPRINT("OHCI_RH_GetPortStatus: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_GetHubStatus(IN PVOID ohciExtension,
                     IN PULONG HubStatus)
{
    DPRINT("OHCI_RH_GetHubStatus: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_SetFeaturePortReset(IN PVOID ohciExtension,
                            IN USHORT Port)
{
    DPRINT("OHCI_RH_SetFeaturePortReset: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_SetFeaturePortPower(IN PVOID ohciExtension,
                            IN USHORT Port)
{
    DPRINT("OHCI_RH_SetFeaturePortPower: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_SetFeaturePortEnable(IN PVOID ohciExtension,
                             IN USHORT Port)
{
    DPRINT("OHCI_RH_SetFeaturePortEnable: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_SetFeaturePortSuspend(IN PVOID ohciExtension,
                              IN USHORT Port)
{
    DPRINT("OHCI_RH_SetFeaturePortSuspend: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortEnable(IN PVOID ohciExtension,
                               IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortEnable: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortPower(IN PVOID ohciExtension,
                              IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortPower: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortSuspend(IN PVOID ohciExtension,
                                IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortSuspend: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortEnableChange(IN PVOID ohciExtension,
                                     IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortEnableChange: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortConnectChange(IN PVOID ohciExtension,
                                      IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortConnectChange: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortResetChange(IN PVOID ohciExtension,
                                    IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortResetChange: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortSuspendChange(IN PVOID ohciExtension,
                                      IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortSuspendChange: UNIMPLEMENTED. FIXME\n");
    return 0;
}

MPSTATUS
NTAPI
OHCI_RH_ClearFeaturePortOvercurrentChange(IN PVOID ohciExtension,
                                          IN USHORT Port)
{
    DPRINT("OHCI_RH_ClearFeaturePortOvercurrentChange: UNIMPLEMENTED. FIXME\n");
    return 0;
}

VOID
NTAPI
OHCI_RH_DisableIrq(IN PVOID ohciExtension)
{
    DPRINT("OHCI_RH_DisableIrq: UNIMPLEMENTED. FIXME\n");
}

VOID
NTAPI
OHCI_RH_EnableIrq(IN PVOID ohciExtension)
{
    DPRINT("OHCI_RH_EnableIrq: UNIMPLEMENTED. FIXME\n");
}
