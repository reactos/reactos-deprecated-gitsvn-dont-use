/*
 * IDirect3D9 implementation
 *
 * Copyright 2002 Jason Edmeades
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

static inline struct d3d9 *impl_from_IDirect3D9Ex(IDirect3D9Ex *iface)
{
    return CONTAINING_RECORD(iface, struct d3d9, IDirect3D9Ex_iface);
}

static HRESULT WINAPI d3d9_QueryInterface(IDirect3D9Ex *iface, REFIID riid, void **out)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);

    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_IDirect3D9)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IDirect3D9Ex_AddRef(&d3d9->IDirect3D9Ex_iface);
        *out = &d3d9->IDirect3D9Ex_iface;
        return S_OK;
    }

    if (IsEqualGUID(riid, &IID_IDirect3D9Ex))
    {
        if (!d3d9->extended)
        {
            WARN("Application asks for IDirect3D9Ex, but this instance wasn't created with Direct3DCreate9Ex.\n");
            *out = NULL;
            return E_NOINTERFACE;
        }

        IDirect3D9Ex_AddRef(&d3d9->IDirect3D9Ex_iface);
        *out = &d3d9->IDirect3D9Ex_iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI d3d9_AddRef(IDirect3D9Ex *iface)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    ULONG refcount = InterlockedIncrement(&d3d9->refcount);

    TRACE("%p increasing refcount to %u.\n", iface, refcount);

    return refcount;
}

static ULONG WINAPI d3d9_Release(IDirect3D9Ex *iface)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    ULONG refcount = InterlockedDecrement(&d3d9->refcount);

    TRACE("%p decreasing refcount to %u.\n", iface, refcount);

    if (!refcount)
    {
        wined3d_mutex_lock();
        wined3d_decref(d3d9->wined3d);
        wined3d_mutex_unlock();

        HeapFree(GetProcessHeap(), 0, d3d9);
    }

    return refcount;
}

static HRESULT WINAPI d3d9_RegisterSoftwareDevice(IDirect3D9Ex *iface, void *init_function)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, init_function %p.\n", iface, init_function);

    wined3d_mutex_lock();
    hr = wined3d_register_software_device(d3d9->wined3d, init_function);
    wined3d_mutex_unlock();

    return hr;
}

static UINT WINAPI d3d9_GetAdapterCount(IDirect3D9Ex *iface)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    UINT ret;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    ret = wined3d_get_adapter_count(d3d9->wined3d);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_GetAdapterIdentifier(IDirect3D9Ex *iface, UINT adapter,
        DWORD flags, D3DADAPTER_IDENTIFIER9 *identifier)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_adapter_identifier adapter_id;
    HRESULT hr;

    TRACE("iface %p, adapter %u, flags %#x, identifier %p.\n",
            iface, adapter, flags, identifier);

    adapter_id.driver = identifier->Driver;
    adapter_id.driver_size = sizeof(identifier->Driver);
    adapter_id.description = identifier->Description;
    adapter_id.description_size = sizeof(identifier->Description);
    adapter_id.device_name = identifier->DeviceName;
    adapter_id.device_name_size = sizeof(identifier->DeviceName);

    wined3d_mutex_lock();
    hr = wined3d_get_adapter_identifier(d3d9->wined3d, adapter, flags, &adapter_id);
    wined3d_mutex_unlock();

    identifier->DriverVersion = adapter_id.driver_version;
    identifier->VendorId = adapter_id.vendor_id;
    identifier->DeviceId = adapter_id.device_id;
    identifier->SubSysId = adapter_id.subsystem_id;
    identifier->Revision = adapter_id.revision;
    memcpy(&identifier->DeviceIdentifier, &adapter_id.device_identifier, sizeof(identifier->DeviceIdentifier));
    identifier->WHQLLevel = adapter_id.whql_level;

    return hr;
}

static UINT WINAPI d3d9_GetAdapterModeCount(IDirect3D9Ex *iface, UINT adapter, D3DFORMAT format)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    UINT ret;

    TRACE("iface %p, adapter %u, format %#x.\n", iface, adapter, format);

    /* Others than that not supported by d3d9, but reported by wined3d for ddraw. Filter them out. */
    if (format != D3DFMT_X8R8G8B8 && format != D3DFMT_R5G6B5)
        return 0;

    wined3d_mutex_lock();
    ret = wined3d_get_adapter_mode_count(d3d9->wined3d, adapter,
            wined3dformat_from_d3dformat(format), WINED3D_SCANLINE_ORDERING_UNKNOWN);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_EnumAdapterModes(IDirect3D9Ex *iface, UINT adapter,
        D3DFORMAT format, UINT mode_idx, D3DDISPLAYMODE *mode)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, adapter %u, format %#x, mode_idx %u, mode %p.\n",
            iface, adapter, format, mode_idx, mode);

    if (format != D3DFMT_X8R8G8B8 && format != D3DFMT_R5G6B5)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_enum_adapter_modes(d3d9->wined3d, adapter, wined3dformat_from_d3dformat(format),
            WINED3D_SCANLINE_ORDERING_UNKNOWN, mode_idx, &wined3d_mode);
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

