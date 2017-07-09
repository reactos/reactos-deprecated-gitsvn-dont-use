/*
 * Copyright 2005 Oliver Stieber
 * Copyright 2007-2008 Stefan Dösinger for CodeWeavers
 * Copyright 2009-2010 Henri Verbeet for CodeWeavers.
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


#include "wined3d_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(d3d);

static void wined3d_query_init(struct wined3d_query *query, struct wined3d_device *device,
        enum wined3d_query_type type, const void *data, DWORD data_size,
        const struct wined3d_query_ops *query_ops, void *parent, const struct wined3d_parent_ops *parent_ops)
{
    query->ref = 1;
    query->parent = parent;
    query->parent_ops = parent_ops;
    query->device = device;
    query->state = QUERY_CREATED;
    query->type = type;
    query->data = data;
    query->data_size = data_size;
    query->query_ops = query_ops;
    list_init(&query->poll_list_entry);
}

static struct wined3d_event_query *wined3d_event_query_from_query(struct wined3d_query *query)
{
    return CONTAINING_RECORD(query, struct wined3d_event_query, query);
}

static struct wined3d_occlusion_query *wined3d_occlusion_query_from_query(struct wined3d_query *query)
{
    return CONTAINING_RECORD(query, struct wined3d_occlusion_query, query);
}

static struct wined3d_timestamp_query *wined3d_timestamp_query_from_query(struct wined3d_query *query)
{
    return CONTAINING_RECORD(query, struct wined3d_timestamp_query, query);
}

static struct wined3d_so_statistics_query *wined3d_so_statistics_query_from_query(struct wined3d_query *query)
{
    return CONTAINING_RECORD(query, struct wined3d_so_statistics_query, query);
}

static struct wined3d_pipeline_statistics_query *wined3d_pipeline_statistics_query_from_query(struct wined3d_query *query)
{
    return CONTAINING_RECORD(query, struct wined3d_pipeline_statistics_query, query);
}

BOOL wined3d_event_query_supported(const struct wined3d_gl_info *gl_info)
{
    return gl_info->supported[ARB_SYNC] || gl_info->supported[NV_FENCE] || gl_info->supported[APPLE_FENCE];
}

void wined3d_event_query_destroy(struct wined3d_event_query *query)
{
    if (query->context) context_free_event_query(query);
    HeapFree(GetProcessHeap(), 0, query);
}

static enum wined3d_event_query_result wined3d_event_query_test(const struct wined3d_event_query *query,
        const struct wined3d_device *device, DWORD flags)
{
    const struct wined3d_gl_info *gl_info;
    enum wined3d_event_query_result ret;
    struct wined3d_context *context;
    BOOL fence_result;

    TRACE("query %p, device %p, flags %#x.\n", query, device, flags);

    if (!query->context)
    {
        TRACE("Query not started.\n");
        return WINED3D_EVENT_QUERY_NOT_STARTED;
    }

    if (!(context = context_reacquire(device, query->context)))
    {
        if (!query->context->gl_info->supported[ARB_SYNC])
        {
            WARN("Event query tested from wrong thread.\n");
            return WINED3D_EVENT_QUERY_WRONG_THREAD;
        }
        context = context_acquire(device, NULL, 0);
    }
    gl_info = context->gl_info;

    if (gl_info->supported[ARB_SYNC])
    {
        GLenum gl_ret = GL_EXTCALL(glClientWaitSync(query->object.sync,
                (flags & WINED3DGETDATA_FLUSH) ? GL_SYNC_FLUSH_COMMANDS_BIT : 0, 0));
        checkGLcall("glClientWaitSync");

        switch (gl_ret)
        {
            case GL_ALREADY_SIGNALED:
            case GL_CONDITION_SATISFIED:
                ret = WINED3D_EVENT_QUERY_OK;
                break;

            case GL_TIMEOUT_EXPIRED:
                ret = WINED3D_EVENT_QUERY_WAITING;
                break;

            case GL_WAIT_FAILED:
            default:
                ERR("glClientWaitSync returned %#x.\n", gl_ret);
                ret = WINED3D_EVENT_QUERY_ERROR;
        }
    }
    else if (gl_info->supported[APPLE_FENCE])
    {
        fence_result = GL_EXTCALL(glTestFenceAPPLE(query->object.id));
        checkGLcall("glTestFenceAPPLE");
        if (fence_result) ret = WINED3D_EVENT_QUERY_OK;
        else ret = WINED3D_EVENT_QUERY_WAITING;
    }
    else if (gl_info->supported[NV_FENCE])
    {
        fence_result = GL_EXTCALL(glTestFenceNV(query->object.id));
        checkGLcall("glTestFenceNV");
        if (fence_result) ret = WINED3D_EVENT_QUERY_OK;
        else ret = WINED3D_EVENT_QUERY_WAITING;
    }
    else
    {
        ERR("Event query created despite lack of GL support\n");
        ret = WINED3D_EVENT_QUERY_ERROR;
    }

    context_release(context);
    return ret;
}

enum wined3d_event_query_result wined3d_event_query_finish(const struct wined3d_event_query *query,
        const struct wined3d_device *device)
{
    const struct wined3d_gl_info *gl_info;
    enum wined3d_event_query_result ret;
    struct wined3d_context *context;

    TRACE("query %p, device %p.\n", query, device);

    if (!query->context)
    {
        TRACE("Query not started.\n");
        return WINED3D_EVENT_QUERY_NOT_STARTED;
    }
    gl_info = query->context->gl_info;

    if (!(context = context_reacquire(device, query->context)))
    {
        /* A glFinish does not reliably wait for draws in other contexts. The caller has
         * to find its own way to cope with the thread switch
         */
        if (!gl_info->supported[ARB_SYNC])
        {
            WARN("Event query finished from wrong thread.\n");
            return WINED3D_EVENT_QUERY_WRONG_THREAD;
        }
        context = context_acquire(device, NULL, 0);
    }
    gl_info = context->gl_info;

    if (gl_info->supported[ARB_SYNC])
    {
        /* Apple seems to be into arbitrary limits, and timeouts larger than
         * 0xfffffffffffffbff immediately return GL_TIMEOUT_EXPIRED. We don't
         * really care and can live with waiting a few μs less. (OS X 10.7.4). */
        GLenum gl_ret = GL_EXTCALL(glClientWaitSync(query->object.sync, GL_SYNC_FLUSH_COMMANDS_BIT, ~(GLuint64)0xffff));
        checkGLcall("glClientWaitSync");

        switch (gl_ret)
        {
            case GL_ALREADY_SIGNALED:
            case GL_CONDITION_SATISFIED:
                ret = WINED3D_EVENT_QUERY_OK;
                break;

                /* We don't expect a timeout for a ~584 year wait */
            default:
                ERR("glClientWaitSync returned %#x.\n", gl_ret);
                ret = WINED3D_EVENT_QUERY_ERROR;
        }
    }
    else if (context->gl_info->supported[APPLE_FENCE])
    {
        GL_EXTCALL(glFinishFenceAPPLE(query->object.id));
        checkGLcall("glFinishFenceAPPLE");
        ret = WINED3D_EVENT_QUERY_OK;
    }
    else if (context->gl_info->supported[NV_FENCE])
    {
        GL_EXTCALL(glFinishFenceNV(query->object.id));
        checkGLcall("glFinishFenceNV");
        ret = WINED3D_EVENT_QUERY_OK;
    }
    else
    {
        ERR("Event query created without GL support\n");
        ret = WINED3D_EVENT_QUERY_ERROR;
    }

    context_release(context);
    return ret;
}

