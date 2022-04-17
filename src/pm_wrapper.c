#include <stdio.h>
#include <stdbool.h>
#include "pm_wrapper.h"
#include "context_map.h"
#include "region_id_map.h"

static PmWrapperConfig config;

void *construct_pm_ptr(pm_region_reference_id reference_id, pm_region_offset offset)
{
    PmThinPointer thin_ptr = {
        .offset = offset,
        .reference_id = reference_id};
    return thin_ptr.ptr;
}

PmThinPointer destruct_pm_ptr(void *ptr)
{
    PmThinPointer thin_ptr = {
        .ptr = ptr};
    return thin_ptr;
}

pm_region_reference_id pm_init_reg(PmRegionConfig region_config)
{
    PmBackendContext *context = (PmBackendContext *)calloc(
        sizeof(PmBackendContext),
        1);
    context->region_config = region_config;

    if (config.backend->open(context) == 0)
    {
        pm_region_reference_id ref_id = rim_get_reference_id(context->id);
        insert_context(ref_id, context);
        return ref_id;
    }

    if (config.backend->create(context) == 0)
    {
        pm_region_reference_id ref_id = rim_register_region(context->id);
        insert_context(ref_id, context);
        return ref_id;
    }

    return 0;
};

int pm_init(PmWrapperConfig pm_config)
{
    config = pm_config;
    config.main_region_config.root_size += sizeof(PmMainRegionRoot);

    PmBackendContext *main_context = (PmBackendContext *)calloc(
        sizeof(PmBackendContext),
        1);
    main_context->region_config = config.main_region_config;

    bool created = false;
    if (config.backend->open(main_context) != 0)
    {
        if (config.backend->create(main_context) != 0)
        {
            return 1;
        }
        created = true;
    }

    rim_init(*(config.backend), main_context, created);
    init_context_map();
    insert_context(MAIN_REGION_REFERENCE_ID, main_context);

    return 0;
};

void *pm_get_root_reg(pm_region_reference_id reference_id)
{
    PmBackendContext *context = get_context(reference_id);
    pm_region_offset offset = config.backend->get_root(context);
    return construct_pm_ptr(reference_id, offset);
};

void *pm_get_root()
{
    PmMainRegionRoot *root = (PmMainRegionRoot *)pm_read_object(pm_get_root_reg(1));
    return construct_pm_ptr(MAIN_REGION_REFERENCE_ID, root->program_root);
}

void *pm_alloc_reg(int size, pm_region_reference_id reference_id)
{
    PmBackendContext *context = get_context(reference_id);
    pm_region_offset offset = config.backend->malloc(context, size);
    return construct_pm_ptr(reference_id, offset);
};

void *pm_alloc(int size)
{
    return pm_alloc_reg(size, MAIN_REGION_REFERENCE_ID);
};

void *pm_read_object(void *ptr)
{
    PmThinPointer thin_ptr = destruct_pm_ptr(ptr);
    PmBackendContext *context = get_context(thin_ptr.reference_id);
    return config.backend->read_object(context, thin_ptr.offset);
};

void pm_write_object(void *ptr, char *data, int size){

};

int pm_close_reg(pm_region_reference_id reference_id)
{
    return 0;
};
int pm_close()
{
    return 0;
};