static HRESULT WINAPI d3d9_GetAdapterDisplayMode(IDirect3D9Ex *iface, UINT adapter, D3DDISPLAYMODE *mode)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, adapter %u, mode %p.\n", iface, adapter, mode);

    wined3d_mutex_lock();
    hr = wined3d_get_adapter_display_mode(d3d9->wined3d, adapter, &wined3d_mode, NULL);
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

static HRESULT WINAPI d3d9_CheckDeviceType(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT display_format, D3DFORMAT backbuffer_format, BOOL windowed)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, display_format %#x, backbuffer_format %#x, windowed %#x.\n",
            iface, adapter, device_type, display_format, backbuffer_format, windowed);

    /* Others than that not supported by d3d9, but reported by wined3d for ddraw. Filter them out. */
    if (!windowed && display_format != D3DFMT_X8R8G8B8 && display_format != D3DFMT_R5G6B5)
        return WINED3DERR_NOTAVAILABLE;

    wined3d_mutex_lock();
    hr = wined3d_check_device_type(d3d9->wined3d, adapter, device_type, wined3dformat_from_d3dformat(display_format),
            wined3dformat_from_d3dformat(backbuffer_format), windowed);
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_CheckDeviceFormat(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, DWORD usage, D3DRESOURCETYPE resource_type, D3DFORMAT format)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    enum wined3d_resource_type wined3d_rtype;
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, adapter_format %#x, usage %#x, resource_type %#x, format %#x.\n",
            iface, adapter, device_type, adapter_format, usage, resource_type, format);

    usage = usage & (WINED3DUSAGE_MASK | WINED3DUSAGE_QUERY_MASK);
    switch (resource_type)
    {
        case D3DRTYPE_CUBETEXTURE:
            usage |= WINED3DUSAGE_LEGACY_CUBEMAP;
        case D3DRTYPE_TEXTURE:
            usage |= WINED3DUSAGE_TEXTURE;
        case D3DRTYPE_SURFACE:
            wined3d_rtype = WINED3D_RTYPE_TEXTURE_2D;
            break;

        case D3DRTYPE_VOLUMETEXTURE:
        case D3DRTYPE_VOLUME:
            usage |= WINED3DUSAGE_TEXTURE;
            wined3d_rtype = WINED3D_RTYPE_TEXTURE_3D;
            break;

        case D3DRTYPE_VERTEXBUFFER:
        case D3DRTYPE_INDEXBUFFER:
            wined3d_rtype = WINED3D_RTYPE_BUFFER;
            break;

        default:
            FIXME("Unhandled resource type %#x.\n", resource_type);
            return WINED3DERR_INVALIDCALL;
    }

    wined3d_mutex_lock();
    hr = wined3d_check_device_format(d3d9->wined3d, adapter, device_type, wined3dformat_from_d3dformat(adapter_format),
            usage, wined3d_rtype, wined3dformat_from_d3dformat(format));
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_CheckDeviceMultiSampleType(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT format, BOOL windowed, D3DMULTISAMPLE_TYPE multisample_type, DWORD *levels)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, format %#x, windowed %#x, multisample_type %#x, levels %p.\n",
            iface, adapter, device_type, format, windowed, multisample_type, levels);

    if (multisample_type > D3DMULTISAMPLE_16_SAMPLES)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_check_device_multisample_type(d3d9->wined3d, adapter, device_type,
            wined3dformat_from_d3dformat(format), windowed, multisample_type, levels);
    wined3d_mutex_unlock();
    if (hr == WINED3DERR_NOTAVAILABLE && levels)
        *levels = 1;

    return hr;
}

