/*
 * Copyright 2002-2004 Jason Edmeades
 * Copyright 2002-2004 Raphael Junqueira
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

static inline struct d3d9_vertexbuffer *impl_from_IDirect3DVertexBuffer9(IDirect3DVertexBuffer9 *iface)
{
    return CONTAINING_RECORD(iface, struct d3d9_vertexbuffer, IDirect3DVertexBuffer9_iface);
}

static HRESULT WINAPI d3d9_vertexbuffer_QueryInterface(IDirect3DVertexBuffer9 *iface, REFIID riid, void **out)
{
    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_IDirect3DVertexBuffer9)
            || IsEqualGUID(riid, &IID_IDirect3DResource9)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IDirect3DVertexBuffer9_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI d3d9_vertexbuffer_AddRef(IDirect3DVertexBuffer9 *iface)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    ULONG refcount = InterlockedIncrement(&buffer->resource.refcount);

    TRACE("%p increasing refcount to %u.\n", iface, refcount);

    if (refcount == 1)
    {
        IDirect3DDevice9Ex_AddRef(buffer->parent_device);
        wined3d_mutex_lock();
        wined3d_buffer_incref(buffer->wined3d_buffer);
        wined3d_mutex_unlock();
    }

    return refcount;
}

static ULONG WINAPI d3d9_vertexbuffer_Release(IDirect3DVertexBuffer9 *iface)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    ULONG refcount = InterlockedDecrement(&buffer->resource.refcount);

    TRACE("%p decreasing refcount to %u.\n", iface, refcount);

    if (!refcount)
    {
        IDirect3DDevice9Ex *device = buffer->parent_device;

        wined3d_mutex_lock();
        wined3d_buffer_decref(buffer->wined3d_buffer);
        wined3d_mutex_unlock();

        /* Release the device last, as it may cause the device to be destroyed. */
        IDirect3DDevice9Ex_Release(device);
    }

    return refcount;
}

static HRESULT WINAPI d3d9_vertexbuffer_GetDevice(IDirect3DVertexBuffer9 *iface, IDirect3DDevice9 **device)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);

    TRACE("iface %p, device %p.\n", iface, device);

    *device = (IDirect3DDevice9 *)buffer->parent_device;
    IDirect3DDevice9_AddRef(*device);

    TRACE("Returning device %p.\n", *device);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_vertexbuffer_SetPrivateData(IDirect3DVertexBuffer9 *iface,
        REFGUID guid, const void *data, DWORD data_size, DWORD flags)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    TRACE("iface %p, guid %s, data %p, data_size %u, flags %#x.\n",
            iface, debugstr_guid(guid), data, data_size, flags);

    return d3d9_resource_set_private_data(&buffer->resource, guid, data, data_size, flags);
}

static HRESULT WINAPI d3d9_vertexbuffer_GetPrivateData(IDirect3DVertexBuffer9 *iface,
        REFGUID guid, void *data, DWORD *data_size)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    TRACE("iface %p, guid %s, data %p, data_size %p.\n",
            iface, debugstr_guid(guid), data, data_size);

    return d3d9_resource_get_private_data(&buffer->resource, guid, data, data_size);
}

static HRESULT WINAPI d3d9_vertexbuffer_FreePrivateData(IDirect3DVertexBuffer9 *iface, REFGUID guid)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    TRACE("iface %p, guid %s.\n", iface, debugstr_guid(guid));

    return d3d9_resource_free_private_data(&buffer->resource, guid);
}

static DWORD WINAPI d3d9_vertexbuffer_SetPriority(IDirect3DVertexBuffer9 *iface, DWORD priority)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    struct wined3d_resource *resource;
    DWORD previous;

    TRACE("iface %p, priority %u.\n", iface, priority);

    wined3d_mutex_lock();
    resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    previous = wined3d_resource_set_priority(resource, priority);
    wined3d_mutex_unlock();

    return previous;
}

