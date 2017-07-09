/*
 * IDirect3DDevice9 implementation
 *
 * Copyright 2002-2005 Jason Edmeades
 * Copyright 2002-2005 Raphael Junqueira
 * Copyright 2005 Oliver Stieber
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#include "d3d9_private.h"

static void STDMETHODCALLTYPE d3d9_null_wined3d_object_destroyed(void *parent) {}

const struct wined3d_parent_ops d3d9_null_wined3d_parent_ops =
{
    d3d9_null_wined3d_object_destroyed,
};

D3DFORMAT d3dformat_from_wined3dformat(enum wined3d_format_id format)
{
    BYTE *c = (BYTE *)&format;

    /* Don't translate FOURCC formats */
    if (isprint(c[0]) && isprint(c[1]) && isprint(c[2]) && isprint(c[3])) return format;

    switch(format)
    {
        case WINED3DFMT_UNKNOWN: return D3DFMT_UNKNOWN;
        case WINED3DFMT_B8G8R8_UNORM: return D3DFMT_R8G8B8;
        case WINED3DFMT_B8G8R8A8_UNORM: return D3DFMT_A8R8G8B8;
        case WINED3DFMT_B8G8R8X8_UNORM: return D3DFMT_X8R8G8B8;
        case WINED3DFMT_B5G6R5_UNORM: return D3DFMT_R5G6B5;
        case WINED3DFMT_B5G5R5X1_UNORM: return D3DFMT_X1R5G5B5;
        case WINED3DFMT_B5G5R5A1_UNORM: return D3DFMT_A1R5G5B5;
        case WINED3DFMT_B4G4R4A4_UNORM: return D3DFMT_A4R4G4B4;
        case WINED3DFMT_B2G3R3_UNORM: return D3DFMT_R3G3B2;
        case WINED3DFMT_A8_UNORM: return D3DFMT_A8;
        case WINED3DFMT_B2G3R3A8_UNORM: return D3DFMT_A8R3G3B2;
        case WINED3DFMT_B4G4R4X4_UNORM: return D3DFMT_X4R4G4B4;
        case WINED3DFMT_R10G10B10A2_UNORM: return D3DFMT_A2B10G10R10;
        case WINED3DFMT_R8G8B8A8_UNORM: return D3DFMT_A8B8G8R8;
        case WINED3DFMT_R8G8B8X8_UNORM: return D3DFMT_X8B8G8R8;
        case WINED3DFMT_R16G16_UNORM: return D3DFMT_G16R16;
        case WINED3DFMT_B10G10R10A2_UNORM: return D3DFMT_A2R10G10B10;
        case WINED3DFMT_R16G16B16A16_UNORM: return D3DFMT_A16B16G16R16;
        case WINED3DFMT_P8_UINT_A8_UNORM: return D3DFMT_A8P8;
        case WINED3DFMT_P8_UINT: return D3DFMT_P8;
        case WINED3DFMT_L8_UNORM: return D3DFMT_L8;
        case WINED3DFMT_L8A8_UNORM: return D3DFMT_A8L8;
        case WINED3DFMT_L4A4_UNORM: return D3DFMT_A4L4;
        case WINED3DFMT_R8G8_SNORM: return D3DFMT_V8U8;
        case WINED3DFMT_R5G5_SNORM_L6_UNORM: return D3DFMT_L6V5U5;
        case WINED3DFMT_R8G8_SNORM_L8X8_UNORM: return D3DFMT_X8L8V8U8;
        case WINED3DFMT_R8G8B8A8_SNORM: return D3DFMT_Q8W8V8U8;
        case WINED3DFMT_R16G16_SNORM: return D3DFMT_V16U16;
        case WINED3DFMT_R10G10B10_SNORM_A2_UNORM: return D3DFMT_A2W10V10U10;
        case WINED3DFMT_D16_LOCKABLE: return D3DFMT_D16_LOCKABLE;
        case WINED3DFMT_D32_UNORM: return D3DFMT_D32;
        case WINED3DFMT_S1_UINT_D15_UNORM: return D3DFMT_D15S1;
        case WINED3DFMT_D24_UNORM_S8_UINT: return D3DFMT_D24S8;
        case WINED3DFMT_X8D24_UNORM: return D3DFMT_D24X8;
        case WINED3DFMT_S4X4_UINT_D24_UNORM: return D3DFMT_D24X4S4;
        case WINED3DFMT_D16_UNORM: return D3DFMT_D16;
        case WINED3DFMT_L16_UNORM: return D3DFMT_L16;
        case WINED3DFMT_D32_FLOAT: return D3DFMT_D32F_LOCKABLE;
        case WINED3DFMT_S8_UINT_D24_FLOAT: return D3DFMT_D24FS8;
        case WINED3DFMT_R16_UINT: return D3DFMT_INDEX16;
        case WINED3DFMT_R32_UINT: return D3DFMT_INDEX32;
        case WINED3DFMT_R16G16B16A16_SNORM: return D3DFMT_Q16W16V16U16;
        case WINED3DFMT_R16_FLOAT: return D3DFMT_R16F;
        case WINED3DFMT_R16G16_FLOAT: return D3DFMT_G16R16F;
        case WINED3DFMT_R16G16B16A16_FLOAT: return D3DFMT_A16B16G16R16F;
        case WINED3DFMT_R32_FLOAT: return D3DFMT_R32F;
        case WINED3DFMT_R32G32_FLOAT: return D3DFMT_G32R32F;
        case WINED3DFMT_R32G32B32A32_FLOAT: return D3DFMT_A32B32G32R32F;
        case WINED3DFMT_R8G8_SNORM_Cx: return D3DFMT_CxV8U8;
        default:
            FIXME("Unhandled wined3d format %#x.\n", format);
            return D3DFMT_UNKNOWN;
    }
}

enum wined3d_format_id wined3dformat_from_d3dformat(D3DFORMAT format)
{
    BYTE *c = (BYTE *)&format;

    /* Don't translate FOURCC formats */
    if (isprint(c[0]) && isprint(c[1]) && isprint(c[2]) && isprint(c[3])) return format;

    switch(format)
    {
        case D3DFMT_UNKNOWN: return WINED3DFMT_UNKNOWN;
        case D3DFMT_R8G8B8: return WINED3DFMT_B8G8R8_UNORM;
        case D3DFMT_A8R8G8B8: return WINED3DFMT_B8G8R8A8_UNORM;
        case D3DFMT_X8R8G8B8: return WINED3DFMT_B8G8R8X8_UNORM;
        case D3DFMT_R5G6B5: return WINED3DFMT_B5G6R5_UNORM;
        case D3DFMT_X1R5G5B5: return WINED3DFMT_B5G5R5X1_UNORM;
        case D3DFMT_A1R5G5B5: return WINED3DFMT_B5G5R5A1_UNORM;
        case D3DFMT_A4R4G4B4: return WINED3DFMT_B4G4R4A4_UNORM;
        case D3DFMT_R3G3B2: return WINED3DFMT_B2G3R3_UNORM;
        case D3DFMT_A8: return WINED3DFMT_A8_UNORM;
        case D3DFMT_A8R3G3B2: return WINED3DFMT_B2G3R3A8_UNORM;
        case D3DFMT_X4R4G4B4: return WINED3DFMT_B4G4R4X4_UNORM;
        case D3DFMT_A2B10G10R10: return WINED3DFMT_R10G10B10A2_UNORM;
        case D3DFMT_A8B8G8R8: return WINED3DFMT_R8G8B8A8_UNORM;
        case D3DFMT_X8B8G8R8: return WINED3DFMT_R8G8B8X8_UNORM;
        case D3DFMT_G16R16: return WINED3DFMT_R16G16_UNORM;
        case D3DFMT_A2R10G10B10: return WINED3DFMT_B10G10R10A2_UNORM;
        case D3DFMT_A16B16G16R16: return WINED3DFMT_R16G16B16A16_UNORM;
        case D3DFMT_A8P8: return WINED3DFMT_P8_UINT_A8_UNORM;
        case D3DFMT_P8: return WINED3DFMT_P8_UINT;
        case D3DFMT_L8: return WINED3DFMT_L8_UNORM;
        case D3DFMT_A8L8: return WINED3DFMT_L8A8_UNORM;
        case D3DFMT_A4L4: return WINED3DFMT_L4A4_UNORM;
        case D3DFMT_V8U8: return WINED3DFMT_R8G8_SNORM;
        case D3DFMT_L6V5U5: return WINED3DFMT_R5G5_SNORM_L6_UNORM;
        case D3DFMT_X8L8V8U8: return WINED3DFMT_R8G8_SNORM_L8X8_UNORM;
        case D3DFMT_Q8W8V8U8: return WINED3DFMT_R8G8B8A8_SNORM;
        case D3DFMT_V16U16: return WINED3DFMT_R16G16_SNORM;
        case D3DFMT_A2W10V10U10: return WINED3DFMT_R10G10B10_SNORM_A2_UNORM;
        case D3DFMT_D16_LOCKABLE: return WINED3DFMT_D16_LOCKABLE;
        case D3DFMT_D32: return WINED3DFMT_D32_UNORM;
        case D3DFMT_D15S1: return WINED3DFMT_S1_UINT_D15_UNORM;
        case D3DFMT_D24S8: return WINED3DFMT_D24_UNORM_S8_UINT;
        case D3DFMT_D24X8: return WINED3DFMT_X8D24_UNORM;
        case D3DFMT_D24X4S4: return WINED3DFMT_S4X4_UINT_D24_UNORM;
        case D3DFMT_D16: return WINED3DFMT_D16_UNORM;
        case D3DFMT_L16: return WINED3DFMT_L16_UNORM;
        case D3DFMT_D32F_LOCKABLE: return WINED3DFMT_D32_FLOAT;
        case D3DFMT_D24FS8: return WINED3DFMT_S8_UINT_D24_FLOAT;
        case D3DFMT_INDEX16: return WINED3DFMT_R16_UINT;
        case D3DFMT_INDEX32: return WINED3DFMT_R32_UINT;
        case D3DFMT_Q16W16V16U16: return WINED3DFMT_R16G16B16A16_SNORM;
        case D3DFMT_R16F: return WINED3DFMT_R16_FLOAT;
        case D3DFMT_G16R16F: return WINED3DFMT_R16G16_FLOAT;
        case D3DFMT_A16B16G16R16F: return WINED3DFMT_R16G16B16A16_FLOAT;
        case D3DFMT_R32F: return WINED3DFMT_R32_FLOAT;
        case D3DFMT_G32R32F: return WINED3DFMT_R32G32_FLOAT;
        case D3DFMT_A32B32G32R32F: return WINED3DFMT_R32G32B32A32_FLOAT;
        case D3DFMT_CxV8U8: return WINED3DFMT_R8G8_SNORM_Cx;
        default:
            FIXME("Unhandled D3DFORMAT %#x.\n", format);
            return WINED3DFMT_UNKNOWN;
    }
}

static UINT vertex_count_from_primitive_count(D3DPRIMITIVETYPE primitive_type, UINT primitive_count)
{
    switch (primitive_type)
    {
        case D3DPT_POINTLIST:
            return primitive_count;

        case D3DPT_LINELIST:
            return primitive_count * 2;

        case D3DPT_LINESTRIP:
            return primitive_count + 1;

        case D3DPT_TRIANGLELIST:
            return primitive_count * 3;

        case D3DPT_TRIANGLESTRIP:
        case D3DPT_TRIANGLEFAN:
            return primitive_count + 2;

        default:
            FIXME("Unhandled primitive type %#x.\n", primitive_type);
            return 0;
    }
}

void present_parameters_from_wined3d_swapchain_desc(D3DPRESENT_PARAMETERS *present_parameters,
        const struct wined3d_swapchain_desc *swapchain_desc)
{
    present_parameters->BackBufferWidth = swapchain_desc->backbuffer_width;
    present_parameters->BackBufferHeight = swapchain_desc->backbuffer_height;
    present_parameters->BackBufferFormat = d3dformat_from_wined3dformat(swapchain_desc->backbuffer_format);
    present_parameters->BackBufferCount = swapchain_desc->backbuffer_count;
    present_parameters->MultiSampleType = swapchain_desc->multisample_type;
    present_parameters->MultiSampleQuality = swapchain_desc->multisample_quality;
    present_parameters->SwapEffect = swapchain_desc->swap_effect;
    present_parameters->hDeviceWindow = swapchain_desc->device_window;
    present_parameters->Windowed = swapchain_desc->windowed;
    present_parameters->EnableAutoDepthStencil = swapchain_desc->enable_auto_depth_stencil;
    present_parameters->AutoDepthStencilFormat
            = d3dformat_from_wined3dformat(swapchain_desc->auto_depth_stencil_format);
    present_parameters->Flags = swapchain_desc->flags & D3DPRESENTFLAGS_MASK;
    present_parameters->FullScreen_RefreshRateInHz = swapchain_desc->refresh_rate;
    present_parameters->PresentationInterval = swapchain_desc->swap_interval;
}

static BOOL wined3d_swapchain_desc_from_present_parameters(struct wined3d_swapchain_desc *swapchain_desc,
        const D3DPRESENT_PARAMETERS *present_parameters, BOOL extended)
{
    D3DSWAPEFFECT highest_swapeffect = extended ? D3DSWAPEFFECT_FLIPEX : D3DSWAPEFFECT_COPY;
    UINT highest_bb_count = extended ? 30 : 3;

    if (!present_parameters->SwapEffect || present_parameters->SwapEffect > highest_swapeffect)
    {
        WARN("Invalid swap effect %u passed.\n", present_parameters->SwapEffect);
        return FALSE;
    }
    if (present_parameters->BackBufferCount > highest_bb_count
            || (present_parameters->SwapEffect == D3DSWAPEFFECT_COPY
            && present_parameters->BackBufferCount > 1))
    {
        WARN("Invalid backbuffer count %u.\n", present_parameters->BackBufferCount);
        return FALSE;
    }

    swapchain_desc->backbuffer_width = present_parameters->BackBufferWidth;
    swapchain_desc->backbuffer_height = present_parameters->BackBufferHeight;
    swapchain_desc->backbuffer_format = wined3dformat_from_d3dformat(present_parameters->BackBufferFormat);
    swapchain_desc->backbuffer_count = max(1, present_parameters->BackBufferCount);
    swapchain_desc->multisample_type = present_parameters->MultiSampleType;
    swapchain_desc->multisample_quality = present_parameters->MultiSampleQuality;
    swapchain_desc->swap_effect = present_parameters->SwapEffect;
    swapchain_desc->device_window = present_parameters->hDeviceWindow;
    swapchain_desc->windowed = present_parameters->Windowed;
    swapchain_desc->enable_auto_depth_stencil = present_parameters->EnableAutoDepthStencil;
    swapchain_desc->auto_depth_stencil_format
            = wined3dformat_from_d3dformat(present_parameters->AutoDepthStencilFormat);
    swapchain_desc->flags
            = (present_parameters->Flags & D3DPRESENTFLAGS_MASK) | WINED3D_SWAPCHAIN_ALLOW_MODE_SWITCH;
    if (is_gdi_compat_wined3dformat(swapchain_desc->backbuffer_format))
        swapchain_desc->flags |= WINED3D_SWAPCHAIN_GDI_COMPATIBLE;
    swapchain_desc->refresh_rate = present_parameters->FullScreen_RefreshRateInHz;
    swapchain_desc->swap_interval = present_parameters->PresentationInterval;
    swapchain_desc->auto_restore_display_mode = TRUE;

    if (present_parameters->Flags & ~D3DPRESENTFLAGS_MASK)
        FIXME("Unhandled flags %#x.\n", present_parameters->Flags & ~D3DPRESENTFLAGS_MASK);

    return TRUE;
}