static HRESULT WINAPI d3d9_CheckDepthStencilMatch(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type,
        D3DFORMAT adapter_format, D3DFORMAT rt_format, D3DFORMAT ds_format)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, adapter_format %#x, rt_format %#x, ds_format %#x.\n",
            iface, adapter, device_type, adapter_format, rt_format, ds_format);

    wined3d_mutex_lock();
    hr = wined3d_check_depth_stencil_match(d3d9->wined3d, adapter, device_type,
            wined3dformat_from_d3dformat(adapter_format), wined3dformat_from_d3dformat(rt_format),
            wined3dformat_from_d3dformat(ds_format));
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_CheckDeviceFormatConversion(IDirect3D9Ex *iface, UINT adapter,
        D3DDEVTYPE device_type, D3DFORMAT src_format, D3DFORMAT dst_format)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, src_format %#x, dst_format %#x.\n",
            iface, adapter, device_type, src_format, dst_format);

    wined3d_mutex_lock();
    hr = wined3d_check_device_format_conversion(d3d9->wined3d, adapter, device_type,
            wined3dformat_from_d3dformat(src_format), wined3dformat_from_d3dformat(dst_format));
    wined3d_mutex_unlock();

    return hr;
}

static HRESULT WINAPI d3d9_GetDeviceCaps(IDirect3D9Ex *iface, UINT adapter, D3DDEVTYPE device_type, D3DCAPS9 *caps)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    WINED3DCAPS wined3d_caps;
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, caps %p.\n", iface, adapter, device_type, caps);

    if (!caps)
        return D3DERR_INVALIDCALL;

    memset(caps, 0, sizeof(*caps));

    wined3d_mutex_lock();
    hr = wined3d_get_device_caps(d3d9->wined3d, adapter, device_type, &wined3d_caps);
    wined3d_mutex_unlock();

    d3dcaps_from_wined3dcaps(caps, &wined3d_caps);

    return hr;
}

static HMONITOR WINAPI d3d9_GetAdapterMonitor(IDirect3D9Ex *iface, UINT adapter)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_output_desc desc;
    HRESULT hr;

    TRACE("iface %p, adapter %u.\n", iface, adapter);

    wined3d_mutex_lock();
    hr = wined3d_get_output_desc(d3d9->wined3d, adapter, &desc);
    wined3d_mutex_unlock();

    if (FAILED(hr))
    {
        WARN("Failed to get output desc, hr %#x.\n", hr);
        return NULL;
    }

    return desc.monitor;
}

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_CreateDevice(IDirect3D9Ex *iface, UINT adapter,
        D3DDEVTYPE device_type, HWND focus_window, DWORD flags, D3DPRESENT_PARAMETERS *parameters,
        IDirect3DDevice9 **device)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct d3d9_device *object;
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, focus_window %p, flags %#x, parameters %p, device %p.\n",
            iface, adapter, device_type, focus_window, flags, parameters, device);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return E_OUTOFMEMORY;

    hr = device_init(object, d3d9, d3d9->wined3d, adapter, device_type, focus_window, flags, parameters, NULL);
    if (FAILED(hr))
    {
        WARN("Failed to initialize device, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created device %p.\n", object);
    *device = (IDirect3DDevice9 *)&object->IDirect3DDevice9Ex_iface;

    return D3D_OK;
}

static UINT WINAPI d3d9_GetAdapterModeCountEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    UINT ret;

    TRACE("iface %p, adapter %u, filter %p.\n", iface, adapter, filter);

    if (filter->Format != D3DFMT_X8R8G8B8 && filter->Format != D3DFMT_R5G6B5)
        return 0;

    wined3d_mutex_lock();
    ret = wined3d_get_adapter_mode_count(d3d9->wined3d, adapter,
            wined3dformat_from_d3dformat(filter->Format), filter->ScanLineOrdering);
    wined3d_mutex_unlock();

    return ret;
}

static HRESULT WINAPI d3d9_EnumAdapterModesEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter, UINT mode_idx, D3DDISPLAYMODEEX *mode)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, adapter %u, filter %p, mode_idx %u, mode %p.\n",
            iface, adapter, filter, mode_idx, mode);

    if (filter->Format != D3DFMT_X8R8G8B8 && filter->Format != D3DFMT_R5G6B5)
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_enum_adapter_modes(d3d9->wined3d, adapter, wined3dformat_from_d3dformat(filter->Format),
            filter->ScanLineOrdering, mode_idx, &wined3d_mode);
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