void wined3d_event_query_issue(struct wined3d_event_query *query, const struct wined3d_device *device)
{
    struct wined3d_context *context = NULL;
    const struct wined3d_gl_info *gl_info;

    if (query->context && !(context = context_reacquire(device, query->context))
            && !query->context->gl_info->supported[ARB_SYNC])
        context_free_event_query(query);
    if (!context)
        context = context_acquire(device, NULL, 0);
    gl_info = context->gl_info;
    if (!query->context)
        context_alloc_event_query(context, query);

    if (gl_info->supported[ARB_SYNC])
    {
        if (query->object.sync) GL_EXTCALL(glDeleteSync(query->object.sync));
        checkGLcall("glDeleteSync");
        query->object.sync = GL_EXTCALL(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0));
        checkGLcall("glFenceSync");
    }
    else if (gl_info->supported[APPLE_FENCE])
    {
        GL_EXTCALL(glSetFenceAPPLE(query->object.id));
        checkGLcall("glSetFenceAPPLE");
    }
    else if (gl_info->supported[NV_FENCE])
    {
        GL_EXTCALL(glSetFenceNV(query->object.id, GL_ALL_COMPLETED_NV));
        checkGLcall("glSetFenceNV");
    }

    context_release(context);
}

ULONG CDECL wined3d_query_incref(struct wined3d_query *query)
{
    ULONG refcount = InterlockedIncrement(&query->ref);

    TRACE("%p increasing refcount to %u.\n", query, refcount);

    return refcount;
}

