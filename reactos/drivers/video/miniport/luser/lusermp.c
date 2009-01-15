/*
 * ReactOS userland
 */

#include "luserprt.h"

VP_STATUS NTAPI
LuserFindAdapter(
    IN PVOID HwDeviceExtension,
    IN PVOID HwContext,
    IN PWSTR ArgumentString,
    IN OUT PVIDEO_PORT_CONFIG_INFO ConfigInfo,
    OUT PUCHAR Again)
{
    DPRINT1("LuserFindAdapter\n");
    return NO_ERROR;
}

BOOLEAN NTAPI
LuserInitialize(PVOID HwDeviceExtension)
{
    DPRINT1("LuserInitialize\n");
    return TRUE;
}

BOOLEAN NTAPI
LuserStartIO(
    PVOID HwDeviceExtension,
    PVIDEO_REQUEST_PACKET RequestPacket)
{
    DPRINT1("LuserStartIO\n");
    return FALSE;
}

BOOLEAN NTAPI
LuserResetHw(
    PVOID DeviceExtension,
    ULONG Columns,
    ULONG Rows)
{
    DPRINT1("LuserResetHw\n");
    return FALSE;
}

VP_STATUS NTAPI
DriverEntry(IN PVOID Context1, IN PVOID Context2)
{
    VIDEO_HW_INITIALIZATION_DATA InitData;

    DPRINT1("DriverEntry\n");

    VideoPortZeroMemory(&InitData, sizeof(InitData));
    InitData.HwInitDataSize = sizeof(VIDEO_HW_INITIALIZATION_DATA);
    InitData.HwFindAdapter = LuserFindAdapter;
    InitData.HwInitialize = LuserInitialize;
    InitData.HwStartIO = LuserStartIO;
    InitData.HwResetHw = LuserResetHw;
    InitData.HwDeviceExtensionSize = sizeof(LUSER_DEVICE_EXTENSION);

    return VideoPortInitialize(Context1, Context2, &InitData, NULL);
}
