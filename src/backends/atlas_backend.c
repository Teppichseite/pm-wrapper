#include "../runtime/pm_wrapper.h"
#include "atlas_backend.h"
#include <stdlib.h>
#include "../../../Atlas/runtime/include/atlas_api.h"
#include "../../../Atlas/runtime/include/atlas_alloc.h"
#include <stdio.h>
#include <string.h>

static int init()
{
    NVM_Initialize();
    return 0;
}

static int open_or_create(PmBackendContext *context, bool *created_new)
{
    int created_new_int = 0;
    uint32_t id = NVM_FindOrCreateRegion(context->region_config.file_path, O_RDWR, &created_new_int);
    context->id = id;

    if (created_new_int)
    {
        void *root = nvm_calloc(context->region_config.root_size, 1, id);
        NVM_SetRegionRoot(id, root);
    }

    *created_new = !!created_new_int;

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

static pm_region_offset p_alloc(PmBackendContext *context, size_t size)
{
    return (pm_region_offset)nvm_alloc(size, context->id);
}

static pm_region_offset p_calloc(PmBackendContext *context, size_t size)
{
    return (pm_region_offset)nvm_calloc(size, 1, context->id);
}

static void p_free(PmBackendContext *context, pm_region_offset offset)
{
    nvm_free((void *)offset);
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
    .open_or_create = open_or_create,
    .close = p_close,
    .finalize = finalize,
    .get_root = get_root,
    .alloc = p_alloc,
    .calloc = p_calloc,
    .free = p_free,
    .read_object = read_object,
    .write_object = write_object};