static DWORD WINAPI d3d9_vertexbuffer_GetPriority(IDirect3DVertexBuffer9 *iface)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    const struct wined3d_resource *resource;
    DWORD priority;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    priority = wined3d_resource_get_priority(resource);
    wined3d_mutex_unlock();

    return priority;
}

static void WINAPI d3d9_vertexbuffer_PreLoad(IDirect3DVertexBuffer9 *iface)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_resource_preload(wined3d_buffer_get_resource(buffer->wined3d_buffer));
    wined3d_mutex_unlock();
}

static D3DRESOURCETYPE WINAPI d3d9_vertexbuffer_GetType(IDirect3DVertexBuffer9 *iface)
{
    TRACE("iface %p.\n", iface);

    return D3DRTYPE_VERTEXBUFFER;
}

static HRESULT WINAPI d3d9_vertexbuffer_Lock(IDirect3DVertexBuffer9 *iface, UINT offset, UINT size,
        void **data, DWORD flags)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    struct wined3d_map_desc wined3d_map_desc;
    struct wined3d_box wined3d_box = {0};
    HRESULT hr;

    TRACE("iface %p, offset %u, size %u, data %p, flags %#x.\n",
            iface, offset, size, data, flags);

    wined3d_box.left = offset;
    wined3d_box.right = offset + size;
    wined3d_mutex_lock();
    hr = wined3d_resource_map(wined3d_buffer_get_resource(buffer->wined3d_buffer),
            0, &wined3d_map_desc, &wined3d_box, flags);
    wined3d_mutex_unlock();
    *data = wined3d_map_desc.data;

    return hr;
}

static HRESULT WINAPI d3d9_vertexbuffer_Unlock(IDirect3DVertexBuffer9 *iface)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_resource_unmap(wined3d_buffer_get_resource(buffer->wined3d_buffer), 0);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_vertexbuffer_GetDesc(IDirect3DVertexBuffer9 *iface,
        D3DVERTEXBUFFER_DESC *desc)
{
    struct d3d9_vertexbuffer *buffer = impl_from_IDirect3DVertexBuffer9(iface);
    struct wined3d_resource_desc wined3d_desc;
    struct wined3d_resource *wined3d_resource;

    TRACE("iface %p, desc %p.\n", iface, desc);

    wined3d_mutex_lock();
    wined3d_resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    wined3d_resource_get_desc(wined3d_resource, &wined3d_desc);
    wined3d_mutex_unlock();

    desc->Format = D3DFMT_VERTEXDATA;
    desc->Usage = wined3d_desc.usage & WINED3DUSAGE_MASK;
    desc->Pool = wined3d_desc.pool;
    desc->Size = wined3d_desc.size;
    desc->Type = D3DRTYPE_VERTEXBUFFER;
    desc->FVF = buffer->fvf;

    return D3D_OK;
}

static const IDirect3DVertexBuffer9Vtbl d3d9_vertexbuffer_vtbl =
{
    /* IUnknown */
    d3d9_vertexbuffer_QueryInterface,
    d3d9_vertexbuffer_AddRef,
    d3d9_vertexbuffer_Release,
    /* IDirect3DResource9 */
    d3d9_vertexbuffer_GetDevice,
    d3d9_vertexbuffer_SetPrivateData,
    d3d9_vertexbuffer_GetPrivateData,
    d3d9_vertexbuffer_FreePrivateData,
    d3d9_vertexbuffer_SetPriority,
    d3d9_vertexbuffer_GetPriority,
    d3d9_vertexbuffer_PreLoad,
    d3d9_vertexbuffer_GetType,
    /* IDirect3DVertexBuffer9 */
    d3d9_vertexbuffer_Lock,
    d3d9_vertexbuffer_Unlock,
    d3d9_vertexbuffer_GetDesc,
};

static void STDMETHODCALLTYPE d3d9_vertexbuffer_wined3d_object_destroyed(void *parent)
{
    struct d3d9_vertexbuffer *buffer = parent;
    d3d9_resource_cleanup(&buffer->resource);
    HeapFree(GetProcessHeap(), 0, buffer);
}