void d3dcaps_from_wined3dcaps(D3DCAPS9 *caps, const WINED3DCAPS *wined3d_caps)
{
    static const DWORD ps_minor_version[] = {0, 4, 0, 0};
    static const DWORD vs_minor_version[] = {0, 1, 0, 0};
    static const DWORD texture_filter_caps =
        D3DPTFILTERCAPS_MINFPOINT      | D3DPTFILTERCAPS_MINFLINEAR    | D3DPTFILTERCAPS_MINFANISOTROPIC |
        D3DPTFILTERCAPS_MINFPYRAMIDALQUAD                              | D3DPTFILTERCAPS_MINFGAUSSIANQUAD|
        D3DPTFILTERCAPS_MIPFPOINT      | D3DPTFILTERCAPS_MIPFLINEAR    | D3DPTFILTERCAPS_MAGFPOINT       |
        D3DPTFILTERCAPS_MAGFLINEAR     |D3DPTFILTERCAPS_MAGFANISOTROPIC|D3DPTFILTERCAPS_MAGFPYRAMIDALQUAD|
        D3DPTFILTERCAPS_MAGFGAUSSIANQUAD;

    caps->DeviceType                        = (D3DDEVTYPE)wined3d_caps->DeviceType;
    caps->AdapterOrdinal                    = wined3d_caps->AdapterOrdinal;
    caps->Caps                              = wined3d_caps->Caps;
    caps->Caps2                             = wined3d_caps->Caps2;
    caps->Caps3                             = wined3d_caps->Caps3;
    caps->PresentationIntervals             = wined3d_caps->PresentationIntervals;
    caps->CursorCaps                        = wined3d_caps->CursorCaps;
    caps->DevCaps                           = wined3d_caps->DevCaps;
    caps->PrimitiveMiscCaps                 = wined3d_caps->PrimitiveMiscCaps;
    caps->RasterCaps                        = wined3d_caps->RasterCaps;
    caps->ZCmpCaps                          = wined3d_caps->ZCmpCaps;
    caps->SrcBlendCaps                      = wined3d_caps->SrcBlendCaps;
    caps->DestBlendCaps                     = wined3d_caps->DestBlendCaps;
    caps->AlphaCmpCaps                      = wined3d_caps->AlphaCmpCaps;
    caps->ShadeCaps                         = wined3d_caps->ShadeCaps;
    caps->TextureCaps                       = wined3d_caps->TextureCaps;
    caps->TextureFilterCaps                 = wined3d_caps->TextureFilterCaps;
    caps->CubeTextureFilterCaps             = wined3d_caps->CubeTextureFilterCaps;
    caps->VolumeTextureFilterCaps           = wined3d_caps->VolumeTextureFilterCaps;
    caps->TextureAddressCaps                = wined3d_caps->TextureAddressCaps;
    caps->VolumeTextureAddressCaps          = wined3d_caps->VolumeTextureAddressCaps;
    caps->LineCaps                          = wined3d_caps->LineCaps;
    caps->MaxTextureWidth                   = wined3d_caps->MaxTextureWidth;
    caps->MaxTextureHeight                  = wined3d_caps->MaxTextureHeight;
    caps->MaxVolumeExtent                   = wined3d_caps->MaxVolumeExtent;
    caps->MaxTextureRepeat                  = wined3d_caps->MaxTextureRepeat;
    caps->MaxTextureAspectRatio             = wined3d_caps->MaxTextureAspectRatio;
    caps->MaxAnisotropy                     = wined3d_caps->MaxAnisotropy;
    caps->MaxVertexW                        = wined3d_caps->MaxVertexW;
    caps->GuardBandLeft                     = wined3d_caps->GuardBandLeft;
    caps->GuardBandTop                      = wined3d_caps->GuardBandTop;
    caps->GuardBandRight                    = wined3d_caps->GuardBandRight;
    caps->GuardBandBottom                   = wined3d_caps->GuardBandBottom;
    caps->ExtentsAdjust                     = wined3d_caps->ExtentsAdjust;
    caps->StencilCaps                       = wined3d_caps->StencilCaps;
    caps->FVFCaps                           = wined3d_caps->FVFCaps;
    caps->TextureOpCaps                     = wined3d_caps->TextureOpCaps;
    caps->MaxTextureBlendStages             = wined3d_caps->MaxTextureBlendStages;
    caps->MaxSimultaneousTextures           = wined3d_caps->MaxSimultaneousTextures;
    caps->VertexProcessingCaps              = wined3d_caps->VertexProcessingCaps;
    caps->MaxActiveLights                   = wined3d_caps->MaxActiveLights;
    caps->MaxUserClipPlanes                 = wined3d_caps->MaxUserClipPlanes;
    caps->MaxVertexBlendMatrices            = wined3d_caps->MaxVertexBlendMatrices;
    caps->MaxVertexBlendMatrixIndex         = wined3d_caps->MaxVertexBlendMatrixIndex;
    caps->MaxPointSize                      = wined3d_caps->MaxPointSize;
    caps->MaxPrimitiveCount                 = wined3d_caps->MaxPrimitiveCount;
    caps->MaxVertexIndex                    = wined3d_caps->MaxVertexIndex;
    caps->MaxStreams                        = wined3d_caps->MaxStreams;
    caps->MaxStreamStride                   = wined3d_caps->MaxStreamStride;
    caps->VertexShaderVersion               = wined3d_caps->VertexShaderVersion;
    caps->MaxVertexShaderConst              = wined3d_caps->MaxVertexShaderConst;
    caps->PixelShaderVersion                = wined3d_caps->PixelShaderVersion;
    caps->PixelShader1xMaxValue             = wined3d_caps->PixelShader1xMaxValue;
    caps->DevCaps2                          = wined3d_caps->DevCaps2;
    caps->MaxNpatchTessellationLevel        = wined3d_caps->MaxNpatchTessellationLevel;
    caps->MasterAdapterOrdinal              = wined3d_caps->MasterAdapterOrdinal;
    caps->AdapterOrdinalInGroup             = wined3d_caps->AdapterOrdinalInGroup;
    caps->NumberOfAdaptersInGroup           = wined3d_caps->NumberOfAdaptersInGroup;
    caps->DeclTypes                         = wined3d_caps->DeclTypes;
    caps->NumSimultaneousRTs                = wined3d_caps->NumSimultaneousRTs;
    caps->StretchRectFilterCaps             = wined3d_caps->StretchRectFilterCaps;
    caps->VS20Caps.Caps                     = wined3d_caps->VS20Caps.caps;
    caps->VS20Caps.DynamicFlowControlDepth  = wined3d_caps->VS20Caps.dynamic_flow_control_depth;
    caps->VS20Caps.NumTemps                 = wined3d_caps->VS20Caps.temp_count;
    caps->VS20Caps.StaticFlowControlDepth   = wined3d_caps->VS20Caps.static_flow_control_depth;
    caps->PS20Caps.Caps                     = wined3d_caps->PS20Caps.caps;
    caps->PS20Caps.DynamicFlowControlDepth  = wined3d_caps->PS20Caps.dynamic_flow_control_depth;
    caps->PS20Caps.NumTemps                 = wined3d_caps->PS20Caps.temp_count;
    caps->PS20Caps.StaticFlowControlDepth   = wined3d_caps->PS20Caps.static_flow_control_depth;
    caps->PS20Caps.NumInstructionSlots      = wined3d_caps->PS20Caps.instruction_slot_count;
    caps->VertexTextureFilterCaps           = wined3d_caps->VertexTextureFilterCaps;
    caps->MaxVShaderInstructionsExecuted    = wined3d_caps->MaxVShaderInstructionsExecuted;
    caps->MaxPShaderInstructionsExecuted    = wined3d_caps->MaxPShaderInstructionsExecuted;
    caps->MaxVertexShader30InstructionSlots = wined3d_caps->MaxVertexShader30InstructionSlots;
    caps->MaxPixelShader30InstructionSlots  = wined3d_caps->MaxPixelShader30InstructionSlots;

    /* Some functionality is implemented in d3d9.dll, not wined3d.dll. Add the needed caps. */
    caps->DevCaps2 |= D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES;

    /* Filter wined3d caps. */
    caps->TextureFilterCaps &= texture_filter_caps;
    caps->CubeTextureFilterCaps &= texture_filter_caps;
    caps->VolumeTextureFilterCaps &= texture_filter_caps;

    caps->DevCaps &=
        D3DDEVCAPS_EXECUTESYSTEMMEMORY | D3DDEVCAPS_EXECUTEVIDEOMEMORY | D3DDEVCAPS_TLVERTEXSYSTEMMEMORY |
        D3DDEVCAPS_TLVERTEXVIDEOMEMORY | D3DDEVCAPS_TEXTURESYSTEMMEMORY| D3DDEVCAPS_TEXTUREVIDEOMEMORY   |
        D3DDEVCAPS_DRAWPRIMTLVERTEX    | D3DDEVCAPS_CANRENDERAFTERFLIP | D3DDEVCAPS_TEXTURENONLOCALVIDMEM|
        D3DDEVCAPS_DRAWPRIMITIVES2     | D3DDEVCAPS_SEPARATETEXTUREMEMORIES                              |
        D3DDEVCAPS_DRAWPRIMITIVES2EX   | D3DDEVCAPS_HWTRANSFORMANDLIGHT| D3DDEVCAPS_CANBLTSYSTONONLOCAL  |
        D3DDEVCAPS_HWRASTERIZATION     | D3DDEVCAPS_PUREDEVICE         | D3DDEVCAPS_QUINTICRTPATCHES     |
        D3DDEVCAPS_RTPATCHES           | D3DDEVCAPS_RTPATCHHANDLEZERO  | D3DDEVCAPS_NPATCHES;

    caps->ShadeCaps &=
        D3DPSHADECAPS_COLORGOURAUDRGB  | D3DPSHADECAPS_SPECULARGOURAUDRGB |
        D3DPSHADECAPS_ALPHAGOURAUDBLEND | D3DPSHADECAPS_FOGGOURAUD;

    caps->RasterCaps &=
        D3DPRASTERCAPS_DITHER          | D3DPRASTERCAPS_ZTEST          | D3DPRASTERCAPS_FOGVERTEX        |
        D3DPRASTERCAPS_FOGTABLE        | D3DPRASTERCAPS_MIPMAPLODBIAS  | D3DPRASTERCAPS_ZBUFFERLESSHSR   |
        D3DPRASTERCAPS_FOGRANGE        | D3DPRASTERCAPS_ANISOTROPY     | D3DPRASTERCAPS_WBUFFER          |
        D3DPRASTERCAPS_WFOG            | D3DPRASTERCAPS_ZFOG           | D3DPRASTERCAPS_COLORPERSPECTIVE |
        D3DPRASTERCAPS_SCISSORTEST     | D3DPRASTERCAPS_SLOPESCALEDEPTHBIAS                              |
        D3DPRASTERCAPS_DEPTHBIAS       | D3DPRASTERCAPS_MULTISAMPLE_TOGGLE;

    caps->DevCaps2 &=
        D3DDEVCAPS2_STREAMOFFSET       | D3DDEVCAPS2_DMAPNPATCH        | D3DDEVCAPS2_ADAPTIVETESSRTPATCH |
        D3DDEVCAPS2_ADAPTIVETESSNPATCH | D3DDEVCAPS2_CAN_STRETCHRECT_FROM_TEXTURES                       |
        D3DDEVCAPS2_PRESAMPLEDDMAPNPATCH| D3DDEVCAPS2_VERTEXELEMENTSCANSHARESTREAMOFFSET;

    caps->Caps2 &=
        D3DCAPS2_FULLSCREENGAMMA       | D3DCAPS2_CANCALIBRATEGAMMA    | D3DCAPS2_RESERVED               |
        D3DCAPS2_CANMANAGERESOURCE     | D3DCAPS2_DYNAMICTEXTURES      | D3DCAPS2_CANAUTOGENMIPMAP;

    caps->VertexProcessingCaps &=
        D3DVTXPCAPS_TEXGEN             | D3DVTXPCAPS_MATERIALSOURCE7   | D3DVTXPCAPS_DIRECTIONALLIGHTS   |
        D3DVTXPCAPS_POSITIONALLIGHTS   | D3DVTXPCAPS_LOCALVIEWER       | D3DVTXPCAPS_TWEENING            |
        D3DVTXPCAPS_TEXGEN_SPHEREMAP   | D3DVTXPCAPS_NO_TEXGEN_NONLOCALVIEWER;

    caps->TextureCaps &=
        D3DPTEXTURECAPS_PERSPECTIVE    | D3DPTEXTURECAPS_POW2          | D3DPTEXTURECAPS_ALPHA           |
        D3DPTEXTURECAPS_SQUAREONLY     | D3DPTEXTURECAPS_TEXREPEATNOTSCALEDBYSIZE                        |
        D3DPTEXTURECAPS_ALPHAPALETTE   | D3DPTEXTURECAPS_NONPOW2CONDITIONAL                              |
        D3DPTEXTURECAPS_PROJECTED      | D3DPTEXTURECAPS_CUBEMAP       | D3DPTEXTURECAPS_VOLUMEMAP       |
        D3DPTEXTURECAPS_MIPMAP         | D3DPTEXTURECAPS_MIPVOLUMEMAP  | D3DPTEXTURECAPS_MIPCUBEMAP      |
        D3DPTEXTURECAPS_CUBEMAP_POW2   | D3DPTEXTURECAPS_VOLUMEMAP_POW2| D3DPTEXTURECAPS_NOPROJECTEDBUMPENV;

    caps->MaxVertexShaderConst = min(D3D9_MAX_VERTEX_SHADER_CONSTANTF, caps->MaxVertexShaderConst);
    caps->NumSimultaneousRTs = min(D3D9_MAX_SIMULTANEOUS_RENDERTARGETS, caps->NumSimultaneousRTs);

    if (caps->PixelShaderVersion > 3)
    {
        caps->PixelShaderVersion = D3DPS_VERSION(3, 0);
    }
    else
    {
        DWORD major = caps->PixelShaderVersion;
        caps->PixelShaderVersion = D3DPS_VERSION(major, ps_minor_version[major]);
    }

    if (caps->VertexShaderVersion > 3)
    {
        caps->VertexShaderVersion = D3DVS_VERSION(3, 0);
    }
    else
    {
        DWORD major = caps->VertexShaderVersion;
        caps->VertexShaderVersion = D3DVS_VERSION(major, vs_minor_version[major]);
    }
}

static HRESULT WINAPI d3d9_device_QueryInterface(IDirect3DDevice9Ex *iface, REFIID riid, void **out)
{
    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_IDirect3DDevice9)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IDirect3DDevice9Ex_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_IDirect3DDevice9Ex))
    {
        struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

        /* Find out if the creating d3d9 interface was created with Direct3DCreate9Ex.
         * It doesn't matter with which function the device was created. */
        if (!device->d3d_parent->extended)
        {
            WARN("IDirect3D9 instance wasn't created with CreateDirect3D9Ex, returning E_NOINTERFACE.\n");
            *out = NULL;
            return E_NOINTERFACE;
        }

        IDirect3DDevice9Ex_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI d3d9_device_AddRef(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    ULONG refcount = InterlockedIncrement(&device->refcount);

    TRACE("%p increasing refcount to %u.\n", iface, refcount);

    return refcount;
}

static ULONG WINAPI DECLSPEC_HOTPATCH d3d9_device_Release(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    ULONG refcount;

    if (device->in_destruction)
        return 0;

    refcount = InterlockedDecrement(&device->refcount);

    TRACE("%p decreasing refcount to %u.\n", iface, refcount);

    if (!refcount)
    {
        unsigned i;
        device->in_destruction = TRUE;

        wined3d_mutex_lock();
        for (i = 0; i < device->fvf_decl_count; ++i)
        {
            wined3d_vertex_declaration_decref(device->fvf_decls[i].decl);
        }
        HeapFree(GetProcessHeap(), 0, device->fvf_decls);

        if (device->vertex_buffer)
            wined3d_buffer_decref(device->vertex_buffer);
        if (device->index_buffer)
            wined3d_buffer_decref(device->index_buffer);

        HeapFree(GetProcessHeap(), 0, device->implicit_swapchains);

        wined3d_device_uninit_3d(device->wined3d_device);
        wined3d_device_release_focus_window(device->wined3d_device);
        wined3d_device_decref(device->wined3d_device);
        wined3d_mutex_unlock();

        IDirect3D9Ex_Release(&device->d3d_parent->IDirect3D9Ex_iface);

        HeapFree(GetProcessHeap(), 0, device);
    }

    return refcount;
}

static HRESULT WINAPI d3d9_device_TestCooperativeLevel(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p.\n", iface);

    TRACE("device state: %#x.\n", device->device_state);

    if (device->d3d_parent->extended)
        return D3D_OK;

    switch (device->device_state)
    {
        default:
        case D3D9_DEVICE_STATE_OK:
            return D3D_OK;
        case D3D9_DEVICE_STATE_LOST:
            return D3DERR_DEVICELOST;
        case D3D9_DEVICE_STATE_NOT_RESET:
            return D3DERR_DEVICENOTRESET;
    }
}

static UINT WINAPI d3d9_device_GetAvailableTextureMem(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    UINT ret;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    ret = wined3d_device_get_available_texture_mem(device->wined3d_device);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_device_EvictManagedResources(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_device_evict_managed_resources(device->wined3d_device);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetDirect3D(IDirect3DDevice9Ex *iface, IDirect3D9 **d3d9)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, d3d9 %p.\n", iface, d3d9);

    if (!d3d9)
        return D3DERR_INVALIDCALL;

    return IDirect3D9Ex_QueryInterface(&device->d3d_parent->IDirect3D9Ex_iface, &IID_IDirect3D9, (void **)d3d9);
}

static HRESULT WINAPI d3d9_device_GetDeviceCaps(IDirect3DDevice9Ex *iface, D3DCAPS9 *caps)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    WINED3DCAPS wined3d_caps;
    HRESULT hr;

    TRACE("iface %p, caps %p.\n", iface, caps);

    if (!caps)
        return D3DERR_INVALIDCALL;

    memset(caps, 0, sizeof(*caps));

    wined3d_mutex_lock();
    hr = wined3d_device_get_device_caps(device->wined3d_device, &wined3d_caps);
    wined3d_mutex_unlock();

    d3dcaps_from_wined3dcaps(caps, &wined3d_caps);

    return hr;
}

static HRESULT WINAPI d3d9_device_GetDisplayMode(IDirect3DDevice9Ex *iface, UINT swapchain, D3DDISPLAYMODE *mode)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, swapchain %u, mode %p.\n", iface, swapchain, mode);

    wined3d_mutex_lock();
    hr = wined3d_device_get_display_mode(device->wined3d_device, swapchain, &wined3d_mode, NULL);
    wined3d_mutex_unlock();

    if (SUCCEEDED(hr))
    {
        mode->Width = wined3d_mode.width;
        mode->Height = wined3d_mode.height;
        mode->RefreshRate = wined3d_mode.refresh_rate;
        mode->Format = d3dformat_from_wined3dformat(wined3d_mode.format_id);
    }

    return hr;
}

static HRESULT WINAPI d3d9_device_GetCreationParameters(IDirect3DDevice9Ex *iface,
        D3DDEVICE_CREATION_PARAMETERS *parameters)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, parameters %p.\n", iface, parameters);

    wined3d_mutex_lock();
    wined3d_device_get_creation_parameters(device->wined3d_device,
            (struct wined3d_device_creation_parameters *)parameters);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetCursorProperties(IDirect3DDevice9Ex *iface,
        UINT hotspot_x, UINT hotspot_y, IDirect3DSurface9 *bitmap)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *bitmap_impl = unsafe_impl_from_IDirect3DSurface9(bitmap);
    HRESULT hr;

    TRACE("iface %p, hotspot_x %u, hotspot_y %u, bitmap %p.\n",
            iface, hotspot_x, hotspot_y, bitmap);

    if (!bitmap)
    {
        WARN("No cursor bitmap, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    hr = wined3d_device_set_cursor_properties(device->wined3d_device,
            hotspot_x, hotspot_y, bitmap_impl->wined3d_texture, bitmap_impl->sub_resource_idx);
    wined3d_mutex_unlock();

    return hr;
}

static void WINAPI d3d9_device_SetCursorPosition(IDirect3DDevice9Ex *iface, int x, int y, DWORD flags)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, x %u, y %u, flags %#x.\n", iface, x, y, flags);

    wined3d_mutex_lock();
    wined3d_device_set_cursor_position(device->wined3d_device, x, y, flags);
    wined3d_mutex_unlock();
}