static void wined3d_query_destroy_object(void *object)
{
    struct wined3d_query *query = object;

    if (!list_empty(&query->poll_list_entry))
        list_remove(&query->poll_list_entry);

    /* Queries are specific to the GL context that created them. Not
     * deleting the query will obviously leak it, but that's still better
     * than potentially deleting a different query with the same id in this
     * context, and (still) leaking the actual query. */
    if (query->type == WINED3D_QUERY_TYPE_EVENT)
    {
        wined3d_event_query_destroy(wined3d_event_query_from_query(query));
    }
    else if (query->type == WINED3D_QUERY_TYPE_OCCLUSION)
    {
        struct wined3d_occlusion_query *oq = wined3d_occlusion_query_from_query(query);

        if (oq->context)
            context_free_occlusion_query(oq);
        HeapFree(GetProcessHeap(), 0, oq);
    }
    else if (query->type == WINED3D_QUERY_TYPE_TIMESTAMP)
    {
        struct wined3d_timestamp_query *tq = wined3d_timestamp_query_from_query(query);

        if (tq->context)
            context_free_timestamp_query(tq);
        HeapFree(GetProcessHeap(), 0, tq);
    }
    else if (query->type == WINED3D_QUERY_TYPE_TIMESTAMP_DISJOINT
            || query->type == WINED3D_QUERY_TYPE_TIMESTAMP_FREQ)
    {
        HeapFree(GetProcessHeap(), 0, query);
    }
    else if (query->type == WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM0
            || query->type ==  WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM1
            || query->type ==  WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM2
            || query->type ==  WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM3)
    {
        struct wined3d_so_statistics_query *pq = wined3d_so_statistics_query_from_query(query);
        if (pq->context)
            context_free_so_statistics_query(pq);
        HeapFree(GetProcessHeap(), 0, pq);
    }
    else if (query->type == WINED3D_QUERY_TYPE_SO_STATISTICS)
    {
        HeapFree(GetProcessHeap(), 0, query);
    }
    else if (query->type == WINED3D_QUERY_TYPE_SO_OVERFLOW)
    {
        HeapFree(GetProcessHeap(), 0, query);
    }
    else if (query->type == WINED3D_QUERY_TYPE_PIPELINE_STATISTICS)
    {
        struct wined3d_pipeline_statistics_query *pq = wined3d_pipeline_statistics_query_from_query(query);
        if (pq->context)
            context_free_pipeline_statistics_query(pq);
        HeapFree(GetProcessHeap(), 0, query);
    }
    else
    {
        ERR("Query %p has invalid type %#x.\n", query, query->type);
    }
}

ULONG CDECL wined3d_query_decref(struct wined3d_query *query)
{
    ULONG refcount = InterlockedDecrement(&query->ref);

    TRACE("%p decreasing refcount to %u.\n", query, refcount);

    if (!refcount)
    {
        query->parent_ops->wined3d_object_destroyed(query->parent);
        wined3d_cs_destroy_object(query->device->cs, wined3d_query_destroy_object, query);
    }

    return refcount;
}

HRESULT CDECL wined3d_query_get_data(struct wined3d_query *query,
        void *data, UINT data_size, DWORD flags)
{
    TRACE("query %p, data %p, data_size %u, flags %#x.\n",
            query, data, data_size, flags);

    if (flags)
        WARN("Ignoring flags %#x.\n", flags);

    if (query->state == QUERY_BUILDING)
    {
        WARN("Query is building, returning S_FALSE.\n");
        return S_FALSE;
    }

    if (query->state == QUERY_CREATED)
    {
        WARN("Query wasn't started yet.\n");
        return WINED3DERR_INVALIDCALL;
    }

    if (!query->device->cs->thread)
    {
        if (!query->query_ops->query_poll(query, flags))
            return S_FALSE;
    }
    else if (query->counter_main != query->counter_retrieved)
    {
        if (flags & WINED3DGETDATA_FLUSH)
            wined3d_cs_emit_flush(query->device->cs);
        return S_FALSE;
    }

    if (data)
        memcpy(data, query->data, min(data_size, query->data_size));

    return S_OK;
}

UINT CDECL wined3d_query_get_data_size(const struct wined3d_query *query)
{
    TRACE("query %p.\n", query);

    return query->data_size;
}

HRESULT CDECL wined3d_query_issue(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    if (flags & WINED3DISSUE_END)
        ++query->counter_main;

    wined3d_cs_emit_query_issue(query->device->cs, query, flags);

    if (flags & WINED3DISSUE_BEGIN)
        query->state = QUERY_BUILDING;
    else
        query->state = QUERY_SIGNALLED;

    return WINED3D_OK;
}

static BOOL wined3d_occlusion_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_occlusion_query *oq = wined3d_occlusion_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info;
    struct wined3d_context *context;
    GLuint available;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (!(context = context_reacquire(device, oq->context)))
    {
        FIXME("%p Wrong thread, returning 1.\n", query);
        oq->samples = 1;
        return TRUE;
    }
    gl_info = context->gl_info;

    GL_EXTCALL(glGetQueryObjectuiv(oq->id, GL_QUERY_RESULT_AVAILABLE, &available));
    checkGLcall("glGetQueryObjectuiv(GL_QUERY_RESULT_AVAILABLE)");
    TRACE("available %#x.\n", available);

    if (available)
    {
        if (gl_info->supported[ARB_TIMER_QUERY])
        {
            GLuint64 result;
            GL_EXTCALL(glGetQueryObjectui64v(oq->id, GL_QUERY_RESULT, &result));
            checkGLcall("glGetQueryObjectui64v(GL_QUERY_RESULT)");
            oq->samples = result;
        }
        else
        {
            GLuint result;
            GL_EXTCALL(glGetQueryObjectuiv(oq->id, GL_QUERY_RESULT, &result));
            checkGLcall("glGetQueryObjectuiv(GL_QUERY_RESULT)");
            oq->samples = result;
        }
        TRACE("Returning 0x%s samples.\n", wine_dbgstr_longlong(oq->samples));
    }

    context_release(context);

    return available;
}