static const struct wined3d_parent_ops d3d9_vertexbuffer_wined3d_parent_ops =
{
    d3d9_vertexbuffer_wined3d_object_destroyed,
};

HRESULT vertexbuffer_init(struct d3d9_vertexbuffer *buffer, struct d3d9_device *device,
        UINT size, UINT usage, DWORD fvf, D3DPOOL pool)
{
    HRESULT hr;

    buffer->IDirect3DVertexBuffer9_iface.lpVtbl = &d3d9_vertexbuffer_vtbl;
    buffer->fvf = fvf;
    d3d9_resource_init(&buffer->resource);

    wined3d_mutex_lock();
    hr = wined3d_buffer_create_vb(device->wined3d_device, size, usage & WINED3DUSAGE_MASK,
            (enum wined3d_pool)pool, buffer, &d3d9_vertexbuffer_wined3d_parent_ops, &buffer->wined3d_buffer);
    wined3d_mutex_unlock();
    if (FAILED(hr))
    {
        WARN("Failed to create wined3d buffer, hr %#x.\n", hr);
        return hr;
    }

    buffer->parent_device = &device->IDirect3DDevice9Ex_iface;
    IDirect3DDevice9Ex_AddRef(buffer->parent_device);

    return D3D_OK;
}

struct d3d9_vertexbuffer *unsafe_impl_from_IDirect3DVertexBuffer9(IDirect3DVertexBuffer9 *iface)
{
    if (!iface)
        return NULL;
    assert(iface->lpVtbl == &d3d9_vertexbuffer_vtbl);

    return impl_from_IDirect3DVertexBuffer9(iface);
}

static inline struct d3d9_indexbuffer *impl_from_IDirect3DIndexBuffer9(IDirect3DIndexBuffer9 *iface)
{
    return CONTAINING_RECORD(iface, struct d3d9_indexbuffer, IDirect3DIndexBuffer9_iface);
}

static HRESULT WINAPI d3d9_indexbuffer_QueryInterface(IDirect3DIndexBuffer9 *iface, REFIID riid, void **out)
{
    TRACE("iface %p, riid %s, out %p.\n", iface, debugstr_guid(riid), out);

    if (IsEqualGUID(riid, &IID_IDirect3DIndexBuffer9)
            || IsEqualGUID(riid, &IID_IDirect3DResource9)
            || IsEqualGUID(riid, &IID_IUnknown))
    {
        IDirect3DIndexBuffer9_AddRef(iface);
        *out = iface;
        return S_OK;
    }

    WARN("%s not implemented, returning E_NOINTERFACE.\n", debugstr_guid(riid));

    *out = NULL;
    return E_NOINTERFACE;
}

static ULONG WINAPI d3d9_indexbuffer_AddRef(IDirect3DIndexBuffer9 *iface)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    ULONG refcount = InterlockedIncrement(&buffer->resource.refcount);

    TRACE("%p increasing refcount to %u.\n", iface, refcount);

    if (refcount == 1)
    {
        IDirect3DDevice9Ex_AddRef(buffer->parent_device);
        wined3d_mutex_lock();
        wined3d_buffer_incref(buffer->wined3d_buffer);
        wined3d_mutex_unlock();
    }

    return refcount;
}

static ULONG WINAPI d3d9_indexbuffer_Release(IDirect3DIndexBuffer9 *iface)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    ULONG refcount = InterlockedDecrement(&buffer->resource.refcount);

    TRACE("%p decreasing refcount to %u.\n", iface, refcount);

    if (!refcount)
    {
        IDirect3DDevice9Ex *device = buffer->parent_device;

        wined3d_mutex_lock();
        wined3d_buffer_decref(buffer->wined3d_buffer);
        wined3d_mutex_unlock();

        /* Release the device last, as it may cause the device to be destroyed. */
        IDirect3DDevice9Ex_Release(device);
    }

    return refcount;
}

