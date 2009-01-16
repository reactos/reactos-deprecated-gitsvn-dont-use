/*
 * ReactOS userland
 */

#include "luserprt.h"
#include <luser.h>
#include <lunix.h>
#include <lstorage.h>

void ReadLuserCPUData(struct LuserCPUData *data);

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
    struct LuserCPUData cpu;
    BOOLEAN Result = TRUE;
    ULONG inIoSpace = VIDEO_MEMORY_SPACE_MEMORY;
    PVIDEO_MODE RequestedMode;
    PVIDEO_MEMORY Vidmem;
    PVIDEO_NUM_MODES NumModes;
    PHYSICAL_ADDRESS FrameBuffer;
    PVIDEO_MODE_INFORMATION Mode;
    PVIDEO_MEMORY_INFORMATION MapInformation;

    DPRINT1("LuserStartIO %x\n", RequestPacket->IoControlCode);

    switch (RequestPacket->IoControlCode)
    {
    case IOCTL_VIDEO_SET_CURRENT_MODE:
        if (RequestPacket->InputBufferLength < sizeof(VIDEO_MODE))
        {
            RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
            return TRUE;
        }
        RequestedMode = (PVIDEO_MODE)RequestPacket->InputBuffer;
        if (RequestedMode->RequestedMode != 0x112)
        {
            DPRINT1
                ("Failure to set mode other than 0x112: %x\n", 
                 RequestedMode->RequestedMode);
            Result = FALSE;
        }
        break;

    case IOCTL_VIDEO_RESET_DEVICE:
        break;

    case IOCTL_VIDEO_MAP_VIDEO_MEMORY:
        if (RequestPacket->OutputBufferLength < sizeof(VIDEO_MEMORY_INFORMATION) ||
            RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY))
        {
            RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
            return TRUE;
        }
        ReadLuserCPUData(&cpu);
        FrameBuffer.QuadPart = cpu.MemSize;
        Vidmem = (PVIDEO_MEMORY)RequestPacket->InputBuffer;
        MapInformation = 
            (PVIDEO_MEMORY_INFORMATION)RequestPacket->OutputBuffer;
        MapInformation->VideoRamBase = Vidmem->RequestedVirtualAddress;
        MapInformation->VideoRamLength = 640*480*24;
        VideoPortMapMemory
            (HwDeviceExtension,
             FrameBuffer,
             &MapInformation->VideoRamLength,
             &inIoSpace,
             &MapInformation->VideoRamBase);

        MapInformation->FrameBufferBase = MapInformation->VideoRamBase;
        MapInformation->FrameBufferLength = MapInformation->VideoRamLength;
        break;

    case IOCTL_VIDEO_UNMAP_VIDEO_MEMORY:
        if (RequestPacket->InputBufferLength < sizeof(VIDEO_MEMORY))
        {
            RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
            return TRUE;
        }
        Vidmem = (PVIDEO_MEMORY)RequestPacket->InputBuffer;
        VideoPortUnmapMemory
            (HwDeviceExtension, Vidmem->RequestedVirtualAddress, NULL);
        break;

    case IOCTL_VIDEO_QUERY_NUM_AVAIL_MODES:
        if (RequestPacket->OutputBufferLength < sizeof(VIDEO_NUM_MODES))
        {
            RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
            return TRUE;
        }  
        NumModes = (PVIDEO_NUM_MODES)RequestPacket->OutputBuffer;
        NumModes->NumModes = 1;
        NumModes->ModeInformationLength = sizeof(VIDEO_MODE_INFORMATION);
        RequestPacket->StatusBlock->Information = sizeof(VIDEO_NUM_MODES);
        break;

    case IOCTL_VIDEO_QUERY_AVAIL_MODES:
        if (RequestPacket->OutputBufferLength < sizeof(VIDEO_MODE_INFORMATION))
        {
            RequestPacket->StatusBlock->Status = ERROR_INSUFFICIENT_BUFFER;
            return TRUE;
        }
        Mode = (PVIDEO_MODE_INFORMATION)RequestPacket->OutputBuffer;
        RequestPacket->StatusBlock->Information = sizeof(VIDEO_MODE_INFORMATION);
        Mode->Length = sizeof(VIDEO_MODE_INFORMATION);
        Mode->ModeIndex = 0x112;
        Mode->VisScreenWidth = 640;
        Mode->VisScreenHeight = 480;
        Mode->ScreenStride = 640 * 3;
        Mode->NumberOfPlanes = 1;
        Mode->BitsPerPlane = 24;
        Mode->Frequency = 1;
        Mode->XMillimeter = Mode->VisScreenWidth * 254 / 960;
        Mode->YMillimeter = Mode->VisScreenHeight * 254 / 960;
        Mode->NumberRedBits = 8;
        Mode->NumberGreenBits = 8;
        Mode->NumberBlueBits = 8;
        Mode->RedMask = 0xff0000;
        Mode->GreenMask = 0xff00;
        Mode->BlueMask = 0xff;
        Mode->VideoMemoryBitmapWidth = 640;
        Mode->VideoMemoryBitmapHeight = 480;
        Mode->AttributeFlags = VIDEO_MODE_GRAPHICS | VIDEO_MODE_COLOR |
            VIDEO_MODE_NO_OFF_SCREEN;
        Mode->DriverSpecificAttributeFlags = 0;
        break;

    default:
        RequestPacket->StatusBlock->Status = ERROR_INVALID_FUNCTION;
        return FALSE;
    }
    
    if (Result)
        RequestPacket->StatusBlock->Status = NO_ERROR;

    return TRUE;
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