static BOOL wined3d_event_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_event_query *event_query = wined3d_event_query_from_query(query);
    enum wined3d_event_query_result ret;

    TRACE("query %p, flags %#x.\n", query, flags);

    ret = wined3d_event_query_test(event_query, query->device, flags);
    switch (ret)
    {
        case WINED3D_EVENT_QUERY_OK:
        case WINED3D_EVENT_QUERY_NOT_STARTED:
            return event_query->signalled = TRUE;

        case WINED3D_EVENT_QUERY_WAITING:
            return event_query->signalled = FALSE;

        case WINED3D_EVENT_QUERY_WRONG_THREAD:
            FIXME("(%p) Wrong thread, reporting GPU idle.\n", query);
            return event_query->signalled = TRUE;

        case WINED3D_EVENT_QUERY_ERROR:
            ERR("The GL event query failed.\n");
            return event_query->signalled = TRUE;

        default:
            ERR("Unexpected wined3d_event_query_test result %#x.\n", ret);
            return event_query->signalled = TRUE;
    }
}

void * CDECL wined3d_query_get_parent(const struct wined3d_query *query)
{
    TRACE("query %p.\n", query);

    return query->parent;
}

enum wined3d_query_type CDECL wined3d_query_get_type(const struct wined3d_query *query)
{
    TRACE("query %p.\n", query);

    return query->type;
}

static BOOL wined3d_event_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    if (flags & WINED3DISSUE_END)
    {
        struct wined3d_event_query *event_query = wined3d_event_query_from_query(query);

        wined3d_event_query_issue(event_query, query->device);
        return TRUE;
    }
    else if (flags & WINED3DISSUE_BEGIN)
    {
        /* Started implicitly at query creation. */
        ERR("Event query issued with START flag - what to do?\n");
    }

    return FALSE;
}

static BOOL wined3d_occlusion_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_occlusion_query *oq = wined3d_occlusion_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_context *context;
    BOOL poll = FALSE;

    TRACE("query %p, flags %#x.\n", query, flags);

    /* This is allowed according to MSDN and our tests. Reset the query and
     * restart. */
    if (flags & WINED3DISSUE_BEGIN)
    {
        if (oq->started)
        {
            if ((context = context_reacquire(device, oq->context)))
            {
                GL_EXTCALL(glEndQuery(GL_SAMPLES_PASSED));
                checkGLcall("glEndQuery()");
            }
            else
            {
                FIXME("Wrong thread, can't restart query.\n");
                context_free_occlusion_query(oq);
                context = context_acquire(device, NULL, 0);
                context_alloc_occlusion_query(context, oq);
            }
        }
        else
        {
            if (oq->context)
                context_free_occlusion_query(oq);
            context = context_acquire(device, NULL, 0);
            context_alloc_occlusion_query(context, oq);
        }

        GL_EXTCALL(glBeginQuery(GL_SAMPLES_PASSED, oq->id));
        checkGLcall("glBeginQuery()");

        context_release(context);
        oq->started = TRUE;
    }
    if (flags & WINED3DISSUE_END)
    {
        /* MSDN says END on a non-building occlusion query returns an error,
         * but our tests show that it returns OK. But OpenGL doesn't like it,
         * so avoid generating an error. */
        if (oq->started)
        {
            if ((context = context_reacquire(device, oq->context)))
            {
                GL_EXTCALL(glEndQuery(GL_SAMPLES_PASSED));
                checkGLcall("glEndQuery()");

                context_release(context);
                poll = TRUE;
            }
            else
            {
                FIXME("Wrong thread, can't end query.\n");
            }
        }
        oq->started = FALSE;
    }

    return poll;
}

static BOOL wined3d_timestamp_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_timestamp_query *tq = wined3d_timestamp_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info;
    struct wined3d_context *context;
    GLuint64 timestamp;
    GLuint available;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (!(context = context_reacquire(device, tq->context)))
    {
        FIXME("%p Wrong thread, returning 1.\n", query);
        tq->timestamp = 1;
        return TRUE;
    }
    gl_info = context->gl_info;

    GL_EXTCALL(glGetQueryObjectuiv(tq->id, GL_QUERY_RESULT_AVAILABLE, &available));
    checkGLcall("glGetQueryObjectuiv(GL_QUERY_RESULT_AVAILABLE)");
    TRACE("available %#x.\n", available);

    if (available)
    {
        GL_EXTCALL(glGetQueryObjectui64v(tq->id, GL_QUERY_RESULT, &timestamp));
        checkGLcall("glGetQueryObjectui64v(GL_QUERY_RESULT)");
        TRACE("Returning timestamp %s.\n", wine_dbgstr_longlong(timestamp));
        tq->timestamp = timestamp;
    }

    context_release(context);

    return available;
}