static HRESULT WINAPI d3d9_indexbuffer_GetDevice(IDirect3DIndexBuffer9 *iface, IDirect3DDevice9 **device)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);

    TRACE("iface %p, device %p.\n", iface, device);

    *device = (IDirect3DDevice9 *)buffer->parent_device;
    IDirect3DDevice9_AddRef(*device);

    TRACE("Returning device %p.\n", *device);

    return D3D_OK;
}

static HRESULT WINAPI d3d9_indexbuffer_SetPrivateData(IDirect3DIndexBuffer9 *iface,
        REFGUID guid, const void *data, DWORD data_size, DWORD flags)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    TRACE("iface %p, guid %s, data %p, data_size %u, flags %#x.\n",
            iface, debugstr_guid(guid), data, data_size, flags);

    return d3d9_resource_set_private_data(&buffer->resource, guid, data, data_size, flags);
}

static HRESULT WINAPI d3d9_indexbuffer_GetPrivateData(IDirect3DIndexBuffer9 *iface,
        REFGUID guid, void *data, DWORD *data_size)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    TRACE("iface %p, guid %s, data %p, data_size %p.\n",
            iface, debugstr_guid(guid), data, data_size);

    return d3d9_resource_get_private_data(&buffer->resource, guid, data, data_size);
}

static HRESULT WINAPI d3d9_indexbuffer_FreePrivateData(IDirect3DIndexBuffer9 *iface, REFGUID guid)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    TRACE("iface %p, guid %s.\n", iface, debugstr_guid(guid));

    return d3d9_resource_free_private_data(&buffer->resource, guid);
}

static DWORD WINAPI d3d9_indexbuffer_SetPriority(IDirect3DIndexBuffer9 *iface, DWORD priority)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    struct wined3d_resource *resource;
    DWORD previous;

    TRACE("iface %p, priority %u.\n", iface, priority);

    wined3d_mutex_lock();
    resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    previous = wined3d_resource_set_priority(resource, priority);
    wined3d_mutex_unlock();

    return previous;
}

static DWORD WINAPI d3d9_indexbuffer_GetPriority(IDirect3DIndexBuffer9 *iface)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    const struct wined3d_resource *resource;
    DWORD priority;

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    priority = wined3d_resource_get_priority(resource);
    wined3d_mutex_unlock();

    return priority;
}

static void WINAPI d3d9_indexbuffer_PreLoad(IDirect3DIndexBuffer9 *iface)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_resource_preload(wined3d_buffer_get_resource(buffer->wined3d_buffer));
    wined3d_mutex_unlock();
}

static D3DRESOURCETYPE WINAPI d3d9_indexbuffer_GetType(IDirect3DIndexBuffer9 *iface)
{
    TRACE("iface %p.\n", iface);

    return D3DRTYPE_INDEXBUFFER;
}

static HRESULT WINAPI d3d9_indexbuffer_Lock(IDirect3DIndexBuffer9 *iface,
        UINT offset, UINT size, void **data, DWORD flags)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    struct wined3d_map_desc wined3d_map_desc;
    struct wined3d_box wined3d_box = {0};
    HRESULT hr;

    TRACE("iface %p, offset %u, size %u, data %p, flags %#x.\n",
            iface, offset, size, data, flags);

    wined3d_box.left = offset;
    wined3d_box.right = offset + size;
    wined3d_mutex_lock();
    hr = wined3d_resource_map(wined3d_buffer_get_resource(buffer->wined3d_buffer),
            0, &wined3d_map_desc, &wined3d_box, flags);
    wined3d_mutex_unlock();
    *data = wined3d_map_desc.data;

    return hr;
}

static HRESULT WINAPI d3d9_indexbuffer_Unlock(IDirect3DIndexBuffer9 *iface)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);

    TRACE("iface %p.\n", iface);

    wined3d_mutex_lock();
    wined3d_resource_unmap(wined3d_buffer_get_resource(buffer->wined3d_buffer), 0);
    wined3d_mutex_unlock();

    return D3D_OK;
}

