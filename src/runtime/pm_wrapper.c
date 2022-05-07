#include <stdio.h>
#include <stdbool.h>
#include "../include/pm_wrapper.h"
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

    if (context == NULL)
    {
        return 0;
    }

    context->region_config = region_config;

    bool inital_creation = false;
    if (config.backend->open(context) != 0)
    {
        if (config.backend->create(context) != 0)
        {
            return 1;
        }
        inital_creation = true;
    }

    pm_region_reference_id ref_id =
        inital_creation
            ? rim_register_region(context->id)
            : rim_get_reference_id(context->id);

    if (ref_id == 0)
    {
        return 0;
    }

    if (cm_insert_context(ref_id, context) != 0)
    {
        return 0;
    }

    return ref_id;
};

int pm_init(PmWrapperConfig pm_config)
{
    config = pm_config;
    config.main_region_config.root_size += sizeof(PmMainRegionRoot);

    PmBackendContext *main_context = (PmBackendContext *)calloc(
        sizeof(PmBackendContext),
        1);

    if (main_context == NULL)
    {
        return 1;
    }

    main_context->region_config = config.main_region_config;

    config.backend->init();

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
    cm_init();
    cm_insert_context(MAIN_REGION_REFERENCE_ID, main_context);

    PmMainRegionRoot *root = (PmMainRegionRoot *)pm_read_object(
        pm_get_root_reg(MAIN_REGION_REFERENCE_ID));

    pm_region_offset program_root_offset = config.backend->alloc(
        main_context, sizeof(config.main_region_config.root_size - sizeof(PmMainRegionRoot)));

    config.backend->write_object(
        main_context,
        (void *)&root->program_root,
        (char *)&program_root_offset,
        sizeof(program_root_offset));

    return 0;
}

void *pm_get_root_reg(pm_region_reference_id reference_id)
{
    PmBackendContext *context = cm_get_context(reference_id);
    if (context == NULL)
    {
        return NULL;
    }

    pm_region_offset offset = config.backend->get_root(context);
    return construct_pm_ptr(reference_id, offset);
}

void *pm_get_root()
{
    PmMainRegionRoot *root = (PmMainRegionRoot *)pm_read_object(pm_get_root_reg(1));

    if (root == NULL)
    {
        return NULL;
    }

    return construct_pm_ptr(MAIN_REGION_REFERENCE_ID, root->program_root);
}

void *pm_alloc_reg(int size, pm_region_reference_id reference_id)
{
    PmBackendContext *context = cm_get_context(reference_id);
    if (context == NULL)
    {
        return NULL;
    }

    pm_region_offset offset = config.backend->alloc(context, size);
    return construct_pm_ptr(reference_id, offset);
}

void *pm_alloc(int size)
{
    return pm_alloc_reg(size, MAIN_REGION_REFERENCE_ID);
}

void *pm_calloc_reg(int size, pm_region_reference_id reference_id)
{
    PmBackendContext *context = cm_get_context(reference_id);
    if (context == NULL)
    {
        return NULL;
    }

    pm_region_offset offset = config.backend->calloc(context, size);
    return construct_pm_ptr(reference_id, offset);
}

void pm_free(void *ptr)
{
    PmThinPointer thin_ptr = destruct_pm_ptr(ptr);
    PmBackendContext *context = cm_get_context(thin_ptr.reference_id);
    if (context == NULL)
    {
        return;
    }

    config.backend->free(context, thin_ptr.offset);
}

void *pm_calloc(int size)
{
    return pm_calloc_reg(size, MAIN_REGION_REFERENCE_ID);
}

void *pm_read_object(void *ptr)
{
    if (ptr == NULL)
    {
        return NULL;
    }

    PmThinPointer thin_ptr = destruct_pm_ptr(ptr);
    PmBackendContext *context = cm_get_context(thin_ptr.reference_id);

    if (context == NULL)
    {
        return NULL;
    }

    return config.backend->read_object(context, thin_ptr.offset);
};

void pm_write_object(void *pm_ptr, void *vm_ptr, char *data, int size)
{
    PmBackendContext *context = cm_get_context(destruct_pm_ptr(pm_ptr).reference_id);
    config.backend->write_object(context, vm_ptr, data, size);
}

void pm_close_reg(pm_region_reference_id reference_id)
{
    PmBackendContext *context = cm_get_context(reference_id);
    config.backend->close(context);
    cm_remove_context(reference_id);
    free(context);
}

void pm_close()
{
    pm_close_reg(MAIN_REGION_REFERENCE_ID);
    cm_free();
    config.backend->finalize();
}