static BOOL wined3d_timestamp_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_timestamp_query *tq = wined3d_timestamp_query_from_query(query);
    const struct wined3d_gl_info *gl_info;
    struct wined3d_context *context;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (flags & WINED3DISSUE_BEGIN)
    {
        WARN("Ignoring WINED3DISSUE_BEGIN with a TIMESTAMP query.\n");
    }
    if (flags & WINED3DISSUE_END)
    {
        if (tq->context)
            context_free_timestamp_query(tq);
        context = context_acquire(query->device, NULL, 0);
        gl_info = context->gl_info;
        context_alloc_timestamp_query(context, tq);
        GL_EXTCALL(glQueryCounter(tq->id, GL_TIMESTAMP));
        checkGLcall("glQueryCounter()");
        context_release(context);

        return TRUE;
    }

    return FALSE;
}

static BOOL wined3d_timestamp_disjoint_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    return TRUE;
}

static BOOL wined3d_timestamp_disjoint_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    return FALSE;
}

static BOOL wined3d_so_statistics_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_so_statistics_query *pq = wined3d_so_statistics_query_from_query(query);
    struct wined3d_device *device = query->device;
    GLuint written_available, generated_available;
    const struct wined3d_gl_info *gl_info;
    struct wined3d_context *context;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (!(context = context_reacquire(device, pq->context)))
    {
        FIXME("%p Wrong thread, returning 0 primitives.\n", query);
        memset(&pq->statistics, 0, sizeof(pq->statistics));
        return TRUE;
    }
    gl_info = context->gl_info;

    GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.written,
            GL_QUERY_RESULT_AVAILABLE, &written_available));
    GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.generated,
            GL_QUERY_RESULT_AVAILABLE, &generated_available));
    TRACE("Available %#x, %#x.\n", written_available, generated_available);

    if (written_available && generated_available)
    {
        if (gl_info->supported[ARB_TIMER_QUERY])
        {
            GLuint64 result;
            GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.written, GL_QUERY_RESULT, &result));
            pq->statistics.primitives_written = result;
            GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.generated, GL_QUERY_RESULT, &result));
            pq->statistics.primitives_generated = result;
        }
        else
        {
            GLuint result;
            GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.written, GL_QUERY_RESULT, &result));
            pq->statistics.primitives_written = result;
            GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.generated, GL_QUERY_RESULT, &result));
            pq->statistics.primitives_generated = result;
        }
        TRACE("Returning %s, %s primitives.\n",
                wine_dbgstr_longlong(pq->statistics.primitives_written),
                wine_dbgstr_longlong(pq->statistics.primitives_generated));
    }

    checkGLcall("poll SO statistics query");
    context_release(context);

    return written_available && generated_available;
}

static BOOL wined3d_so_statistics_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_so_statistics_query *pq = wined3d_so_statistics_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_context *context;
    BOOL poll = FALSE;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (flags & WINED3DISSUE_BEGIN)
    {
        if (pq->started)
        {
            if ((context = context_reacquire(device, pq->context)))
            {
                GL_EXTCALL(glEndQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, pq->stream_idx));
                GL_EXTCALL(glEndQueryIndexed(GL_PRIMITIVES_GENERATED, pq->stream_idx));
            }
            else
            {
                FIXME("Wrong thread, can't restart query.\n");
                context_free_so_statistics_query(pq);
                context = context_acquire(device, NULL, 0);
                context_alloc_so_statistics_query(context, pq);
            }
        }
        else
        {
            if (pq->context)
                context_free_so_statistics_query(pq);
            context = context_acquire(device, NULL, 0);
            context_alloc_so_statistics_query(context, pq);
        }

        GL_EXTCALL(glBeginQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN,
                pq->stream_idx, pq->u.query.written));
        GL_EXTCALL(glBeginQueryIndexed(GL_PRIMITIVES_GENERATED,
                pq->stream_idx, pq->u.query.generated));
        checkGLcall("begin query");

        context_release(context);
        pq->started = TRUE;
    }
    if (flags & WINED3DISSUE_END)
    {
        if (pq->started)
        {
            if ((context = context_reacquire(device, pq->context)))
            {
                GL_EXTCALL(glEndQueryIndexed(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, pq->stream_idx));
                GL_EXTCALL(glEndQueryIndexed(GL_PRIMITIVES_GENERATED, pq->stream_idx));
                checkGLcall("end query");

                context_release(context);
                poll = TRUE;
            }
            else
            {
                FIXME("Wrong thread, can't end query.\n");
            }
        }
        pq->started = FALSE;
    }

    return poll;
}

static BOOL wined3d_statistics_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    return TRUE;
}

static BOOL wined3d_statistics_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    FIXME("query %p, flags %#x.\n", query, flags);

    return FALSE;
}

static BOOL wined3d_overflow_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    TRACE("query %p, flags %#x.\n", query, flags);

    return TRUE;
}