static HRESULT WINAPI d3d9_indexbuffer_GetDesc(IDirect3DIndexBuffer9 *iface, D3DINDEXBUFFER_DESC *desc)
{
    struct d3d9_indexbuffer *buffer = impl_from_IDirect3DIndexBuffer9(iface);
    struct wined3d_resource_desc wined3d_desc;
    struct wined3d_resource *wined3d_resource;

    TRACE("iface %p, desc %p.\n", iface, desc);

    wined3d_mutex_lock();
    wined3d_resource = wined3d_buffer_get_resource(buffer->wined3d_buffer);
    wined3d_resource_get_desc(wined3d_resource, &wined3d_desc);
    wined3d_mutex_unlock();

    desc->Format = d3dformat_from_wined3dformat(buffer->format);
    desc->Usage = wined3d_desc.usage & WINED3DUSAGE_MASK;
    desc->Pool = wined3d_desc.pool;
    desc->Size = wined3d_desc.size;
    desc->Type = D3DRTYPE_INDEXBUFFER;

    return D3D_OK;
}

static const IDirect3DIndexBuffer9Vtbl d3d9_indexbuffer_vtbl =
{
    /* IUnknown */
    d3d9_indexbuffer_QueryInterface,
    d3d9_indexbuffer_AddRef,
    d3d9_indexbuffer_Release,
    /* IDirect3DResource9 */
    d3d9_indexbuffer_GetDevice,
    d3d9_indexbuffer_SetPrivateData,
    d3d9_indexbuffer_GetPrivateData,
    d3d9_indexbuffer_FreePrivateData,
    d3d9_indexbuffer_SetPriority,
    d3d9_indexbuffer_GetPriority,
    d3d9_indexbuffer_PreLoad,
    d3d9_indexbuffer_GetType,
    /* IDirect3DIndexBuffer9 */
    d3d9_indexbuffer_Lock,
    d3d9_indexbuffer_Unlock,
    d3d9_indexbuffer_GetDesc,
};

static void STDMETHODCALLTYPE d3d9_indexbuffer_wined3d_object_destroyed(void *parent)
{
    struct d3d9_indexbuffer *buffer = parent;
    d3d9_resource_cleanup(&buffer->resource);
    HeapFree(GetProcessHeap(), 0, buffer);
}

static const struct wined3d_parent_ops d3d9_indexbuffer_wined3d_parent_ops =
{
    d3d9_indexbuffer_wined3d_object_destroyed,
};

HRESULT indexbuffer_init(struct d3d9_indexbuffer *buffer, struct d3d9_device *device,
        UINT size, DWORD usage, D3DFORMAT format, D3DPOOL pool)
{
    HRESULT hr;

    buffer->IDirect3DIndexBuffer9_iface.lpVtbl = &d3d9_indexbuffer_vtbl;
    buffer->format = wined3dformat_from_d3dformat(format);
    d3d9_resource_init(&buffer->resource);

    wined3d_mutex_lock();
    hr = wined3d_buffer_create_ib(device->wined3d_device, size, usage & WINED3DUSAGE_MASK,
            (enum wined3d_pool)pool, buffer, &d3d9_indexbuffer_wined3d_parent_ops, &buffer->wined3d_buffer);
    wined3d_mutex_unlock();
    if (FAILED(hr))
    {
        WARN("Failed to create wined3d buffer, hr %#x.\n", hr);
        return hr;
    }

    buffer->parent_device = &device->IDirect3DDevice9Ex_iface;
    IDirect3DDevice9Ex_AddRef(buffer->parent_device);

    return D3D_OK;
}

struct d3d9_indexbuffer *unsafe_impl_from_IDirect3DIndexBuffer9(IDirect3DIndexBuffer9 *iface)
{
    if (!iface)
        return NULL;
    assert(iface->lpVtbl == &d3d9_indexbuffer_vtbl);

    return impl_from_IDirect3DIndexBuffer9(iface);
}