static HRESULT WINAPI d3d9_GetAdapterDisplayModeEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDISPLAYMODEEX *mode, D3DDISPLAYROTATION *rotation)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_display_mode wined3d_mode;
    HRESULT hr;

    TRACE("iface %p, adapter %u, mode %p, rotation %p.\n",
            iface, adapter, mode, rotation);

    if (mode->Size != sizeof(*mode))
        return D3DERR_INVALIDCALL;

    wined3d_mutex_lock();
    hr = wined3d_get_adapter_display_mode(d3d9->wined3d, adapter, &wined3d_mode,
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

static HRESULT WINAPI DECLSPEC_HOTPATCH d3d9_CreateDeviceEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDEVTYPE device_type, HWND focus_window, DWORD flags,
        D3DPRESENT_PARAMETERS *parameters, D3DDISPLAYMODEEX *mode, IDirect3DDevice9Ex **device)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct d3d9_device *object;
    HRESULT hr;

    TRACE("iface %p, adapter %u, device_type %#x, focus_window %p, flags %#x, parameters %p, mode %p, device %p.\n",
            iface, adapter, device_type, focus_window, flags, parameters, mode, device);

    object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object));
    if (!object)
        return E_OUTOFMEMORY;

    hr = device_init(object, d3d9, d3d9->wined3d, adapter, device_type, focus_window, flags, parameters, mode);
    if (FAILED(hr))
    {
        WARN("Failed to initialize device, hr %#x.\n", hr);
        HeapFree(GetProcessHeap(), 0, object);
        return hr;
    }

    TRACE("Created device %p.\n", object);
    *device = &object->IDirect3DDevice9Ex_iface;

    return D3D_OK;
}

static HRESULT WINAPI d3d9_GetAdapterLUID(IDirect3D9Ex *iface, UINT adapter, LUID *luid)
{
    struct d3d9 *d3d9 = impl_from_IDirect3D9Ex(iface);
    struct wined3d_adapter_identifier adapter_id;
    HRESULT hr;

    TRACE("iface %p, adapter %u, luid %p.\n", iface, adapter, luid);

    adapter_id.driver_size = 0;
    adapter_id.description_size = 0;
    adapter_id.device_name_size = 0;

    wined3d_mutex_lock();
    hr = wined3d_get_adapter_identifier(d3d9->wined3d, adapter, 0, &adapter_id);
    wined3d_mutex_unlock();

    memcpy(luid, &adapter_id.adapter_luid, sizeof(*luid));

    return hr;
}

static const struct IDirect3D9ExVtbl d3d9_vtbl =
{
    /* IUnknown */
    d3d9_QueryInterface,
    d3d9_AddRef,
    d3d9_Release,
    /* IDirect3D9 */
    d3d9_RegisterSoftwareDevice,
    d3d9_GetAdapterCount,
    d3d9_GetAdapterIdentifier,
    d3d9_GetAdapterModeCount,
    d3d9_EnumAdapterModes,
    d3d9_GetAdapterDisplayMode,
    d3d9_CheckDeviceType,
    d3d9_CheckDeviceFormat,
    d3d9_CheckDeviceMultiSampleType,
    d3d9_CheckDepthStencilMatch,
    d3d9_CheckDeviceFormatConversion,
    d3d9_GetDeviceCaps,
    d3d9_GetAdapterMonitor,
    d3d9_CreateDevice,
    /* IDirect3D9Ex */
    d3d9_GetAdapterModeCountEx,
    d3d9_EnumAdapterModesEx,
    d3d9_GetAdapterDisplayModeEx,
    d3d9_CreateDeviceEx,
    d3d9_GetAdapterLUID,
};

BOOL d3d9_init(struct d3d9 *d3d9, BOOL extended)
{
    DWORD flags = WINED3D_PRESENT_CONVERSION | WINED3D_HANDLE_RESTORE | WINED3D_PIXEL_CENTER_INTEGER
            | WINED3D_SRGB_READ_WRITE_CONTROL | WINED3D_LEGACY_UNBOUND_RESOURCE_COLOR
            | WINED3D_NO_PRIMITIVE_RESTART | WINED3D_LEGACY_CUBEMAP_FILTERING;

    if (!extended)
        flags |= WINED3D_VIDMEM_ACCOUNTING;
    else
        flags |= WINED3D_RESTORE_MODE_ON_ACTIVATE;

    d3d9->IDirect3D9Ex_iface.lpVtbl = &d3d9_vtbl;
    d3d9->refcount = 1;

    wined3d_mutex_lock();
    d3d9->wined3d = wined3d_create(flags);
    wined3d_mutex_unlock();
    if (!d3d9->wined3d)
        return FALSE;
    d3d9->extended = extended;

    return TRUE;
}