static BOOL wined3d_overflow_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    FIXME("query %p, flags %#x.\n", query, flags);

    return FALSE;
}

static BOOL wined3d_pipeline_query_ops_poll(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_pipeline_statistics_query *pq = wined3d_pipeline_statistics_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info;
    struct wined3d_context *context;
    GLuint available;
    int i;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (!(context = context_reacquire(device, pq->context)))
    {
        FIXME("%p Wrong thread, returning 0 primitives.\n", query);
        memset(&pq->statistics, 0, sizeof(pq->statistics));
        return TRUE;
    }
    gl_info = context->gl_info;

    for (i = 0; i < ARRAY_SIZE(pq->u.id); i++)
    {
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.id[i], GL_QUERY_RESULT_AVAILABLE, &available));
        if (!available) goto done;
    }

    if (gl_info->supported[ARB_TIMER_QUERY])
    {
        GLuint64 result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.vertices, GL_QUERY_RESULT, &result));
        pq->statistics.ia_vertices = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.primitives, GL_QUERY_RESULT, &result));
        pq->statistics.ia_primitives = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.vertex_shader, GL_QUERY_RESULT, &result));
        pq->statistics.vs_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.tess_control_shader, GL_QUERY_RESULT, &result));
        pq->statistics.hs_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.tess_eval_shader, GL_QUERY_RESULT, &result));
        pq->statistics.ds_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.geometry_shader, GL_QUERY_RESULT, &result));
        pq->statistics.gs_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.geometry_primitives, GL_QUERY_RESULT, &result));
        pq->statistics.gs_primitives = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.fragment_shader, GL_QUERY_RESULT, &result));
        pq->statistics.ps_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.compute_shader, GL_QUERY_RESULT, &result));
        pq->statistics.cs_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.clipping_input, GL_QUERY_RESULT, &result));
        pq->statistics.c_invocations = result;
        GL_EXTCALL(glGetQueryObjectui64v(pq->u.query.clipping_output, GL_QUERY_RESULT, &result));
        pq->statistics.c_primitives = result;
    }
    else
    {
        GLuint result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.vertices, GL_QUERY_RESULT, &result));
        pq->statistics.ia_vertices = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.primitives, GL_QUERY_RESULT, &result));
        pq->statistics.ia_primitives = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.vertex_shader, GL_QUERY_RESULT, &result));
        pq->statistics.vs_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.tess_control_shader, GL_QUERY_RESULT, &result));
        pq->statistics.hs_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.tess_eval_shader, GL_QUERY_RESULT, &result));
        pq->statistics.ds_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.geometry_shader, GL_QUERY_RESULT, &result));
        pq->statistics.gs_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.geometry_primitives, GL_QUERY_RESULT, &result));
        pq->statistics.gs_primitives = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.fragment_shader, GL_QUERY_RESULT, &result));
        pq->statistics.ps_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.compute_shader, GL_QUERY_RESULT, &result));
        pq->statistics.cs_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.clipping_input, GL_QUERY_RESULT, &result));
        pq->statistics.c_invocations = result;
        GL_EXTCALL(glGetQueryObjectuiv(pq->u.query.clipping_output, GL_QUERY_RESULT, &result));
        pq->statistics.c_primitives = result;
    }

done:
    checkGLcall("poll pipeline statistics query");
    context_release(context);
    return available;
}

