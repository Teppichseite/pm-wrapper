#include "pm_wrapper.h"
#include "pmdk_backend.h"
#include "ex_common.h"
#include <libpmemobj.h>
#include <stdlib.h>

static PMEMobjpool *pop;
static PMEMoid root_oid;

static PMEMoid get_oid_by_offset(pm_offset offset)
{
    PMEMoid oid = {
        .pool_uuid_lo = root_oid.pool_uuid_lo,
        .off = offset};

    return oid;
}

static int open(PmWrapperConfig config)
{
    if (file_exists(config.file_path) != 0)
    {
        return 1;
    }

    if ((pop = pmemobj_open(config.file_path, config.key)) == NULL)
    {
        return 1;
    }

    root_oid = pmemobj_root(pop, config.root_size);

    return 0;
}

static int create(PmWrapperConfig config)
{
    if ((pop = pmemobj_create(config.file_path, config.key, PMEMOBJ_MIN_POOL, 0666)) == NULL)
    {
        return 1;
    }

    root_oid = pmemobj_root(pop, config.root_size);

    return 0;
}

static int p_close(PmWrapperConfig config)
{
    pmemobj_close(pop);
    return 0;
}

static pm_offset get_root()
{
    return root_oid.off;
}

static pm_offset p_malloc(size_t size)
{
    PMEMoid oid = get_oid_by_offset(0);
    pmemobj_zalloc(pop, &oid, size, 0);
    return oid.off;
}

static void *read_object(pm_offset offset)
{
    return pmemobj_direct_inline(get_oid_by_offset(offset));
}

static void write_object(void *dst, char *data, size_t len)
{
    pmemobj_memcpy_persist(pop, dst, data, len);
}

const PmBackend PMDK_BACKEND = {
    .open = open,
    .create = create,
    .close = p_close,
    .get_root = get_root,
    .malloc = p_malloc,
    .read_object = read_object,
    .write_object = write_object};