static BOOL WINAPI d3d9_device_ShowCursor(IDirect3DDevice9Ex *iface, BOOL show)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    BOOL ret;

    TRACE("iface %p, show %#x.\n", iface, show);

    wined3d_mutex_lock();
    ret = wined3d_device_show_cursor(device->wined3d_device, show);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_CreateAdditionalSwapChain(IDirect3DDevice9Ex *iface,
        D3DPRESENT_PARAMETERS *present_parameters, IDirect3DSwapChain9 **swapchain)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_swapchain_desc desc;
    struct d3d9_swapchain *object;
    UINT i, count;
    HRESULT hr;

    TRACE("iface %p, present_parameters %p, swapchain %p.\n",
            iface, present_parameters, swapchain);

    if (!present_parameters->Windowed)
    {
        WARN("Trying to create an additional fullscreen swapchain, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    count = wined3d_device_get_swapchain_count(device->wined3d_device);
    for (i = 0; i < count; ++i)
    {
        struct wined3d_swapchain *wined3d_swapchain;

        wined3d_swapchain = wined3d_device_get_swapchain(device->wined3d_device, i);
        wined3d_swapchain_get_desc(wined3d_swapchain, &desc);

        if (!desc.windowed)
        {
            wined3d_mutex_unlock();
            WARN("Trying to create an additional swapchain in fullscreen mode, returning D3DERR_INVALIDCALL.\n");
            return D3DERR_INVALIDCALL;
        }
    }
    wined3d_mutex_unlock();

    if (!wined3d_swapchain_desc_from_present_parameters(&desc, present_parameters,
            device->d3d_parent->extended))
        return D3DERR_INVALIDCALL;
    if (SUCCEEDED(hr = d3d9_swapchain_create(device, &desc, &object)))
        *swapchain = (IDirect3DSwapChain9 *)&object->IDirect3DSwapChain9Ex_iface;
    present_parameters_from_wined3d_swapchain_desc(present_parameters, &desc);

    return hr;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_GetSwapChain(IDirect3DDevice9Ex *iface,
        UINT swapchain_idx, IDirect3DSwapChain9 **swapchain)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, swapchain_idx %u, swapchain %p.\n", iface, swapchain_idx, swapchain);

    wined3d_mutex_lock();
    if (swapchain_idx < device->implicit_swapchain_count)
    {
        *swapchain = (IDirect3DSwapChain9 *)&device->implicit_swapchains[swapchain_idx]->IDirect3DSwapChain9Ex_iface;
        IDirect3DSwapChain9Ex_AddRef(*swapchain);
        hr = D3D_OK;
    }
    else
    {
        *swapchain = NULL;
        hr = D3DERR_INVALIDCALL;
    }
    wined3d_mutex_unlock();

    return hr;
}

static UINT WINAPI d3d9_device_GetNumberOfSwapChains(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    UINT count;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    count = wined3d_device_get_swapchain_count(device->wined3d_device);
    wined3d_mutex_unlock();

    return count;
}

static HRESULT CDECL reset_enum_callback(struct wined3d_resource *resource)
{
    struct wined3d_resource_desc desc;
    IDirect3DBaseTexture9 *texture;
    struct d3d9_surface *surface;
    IUnknown *parent;

    wined3d_resource_get_desc(resource, &desc);
    if (desc.pool != WINED3D_POOL_DEFAULT)
        return D3D_OK;

    if (desc.resource_type != WINED3D_RTYPE_TEXTURE_2D)
    {
        WARN("Resource %p in pool D3DPOOL_DEFAULT blocks the Reset call.\n", resource);
        return D3DERR_INVALIDCALL;
    }

    parent = wined3d_resource_get_parent(resource);
    if (parent && SUCCEEDED(IUnknown_QueryInterface(parent, &IID_IDirect3DBaseTexture9, (void **)&texture)))
    {
        IDirect3DBaseTexture9_Release(texture);
        WARN("Texture %p (resource %p) in pool D3DPOOL_DEFAULT blocks the Reset call.\n", texture, resource);
        return D3DERR_INVALIDCALL;
    }

    surface = wined3d_texture_get_sub_resource_parent(wined3d_texture_from_resource(resource), 0);
    if (!surface->resource.refcount)
        return D3D_OK;

    WARN("Surface %p in pool D3DPOOL_DEFAULT blocks the Reset call.\n", surface);
    return D3DERR_INVALIDCALL;
}

static HRESULT d3d9_device_get_swapchains(struct d3d9_device *device)
{
    UINT i, new_swapchain_count = wined3d_device_get_swapchain_count(device->wined3d_device);
    struct wined3d_swapchain *wined3d_swapchain;

    if (!(device->implicit_swapchains = HeapAlloc(GetProcessHeap(), 0,
            new_swapchain_count * sizeof(*device->implicit_swapchains))))
        return E_OUTOFMEMORY;

    for (i = 0; i < new_swapchain_count; ++i)
    {
        wined3d_swapchain = wined3d_device_get_swapchain(device->wined3d_device, i);
        device->implicit_swapchains[i] = wined3d_swapchain_get_parent(wined3d_swapchain);
    }
    device->implicit_swapchain_count = new_swapchain_count;

    return D3D_OK;
}

static HRESULT d3d9_device_reset(struct d3d9_device *device,
        D3DPRESENT_PARAMETERS *present_parameters, D3DDISPLAYMODEEX *mode)
{
    struct wined3d_swapchain_desc swapchain_desc;
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    if (!device->d3d_parent->extended && device->device_state == D3D9_DEVICE_STATE_LOST)
    {
        WARN("App not active, returning D3DERR_DEVICELOST.\n");
        return D3DERR_DEVICELOST;
    }

    if (mode)
    {
        wined3d_mode.width = mode->Width;
        wined3d_mode.height = mode->Height;
        wined3d_mode.refresh_rate = mode->RefreshRate;
        wined3d_mode.format_id = wined3dformat_from_d3dformat(mode->Format);
        wined3d_mode.scanline_ordering = mode->ScanLineOrdering;
    }

    if (!wined3d_swapchain_desc_from_present_parameters(&swapchain_desc, present_parameters,
            device->d3d_parent->extended))
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();

    if (device->vertex_buffer)
    {
        wined3d_buffer_decref(device->vertex_buffer);
        device->vertex_buffer = NULL;
        device->vertex_buffer_size = 0;
    }

    if (device->index_buffer)
    {
        wined3d_buffer_decref(device->index_buffer);
        device->index_buffer = NULL;
        device->index_buffer_size = 0;
    }

    if (SUCCEEDED(hr = wined3d_device_reset(device->wined3d_device, &swapchain_desc,
            mode ? &wined3d_mode : NULL, reset_enum_callback, !device->d3d_parent->extended)))
    {
        HeapFree(GetProcessHeap(), 0, device->implicit_swapchains);

        if (FAILED(hr = d3d9_device_get_swapchains(device)))
        {
            device->device_state = D3D9_DEVICE_STATE_NOT_RESET;
        }
        else
        {
            wined3d_swapchain_get_desc(device->implicit_swapchains[0]->wined3d_swapchain, &swapchain_desc);
            present_parameters->BackBufferWidth = swapchain_desc.backbuffer_width;
            present_parameters->BackBufferHeight = swapchain_desc.backbuffer_height;
            present_parameters->BackBufferFormat = d3dformat_from_wined3dformat(swapchain_desc.backbuffer_format);
            present_parameters->BackBufferCount = swapchain_desc.backbuffer_count;

            device->device_state = D3D9_DEVICE_STATE_OK;
        }
    }
    else if (!device->d3d_parent->extended)
    {
        device->device_state = D3D9_DEVICE_STATE_NOT_RESET;
    }

    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_Reset(IDirect3DDevice9Ex *iface,
        D3DPRESENT_PARAMETERS *present_parameters)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, present_parameters %p.\n", iface, present_parameters);

    return d3d9_device_reset(device, present_parameters, NULL);
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_Present(IDirect3DDevice9Ex *iface,
        const RECT *src_rect, const RECT *dst_rect, HWND dst_window_override, const RGNDATA *dirty_region)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    UINT i;
    HRESULT hr;

    TRACE("iface %p, src_rect %p, dst_rect %p, dst_window_override %p, dirty_region %p.\n",
            iface, src_rect, dst_rect, dst_window_override, dirty_region);

    if (device->device_state != D3D9_DEVICE_STATE_OK)
        return device->d3d_parent->extended ? S_PRESENT_OCCLUDED : D3DERR_DEVICELOST;

    if (dirty_region)
        FIXME("Ignoring dirty_region %p.\n", dirty_region);

    wined3d_mutex_lock();
    for (i = 0; i < device->implicit_swapchain_count; ++i)
    {
        if (FAILED(hr = wined3d_swapchain_present(device->implicit_swapchains[i]->wined3d_swapchain,
                src_rect, dst_rect, dst_window_override, 0)))
        {
            wined3d_mutex_unlock();
            return hr;
        }
    }
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetBackBuffer(IDirect3DDevice9Ex *iface, UINT swapchain,
        UINT backbuffer_idx, D3DBACKBUFFER_TYPE backbuffer_type, IDirect3DSurface9 **backbuffer)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, swapchain %u, backbuffer_idx %u, backbuffer_type %#x, backbuffer %p.\n",
            iface, swapchain, backbuffer_idx, backbuffer_type, backbuffer);

    /* backbuffer_type is ignored by native. */

    /* No need to check for backbuffer == NULL, Windows crashes in that case. */
    *backbuffer = NULL;

    wined3d_mutex_lock();
    if (swapchain >= device->implicit_swapchain_count)
    {
        wined3d_mutex_unlock();
        WARN("Swapchain index %u is out of range, returning D3DERR_INVALIDCALL.\n", swapchain);
        return D3DERR_INVALIDCALL;
    }

    hr = IDirect3DSwapChain9Ex_GetBackBuffer(&device->implicit_swapchains[swapchain]->IDirect3DSwapChain9Ex_iface,
            backbuffer_idx, backbuffer_type, backbuffer);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetRasterStatus(IDirect3DDevice9Ex *iface,
        UINT swapchain, D3DRASTER_STATUS *raster_status)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, swapchain %u, raster_status %p.\n", iface, swapchain, raster_status);

    wined3d_mutex_lock();
    hr = wined3d_device_get_raster_status(device->wined3d_device,
            swapchain, (struct wined3d_raster_status *)raster_status);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetDialogBoxMode(IDirect3DDevice9Ex *iface, BOOL enable)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, enable %#x.\n", iface, enable);

    wined3d_mutex_lock();
    hr = wined3d_device_set_dialog_box_mode(device->wined3d_device, enable);
    wined3d_mutex_unlock();

    return hr;
}

static void WINAPI d3d9_device_SetGammaRamp(IDirect3DDevice9Ex *iface,
        UINT swapchain, DWORD flags, const D3DGAMMARAMP *ramp)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, swapchain %u, flags %#x, ramp %p.\n", iface, swapchain, flags, ramp);

    /* Note: D3DGAMMARAMP is compatible with struct wined3d_gamma_ramp. */
    wined3d_mutex_lock();
    wined3d_device_set_gamma_ramp(device->wined3d_device, swapchain, flags, (const struct wined3d_gamma_ramp *)ramp);
    wined3d_mutex_unlock();
}

static void WINAPI d3d9_device_GetGammaRamp(IDirect3DDevice9Ex *iface, UINT swapchain, D3DGAMMARAMP *ramp)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, swapchain %u, ramp %p.\n", iface, swapchain, ramp);

    /* Note: D3DGAMMARAMP is compatible with struct wined3d_gamma_ramp. */
    wined3d_mutex_lock();
    wined3d_device_get_gamma_ramp(device->wined3d_device, swapchain, (struct wined3d_gamma_ramp *)ramp);
    wined3d_mutex_unlock();
}