static BOOL wined3d_pipeline_query_ops_issue(struct wined3d_query *query, DWORD flags)
{
    struct wined3d_pipeline_statistics_query *pq = wined3d_pipeline_statistics_query_from_query(query);
    struct wined3d_device *device = query->device;
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_context *context;
    BOOL poll = FALSE;

    TRACE("query %p, flags %#x.\n", query, flags);

    if (flags & WINED3DISSUE_BEGIN)
    {
        if (pq->started)
        {
            if ((context = context_reacquire(device, pq->context)))
            {
                GL_EXTCALL(glEndQuery(GL_VERTICES_SUBMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_PRIMITIVES_SUBMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_VERTEX_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_TESS_CONTROL_SHADER_PATCHES_ARB));
                GL_EXTCALL(glEndQuery(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_GEOMETRY_SHADER_INVOCATIONS));
                GL_EXTCALL(glEndQuery(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_FRAGMENT_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_COMPUTE_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_CLIPPING_INPUT_PRIMITIVES_ARB));
                GL_EXTCALL(glEndQuery(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB));
            }
            else
            {
                FIXME("Wrong thread, can't restart query.\n");
                context_free_pipeline_statistics_query(pq);
                context = context_acquire(device, NULL, 0);
                context_alloc_pipeline_statistics_query(context, pq);
            }
        }
        else
        {
            if (pq->context)
                context_free_pipeline_statistics_query(pq);
            context = context_acquire(device, NULL, 0);
            context_alloc_pipeline_statistics_query(context, pq);
        }

        GL_EXTCALL(glBeginQuery(GL_VERTICES_SUBMITTED_ARB, pq->u.query.vertices));
        GL_EXTCALL(glBeginQuery(GL_PRIMITIVES_SUBMITTED_ARB, pq->u.query.primitives));
        GL_EXTCALL(glBeginQuery(GL_VERTEX_SHADER_INVOCATIONS_ARB, pq->u.query.vertex_shader));
        GL_EXTCALL(glBeginQuery(GL_TESS_CONTROL_SHADER_PATCHES_ARB, pq->u.query.tess_control_shader));
        GL_EXTCALL(glBeginQuery(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB, pq->u.query.tess_eval_shader));
        GL_EXTCALL(glBeginQuery(GL_GEOMETRY_SHADER_INVOCATIONS, pq->u.query.geometry_shader));
        GL_EXTCALL(glBeginQuery(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB, pq->u.query.geometry_primitives));
        GL_EXTCALL(glBeginQuery(GL_FRAGMENT_SHADER_INVOCATIONS_ARB, pq->u.query.fragment_shader));
        GL_EXTCALL(glBeginQuery(GL_COMPUTE_SHADER_INVOCATIONS_ARB, pq->u.query.compute_shader));
        GL_EXTCALL(glBeginQuery(GL_CLIPPING_INPUT_PRIMITIVES_ARB, pq->u.query.clipping_input));
        GL_EXTCALL(glBeginQuery(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB, pq->u.query.clipping_output));
        checkGLcall("begin query");

        context_release(context);
        pq->started = TRUE;
    }
    if (flags & WINED3DISSUE_END)
    {
        if (pq->started)
        {
            if ((context = context_reacquire(device, pq->context)))
            {
                GL_EXTCALL(glEndQuery(GL_VERTICES_SUBMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_PRIMITIVES_SUBMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_VERTEX_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_TESS_CONTROL_SHADER_PATCHES_ARB));
                GL_EXTCALL(glEndQuery(GL_TESS_EVALUATION_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_GEOMETRY_SHADER_INVOCATIONS));
                GL_EXTCALL(glEndQuery(GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED_ARB));
                GL_EXTCALL(glEndQuery(GL_FRAGMENT_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_COMPUTE_SHADER_INVOCATIONS_ARB));
                GL_EXTCALL(glEndQuery(GL_CLIPPING_INPUT_PRIMITIVES_ARB));
                GL_EXTCALL(glEndQuery(GL_CLIPPING_OUTPUT_PRIMITIVES_ARB));
                checkGLcall("end query");

                context_release(context);
                poll = TRUE;
            }
            else
            {
                FIXME("Wrong thread, can't end query.\n");
            }
        }
        pq->started = FALSE;
    }

    return poll;
}

static const struct wined3d_query_ops event_query_ops =
{
    wined3d_event_query_ops_poll,
    wined3d_event_query_ops_issue,
};

