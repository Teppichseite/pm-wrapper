#include "pm_wrapper.h"
#include "pmdk_backend.h"
#include "ex_common.h"
#include <libpmemobj.h>
#include <stdlib.h>

#define POOL(context) ((PMEMobjpool *)context->data)

static int open(PmBackendContext *context)
{
    if (file_exists(context->region_config.file_path) != 0)
    {
        return 1;
    }

    context->data = pmemobj_open(context->region_config.file_path, "key");
    if (context->data == NULL)
    {
        return 1;
    }

    context->id = pmemobj_root(POOL(context), context->region_config.root_size).pool_uuid_lo;

    return 0;
}

static int create(PmBackendContext *context)
{
    context->data = pmemobj_create(context->region_config.file_path, "key", PMEMOBJ_MIN_POOL, 0666);
    if (context->data == NULL)
    {
        return 1;
    }

    context->id = pmemobj_root(POOL(context), context->region_config.root_size).pool_uuid_lo;

    return 0;
}

static int p_close(PmBackendContext *context)
{
    pmemobj_close(POOL(context));
    return 0;
}

static pm_region_offset get_root(PmBackendContext *context)
{
    return pmemobj_root(POOL(context), context->region_config.root_size).off;
}

static pm_region_offset p_malloc(PmBackendContext *context, size_t size)
{
    PMEMoid oid = {};
    pmemobj_zalloc(POOL(context), &oid, size, 0);
    return oid.off;
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
}

PmBackend PMDK_BACKEND = {
    .open = open,
    .create = create,
    .close = p_close,
    .get_root = get_root,
    .malloc = p_malloc,
    .read_object = read_object,
    .write_object = write_object};