static HRESULT WINAPI d3d9_device_CreateTexture(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, UINT levels, DWORD usage, D3DFORMAT format,
        D3DPOOL pool, IDirect3DTexture9 **texture, HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_texture *object;
    BOOL set_mem = FALSE;
    HRESULT hr;

    TRACE("iface %p, width %u, height %u, levels %u, usage %#x, format %#x, pool %#x, texture %p, shared_handle %p.\n",
            iface, width, height, levels, usage, format, pool, texture, shared_handle);

    *texture = NULL;
    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared or user memory texture on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool == D3DPOOL_SYSTEMMEM)
        {
            if (levels != 1)
                return D3DERR_INVALIDCALL;
            set_mem = TRUE;
        }
        else
        {
            if (pool != D3DPOOL_DEFAULT)
            {
                WARN("Trying to create a shared texture in pool %#x.\n", pool);
                return D3DERR_INVALIDCALL;
            }
            FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
        }
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return D3DERR_OUTOFVIDEOMEMORY;

    hr = texture_init(object, device, width, height, levels, usage, format, pool);
    if (FAILED(hr))
    {
        WARN("Failed to initialize texture, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    if (set_mem)
        wined3d_texture_update_desc(object->wined3d_texture, width, height,
                wined3dformat_from_d3dformat(format), WINED3D_MULTISAMPLE_NONE, 0,
                *shared_handle, 0);

    TRACE("Created texture %p.\n", object);
    *texture = (IDirect3DTexture9 *)&object->IDirect3DBaseTexture9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateVolumeTexture(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, UINT depth, UINT levels, DWORD usage, D3DFORMAT format,
        D3DPOOL pool, IDirect3DVolumeTexture9 **texture, HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_texture *object;
    HRESULT hr;

    TRACE("iface %p, width %u, height %u, depth %u, levels %u, "
            "usage %#x, format %#x, pool %#x, texture %p, shared_handle %p.\n",
            iface, width, height, depth, levels,
            usage, format, pool, texture, shared_handle);

    *texture = NULL;
    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared volume texture on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool != D3DPOOL_DEFAULT)
        {
            WARN("Trying to create a shared volume texture in pool %#x.\n", pool);
            return D3DERR_INVALIDCALL;
        }
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return D3DERR_OUTOFVIDEOMEMORY;

    hr = volumetexture_init(object, device, width, height, depth, levels, usage, format, pool);
    if (FAILED(hr))
    {
        WARN("Failed to initialize volume texture, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created volume texture %p.\n", object);
    *texture = (IDirect3DVolumeTexture9 *)&object->IDirect3DBaseTexture9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateCubeTexture(IDirect3DDevice9Ex *iface,
        UINT edge_length, UINT levels, DWORD usage, D3DFORMAT format, D3DPOOL pool,
        IDirect3DCubeTexture9 **texture, HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_texture *object;
    HRESULT hr;

    TRACE("iface %p, edge_length %u, levels %u, usage %#x, format %#x, pool %#x, texture %p, shared_handle %p.\n",
            iface, edge_length, levels, usage, format, pool, texture, shared_handle);

    *texture = NULL;
    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared cube texture on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool != D3DPOOL_DEFAULT)
        {
            WARN("Trying to create a shared cube texture in pool %#x.\n", pool);
            return D3DERR_INVALIDCALL;
        }
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return D3DERR_OUTOFVIDEOMEMORY;

    hr = cubetexture_init(object, device, edge_length, levels, usage, format, pool);
    if (FAILED(hr))
    {
        WARN("Failed to initialize cube texture, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created cube texture %p.\n", object);
    *texture = (IDirect3DCubeTexture9 *)&object->IDirect3DBaseTexture9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateVertexBuffer(IDirect3DDevice9Ex *iface, UINT size,
        DWORD usage, DWORD fvf, D3DPOOL pool, IDirect3DVertexBuffer9 **buffer,
        HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexbuffer *object;
    HRESULT hr;

    TRACE("iface %p, size %u, usage %#x, fvf %#x, pool %#x, buffer %p, shared_handle %p.\n",
            iface, size, usage, fvf, pool, buffer, shared_handle);

    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared vertex buffer on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool != D3DPOOL_DEFAULT)
        {
            WARN("Trying to create a shared vertex buffer in pool %#x.\n", pool);
            return D3DERR_NOTAVAILABLE;
        }
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return D3DERR_OUTOFVIDEOMEMORY;

    hr = vertexbuffer_init(object, device, size, usage, fvf, pool);
    if (FAILED(hr))
    {
        WARN("Failed to initialize vertex buffer, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created vertex buffer %p.\n", object);
    *buffer = &object->IDirect3DVertexBuffer9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateIndexBuffer(IDirect3DDevice9Ex *iface, UINT size,
        DWORD usage, D3DFORMAT format, D3DPOOL pool, IDirect3DIndexBuffer9 **buffer,
        HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_indexbuffer *object;
    HRESULT hr;

    TRACE("iface %p, size %u, usage %#x, format %#x, pool %#x, buffer %p, shared_handle %p.\n",
            iface, size, usage, format, pool, buffer, shared_handle);

    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared index buffer on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool != D3DPOOL_DEFAULT)
        {
            WARN("Trying to create a shared index buffer in pool %#x.\n", pool);
            return D3DERR_NOTAVAILABLE;
        }
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return D3DERR_OUTOFVIDEOMEMORY;

    hr = indexbuffer_init(object, device, size, usage, format, pool);
    if (FAILED(hr))
    {
        WARN("Failed to initialize index buffer, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created index buffer %p.\n", object);
    *buffer = &object->IDirect3DIndexBuffer9_iface;

    return D3D_OK;
}

static HRESULT d3d9_device_create_surface(struct d3d9_device *device, UINT width, UINT height,
        D3DFORMAT format, DWORD flags, IDirect3DSurface9 **surface, UINT usage, D3DPOOL pool,
        D3DMULTISAMPLE_TYPE multisample_type, DWORD multisample_quality, void *user_mem)
{
    struct wined3d_resource_desc desc;
    struct d3d9_surface *surface_impl;
    struct wined3d_texture *texture;
    HRESULT hr;

    TRACE("device %p, width %u, height %u, format %#x, flags %#x, surface %p.\n"
            "usage %#x, pool %#x, multisample_type %#x, multisample_quality %u.\n",
            device, width, height, format, flags, surface, usage, pool,
            multisample_type, multisample_quality);

    desc.resource_type = WINED3D_RTYPE_TEXTURE_2D;
    desc.format = wined3dformat_from_d3dformat(format);
    desc.multisample_type = multisample_type;
    desc.multisample_quality = multisample_quality;
    desc.usage = usage & WINED3DUSAGE_MASK;
    desc.pool = pool;
    desc.width = width;
    desc.height = height;
    desc.depth = 1;
    desc.size = 0;

    if (is_gdi_compat_wined3dformat(desc.format))
        flags |= WINED3D_TEXTURE_CREATE_GET_DC;

    wined3d_mutex_lock();

    if (FAILED(hr = wined3d_texture_create(device->wined3d_device, &desc,
            1, 1, flags, NULL, NULL, &d3d9_null_wined3d_parent_ops, &texture)))
    {
        wined3d_mutex_unlock();
        WARN("Failed to create texture, hr %#x.\n", hr);
        if (hr == WINED3DERR_NOTAVAILABLE)
            hr = D3DERR_INVALIDCALL;
        return hr;
    }

    surface_impl = wined3d_texture_get_sub_resource_parent(texture, 0);
    surface_impl->parent_device = &device->IDirect3DDevice9Ex_iface;
    *surface = &surface_impl->IDirect3DSurface9_iface;
    IDirect3DSurface9_AddRef(*surface);

    if (user_mem)
        wined3d_texture_update_desc(texture, width, height,
                desc.format, multisample_type, multisample_quality, user_mem, 0);

    wined3d_texture_decref(texture);

    wined3d_mutex_unlock();

    return D3D_OK;
}

BOOL is_gdi_compat_wined3dformat(enum wined3d_format_id format)
{
    switch (format)
    {
        case WINED3DFMT_B8G8R8A8_UNORM:
        case WINED3DFMT_B8G8R8X8_UNORM:
        case WINED3DFMT_B5G6R5_UNORM:
        case WINED3DFMT_B5G5R5X1_UNORM:
        case WINED3DFMT_B5G5R5A1_UNORM:
        case WINED3DFMT_B8G8R8_UNORM:
            return TRUE;
        default:
            return FALSE;
    }
}

static HRESULT WINAPI d3d9_device_CreateRenderTarget(IDirect3DDevice9Ex *iface, UINT width, UINT height,
        D3DFORMAT format, D3DMULTISAMPLE_TYPE multisample_type, DWORD multisample_quality,
        BOOL lockable, IDirect3DSurface9 **surface, HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    DWORD flags = 0;

    TRACE("iface %p, width %u, height %u, format %#x, multisample_type %#x, multisample_quality %u.\n"
            "lockable %#x, surface %p, shared_handle %p.\n",
            iface, width, height, format, multisample_type, multisample_quality,
            lockable, surface, shared_handle);

    *surface = NULL;
    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared render target on a non-ex device.\n");
            return E_NOTIMPL;
        }

        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    if (lockable)
        flags |= WINED3D_TEXTURE_CREATE_MAPPABLE;

    return d3d9_device_create_surface(device, width, height, format, flags, surface,
            D3DUSAGE_RENDERTARGET, D3DPOOL_DEFAULT, multisample_type, multisample_quality, NULL);
}

static HRESULT WINAPI d3d9_device_CreateDepthStencilSurface(IDirect3DDevice9Ex *iface, UINT width, UINT height,
        D3DFORMAT format, D3DMULTISAMPLE_TYPE multisample_type, DWORD multisample_quality,
        BOOL discard, IDirect3DSurface9 **surface, HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    DWORD flags = WINED3D_TEXTURE_CREATE_MAPPABLE;

    TRACE("iface %p, width %u, height %u, format %#x, multisample_type %#x, multisample_quality %u.\n"
            "discard %#x, surface %p, shared_handle %p.\n",
            iface, width, height, format, multisample_type, multisample_quality,
            discard, surface, shared_handle);

    *surface = NULL;
    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared depth stencil on a non-ex device.\n");
            return E_NOTIMPL;
        }

        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
    }

    if (discard)
        flags |= WINED3D_TEXTURE_CREATE_DISCARD;

    return d3d9_device_create_surface(device, width, height, format, flags, surface,
            D3DUSAGE_DEPTHSTENCIL, D3DPOOL_DEFAULT, multisample_type, multisample_quality, NULL);
}


static HRESULT WINAPI d3d9_device_UpdateSurface(IDirect3DDevice9Ex *iface,
        IDirect3DSurface9 *src_surface, const RECT *src_rect,
        IDirect3DSurface9 *dst_surface, const POINT *dst_point)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *src = unsafe_impl_from_IDirect3DSurface9(src_surface);
    struct d3d9_surface *dst = unsafe_impl_from_IDirect3DSurface9(dst_surface);
    struct wined3d_box src_box;
    HRESULT hr;

    TRACE("iface %p, src_surface %p, src_rect %p, dst_surface %p, dst_point %p.\n",
            iface, src_surface, src_rect, dst_surface, dst_point);

    if (src_rect)
        wined3d_box_set(&src_box, src_rect->left, src_rect->top, src_rect->right, src_rect->bottom, 0, 1);

    wined3d_mutex_lock();
    hr = wined3d_device_copy_sub_resource_region(device->wined3d_device,
            wined3d_texture_get_resource(dst->wined3d_texture), dst->sub_resource_idx, dst_point ? dst_point->x : 0,
            dst_point ? dst_point->y : 0, 0, wined3d_texture_get_resource(src->wined3d_texture),
            src->sub_resource_idx, src_rect ? &src_box : NULL);
    wined3d_mutex_unlock();

    if (FAILED(hr))
        return D3DERR_INVALIDCALL;

    return hr;
}

static HRESULT WINAPI d3d9_device_UpdateTexture(IDirect3DDevice9Ex *iface,
        IDirect3DBaseTexture9 *src_texture, IDirect3DBaseTexture9 *dst_texture)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_texture *src_impl, *dst_impl;
    HRESULT hr;

    TRACE("iface %p, src_texture %p, dst_texture %p.\n", iface, src_texture, dst_texture);

    src_impl = unsafe_impl_from_IDirect3DBaseTexture9(src_texture);
    dst_impl = unsafe_impl_from_IDirect3DBaseTexture9(dst_texture);

    wined3d_mutex_lock();
    hr = wined3d_device_update_texture(device->wined3d_device,
            src_impl->wined3d_texture, dst_impl->wined3d_texture);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetRenderTargetData(IDirect3DDevice9Ex *iface,
        IDirect3DSurface9 *render_target, IDirect3DSurface9 *dst_surface)
{
    struct d3d9_surface *rt_impl = unsafe_impl_from_IDirect3DSurface9(render_target);
    struct d3d9_surface *dst_impl = unsafe_impl_from_IDirect3DSurface9(dst_surface);
    struct wined3d_sub_resource_desc wined3d_desc;
    RECT dst_rect, src_rect;
    HRESULT hr;

    TRACE("iface %p, render_target %p, dst_surface %p.\n", iface, render_target, dst_surface);

    if (!render_target || !dst_surface)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    wined3d_texture_get_sub_resource_desc(dst_impl->wined3d_texture, dst_impl->sub_resource_idx, &wined3d_desc);
    SetRect(&dst_rect, 0, 0, wined3d_desc.width, wined3d_desc.height);

    wined3d_texture_get_sub_resource_desc(rt_impl->wined3d_texture, rt_impl->sub_resource_idx, &wined3d_desc);
    SetRect(&src_rect, 0, 0, wined3d_desc.width, wined3d_desc.height);

    /* TODO: Check surface sizes, pools, etc. */
    if (wined3d_desc.multisample_type)
        hr = D3DERR_INVALIDCALL;
    else
        hr = wined3d_texture_blt(dst_impl->wined3d_texture, dst_impl->sub_resource_idx, &dst_rect,
                rt_impl->wined3d_texture, rt_impl->sub_resource_idx, &src_rect, 0, NULL, WINED3D_TEXF_POINT);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetFrontBufferData(IDirect3DDevice9Ex *iface,
        UINT swapchain, IDirect3DSurface9 *dst_surface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *dst_impl = unsafe_impl_from_IDirect3DSurface9(dst_surface);
    HRESULT hr = D3DERR_INVALIDCALL;

    TRACE("iface %p, swapchain %u, dst_surface %p.\n", iface, swapchain, dst_surface);

    wined3d_mutex_lock();
    if (swapchain < device->implicit_swapchain_count)
        hr = wined3d_swapchain_get_front_buffer_data(device->implicit_swapchains[swapchain]->wined3d_swapchain,
                dst_impl->wined3d_texture, dst_impl->sub_resource_idx);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_StretchRect(IDirect3DDevice9Ex *iface, IDirect3DSurface9 *src_surface,
        const RECT *src_rect, IDirect3DSurface9 *dst_surface, const RECT *dst_rect, D3DTEXTUREFILTERTYPE filter)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *src = unsafe_impl_from_IDirect3DSurface9(src_surface);
    struct d3d9_surface *dst = unsafe_impl_from_IDirect3DSurface9(dst_surface);
    struct wined3d_sub_resource_desc src_desc, dst_desc;
    HRESULT hr = D3DERR_INVALIDCALL;
    RECT d, s;

    TRACE("iface %p, src_surface %p, src_rect %p, dst_surface %p, dst_rect %p, filter %#x.\n",
            iface, src_surface, src_rect, dst_surface, dst_rect, filter);

    wined3d_mutex_lock();
    wined3d_texture_get_sub_resource_desc(dst->wined3d_texture, dst->sub_resource_idx, &dst_desc);
    if (!dst_rect)
    {
        SetRect(&d, 0, 0, dst_desc.width, dst_desc.height);
        dst_rect = &d;
    }

    wined3d_texture_get_sub_resource_desc(src->wined3d_texture, src->sub_resource_idx, &src_desc);
    if (!src_rect)
    {
        SetRect(&s, 0, 0, src_desc.width, src_desc.height);
        src_rect = &s;
    }

    if (src_desc.usage & WINED3DUSAGE_DEPTHSTENCIL)
    {
        if (device->in_scene)
        {
            WARN("Rejecting depth / stencil blit while in scene.\n");
            goto done;
        }

        if (src_rect->left || src_rect->top || src_rect->right != src_desc.width
                || src_rect->bottom != src_desc.height)
        {
            WARN("Rejecting depth / stencil blit with invalid source rect %s.\n",
                    wine_dbgstr_rect(src_rect));
            goto done;
        }

        if (dst_rect->left || dst_rect->top || dst_rect->right != dst_desc.width
                || dst_rect->bottom != dst_desc.height)
        {
            WARN("Rejecting depth / stencil blit with invalid destination rect %s.\n",
                    wine_dbgstr_rect(dst_rect));
            goto done;
        }

        if (src_desc.width != dst_desc.width || src_desc.height != dst_desc.height)
        {
            WARN("Rejecting depth / stencil blit with mismatched surface sizes.\n");
            goto done;
        }
    }

    hr = wined3d_texture_blt(dst->wined3d_texture, dst->sub_resource_idx, dst_rect,
            src->wined3d_texture, src->sub_resource_idx, src_rect, 0, NULL, filter);
    if (hr == WINEDDERR_INVALIDRECT)
        hr = D3DERR_INVALIDCALL;

done:
    wined3d_mutex_unlock();
    return hr;
}

static HRESULT WINAPI d3d9_device_ColorFill(IDirect3DDevice9Ex *iface,
        IDirect3DSurface9 *surface, const RECT *rect, D3DCOLOR color)
{
    const struct wined3d_color c =
    {
        ((color >> 16) & 0xff) / 255.0f,
        ((color >>  8) & 0xff) / 255.0f,
        (color & 0xff) / 255.0f,
        ((color >> 24) & 0xff) / 255.0f,
    };
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *surface_impl = unsafe_impl_from_IDirect3DSurface9(surface);
    struct wined3d_sub_resource_desc desc;
    struct wined3d_rendertarget_view *rtv;
    HRESULT hr;

    TRACE("iface %p, surface %p, rect %p, color 0x%08x.\n", iface, surface, rect, color);

    wined3d_mutex_lock();

    if (FAILED(wined3d_texture_get_sub_resource_desc(surface_impl->wined3d_texture,
            surface_impl->sub_resource_idx, &desc)))
    {
        wined3d_mutex_unlock();
        return D3DERR_INVALIDCALL;
    }

    if (desc.pool != WINED3D_POOL_DEFAULT)
    {
        wined3d_mutex_unlock();
        WARN("Colorfill is not allowed on surfaces in pool %#x, returning D3DERR_INVALIDCALL.\n", desc.pool);
        return D3DERR_INVALIDCALL;
    }
    if ((desc.usage & (WINED3DUSAGE_RENDERTARGET | WINED3DUSAGE_TEXTURE)) == WINED3DUSAGE_TEXTURE)
    {
        wined3d_mutex_unlock();
        WARN("Colorfill is not allowed on non-RT textures, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }
    if (desc.usage & WINED3DUSAGE_DEPTHSTENCIL)
    {
        wined3d_mutex_unlock();
        WARN("Colorfill is not allowed on depth stencil surfaces, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }

    rtv = d3d9_surface_acquire_rendertarget_view(surface_impl);
    hr = wined3d_device_clear_rendertarget_view(device->wined3d_device,
            rtv, rect, WINED3DCLEAR_TARGET, &c, 0.0f, 0);
    d3d9_surface_release_rendertarget_view(surface_impl, rtv);

    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_CreateOffscreenPlainSurface(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, D3DFORMAT format, D3DPOOL pool, IDirect3DSurface9 **surface,
        HANDLE *shared_handle)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    void *user_mem = NULL;

    TRACE("iface %p, width %u, height %u, format %#x, pool %#x, surface %p, shared_handle %p.\n",
            iface, width, height, format, pool, surface, shared_handle);

    *surface = NULL;
    if (pool == D3DPOOL_MANAGED)
    {
        WARN("Attempting to create a managed offscreen plain surface.\n");
        return D3DERR_INVALIDCALL;
    }

    if (shared_handle)
    {
        if (!device->d3d_parent->extended)
        {
            WARN("Trying to create a shared or user memory surface on a non-ex device.\n");
            return E_NOTIMPL;
        }

        if (pool == D3DPOOL_SYSTEMMEM)
            user_mem = *shared_handle;
        else
        {
            if (pool != D3DPOOL_DEFAULT)
            {
                WARN("Trying to create a shared surface in pool %#x.\n", pool);
                return D3DERR_INVALIDCALL;
            }
            FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);
        }
    }

    /* FIXME: Offscreen surfaces are supposed to be always lockable,
     * regardless of the pool they're created in. Should we set dynamic usage
     * here? */
    return d3d9_device_create_surface(device, width, height, format,
            WINED3D_TEXTURE_CREATE_MAPPABLE, surface, 0, pool, D3DMULTISAMPLE_NONE, 0, user_mem);
}

static HRESULT WINAPI d3d9_device_SetRenderTarget(IDirect3DDevice9Ex *iface, DWORD idx, IDirect3DSurface9 *surface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *surface_impl = unsafe_impl_from_IDirect3DSurface9(surface);
    struct wined3d_rendertarget_view *rtv;
    HRESULT hr;

    TRACE("iface %p, idx %u, surface %p.\n", iface, idx, surface);

    if (idx >= D3D9_MAX_SIMULTANEOUS_RENDERTARGETS)
    {
        WARN("Invalid index %u specified.\n", idx);
        return D3DERR_INVALIDCALL;
    }

    if (!idx && !surface_impl)
    {
        WARN("Trying to set render target 0 to NULL.\n");
        return D3DERR_INVALIDCALL;
    }

    if (surface_impl && d3d9_surface_get_device(surface_impl) != device)
    {
        WARN("Render target surface does not match device.\n");
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    rtv = surface_impl ? d3d9_surface_acquire_rendertarget_view(surface_impl) : NULL;
    hr = wined3d_device_set_rendertarget_view(device->wined3d_device, idx, rtv, TRUE);
    d3d9_surface_release_rendertarget_view(surface_impl, rtv);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetRenderTarget(IDirect3DDevice9Ex *iface, DWORD idx, IDirect3DSurface9 **surface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_rendertarget_view *wined3d_rtv;
    struct d3d9_surface *surface_impl;
    HRESULT hr = D3D_OK;

    TRACE("iface %p, idx %u, surface %p.\n", iface, idx, surface);

    if (!surface)
        return D3DERR_INVALIDCALL;

    if (idx >= D3D9_MAX_SIMULTANEOUS_RENDERTARGETS)
    {
        WARN("Invalid index %u specified.\n", idx);
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    if ((wined3d_rtv = wined3d_device_get_rendertarget_view(device->wined3d_device, idx)))
    {
        /* We want the sub resource parent here, since the view itself may be
         * internal to wined3d and may not have a parent. */
        surface_impl = wined3d_rendertarget_view_get_sub_resource_parent(wined3d_rtv);
        *surface = &surface_impl->IDirect3DSurface9_iface;
        IDirect3DSurface9_AddRef(*surface);
    }
    else
    {
        hr = D3DERR_NOTFOUND;
        *surface = NULL;
    }
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetDepthStencilSurface(IDirect3DDevice9Ex *iface, IDirect3DSurface9 *depth_stencil)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_surface *ds_impl = unsafe_impl_from_IDirect3DSurface9(depth_stencil);
    struct wined3d_rendertarget_view *rtv;

    TRACE("iface %p, depth_stencil %p.\n", iface, depth_stencil);

    wined3d_mutex_lock();
    rtv = ds_impl ? d3d9_surface_acquire_rendertarget_view(ds_impl) : NULL;
    wined3d_device_set_depth_stencil_view(device->wined3d_device, rtv);
    d3d9_surface_release_rendertarget_view(ds_impl, rtv);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetDepthStencilSurface(IDirect3DDevice9Ex *iface, IDirect3DSurface9 **depth_stencil)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_rendertarget_view *wined3d_dsv;
    struct d3d9_surface *surface_impl;
    HRESULT hr = D3D_OK;

    TRACE("iface %p, depth_stencil %p.\n", iface, depth_stencil);

    if (!depth_stencil)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    if ((wined3d_dsv = wined3d_device_get_depth_stencil_view(device->wined3d_device)))
    {
        /* We want the sub resource parent here, since the view itself may be
         * internal to wined3d and may not have a parent. */
        surface_impl = wined3d_rendertarget_view_get_sub_resource_parent(wined3d_dsv);
        *depth_stencil = &surface_impl->IDirect3DSurface9_iface;
        IDirect3DSurface9_AddRef(*depth_stencil);
    }
    else
    {
        hr = D3DERR_NOTFOUND;
        *depth_stencil = NULL;
    }
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_BeginScene(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    if (SUCCEEDED(hr = wined3d_device_begin_scene(device->wined3d_device)))
        device->in_scene = TRUE;
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_EndScene(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    if (SUCCEEDED(hr = wined3d_device_end_scene(device->wined3d_device)))
        device->in_scene = FALSE;
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_Clear(IDirect3DDevice9Ex *iface, DWORD rect_count,
        const D3DRECT *rects, DWORD flags, D3DCOLOR color, float z, DWORD stencil)
{
    const struct wined3d_color c =
    {
        ((color >> 16) & 0xff) / 255.0f,
        ((color >>  8) & 0xff) / 255.0f,
        (color & 0xff) / 255.0f,
        ((color >> 24) & 0xff) / 255.0f,
    };
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, rect_count %u, rects %p, flags %#x, color 0x%08x, z %.8e, stencil %u.\n",
            iface, rect_count, rects, flags, color, z, stencil);

    if (rect_count && !rects)
    {
        WARN("count %u with NULL rects.\n", rect_count);
        rect_count = 0;
    }

    wined3d_mutex_lock();
    hr = wined3d_device_clear(device->wined3d_device, rect_count, (const RECT *)rects, flags, &c, z, stencil);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetTransform(IDirect3DDevice9Ex *iface,
        D3DTRANSFORMSTATETYPE state, const D3DMATRIX *matrix)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, state %#x, matrix %p.\n", iface, state, matrix);

    /* Note: D3DMATRIX is compatible with struct wined3d_matrix. */
    wined3d_mutex_lock();
    wined3d_device_set_transform(device->wined3d_device, state, (const struct wined3d_matrix *)matrix);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetTransform(IDirect3DDevice9Ex *iface,
        D3DTRANSFORMSTATETYPE state, D3DMATRIX *matrix)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, state %#x, matrix %p.\n", iface, state, matrix);

    /* Note: D3DMATRIX is compatible with struct wined3d_matrix. */
    wined3d_mutex_lock();
    wined3d_device_get_transform(device->wined3d_device, state, (struct wined3d_matrix *)matrix);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_MultiplyTransform(IDirect3DDevice9Ex *iface,
        D3DTRANSFORMSTATETYPE state, const D3DMATRIX *matrix)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, state %#x, matrix %p.\n", iface, state, matrix);

    /* Note: D3DMATRIX is compatible with struct wined3d_matrix. */
    wined3d_mutex_lock();
    wined3d_device_multiply_transform(device->wined3d_device, state, (const struct wined3d_matrix *)matrix);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetViewport(IDirect3DDevice9Ex *iface, const D3DVIEWPORT9 *viewport)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_viewport vp;

    TRACE("iface %p, viewport %p.\n", iface, viewport);

    vp.x = viewport->X;
    vp.y = viewport->Y;
    vp.width = viewport->Width;
    vp.height = viewport->Height;
    vp.min_z = viewport->MinZ;
    vp.max_z = viewport->MaxZ;

    wined3d_mutex_lock();
    wined3d_device_set_viewport(device->wined3d_device, &vp);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetViewport(IDirect3DDevice9Ex *iface, D3DVIEWPORT9 *viewport)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_viewport wined3d_viewport;

    TRACE("iface %p, viewport %p.\n", iface, viewport);

    wined3d_mutex_lock();
    wined3d_device_get_viewport(device->wined3d_device, &wined3d_viewport);
    wined3d_mutex_unlock();

    viewport->X = wined3d_viewport.x;
    viewport->Y = wined3d_viewport.y;
    viewport->Width = wined3d_viewport.width;
    viewport->Height = wined3d_viewport.height;
    viewport->MinZ = wined3d_viewport.min_z;
    viewport->MaxZ = wined3d_viewport.max_z;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetMaterial(IDirect3DDevice9Ex *iface, const D3DMATERIAL9 *material)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, material %p.\n", iface, material);

    /* Note: D3DMATERIAL9 is compatible with struct wined3d_material. */
    wined3d_mutex_lock();
    wined3d_device_set_material(device->wined3d_device, (const struct wined3d_material *)material);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetMaterial(IDirect3DDevice9Ex *iface, D3DMATERIAL9 *material)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, material %p.\n", iface, material);

    /* Note: D3DMATERIAL9 is compatible with struct wined3d_material. */
    wined3d_mutex_lock();
    wined3d_device_get_material(device->wined3d_device, (struct wined3d_material *)material);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetLight(IDirect3DDevice9Ex *iface, DWORD index, const D3DLIGHT9 *light)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, light %p.\n", iface, index, light);

    /* Note: D3DLIGHT9 is compatible with struct wined3d_light. */
    wined3d_mutex_lock();
    hr = wined3d_device_set_light(device->wined3d_device, index, (const struct wined3d_light *)light);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetLight(IDirect3DDevice9Ex *iface, DWORD index, D3DLIGHT9 *light)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, light %p.\n", iface, index, light);

    /* Note: D3DLIGHT9 is compatible with struct wined3d_light. */
    wined3d_mutex_lock();
    hr = wined3d_device_get_light(device->wined3d_device, index, (struct wined3d_light *)light);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_LightEnable(IDirect3DDevice9Ex *iface, DWORD index, BOOL enable)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, enable %#x.\n", iface, index, enable);

    wined3d_mutex_lock();
    hr = wined3d_device_set_light_enable(device->wined3d_device, index, enable);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetLightEnable(IDirect3DDevice9Ex *iface, DWORD index, BOOL *enable)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, enable %p.\n", iface, index, enable);

    wined3d_mutex_lock();
    hr = wined3d_device_get_light_enable(device->wined3d_device, index, enable);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetClipPlane(IDirect3DDevice9Ex *iface, DWORD index, const float *plane)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, plane %p.\n", iface, index, plane);

    wined3d_mutex_lock();
    hr = wined3d_device_set_clip_plane(device->wined3d_device, index, (const struct wined3d_vec4 *)plane);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetClipPlane(IDirect3DDevice9Ex *iface, DWORD index, float *plane)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, index %u, plane %p.\n", iface, index, plane);

    wined3d_mutex_lock();
    hr = wined3d_device_get_clip_plane(device->wined3d_device, index, (struct wined3d_vec4 *)plane);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_SetRenderState(IDirect3DDevice9Ex *iface,
        D3DRENDERSTATETYPE state, DWORD value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, state %#x, value %#x.\n", iface, state, value);

    wined3d_mutex_lock();
    wined3d_device_set_render_state(device->wined3d_device, state, value);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetRenderState(IDirect3DDevice9Ex *iface,
        D3DRENDERSTATETYPE state, DWORD *value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, state %#x, value %p.\n", iface, state, value);

    wined3d_mutex_lock();
    *value = wined3d_device_get_render_state(device->wined3d_device, state);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateStateBlock(IDirect3DDevice9Ex *iface,
        D3DSTATEBLOCKTYPE type, IDirect3DStateBlock9 **stateblock)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_stateblock *object;
    HRESULT hr;

    TRACE("iface %p, type %#x, stateblock %p.\n", iface, type, stateblock);

    if (type != D3DSBT_ALL && type != D3DSBT_PIXELSTATE && type != D3DSBT_VERTEXSTATE)
    {
        WARN("Unexpected stateblock type, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return E_OUTOFMEMORY;

    hr = stateblock_init(object, device, type, NULL);
    if (FAILED(hr))
    {
        WARN("Failed to initialize stateblock, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created stateblock %p.\n", object);
    *stateblock = &object->IDirect3DStateBlock9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_BeginStateBlock(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    hr = wined3d_device_begin_stateblock(device->wined3d_device);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_EndStateBlock(IDirect3DDevice9Ex *iface, IDirect3DStateBlock9 **stateblock)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_stateblock *wined3d_stateblock;
    struct d3d9_stateblock *object;
    HRESULT hr;

    TRACE("iface %p, stateblock %p.\n", iface, stateblock);

    wined3d_mutex_lock();
    hr = wined3d_device_end_stateblock(device->wined3d_device, &wined3d_stateblock);
    wined3d_mutex_unlock();
    if (FAILED(hr))
    {
       WARN("Failed to end the state block, hr %#x.\n", hr);
       return hr;
    }

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
    {
        wined3d_mutex_lock();
        wined3d_stateblock_decref(wined3d_stateblock);
        wined3d_mutex_unlock();
        return E_OUTOFMEMORY;
    }

    hr = stateblock_init(object, device, 0, wined3d_stateblock);
    if (FAILED(hr))
    {
        WARN("Failed to initialize stateblock, hr %#x.\n", hr);
        wined3d_mutex_lock();
        wined3d_stateblock_decref(wined3d_stateblock);
        wined3d_mutex_unlock();
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created stateblock %p.\n", object);
    *stateblock = &object->IDirect3DStateBlock9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetClipStatus(IDirect3DDevice9Ex *iface, const D3DCLIPSTATUS9 *clip_status)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, clip_status %p.\n", iface, clip_status);

    wined3d_mutex_lock();
    hr = wined3d_device_set_clip_status(device->wined3d_device, (const struct wined3d_clip_status *)clip_status);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetClipStatus(IDirect3DDevice9Ex *iface, D3DCLIPSTATUS9 *clip_status)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, clip_status %p.\n", iface, clip_status);

    wined3d_mutex_lock();
    hr = wined3d_device_get_clip_status(device->wined3d_device, (struct wined3d_clip_status *)clip_status);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetTexture(IDirect3DDevice9Ex *iface, DWORD stage, IDirect3DBaseTexture9 **texture)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_texture *wined3d_texture = NULL;
    struct d3d9_texture *texture_impl;

    TRACE("iface %p, stage %u, texture %p.\n", iface, stage, texture);

    if (!texture)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    if ((wined3d_texture = wined3d_device_get_texture(device->wined3d_device, stage)))
    {
        texture_impl = wined3d_texture_get_parent(wined3d_texture);
        *texture = &texture_impl->IDirect3DBaseTexture9_iface;
        IDirect3DBaseTexture9_AddRef(*texture);
    }
    else
    {
        *texture = NULL;
    }
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetTexture(IDirect3DDevice9Ex *iface, DWORD stage, IDirect3DBaseTexture9 *texture)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_texture *texture_impl;
    HRESULT hr;

    TRACE("iface %p, stage %u, texture %p.\n", iface, stage, texture);

    texture_impl = unsafe_impl_from_IDirect3DBaseTexture9(texture);

    wined3d_mutex_lock();
    hr = wined3d_device_set_texture(device->wined3d_device, stage,
            texture_impl ? texture_impl->wined3d_texture : NULL);
    wined3d_mutex_unlock();

    return hr;
}

static const enum wined3d_texture_stage_state tss_lookup[] =
{
    WINED3D_TSS_INVALID,                    /*  0, unused */
    WINED3D_TSS_COLOR_OP,                   /*  1, D3DTSS_COLOROP */
    WINED3D_TSS_COLOR_ARG1,                 /*  2, D3DTSS_COLORARG1 */
    WINED3D_TSS_COLOR_ARG2,                 /*  3, D3DTSS_COLORARG2 */
    WINED3D_TSS_ALPHA_OP,                   /*  4, D3DTSS_ALPHAOP */
    WINED3D_TSS_ALPHA_ARG1,                 /*  5, D3DTSS_ALPHAARG1 */
    WINED3D_TSS_ALPHA_ARG2,                 /*  6, D3DTSS_ALPHAARG2 */
    WINED3D_TSS_BUMPENV_MAT00,              /*  7, D3DTSS_BUMPENVMAT00 */
    WINED3D_TSS_BUMPENV_MAT01,              /*  8, D3DTSS_BUMPENVMAT01 */
    WINED3D_TSS_BUMPENV_MAT10,              /*  9, D3DTSS_BUMPENVMAT10 */
    WINED3D_TSS_BUMPENV_MAT11,              /* 10, D3DTSS_BUMPENVMAT11 */
    WINED3D_TSS_TEXCOORD_INDEX,             /* 11, D3DTSS_TEXCOORDINDEX */
    WINED3D_TSS_INVALID,                    /* 12, unused */
    WINED3D_TSS_INVALID,                    /* 13, unused */
    WINED3D_TSS_INVALID,                    /* 14, unused */
    WINED3D_TSS_INVALID,                    /* 15, unused */
    WINED3D_TSS_INVALID,                    /* 16, unused */
    WINED3D_TSS_INVALID,                    /* 17, unused */
    WINED3D_TSS_INVALID,                    /* 18, unused */
    WINED3D_TSS_INVALID,                    /* 19, unused */
    WINED3D_TSS_INVALID,                    /* 20, unused */
    WINED3D_TSS_INVALID,                    /* 21, unused */
    WINED3D_TSS_BUMPENV_LSCALE,             /* 22, D3DTSS_BUMPENVLSCALE */
    WINED3D_TSS_BUMPENV_LOFFSET,            /* 23, D3DTSS_BUMPENVLOFFSET */
    WINED3D_TSS_TEXTURE_TRANSFORM_FLAGS,    /* 24, D3DTSS_TEXTURETRANSFORMFLAGS */
    WINED3D_TSS_INVALID,                    /* 25, unused */
    WINED3D_TSS_COLOR_ARG0,                 /* 26, D3DTSS_COLORARG0 */
    WINED3D_TSS_ALPHA_ARG0,                 /* 27, D3DTSS_ALPHAARG0 */
    WINED3D_TSS_RESULT_ARG,                 /* 28, D3DTSS_RESULTARG */
    WINED3D_TSS_INVALID,                    /* 29, unused */
    WINED3D_TSS_INVALID,                    /* 30, unused */
    WINED3D_TSS_INVALID,                    /* 31, unused */
    WINED3D_TSS_CONSTANT,                   /* 32, D3DTSS_CONSTANT */
};

static HRESULT WINAPI d3d9_device_GetTextureStageState(IDirect3DDevice9Ex *iface,
        DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD *value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, stage %u, state %#x, value %p.\n", iface, stage, state, value);

    if (state >= sizeof(tss_lookup) / sizeof(*tss_lookup))
    {
        WARN("Invalid state %#x passed.\n", state);
        return D3D_OK;
    }

    wined3d_mutex_lock();
    *value = wined3d_device_get_texture_stage_state(device->wined3d_device, stage, tss_lookup[state]);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetTextureStageState(IDirect3DDevice9Ex *iface,
        DWORD stage, D3DTEXTURESTAGESTATETYPE state, DWORD value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, stage %u, state %#x, value %#x.\n", iface, stage, state, value);

    if (state >= sizeof(tss_lookup) / sizeof(*tss_lookup))
    {
        WARN("Invalid state %#x passed.\n", state);
        return D3D_OK;
    }

    wined3d_mutex_lock();
    wined3d_device_set_texture_stage_state(device->wined3d_device, stage, tss_lookup[state], value);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetSamplerState(IDirect3DDevice9Ex *iface,
        DWORD sampler, D3DSAMPLERSTATETYPE state, DWORD *value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, sampler %u, state %#x, value %p.\n", iface, sampler, state, value);

    wined3d_mutex_lock();
    *value = wined3d_device_get_sampler_state(device->wined3d_device, sampler, state);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_SetSamplerState(IDirect3DDevice9Ex *iface,
        DWORD sampler, D3DSAMPLERSTATETYPE state, DWORD value)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, sampler %u, state %#x, value %#x.\n", iface, sampler, state, value);

    wined3d_mutex_lock();
    wined3d_device_set_sampler_state(device->wined3d_device, sampler, state, value);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_ValidateDevice(IDirect3DDevice9Ex *iface, DWORD *pass_count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, pass_count %p.\n", iface, pass_count);

    wined3d_mutex_lock();
    hr = wined3d_device_validate_device(device->wined3d_device, pass_count);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetPaletteEntries(IDirect3DDevice9Ex *iface,
        UINT palette_idx, const PALETTEENTRY *entries)
{
    WARN("iface %p, palette_idx %u, entries %p unimplemented.\n", iface, palette_idx, entries);

    /* The d3d9 palette API is non-functional on Windows. Getters and setters are implemented,
     * and some drivers allow the creation of P8 surfaces. These surfaces can be copied to
     * other P8 surfaces with StretchRect, but cannot be converted to (A)RGB.
     *
     * Some older(dx7) cards may have support for P8 textures, but games cannot rely on this. */
    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetPaletteEntries(IDirect3DDevice9Ex *iface,
        UINT palette_idx, PALETTEENTRY *entries)
{
    FIXME("iface %p, palette_idx %u, entries %p unimplemented.\n", iface, palette_idx, entries);

    return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI d3d9_device_SetCurrentTexturePalette(IDirect3DDevice9Ex *iface, UINT palette_idx)
{
    WARN("iface %p, palette_idx %u unimplemented.\n", iface, palette_idx);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetCurrentTexturePalette(IDirect3DDevice9Ex *iface, UINT *palette_idx)
{
    FIXME("iface %p, palette_idx %p.\n", iface, palette_idx);

    return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI d3d9_device_SetScissorRect(IDirect3DDevice9Ex *iface, const RECT *rect)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, rect %p.\n", iface, rect);

    wined3d_mutex_lock();
    wined3d_device_set_scissor_rect(device->wined3d_device, rect);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetScissorRect(IDirect3DDevice9Ex *iface, RECT *rect)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, rect %p.\n", iface, rect);

    wined3d_mutex_lock();
    wined3d_device_get_scissor_rect(device->wined3d_device, rect);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetSoftwareVertexProcessing(IDirect3DDevice9Ex *iface, BOOL software)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, software %#x.\n", iface, software);

    wined3d_mutex_lock();
    wined3d_device_set_software_vertex_processing(device->wined3d_device, software);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static BOOL WINAPI d3d9_device_GetSoftwareVertexProcessing(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    BOOL ret;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    ret = wined3d_device_get_software_vertex_processing(device->wined3d_device);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_device_SetNPatchMode(IDirect3DDevice9Ex *iface, float segment_count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, segment_count %.8e.\n", iface, segment_count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_npatch_mode(device->wined3d_device, segment_count);
    wined3d_mutex_unlock();

    return hr;
}

static float WINAPI d3d9_device_GetNPatchMode(IDirect3DDevice9Ex *iface)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    float ret;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    ret = wined3d_device_get_npatch_mode(device->wined3d_device);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_device_DrawPrimitive(IDirect3DDevice9Ex *iface,
        D3DPRIMITIVETYPE primitive_type, UINT start_vertex, UINT primitive_count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, primitive_type %#x, start_vertex %u, primitive_count %u.\n",
            iface, primitive_type, start_vertex, primitive_count);

    wined3d_mutex_lock();
    if (!device->has_vertex_declaration)
    {
        wined3d_mutex_unlock();
        WARN("Called without a valid vertex declaration set.\n");
        return D3DERR_INVALIDCALL;
    }
    wined3d_device_set_primitive_type(device->wined3d_device, primitive_type, 0);
    hr = wined3d_device_draw_primitive(device->wined3d_device, start_vertex,
            vertex_count_from_primitive_count(primitive_type, primitive_count));
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_DrawIndexedPrimitive(IDirect3DDevice9Ex *iface,
        D3DPRIMITIVETYPE primitive_type, INT base_vertex_idx, UINT min_vertex_idx,
        UINT vertex_count, UINT start_idx, UINT primitive_count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, primitive_type %#x, base_vertex_idx %u, min_vertex_idx %u, "
            "vertex_count %u, start_idx %u, primitive_count %u.\n",
            iface, primitive_type, base_vertex_idx, min_vertex_idx,
            vertex_count, start_idx, primitive_count);

    wined3d_mutex_lock();
    if (!device->has_vertex_declaration)
    {
        wined3d_mutex_unlock();
        WARN("Called without a valid vertex declaration set.\n");
        return D3DERR_INVALIDCALL;
    }
    wined3d_device_set_base_vertex_index(device->wined3d_device, base_vertex_idx);
    wined3d_device_set_primitive_type(device->wined3d_device, primitive_type, 0);
    hr = wined3d_device_draw_indexed_primitive(device->wined3d_device, start_idx,
            vertex_count_from_primitive_count(primitive_type, primitive_count));
    wined3d_mutex_unlock();

    return hr;
}

/* The caller is responsible for wined3d locking */
static HRESULT d3d9_device_prepare_vertex_buffer(struct d3d9_device *device, UINT min_size)
{
    HRESULT hr;

    if (device->vertex_buffer_size < min_size || !device->vertex_buffer)
    {
        UINT size = max(device->vertex_buffer_size * 2, min_size);
        struct wined3d_buffer *buffer;

        TRACE("Growing vertex buffer to %u bytes.\n", size);

        hr = wined3d_buffer_create_vb(device->wined3d_device, size, WINED3DUSAGE_DYNAMIC | WINED3DUSAGE_WRITEONLY,
                WINED3D_POOL_DEFAULT, NULL, &d3d9_null_wined3d_parent_ops, &buffer);
        if (FAILED(hr))
        {
            ERR("(%p) wined3d_buffer_create_vb failed with hr = %08x.\n", device, hr);
            return hr;
        }

        if (device->vertex_buffer)
            wined3d_buffer_decref(device->vertex_buffer);

        device->vertex_buffer = buffer;
        device->vertex_buffer_size = size;
        device->vertex_buffer_pos = 0;
    }
    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_DrawPrimitiveUP(IDirect3DDevice9Ex *iface,
        D3DPRIMITIVETYPE primitive_type, UINT primitive_count, const void *data, UINT stride)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;
    UINT vtx_count = vertex_count_from_primitive_count(primitive_type, primitive_count);
    struct wined3d_map_desc wined3d_map_desc;
    struct wined3d_box wined3d_box = {0};
    UINT size = vtx_count * stride;
    struct wined3d_resource *vb;
    UINT vb_pos, align;

    TRACE("iface %p, primitive_type %#x, primitive_count %u, data %p, stride %u.\n",
            iface, primitive_type, primitive_count, data, stride);

    if (!primitive_count)
    {
        WARN("primitive_count is 0, returning D3D_OK\n");
        return D3D_OK;
    }

    wined3d_mutex_lock();

    if (!device->has_vertex_declaration)
    {
        wined3d_mutex_unlock();
        WARN("Called without a valid vertex declaration set.\n");
        return D3DERR_INVALIDCALL;
    }

    hr = d3d9_device_prepare_vertex_buffer(device, size);
    if (FAILED(hr))
        goto done;

    vb_pos = device->vertex_buffer_pos;
    align = vb_pos % stride;
    if (align) align = stride - align;
    if (vb_pos + size + align > device->vertex_buffer_size)
        vb_pos = 0;
    else
        vb_pos += align;

    wined3d_box.left = vb_pos;
    wined3d_box.right = vb_pos + size;
    vb = wined3d_buffer_get_resource(device->vertex_buffer);
    if (FAILED(hr = wined3d_resource_map(vb, 0, &wined3d_map_desc, &wined3d_box,
            vb_pos ? WINED3D_MAP_NOOVERWRITE : WINED3D_MAP_DISCARD)))
        goto done;
    memcpy(wined3d_map_desc.data, data, size);
    wined3d_resource_unmap(vb, 0);
    device->vertex_buffer_pos = vb_pos + size;

    hr = wined3d_device_set_stream_source(device->wined3d_device, 0, device->vertex_buffer, 0, stride);
    if (FAILED(hr))
        goto done;

    wined3d_device_set_primitive_type(device->wined3d_device, primitive_type, 0);
    hr = wined3d_device_draw_primitive(device->wined3d_device, vb_pos / stride, vtx_count);
    wined3d_device_set_stream_source(device->wined3d_device, 0, NULL, 0, 0);

done:
    wined3d_mutex_unlock();
    return hr;
}

/* The caller is responsible for wined3d locking */
static HRESULT d3d9_device_prepare_index_buffer(struct d3d9_device *device, UINT min_size)
{
    HRESULT hr;

    if (device->index_buffer_size < min_size || !device->index_buffer)
    {
        UINT size = max(device->index_buffer_size * 2, min_size);
        struct wined3d_buffer *buffer;

        TRACE("Growing index buffer to %u bytes.\n", size);

        hr = wined3d_buffer_create_ib(device->wined3d_device, size, WINED3DUSAGE_DYNAMIC | WINED3DUSAGE_WRITEONLY,
                WINED3D_POOL_DEFAULT, NULL, &d3d9_null_wined3d_parent_ops, &buffer);
        if (FAILED(hr))
        {
            ERR("(%p) wined3d_buffer_create_ib failed with hr = %08x.\n", device, hr);
            return hr;
        }

        if (device->index_buffer)
            wined3d_buffer_decref(device->index_buffer);

        device->index_buffer = buffer;
        device->index_buffer_size = size;
        device->index_buffer_pos = 0;
    }
    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_DrawIndexedPrimitiveUP(IDirect3DDevice9Ex *iface,
        D3DPRIMITIVETYPE primitive_type, UINT min_vertex_idx, UINT vertex_count,
        UINT primitive_count, const void *index_data, D3DFORMAT index_format,
        const void *vertex_data, UINT vertex_stride)
{
    UINT idx_count = vertex_count_from_primitive_count(primitive_type, primitive_count);
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    UINT idx_fmt_size = index_format == D3DFMT_INDEX16 ? 2 : 4;
    UINT vtx_size = vertex_count * vertex_stride;
    UINT idx_size = idx_count * idx_fmt_size;
    struct wined3d_map_desc wined3d_map_desc;
    struct wined3d_box wined3d_box = {0};
    struct wined3d_resource *ib, *vb;
    UINT vb_pos, ib_pos, align;
    HRESULT hr;

    TRACE("iface %p, primitive_type %#x, min_vertex_idx %u, vertex_count %u, primitive_count %u, "
            "index_data %p, index_format %#x, vertex_data %p, vertex_stride %u.\n",
            iface, primitive_type, min_vertex_idx, vertex_count, primitive_count,
            index_data, index_format, vertex_data, vertex_stride);

    if (!primitive_count)
    {
        WARN("primitive_count is 0, returning D3D_OK.\n");
        return D3D_OK;
    }

    wined3d_mutex_lock();

    if (!device->has_vertex_declaration)
    {
        wined3d_mutex_unlock();
        WARN("Called without a valid vertex declaration set.\n");
        return D3DERR_INVALIDCALL;
    }

    hr = d3d9_device_prepare_vertex_buffer(device, vtx_size);
    if (FAILED(hr))
        goto done;

    vb_pos = device->vertex_buffer_pos;
    align = vb_pos % vertex_stride;
    if (align) align = vertex_stride - align;
    if (vb_pos + vtx_size + align > device->vertex_buffer_size)
        vb_pos = 0;
    else
        vb_pos += align;

    wined3d_box.left = vb_pos;
    wined3d_box.right = vb_pos + vtx_size;
    vb = wined3d_buffer_get_resource(device->vertex_buffer);
    if (FAILED(hr = wined3d_resource_map(vb, 0, &wined3d_map_desc, &wined3d_box,
            vb_pos ? WINED3D_MAP_NOOVERWRITE : WINED3D_MAP_DISCARD)))
        goto done;
    memcpy(wined3d_map_desc.data, (char *)vertex_data + min_vertex_idx * vertex_stride, vtx_size);
    wined3d_resource_unmap(vb, 0);
    device->vertex_buffer_pos = vb_pos + vtx_size;

    hr = d3d9_device_prepare_index_buffer(device, idx_size);
    if (FAILED(hr))
        goto done;

    ib_pos = device->index_buffer_pos;
    align = ib_pos % idx_fmt_size;
    if (align) align = idx_fmt_size - align;
    if (ib_pos + idx_size + align > device->index_buffer_size)
        ib_pos = 0;
    else
        ib_pos += align;

    wined3d_box.left = ib_pos;
    wined3d_box.right = ib_pos + idx_size;
    ib = wined3d_buffer_get_resource(device->index_buffer);
    if (FAILED(hr = wined3d_resource_map(ib, 0, &wined3d_map_desc, &wined3d_box,
            ib_pos ? WINED3D_MAP_NOOVERWRITE : WINED3D_MAP_DISCARD)))
        goto done;
    memcpy(wined3d_map_desc.data, index_data, idx_size);
    wined3d_resource_unmap(ib, 0);
    device->index_buffer_pos = ib_pos + idx_size;

    hr = wined3d_device_set_stream_source(device->wined3d_device, 0, device->vertex_buffer, 0, vertex_stride);
    if (FAILED(hr))
        goto done;

    wined3d_device_set_index_buffer(device->wined3d_device, device->index_buffer,
            wined3dformat_from_d3dformat(index_format), 0);
    wined3d_device_set_base_vertex_index(device->wined3d_device, vb_pos / vertex_stride - min_vertex_idx);

    wined3d_device_set_primitive_type(device->wined3d_device, primitive_type, 0);
    hr = wined3d_device_draw_indexed_primitive(device->wined3d_device, ib_pos / idx_fmt_size, idx_count);

    wined3d_device_set_stream_source(device->wined3d_device, 0, NULL, 0, 0);
    wined3d_device_set_index_buffer(device->wined3d_device, NULL, WINED3DFMT_UNKNOWN, 0);
    wined3d_device_set_base_vertex_index(device->wined3d_device, 0);

done:
    wined3d_mutex_unlock();
    return hr;
}

static HRESULT WINAPI d3d9_device_ProcessVertices(IDirect3DDevice9Ex *iface,
        UINT src_start_idx, UINT dst_idx, UINT vertex_count, IDirect3DVertexBuffer9 *dst_buffer,
        IDirect3DVertexDeclaration9 *declaration, DWORD flags)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexbuffer *dst_impl = unsafe_impl_from_IDirect3DVertexBuffer9(dst_buffer);
    struct d3d9_vertex_declaration *decl_impl = unsafe_impl_from_IDirect3DVertexDeclaration9(declaration);
    HRESULT hr;

    TRACE("iface %p, src_start_idx %u, dst_idx %u, vertex_count %u, dst_buffer %p, declaration %p, flags %#x.\n",
            iface, src_start_idx, dst_idx, vertex_count, dst_buffer, declaration, flags);

    wined3d_mutex_lock();
    hr = wined3d_device_process_vertices(device->wined3d_device, src_start_idx, dst_idx, vertex_count,
            dst_impl->wined3d_buffer, decl_impl ? decl_impl->wined3d_declaration : NULL,
            flags, dst_impl->fvf);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_CreateVertexDeclaration(IDirect3DDevice9Ex *iface,
        const D3DVERTEXELEMENT9 *elements, IDirect3DVertexDeclaration9 **declaration)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertex_declaration *object;
    HRESULT hr;

    TRACE("iface %p, elements %p, declaration %p.\n", iface, elements, declaration);

    if (!declaration)
    {
        WARN("Caller passed a NULL declaration, returning D3DERR_INVALIDCALL.\n");
        return D3DERR_INVALIDCALL;
    }

    if (SUCCEEDED(hr = d3d9_vertex_declaration_create(device, elements, &object)))
        *declaration = &object->IDirect3DVertexDeclaration9_iface;

    return hr;
}

static HRESULT WINAPI d3d9_device_SetVertexDeclaration(IDirect3DDevice9Ex *iface,
        IDirect3DVertexDeclaration9 *declaration)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertex_declaration *decl_impl = unsafe_impl_from_IDirect3DVertexDeclaration9(declaration);

    TRACE("iface %p, declaration %p.\n", iface, declaration);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_declaration(device->wined3d_device,
            decl_impl ? decl_impl->wined3d_declaration : NULL);
    device->has_vertex_declaration = !!decl_impl;
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetVertexDeclaration(IDirect3DDevice9Ex *iface,
        IDirect3DVertexDeclaration9 **declaration)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_vertex_declaration *wined3d_declaration;
    struct d3d9_vertex_declaration *declaration_impl;

    TRACE("iface %p, declaration %p.\n", iface, declaration);

    if (!declaration) return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    if ((wined3d_declaration = wined3d_device_get_vertex_declaration(device->wined3d_device)))
    {
        declaration_impl = wined3d_vertex_declaration_get_parent(wined3d_declaration);
        *declaration = &declaration_impl->IDirect3DVertexDeclaration9_iface;
        IDirect3DVertexDeclaration9_AddRef(*declaration);
    }
    else
    {
        *declaration = NULL;
    }
    wined3d_mutex_unlock();

    TRACE("Returning %p.\n", *declaration);
    return D3D_OK;
}

static struct wined3d_vertex_declaration *device_get_fvf_declaration(struct d3d9_device *device, DWORD fvf)
{
    struct wined3d_vertex_declaration *wined3d_declaration;
    struct fvf_declaration *fvf_decls = device->fvf_decls;
    struct d3d9_vertex_declaration *d3d9_declaration;
    D3DVERTEXELEMENT9 *elements;
    int p, low, high; /* deliberately signed */
    HRESULT hr;

    TRACE("Searching for declaration for fvf %08x... ", fvf);

    low = 0;
    high = device->fvf_decl_count - 1;
    while (low <= high)
    {
        p = (low + high) >> 1;
        TRACE("%d ", p);

        if (fvf_decls[p].fvf == fvf)
        {
            TRACE("found %p.\n", fvf_decls[p].decl);
            return fvf_decls[p].decl;
        }

        if (fvf_decls[p].fvf < fvf)
            low = p + 1;
        else
            high = p - 1;
    }
    TRACE("not found. Creating and inserting at position %d.\n", low);

    if (FAILED(hr = vdecl_convert_fvf(fvf, &elements)))
        return NULL;

    hr = d3d9_vertex_declaration_create(device, elements, &d3d9_declaration);
    HeapFree(GetProcessHeap(), 0, elements);
    if (FAILED(hr))
        return NULL;

    if (device->fvf_decl_size == device->fvf_decl_count)
    {
        UINT grow = max(device->fvf_decl_size / 2, 8);

        fvf_decls = HeapReAlloc(GetProcessHeap(), 0, fvf_decls, sizeof(*fvf_decls) * (device->fvf_decl_size + grow));
        if (!fvf_decls)
        {
            IDirect3DVertexDeclaration9_Release(&d3d9_declaration->IDirect3DVertexDeclaration9_iface);
            return NULL;
        }
        device->fvf_decls = fvf_decls;
        device->fvf_decl_size += grow;
    }

    d3d9_declaration->fvf = fvf;
    wined3d_declaration = d3d9_declaration->wined3d_declaration;
    wined3d_vertex_declaration_incref(wined3d_declaration);
    IDirect3DVertexDeclaration9_Release(&d3d9_declaration->IDirect3DVertexDeclaration9_iface);

    memmove(fvf_decls + low + 1, fvf_decls + low, sizeof(*fvf_decls) * (device->fvf_decl_count - low));
    fvf_decls[low].decl = wined3d_declaration;
    fvf_decls[low].fvf = fvf;
    ++device->fvf_decl_count;

    TRACE("Returning %p. %u declarations in array.\n", wined3d_declaration, device->fvf_decl_count);

    return wined3d_declaration;
}

static HRESULT WINAPI d3d9_device_SetFVF(IDirect3DDevice9Ex *iface, DWORD fvf)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_vertex_declaration *decl;

    TRACE("iface %p, fvf %#x.\n", iface, fvf);

    if (!fvf)
    {
        WARN("%#x is not a valid FVF.\n", fvf);
        return D3D_OK;
    }

    wined3d_mutex_lock();
    if (!(decl = device_get_fvf_declaration(device, fvf)))
    {
        wined3d_mutex_unlock();
        ERR("Failed to create a vertex declaration for fvf %#x.\n", fvf);
        return D3DERR_DRIVERINTERNALERROR;
    }

    wined3d_device_set_vertex_declaration(device->wined3d_device, decl);
    device->has_vertex_declaration = TRUE;
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetFVF(IDirect3DDevice9Ex *iface, DWORD *fvf)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_vertex_declaration *wined3d_declaration;
    struct d3d9_vertex_declaration *d3d9_declaration;

    TRACE("iface %p, fvf %p.\n", iface, fvf);

    wined3d_mutex_lock();
    if ((wined3d_declaration = wined3d_device_get_vertex_declaration(device->wined3d_device)))
    {
        d3d9_declaration = wined3d_vertex_declaration_get_parent(wined3d_declaration);
        *fvf = d3d9_declaration->fvf;
    }
    else
    {
        *fvf = 0;
    }
    wined3d_mutex_unlock();

    TRACE("Returning FVF %#x.\n", *fvf);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreateVertexShader(IDirect3DDevice9Ex *iface,
        const DWORD *byte_code, IDirect3DVertexShader9 **shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexshader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, shader %p.\n", iface, byte_code, shader);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return E_OUTOFMEMORY;

    hr = vertexshader_init(object, device, byte_code);
    if (FAILED(hr))
    {
        WARN("Failed to initialize vertex shader, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created vertex shader %p.\n", object);
    *shader = &object->IDirect3DVertexShader9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetVertexShader(IDirect3DDevice9Ex *iface, IDirect3DVertexShader9 *shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexshader *shader_obj = unsafe_impl_from_IDirect3DVertexShader9(shader);

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    wined3d_device_set_vertex_shader(device->wined3d_device,
            shader_obj ? shader_obj->wined3d_shader : NULL);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetVertexShader(IDirect3DDevice9Ex *iface, IDirect3DVertexShader9 **shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexshader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    if ((wined3d_shader = wined3d_device_get_vertex_shader(device->wined3d_device)))
    {
        shader_impl = wined3d_shader_get_parent(wined3d_shader);
        *shader = &shader_impl->IDirect3DVertexShader9_iface;
        IDirect3DVertexShader9_AddRef(*shader);
    }
    else
    {
        *shader = NULL;
    }
    wined3d_mutex_unlock();

    TRACE("Returning %p.\n", *shader);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetVertexShaderConstantF(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const float *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    if (reg_idx + count > D3D9_MAX_VERTEX_SHADER_CONSTANTF)
    {
        WARN("Trying to access %u constants, but d3d9 only supports %u\n",
             reg_idx + count, D3D9_MAX_VERTEX_SHADER_CONSTANTF);
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    hr = wined3d_device_set_vs_consts_f(device->wined3d_device,
            reg_idx, count, (const struct wined3d_vec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetVertexShaderConstantF(IDirect3DDevice9Ex *iface,
        UINT reg_idx, float *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    if (reg_idx + count > D3D9_MAX_VERTEX_SHADER_CONSTANTF)
    {
        WARN("Trying to access %u constants, but d3d9 only supports %u\n",
             reg_idx + count, D3D9_MAX_VERTEX_SHADER_CONSTANTF);
        return D3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    hr = wined3d_device_get_vs_consts_f(device->wined3d_device,
            reg_idx, count, (struct wined3d_vec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetVertexShaderConstantI(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const int *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_vs_consts_i(device->wined3d_device,
            reg_idx, count, (const struct wined3d_ivec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetVertexShaderConstantI(IDirect3DDevice9Ex *iface,
        UINT reg_idx, int *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_get_vs_consts_i(device->wined3d_device,
            reg_idx, count, (struct wined3d_ivec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetVertexShaderConstantB(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const BOOL *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_vs_consts_b(device->wined3d_device, reg_idx, count, data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetVertexShaderConstantB(IDirect3DDevice9Ex *iface,
        UINT reg_idx, BOOL *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_get_vs_consts_b(device->wined3d_device, reg_idx, count, data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetStreamSource(IDirect3DDevice9Ex *iface,
        UINT stream_idx, IDirect3DVertexBuffer9 *buffer, UINT offset, UINT stride)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexbuffer *buffer_impl = unsafe_impl_from_IDirect3DVertexBuffer9(buffer);
    HRESULT hr;

    TRACE("iface %p, stream_idx %u, buffer %p, offset %u, stride %u.\n",
            iface, stream_idx, buffer, offset, stride);

    wined3d_mutex_lock();
    hr = wined3d_device_set_stream_source(device->wined3d_device, stream_idx,
            buffer_impl ? buffer_impl->wined3d_buffer : NULL, offset, stride);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetStreamSource(IDirect3DDevice9Ex *iface,
        UINT stream_idx, IDirect3DVertexBuffer9 **buffer, UINT *offset, UINT *stride)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_vertexbuffer *buffer_impl;
    struct wined3d_buffer *wined3d_buffer;
    HRESULT hr;

    TRACE("iface %p, stream_idx %u, buffer %p, offset %p, stride %p.\n",
            iface, stream_idx, buffer, offset, stride);

    if (!buffer)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_device_get_stream_source(device->wined3d_device, stream_idx, &wined3d_buffer, offset, stride);
    if (SUCCEEDED(hr) && wined3d_buffer)
    {
        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        *buffer = &buffer_impl->IDirect3DVertexBuffer9_iface;
        IDirect3DVertexBuffer9_AddRef(*buffer);
    }
    else
    {
        if (FAILED(hr))
            FIXME("Call to GetStreamSource failed %p %p\n", offset, stride);
        *buffer = NULL;
    }
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetStreamSourceFreq(IDirect3DDevice9Ex *iface, UINT stream_idx, UINT freq)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, stream_idx %u, freq %u.\n", iface, stream_idx, freq);

    wined3d_mutex_lock();
    hr = wined3d_device_set_stream_source_freq(device->wined3d_device, stream_idx, freq);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetStreamSourceFreq(IDirect3DDevice9Ex *iface, UINT stream_idx, UINT *freq)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, stream_idx %u, freq %p.\n", iface, stream_idx, freq);

    wined3d_mutex_lock();
    hr = wined3d_device_get_stream_source_freq(device->wined3d_device, stream_idx, freq);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetIndices(IDirect3DDevice9Ex *iface, IDirect3DIndexBuffer9 *buffer)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_indexbuffer *ib = unsafe_impl_from_IDirect3DIndexBuffer9(buffer);

    TRACE("iface %p, buffer %p.\n", iface, buffer);

    wined3d_mutex_lock();
    wined3d_device_set_index_buffer(device->wined3d_device,
            ib ? ib->wined3d_buffer : NULL, ib ? ib->format : WINED3DFMT_UNKNOWN, 0);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetIndices(IDirect3DDevice9Ex *iface, IDirect3DIndexBuffer9 **buffer)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    enum wined3d_format_id wined3d_format;
    struct wined3d_buffer *wined3d_buffer;
    struct d3d9_indexbuffer *buffer_impl;

    TRACE("iface %p, buffer %p.\n", iface, buffer);

    if (!buffer)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    if ((wined3d_buffer = wined3d_device_get_index_buffer(device->wined3d_device, &wined3d_format, NULL)))
    {
        buffer_impl = wined3d_buffer_get_parent(wined3d_buffer);
        *buffer = &buffer_impl->IDirect3DIndexBuffer9_iface;
        IDirect3DIndexBuffer9_AddRef(*buffer);
    }
    else
    {
        *buffer = NULL;
    }
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_CreatePixelShader(IDirect3DDevice9Ex *iface,
        const DWORD *byte_code, IDirect3DPixelShader9 **shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_pixelshader *object;
    HRESULT hr;

    TRACE("iface %p, byte_code %p, shader %p.\n", iface, byte_code, shader);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
    {
        FIXME("Failed to allocate pixel shader memory.\n");
        return E_OUTOFMEMORY;
    }

    hr = pixelshader_init(object, device, byte_code);
    if (FAILED(hr))
    {
        WARN("Failed to initialize pixel shader, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created pixel shader %p.\n", object);
    *shader = &object->IDirect3DPixelShader9_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetPixelShader(IDirect3DDevice9Ex *iface, IDirect3DPixelShader9 *shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_pixelshader *shader_obj = unsafe_impl_from_IDirect3DPixelShader9(shader);

    TRACE("iface %p, shader %p.\n", iface, shader);

    wined3d_mutex_lock();
    wined3d_device_set_pixel_shader(device->wined3d_device,
            shader_obj ? shader_obj->wined3d_shader : NULL);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetPixelShader(IDirect3DDevice9Ex *iface, IDirect3DPixelShader9 **shader)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_pixelshader *shader_impl;
    struct wined3d_shader *wined3d_shader;

    TRACE("iface %p, shader %p.\n", iface, shader);

    if (!shader) return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    if ((wined3d_shader = wined3d_device_get_pixel_shader(device->wined3d_device)))
    {
        shader_impl = wined3d_shader_get_parent(wined3d_shader);
        *shader = &shader_impl->IDirect3DPixelShader9_iface;
        IDirect3DPixelShader9_AddRef(*shader);
    }
    else
    {
        *shader = NULL;
    }
    wined3d_mutex_unlock();

    TRACE("Returning %p.\n", *shader);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetPixelShaderConstantF(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const float *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_ps_consts_f(device->wined3d_device,
            reg_idx, count, (const struct wined3d_vec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetPixelShaderConstantF(IDirect3DDevice9Ex *iface,
        UINT reg_idx, float *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_get_ps_consts_f(device->wined3d_device,
            reg_idx, count, (struct wined3d_vec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetPixelShaderConstantI(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const int *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_ps_consts_i(device->wined3d_device,
            reg_idx, count, (const struct wined3d_ivec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetPixelShaderConstantI(IDirect3DDevice9Ex *iface,
        UINT reg_idx, int *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_get_ps_consts_i(device->wined3d_device,
            reg_idx, count, (struct wined3d_ivec4 *)data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_SetPixelShaderConstantB(IDirect3DDevice9Ex *iface,
        UINT reg_idx, const BOOL *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_set_ps_consts_b(device->wined3d_device, reg_idx, count, data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_GetPixelShaderConstantB(IDirect3DDevice9Ex *iface,
        UINT reg_idx, BOOL *data, UINT count)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, reg_idx %u, data %p, count %u.\n", iface, reg_idx, data, count);

    wined3d_mutex_lock();
    hr = wined3d_device_get_ps_consts_b(device->wined3d_device, reg_idx, count, data);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_device_DrawRectPatch(IDirect3DDevice9Ex *iface, UINT handle,
        const float *segment_count, const D3DRECTPATCH_INFO *patch_info)
{
    FIXME("iface %p, handle %#x, segment_count %p, patch_info %p unimplemented.\n",
            iface, handle, segment_count, patch_info);
    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_DrawTriPatch(IDirect3DDevice9Ex *iface, UINT handle,
        const float *segment_count, const D3DTRIPATCH_INFO *patch_info)
{
    FIXME("iface %p, handle %#x, segment_count %p, patch_info %p unimplemented.\n",
            iface, handle, segment_count, patch_info);
    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_DeletePatch(IDirect3DDevice9Ex *iface, UINT handle)
{
    FIXME("iface %p, handle %#x unimplemented.\n", iface, handle);
    return D3DERR_INVALIDCALL;
}

static HRESULT WINAPI d3d9_device_CreateQuery(IDirect3DDevice9Ex *iface, D3DQUERYTYPE type, IDirect3DQuery9 **query)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct d3d9_query *object;
    HRESULT hr;

    TRACE("iface %p, type %#x, query %p.\n", iface, type, query);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return E_OUTOFMEMORY;

    hr = query_init(object, device, type);
    if (FAILED(hr))
    {
        WARN("Failed to initialize query, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created query %p.\n", object);
    if (query) *query = &object->IDirect3DQuery9_iface;
    else IDirect3DQuery9_Release(&object->IDirect3DQuery9_iface);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_SetConvolutionMonoKernel(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, float *rows, float *columns)
{
    FIXME("iface %p, width %u, height %u, rows %p, columns %p stub!\n",
            iface, width, height, rows, columns);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_ComposeRects(IDirect3DDevice9Ex *iface,
        IDirect3DSurface9 *src_surface, IDirect3DSurface9 *dst_surface, IDirect3DVertexBuffer9 *src_descs,
        UINT rect_count, IDirect3DVertexBuffer9 *dst_descs, D3DCOMPOSERECTSOP operation, INT offset_x, INT offset_y)
{
    FIXME("iface %p, src_surface %p, dst_surface %p, src_descs %p, rect_count %u, "
            "dst_descs %p, operation %#x, offset_x %u, offset_y %u stub!\n",
            iface, src_surface, dst_surface, src_descs, rect_count,
            dst_descs, operation, offset_x, offset_y);

    return E_NOTIMPL;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_PresentEx(IDirect3DDevice9Ex *iface,
        const RECT *src_rect, const RECT *dst_rect, HWND dst_window_override,
        const RGNDATA *dirty_region, DWORD flags)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    UINT i;
    HRESULT hr;

    TRACE("iface %p, src_rect %s, dst_rect %s, dst_window_override %p, dirty_region %p, flags %#x.\n",
            iface, wine_dbgstr_rect(src_rect), wine_dbgstr_rect(dst_rect),
            dst_window_override, dirty_region, flags);

    if (device->device_state != D3D9_DEVICE_STATE_OK)
        return S_PRESENT_OCCLUDED;

    if (dirty_region)
        FIXME("Ignoring dirty_region %p.\n", dirty_region);

    wined3d_mutex_lock();
    for (i = 0; i < device->implicit_swapchain_count; ++i)
    {
        if (FAILED(hr = wined3d_swapchain_present(device->implicit_swapchains[i]->wined3d_swapchain,
                src_rect, dst_rect, dst_window_override, flags)))
        {
            wined3d_mutex_unlock();
            return hr;
        }
    }
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_device_GetGPUThreadPriority(IDirect3DDevice9Ex *iface, INT *priority)
{
    FIXME("iface %p, priority %p stub!\n", iface, priority);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_SetGPUThreadPriority(IDirect3DDevice9Ex *iface, INT priority)
{
    FIXME("iface %p, priority %d stub!\n", iface, priority);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_WaitForVBlank(IDirect3DDevice9Ex *iface, UINT swapchain_idx)
{
    FIXME("iface %p, swapchain_idx %u stub!\n", iface, swapchain_idx);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_CheckResourceResidency(IDirect3DDevice9Ex *iface,
        IDirect3DResource9 **resources, UINT32 resource_count)
{
    FIXME("iface %p, resources %p, resource_count %u stub!\n",
            iface, resources, resource_count);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_SetMaximumFrameLatency(IDirect3DDevice9Ex *iface, UINT max_latency)
{
    FIXME("iface %p, max_latency %u stub!\n", iface, max_latency);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_GetMaximumFrameLatency(IDirect3DDevice9Ex *iface, UINT *max_latency)
{
    FIXME("iface %p, max_latency %p stub!\n", iface, max_latency);

    *max_latency = 2;

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_CheckDeviceState(IDirect3DDevice9Ex *iface, HWND dst_window)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_swapchain_desc swapchain_desc;

    TRACE("iface %p, dst_window %p.\n", iface, dst_window);

    wined3d_mutex_lock();
    wined3d_swapchain_get_desc(device->implicit_swapchains[0]->wined3d_swapchain, &swapchain_desc);
    wined3d_mutex_unlock();

    if (swapchain_desc.windowed)
        return D3D_OK;

    /* FIXME: This is actually supposed to check if any other device is in
     * fullscreen mode. */
    if (dst_window != swapchain_desc.device_window)
        return device->device_state == D3D9_DEVICE_STATE_OK ? S_PRESENT_OCCLUDED : D3D_OK;

    return device->device_state == D3D9_DEVICE_STATE_OK ? D3D_OK : S_PRESENT_OCCLUDED;
}

static HRESULT WINAPI d3d9_device_CreateRenderTargetEx(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multisample_type, DWORD multisample_quality,
        BOOL lockable, IDirect3DSurface9 **surface, HANDLE *shared_handle, DWORD usage)
{
    FIXME("iface %p, width %u, height %u, format %#x, multisample_type %#x, multisample_quality %u, "
            "lockable %#x, surface %p, shared_handle %p, usage %#x stub!\n",
            iface, width, height, format, multisample_type, multisample_quality,
            lockable, surface, shared_handle, usage);

    *surface = NULL;
    if (shared_handle)
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_CreateOffscreenPlainSurfaceEx(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, D3DFORMAT format, D3DPOOL pool, IDirect3DSurface9 **surface,
        HANDLE *shared_handle, DWORD usage)
{
    FIXME("iface %p, width %u, height %u, format %#x, pool %#x, surface %p, shared_handle %p, usage %#x stub!\n",
            iface, width, height, format, pool, surface, shared_handle, usage);

    return E_NOTIMPL;
}

static HRESULT WINAPI d3d9_device_CreateDepthStencilSurfaceEx(IDirect3DDevice9Ex *iface,
        UINT width, UINT height, D3DFORMAT format, D3DMULTISAMPLE_TYPE multisample_type, DWORD multisample_quality,
        BOOL discard, IDirect3DSurface9 **surface, HANDLE *shared_handle, DWORD usage)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    DWORD flags = WINED3D_TEXTURE_CREATE_MAPPABLE;

    TRACE("iface %p, width %u, height %u, format %#x, multisample_type %#x, multisample_quality %u, "
            "discard %#x, surface %p, shared_handle %p, usage %#x.\n",
            iface, width, height, format, multisample_type, multisample_quality,
            discard, surface, shared_handle, usage);

    if (usage & D3DUSAGE_DEPTHSTENCIL)
    {
        WARN("Invalid usage %#x.\n", usage);
        return D3DERR_INVALIDCALL;
    }

    if (shared_handle)
        FIXME("Resource sharing not implemented, *shared_handle %p.\n", *shared_handle);

    if (discard)
        flags |= WINED3D_TEXTURE_CREATE_DISCARD;

    *surface = NULL;
    return d3d9_device_create_surface(device, width, height, format, flags, surface,
            D3DUSAGE_DEPTHSTENCIL | usage, D3DPOOL_DEFAULT, multisample_type, multisample_quality, NULL);
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_device_ResetEx(IDirect3DDevice9Ex *iface,
        D3DPRESENT_PARAMETERS *present_parameters, D3DDISPLAYMODEEX *mode)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);

    TRACE("iface %p, present_parameters %p, mode %p.\n", iface, present_parameters, mode);

    if (!present_parameters->Windowed == !mode)
    {
        WARN("Mode can be passed if and only if Windowed is FALSE.\n");
        return D3DERR_INVALIDCALL;
    }

    if (mode && (mode->Width != present_parameters->BackBufferWidth
            || mode->Height != present_parameters->BackBufferHeight))
    {
        WARN("Mode and back buffer mismatch (mode %ux%u, backbuffer %ux%u).\n",
                mode->Width, mode->Height,
                present_parameters->BackBufferWidth, present_parameters->BackBufferHeight);
        return D3DERR_INVALIDCALL;
    }

    return d3d9_device_reset(device, present_parameters, mode);
}

static HRESULT WINAPI d3d9_device_GetDisplayModeEx(IDirect3DDevice9Ex *iface,
        UINT swapchain_idx, D3DDISPLAYMODEEX *mode, D3DDISPLAYROTATION *rotation)
{
    struct d3d9_device *device = impl_from_IDirect3DDevice9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, swapchain_idx %u, mode %p, rotation %p.\n",
            iface, swapchain_idx, mode, rotation);

    if (mode->Size != sizeof(*mode))
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_device_get_display_mode(device->wined3d_device, swapchain_idx, &wined3d_mode,
            (enum wined3d_display_rotation *)rotation);
    wined3d_mutex_unlock();

    if (SUCCEEDED(hr))
    {
        mode->Width = wined3d_mode.width;
        mode->Height = wined3d_mode.height;
        mode->RefreshRate = wined3d_mode.refresh_rate;
        mode->Format = d3dformat_from_wined3dformat(wined3d_mode.format_id);
        mode->ScanLineOrdering = wined3d_mode.scanline_ordering;
    }

    return hr;
}

static const struct IDirect3DDevice9ExVtbl d3d9_device_vtbl =
{
    /* IUnknown */
    d3d9_device_QueryInterface,
    d3d9_device_AddRef,
    d3d9_device_Release,
    /* IDirect3DDevice9 */
    d3d9_device_TestCooperativeLevel,
    d3d9_device_GetAvailableTextureMem,
    d3d9_device_EvictManagedResources,
    d3d9_device_GetDirect3D,
    d3d9_device_GetDeviceCaps,
    d3d9_device_GetDisplayMode,
    d3d9_device_GetCreationParameters,
    d3d9_device_SetCursorProperties,
    d3d9_device_SetCursorPosition,
    d3d9_device_ShowCursor,
    d3d9_device_CreateAdditionalSwapChain,
    d3d9_device_GetSwapChain,
    d3d9_device_GetNumberOfSwapChains,
    d3d9_device_Reset,
    d3d9_device_Present,
    d3d9_device_GetBackBuffer,
    d3d9_device_GetRasterStatus,
    d3d9_device_SetDialogBoxMode,
    d3d9_device_SetGammaRamp,
    d3d9_device_GetGammaRamp,
    d3d9_device_CreateTexture,
    d3d9_device_CreateVolumeTexture,
    d3d9_device_CreateCubeTexture,
    d3d9_device_CreateVertexBuffer,
    d3d9_device_CreateIndexBuffer,
    d3d9_device_CreateRenderTarget,
    d3d9_device_CreateDepthStencilSurface,
    d3d9_device_UpdateSurface,
    d3d9_device_UpdateTexture,
    d3d9_device_GetRenderTargetData,
    d3d9_device_GetFrontBufferData,
    d3d9_device_StretchRect,
    d3d9_device_ColorFill,
    d3d9_device_CreateOffscreenPlainSurface,
    d3d9_device_SetRenderTarget,
    d3d9_device_GetRenderTarget,
    d3d9_device_SetDepthStencilSurface,
    d3d9_device_GetDepthStencilSurface,
    d3d9_device_BeginScene,
    d3d9_device_EndScene,
    d3d9_device_Clear,
    d3d9_device_SetTransform,
    d3d9_device_GetTransform,
    d3d9_device_MultiplyTransform,
    d3d9_device_SetViewport,
    d3d9_device_GetViewport,
    d3d9_device_SetMaterial,
    d3d9_device_GetMaterial,
    d3d9_device_SetLight,
    d3d9_device_GetLight,
    d3d9_device_LightEnable,
    d3d9_device_GetLightEnable,
    d3d9_device_SetClipPlane,
    d3d9_device_GetClipPlane,
    d3d9_device_SetRenderState,
    d3d9_device_GetRenderState,
    d3d9_device_CreateStateBlock,
    d3d9_device_BeginStateBlock,
    d3d9_device_EndStateBlock,
    d3d9_device_SetClipStatus,
    d3d9_device_GetClipStatus,
    d3d9_device_GetTexture,
    d3d9_device_SetTexture,
    d3d9_device_GetTextureStageState,
    d3d9_device_SetTextureStageState,
    d3d9_device_GetSamplerState,
    d3d9_device_SetSamplerState,
    d3d9_device_ValidateDevice,
    d3d9_device_SetPaletteEntries,
    d3d9_device_GetPaletteEntries,
    d3d9_device_SetCurrentTexturePalette,
    d3d9_device_GetCurrentTexturePalette,
    d3d9_device_SetScissorRect,
    d3d9_device_GetScissorRect,
    d3d9_device_SetSoftwareVertexProcessing,
    d3d9_device_GetSoftwareVertexProcessing,
    d3d9_device_SetNPatchMode,
    d3d9_device_GetNPatchMode,
    d3d9_device_DrawPrimitive,
    d3d9_device_DrawIndexedPrimitive,
    d3d9_device_DrawPrimitiveUP,
    d3d9_device_DrawIndexedPrimitiveUP,
    d3d9_device_ProcessVertices,
    d3d9_device_CreateVertexDeclaration,
    d3d9_device_SetVertexDeclaration,
    d3d9_device_GetVertexDeclaration,
    d3d9_device_SetFVF,
    d3d9_device_GetFVF,
    d3d9_device_CreateVertexShader,
    d3d9_device_SetVertexShader,
    d3d9_device_GetVertexShader,
    d3d9_device_SetVertexShaderConstantF,
    d3d9_device_GetVertexShaderConstantF,
    d3d9_device_SetVertexShaderConstantI,
    d3d9_device_GetVertexShaderConstantI,
    d3d9_device_SetVertexShaderConstantB,
    d3d9_device_GetVertexShaderConstantB,
    d3d9_device_SetStreamSource,
    d3d9_device_GetStreamSource,
    d3d9_device_SetStreamSourceFreq,
    d3d9_device_GetStreamSourceFreq,
    d3d9_device_SetIndices,
    d3d9_device_GetIndices,
    d3d9_device_CreatePixelShader,
    d3d9_device_SetPixelShader,
    d3d9_device_GetPixelShader,
    d3d9_device_SetPixelShaderConstantF,
    d3d9_device_GetPixelShaderConstantF,
    d3d9_device_SetPixelShaderConstantI,
    d3d9_device_GetPixelShaderConstantI,
    d3d9_device_SetPixelShaderConstantB,
    d3d9_device_GetPixelShaderConstantB,
    d3d9_device_DrawRectPatch,
    d3d9_device_DrawTriPatch,
    d3d9_device_DeletePatch,
    d3d9_device_CreateQuery,
    /* IDirect3DDevice9Ex */
    d3d9_device_SetConvolutionMonoKernel,
    d3d9_device_ComposeRects,
    d3d9_device_PresentEx,
    d3d9_device_GetGPUThreadPriority,
    d3d9_device_SetGPUThreadPriority,
    d3d9_device_WaitForVBlank,
    d3d9_device_CheckResourceResidency,
    d3d9_device_SetMaximumFrameLatency,
    d3d9_device_GetMaximumFrameLatency,
    d3d9_device_CheckDeviceState,
    d3d9_device_CreateRenderTargetEx,
    d3d9_device_CreateOffscreenPlainSurfaceEx,
    d3d9_device_CreateDepthStencilSurfaceEx,
    d3d9_device_ResetEx,
    d3d9_device_GetDisplayModeEx,
};

static inline struct d3d9_device *device_from_device_parent(struct wined3d_device_parent *device_parent)
{
    return CONTAINING_RECORD(device_parent, struct d3d9_device, device_parent);
}

static void CDECL device_parent_wined3d_device_created(struct wined3d_device_parent *device_parent,
        struct wined3d_device *device)
{
    TRACE("device_parent %p, device %p.\n", device_parent, device);
}

static void CDECL device_parent_mode_changed(struct wined3d_device_parent *device_parent)
{
    TRACE("device_parent %p.\n", device_parent);
}

static void CDECL device_parent_activate(struct wined3d_device_parent *device_parent, BOOL activate)
{
    struct d3d9_device *device = device_from_device_parent(device_parent);

    TRACE("device_parent %p, activate %#x.\n", device_parent, activate);

    if (!device->d3d_parent)
        return;

    if (!activate)
        InterlockedCompareExchange(&device->device_state, D3D9_DEVICE_STATE_LOST, D3D9_DEVICE_STATE_OK);
    else if (device->d3d_parent->extended)
        InterlockedCompareExchange(&device->device_state, D3D9_DEVICE_STATE_OK, D3D9_DEVICE_STATE_LOST);
    else
        InterlockedCompareExchange(&device->device_state, D3D9_DEVICE_STATE_NOT_RESET, D3D9_DEVICE_STATE_LOST);
}

static HRESULT CDECL device_parent_surface_created(struct wined3d_device_parent *device_parent,
        struct wined3d_texture *wined3d_texture, unsigned int sub_resource_idx,
        void **parent, const struct wined3d_parent_ops **parent_ops)
{
    struct d3d9_surface *d3d_surface;

    TRACE("device_parent %p, wined3d_texture %p, sub_resource_idx %u, parent %p, parent_ops %p.\n",
            device_parent, wined3d_texture, sub_resource_idx, parent, parent_ops);

    if (!(d3d_surface = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*d3d_surface))))
        return E_OUTOFMEMORY;

    surface_init(d3d_surface, wined3d_texture, sub_resource_idx, parent_ops);
    *parent = d3d_surface;
    TRACE("Created surface %p.\n", d3d_surface);

    return D3D_OK;
}

static HRESULT CDECL device_parent_volume_created(struct wined3d_device_parent *device_parent,
        struct wined3d_texture *wined3d_texture, unsigned int sub_resource_idx,
        void **parent, const struct wined3d_parent_ops **parent_ops)
{
    struct d3d9_volume *d3d_volume;

    TRACE("device_parent %p, texture %p, sub_resource_idx %u, parent %p, parent_ops %p.\n",
            device_parent, wined3d_texture, sub_resource_idx, parent, parent_ops);

    if (!(d3d_volume = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*d3d_volume))))
        return E_OUTOFMEMORY;

    volume_init(d3d_volume, wined3d_texture, sub_resource_idx, parent_ops);
    *parent = d3d_volume;
    TRACE("Created volume %p.\n", d3d_volume);

    return D3D_OK;
}

static HRESULT CDECL device_parent_create_swapchain_texture(struct wined3d_device_parent *device_parent,
        void *container_parent, const struct wined3d_resource_desc *desc, DWORD texture_flags,
        struct wined3d_texture **texture)
{
    struct d3d9_device *device = device_from_device_parent(device_parent);
    struct d3d9_surface *d3d_surface;
    HRESULT hr;

    TRACE("device_parent %p, container_parent %p, desc %p, texture flags %#x, texture %p.\n",
            device_parent, container_parent, desc, texture_flags, texture);

    if (container_parent == device_parent)
        container_parent = &device->IDirect3DDevice9Ex_iface;

    if (FAILED(hr = wined3d_texture_create(device->wined3d_device, desc, 1, 1,
            texture_flags | WINED3D_TEXTURE_CREATE_MAPPABLE, NULL, container_parent,
            &d3d9_null_wined3d_parent_ops, texture)))
    {
        WARN("Failed to create texture, hr %#x.\n", hr);
        return hr;
    }

    d3d_surface = wined3d_texture_get_sub_resource_parent(*texture, 0);
    d3d_surface->parent_device = &device->IDirect3DDevice9Ex_iface;

    return hr;
}

static HRESULT CDECL device_parent_create_swapchain(struct wined3d_device_parent *device_parent,
        struct wined3d_swapchain_desc *desc, struct wined3d_swapchain **swapchain)
{
    struct d3d9_device *device = device_from_device_parent(device_parent);
    struct d3d9_swapchain *d3d_swapchain;
    HRESULT hr;

    TRACE("device_parent %p, desc %p, swapchain %p\n", device_parent, desc, swapchain);

    hr = d3d9_swapchain_create(device, desc, &d3d_swapchain);
    if (FAILED(hr))
    {
        WARN("Failed to create swapchain, hr %#x.\n", hr);
        *swapchain = NULL;
        return hr;
    }

    *swapchain = d3d_swapchain->wined3d_swapchain;
    wined3d_swapchain_incref(*swapchain);
    IDirect3DSwapChain9Ex_Release(&d3d_swapchain->IDirect3DSwapChain9Ex_iface);

    return hr;
}

static const struct wined3d_device_parent_ops d3d9_wined3d_device_parent_ops =
{
    device_parent_wined3d_device_created,
    device_parent_mode_changed,
    device_parent_activate,
    device_parent_surface_created,
    device_parent_volume_created,
    device_parent_create_swapchain_texture,
    device_parent_create_swapchain,
};

static void setup_fpu(void)
{
#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
    WORD cw;
    __asm__ volatile ("fnstcw %0" : "=m" (cw));
    cw = (cw & ~0xf3f) | 0x3f;
    __asm__ volatile ("fldcw %0" : : "m" (cw));
#elif defined(__i386__) && defined(_MSC_VER)
    WORD cw;
    __asm fnstcw cw;
    cw = (cw & ~0xf3f) | 0x3f;
    __asm fldcw cw;
#else
    FIXME("FPU setup not implemented for this platform.\n");
#endif
}

HRESULT device_init(struct d3d9_device *device, struct d3d9 *parent, struct wined3d *wined3d,
        UINT adapter, D3DDEVTYPE device_type, HWND focus_window, DWORD flags,
        D3DPRESENT_PARAMETERS *parameters, D3DDISPLAYMODEEX *mode)
{
    struct wined3d_swapchain_desc *swapchain_desc;
    UINT i, count = 1;
    HRESULT hr;

    if (mode)
        FIXME("Ignoring display mode.\n");

    device->IDirect3DDevice9Ex_iface.lpVtbl = &d3d9_device_vtbl;
    device->device_parent.ops = &d3d9_wined3d_device_parent_ops;
    device->refcount = 1;

    if (!(flags & D3DCREATE_FPU_PRESERVE)) setup_fpu();

    wined3d_mutex_lock();
    hr = wined3d_device_create(wined3d, adapter, device_type, focus_window, flags, 4,
            &device->device_parent, &device->wined3d_device);
    if (FAILED(hr))
    {
        WARN("Failed to create wined3d device, hr %#x.\n", hr);
        wined3d_mutex_unlock();
        return hr;
    }

    if (flags & D3DCREATE_ADAPTERGROUP_DEVICE)
    {
        WINED3DCAPS caps;

        wined3d_get_device_caps(wined3d, adapter, device_type, &caps);
        count = caps.NumberOfAdaptersInGroup;
    }

    if (flags & D3DCREATE_MULTITHREADED)
        wined3d_device_set_multithreaded(device->wined3d_device);

    if (!parameters->Windowed)
    {
        if (!focus_window)
            focus_window = parameters->hDeviceWindow;
        if (FAILED(hr = wined3d_device_acquire_focus_window(device->wined3d_device, focus_window)))
        {
            ERR("Failed to acquire focus window, hr %#x.\n", hr);
            wined3d_device_decref(device->wined3d_device);
            wined3d_mutex_unlock();
            return hr;
        }

        for (i = 0; i < count; ++i)
        {
            HWND device_window = parameters[i].hDeviceWindow;

            if (!device_window) device_window = focus_window;
            wined3d_device_setup_fullscreen_window(device->wined3d_device, device_window,
                    parameters[i].BackBufferWidth,
                    parameters[i].BackBufferHeight);
        }
    }

    swapchain_desc = HeapAlloc(GetProcessHeap(), 0, sizeof(*swapchain_desc) * count);
    if (!swapchain_desc)
    {
        ERR("Failed to allocate wined3d parameters.\n");
        wined3d_device_release_focus_window(device->wined3d_device);
        wined3d_device_decref(device->wined3d_device);
        wined3d_mutex_unlock();
        return E_OUTOFMEMORY;
    }

    for (i = 0; i < count; ++i)
    {
        if (!wined3d_swapchain_desc_from_present_parameters(&swapchain_desc[i], &parameters[i],
                parent->extended))
        {
            wined3d_device_release_focus_window(device->wined3d_device);
            wined3d_device_decref(device->wined3d_device);
            HeapFree(GetProcessHeap(), 0, swapchain_desc);
            wined3d_mutex_unlock();
            return D3DERR_INVALIDCALL;
        }
    }

    hr = wined3d_device_init_3d(device->wined3d_device, swapchain_desc);
    if (FAILED(hr))
    {
        WARN("Failed to initialize 3D, hr %#x.\n", hr);
        wined3d_device_release_focus_window(device->wined3d_device);
        HeapFree(GetProcessHeap(), 0, swapchain_desc);
        wined3d_device_decref(device->wined3d_device);
        wined3d_mutex_unlock();
        return hr;
    }

    if (FAILED(hr = d3d9_device_get_swapchains(device)))
    {
        wined3d_device_uninit_3d(device->wined3d_device);
        wined3d_device_release_focus_window(device->wined3d_device);
        wined3d_device_decref(device->wined3d_device);
        wined3d_mutex_unlock();
        return E_OUTOFMEMORY;
    }

    for (i = 0; i < count; ++i)
    {
        present_parameters_from_wined3d_swapchain_desc(&parameters[i], &swapchain_desc[i]);
    }

    wined3d_mutex_unlock();

    HeapFree(GetProcessHeap(), 0, swapchain_desc);

    /* Initialize the converted declaration array. This creates a valid pointer
     * and when adding decls HeapReAlloc() can be used without further checking. */
    device->fvf_decls = HeapAlloc(GetProcessHeap(), 0, 0);
    if (!device->fvf_decls)
    {
        ERR("Failed to allocate FVF vertex declaration map memory.\n");
        wined3d_mutex_lock();
        HeapFree(GetProcessHeap(), 0, device->implicit_swapchains);
        wined3d_device_uninit_3d(device->wined3d_device);
        wined3d_device_release_focus_window(device->wined3d_device);
        wined3d_device_decref(device->wined3d_device);
        wined3d_mutex_unlock();
        return E_OUTOFMEMORY;
    }

    IDirect3D9Ex_AddRef(&parent->IDirect3D9Ex_iface);
    device->d3d_parent = parent;

    return D3D_OK;
}