static HRESULT wined3d_event_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_event_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!wined3d_event_query_supported(gl_info))
    {
        WARN("Event queries not supported.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(&object->query, device, type, &object->signalled,
            sizeof(object->signalled), &event_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = &object->query;

    return WINED3D_OK;
}

static const struct wined3d_query_ops occlusion_query_ops =
{
    wined3d_occlusion_query_ops_poll,
    wined3d_occlusion_query_ops_issue,
};

static HRESULT wined3d_occlusion_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_occlusion_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!gl_info->supported[ARB_OCCLUSION_QUERY])
    {
        WARN("Unsupported in local OpenGL implementation: ARB_OCCLUSION_QUERY.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(&object->query, device, type, &object->samples,
            sizeof(object->samples), &occlusion_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = &object->query;

    return WINED3D_OK;
}

static const struct wined3d_query_ops timestamp_query_ops =
{
    wined3d_timestamp_query_ops_poll,
    wined3d_timestamp_query_ops_issue,
};

static HRESULT wined3d_timestamp_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_timestamp_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!gl_info->supported[ARB_TIMER_QUERY])
    {
        WARN("Unsupported in local OpenGL implementation: ARB_TIMER_QUERY.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(&object->query, device, type, &object->timestamp,
            sizeof(object->timestamp), &timestamp_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = &object->query;

    return WINED3D_OK;
}

static const struct wined3d_query_ops timestamp_disjoint_query_ops =
{
    wined3d_timestamp_disjoint_query_ops_poll,
    wined3d_timestamp_disjoint_query_ops_issue,
};

static HRESULT wined3d_timestamp_disjoint_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!gl_info->supported[ARB_TIMER_QUERY])
    {
        WARN("Unsupported in local OpenGL implementation: ARB_TIMER_QUERY.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    if (type == WINED3D_QUERY_TYPE_TIMESTAMP_DISJOINT)
    {
        static const struct wined3d_query_data_timestamp_disjoint disjoint_data = {1000 * 1000 * 1000, FALSE};

        wined3d_query_init(object, device, type, &disjoint_data,
                sizeof(disjoint_data), &timestamp_disjoint_query_ops, parent, parent_ops);
    }
    else
    {
        static const UINT64 freq = 1000 * 1000 * 1000;

        wined3d_query_init(object, device, type, &freq,
                sizeof(freq), &timestamp_disjoint_query_ops, parent, parent_ops);
    }

    TRACE("Created query %p.\n", object);
    *query = object;

    return WINED3D_OK;
}

static const struct wined3d_query_ops so_statistics_query_ops =
{
    wined3d_so_statistics_query_ops_poll,
    wined3d_so_statistics_query_ops_issue,
};

static HRESULT wined3d_so_statistics_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_so_statistics_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!gl_info->supported[WINED3D_GL_PRIMITIVE_QUERY])
    {
        WARN("OpenGL implementation does not support primitive queries.\n");
        return WINED3DERR_NOTAVAILABLE;
    }
    if (!gl_info->supported[ARB_TRANSFORM_FEEDBACK3])
    {
        WARN("OpenGL implementation does not support indexed queries.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    switch (type)
    {
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM0:
            object->stream_idx = 0;
            break;
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM1:
            object->stream_idx = 1;
            break;
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM2:
            object->stream_idx = 2;
            break;
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM3:
            object->stream_idx = 3;
            break;
        default:
            HeapFree(GetProcessHeap(), 0, object);
            return WINED3DERR_NOTAVAILABLE;
    }

    wined3d_query_init(&object->query, device, type, &object->statistics,
            sizeof(object->statistics), &so_statistics_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = &object->query;

    return WINED3D_OK;
}

static const struct wined3d_query_ops statistics_query_ops =
{
    wined3d_statistics_query_ops_poll,
    wined3d_statistics_query_ops_issue,
};

static HRESULT wined3d_statistics_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    static const struct wined3d_query_data_so_statistics statistics = { 1, 1 };
    struct wined3d_query *object;

    FIXME("device %p, type %#x, parent %p, query %p.\n", device, type, parent, query);

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(object, device, type, &statistics,
            sizeof(statistics), &statistics_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = object;

    return WINED3D_OK;
}

static const struct wined3d_query_ops overflow_query_ops =
{
    wined3d_overflow_query_ops_poll,
    wined3d_overflow_query_ops_issue,
};

static HRESULT wined3d_overflow_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    static const BOOL overflow = FALSE;
    struct wined3d_query *object;

    FIXME("device %p, type %#x, parent %p, query %p.\n", device, type, parent, query);

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(object, device, type, &overflow,
            sizeof(overflow), &overflow_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = object;

    return WINED3D_OK;
}

static const struct wined3d_query_ops pipeline_query_ops =
{
    wined3d_pipeline_query_ops_poll,
    wined3d_pipeline_query_ops_issue,
};

static HRESULT wined3d_pipeline_query_create(struct wined3d_device *device,
        enum wined3d_query_type type, void *parent, const struct wined3d_parent_ops *parent_ops,
        struct wined3d_query **query)
{
    const struct wined3d_gl_info *gl_info = &device->adapter->gl_info;
    struct wined3d_pipeline_statistics_query *object;

    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    if (!gl_info->supported[ARB_PIPELINE_STATISTICS_QUERY])
    {
        WARN("OpenGL implementation does not support pipeline statistic queries.\n");
        return WINED3DERR_NOTAVAILABLE;
    }

    if (!(object = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*object))))
        return E_OUTOFMEMORY;

    wined3d_query_init(&object->query, device, type, &object->statistics,
            sizeof(object->statistics), &pipeline_query_ops, parent, parent_ops);

    TRACE("Created query %p.\n", object);
    *query = &object->query;

    return WINED3D_OK;
}

HRESULT CDECL wined3d_query_create(struct wined3d_device *device, enum wined3d_query_type type,
        void *parent, const struct wined3d_parent_ops *parent_ops, struct wined3d_query **query)
{
    TRACE("device %p, type %#x, parent %p, parent_ops %p, query %p.\n",
            device, type, parent, parent_ops, query);

    switch (type)
    {
        case WINED3D_QUERY_TYPE_EVENT:
            return wined3d_event_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_OCCLUSION:
            return wined3d_occlusion_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_TIMESTAMP:
            return wined3d_timestamp_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_TIMESTAMP_DISJOINT:
        case WINED3D_QUERY_TYPE_TIMESTAMP_FREQ:
            return wined3d_timestamp_disjoint_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM0:
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM1:
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM2:
        case WINED3D_QUERY_TYPE_SO_STATISTICS_STREAM3:
            return wined3d_so_statistics_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_SO_STATISTICS:
            return wined3d_statistics_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_SO_OVERFLOW:
            return wined3d_overflow_query_create(device, type, parent, parent_ops, query);

        case WINED3D_QUERY_TYPE_PIPELINE_STATISTICS:
            return wined3d_pipeline_query_create(device, type, parent, parent_ops, query);

        default:
            FIXME("Unhandled query type %#x.\n", type);
            return WINED3DERR_NOTAVAILABLE;
    }
}
