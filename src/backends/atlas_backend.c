#include "../runtime/pm_wrapper.h"
#include "atlas_backend.h"
#include <stdlib.h>
#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"
#include <stdio.h>

static int init()
{
    NVM_Initialize();
    return 0;
}

static int open(PmBackendContext *context)
{
    uint32_t id = NVM_FindRegion(context->region_config.file_path, O_RDWR);
    context->id = id;
    return 0;
}

static int create(PmBackendContext *context)
{
    uint32_t id = NVM_FindOrCreateRegion(context->region_config.file_path, O_RDWR, NULL);
    context->id = id;
    return 0;
}

static void p_close(PmBackendContext *context)
{
    NVM_CloseRegion(context->id);
}

static void finalize()
{
    NVM_Finalize();
}

static pm_region_offset get_root(PmBackendContext *context)
{
    return (pm_region_offset)NVM_GetRegionRoot(context->id);
}

static pm_region_offset p_malloc(PmBackendContext *context, size_t size)
{
    return (pm_region_offset)nvm_alloc(size, context->id);
}

static void *read_object(PmBackendContext *context, pm_region_offset offset)
{
    return (void *)offset;
}

static void write_object(PmBackendContext *context, void *dst, char *data, size_t len)
{
    NVM_BEGIN_DURABLE();
    memcpy(dst, data, len);
    NVM_END_DURABLE();
}

PmBackend ATLAS_BACKEND = {
    .init = init,
    .open = open,
    .create = create,
    .close = p_close,
    .finalize = finalize,
    .get_root = get_root,
    .alloc = p_malloc,
    .read_object = read_object,
    .write_object = write_object};