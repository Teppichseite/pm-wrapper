#include "../runtime/pm_wrapper.h"
#include "pmdk_backend.h"
#include <libpmemobj.h>
#include <stdlib.h>
#include <unistd.h>

#define POOL(context) ((PMEMobjpool *)context->data)

static int init()
{
    return 0;
}

static int open_or_create(PmBackendContext *context, bool *created_new)
{

    if (access(context->region_config.file_path, F_OK) == 0)
    {
        context->data = pmemobj_open(context->region_config.file_path, "key");
        if (context->data == NULL)
        {
            return 1;
        }
        *created_new = false;
    }
    else
    {
        context->data = pmemobj_create(context->region_config.file_path, "key", PMEMOBJ_MIN_POOL, 0666);
        if (context->data == NULL)
        {
            return 1;
        }
        *created_new = true;
    }

    context->id = pmemobj_root(POOL(context), context->region_config.root_size).pool_uuid_lo;

    return 0;
}

static void p_close(PmBackendContext *context)
{
    pmemobj_close(POOL(context));
}

static void finalize()
{
}

static pm_region_offset get_root(PmBackendContext *context)
{
    return pmemobj_root(POOL(context), context->region_config.root_size).off;
}

static pm_region_offset p_alloc(PmBackendContext *context, size_t size)
{
    PMEMoid oid = {};
    pmemobj_zalloc(POOL(context), &oid, size, 0);
    return oid.off;
}

static pm_region_offset p_calloc(PmBackendContext *context, size_t size)
{
    return p_alloc(context, size);
}

static void p_free(PmBackendContext *context, pm_region_offset offset)
{
    PMEMoid oid = {
        .pool_uuid_lo = context->id,
        .off = offset};
    return pmemobj_free(&oid);
}

static void *read_object(PmBackendContext *context, pm_region_offset offset)
{
    PMEMoid oid = {
        .pool_uuid_lo = context->id,
        .off = offset};

    return pmemobj_direct_inline(oid);
}

static void write_object(PmBackendContext *context, void *dst, char *data, size_t len)
{
    pmemobj_memcpy_persist(POOL(context), dst, data, len);
    //memcpy(dst, data, len);
}

PmBackend PMDK_BACKEND = {
    .init = init,
    .open_or_create = open_or_create,
    .close = p_close,
    .finalize = finalize,
    .get_root = get_root,
    .alloc = p_alloc,
    .calloc = p_calloc,
    .free = p_free,
    .read_object = read_object,
    .write_object = write_object};