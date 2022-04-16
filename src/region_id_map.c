#include "region_id_map.h"

typedef struct RegionIdMap
{
    pm_region_reference_id current_ref_id;
    pm_region_id region_ids[];
} RegionIdMap;

static PmBackend backend;
static PmBackendContext *main_context;

static RegionIdMap *region_id_map;

void rim_init(PmBackend pm_backend, PmBackendContext *pm_main_context, bool created)
{
    backend = pm_backend;
    main_context = pm_main_context;

    PmMainRegionRoot *root = (PmMainRegionRoot *)backend.read_object(
        main_context,
        backend.get_root(main_context));

    if (!created)
    {
        return;
    }

    pm_region_offset rim_offset = backend.malloc(
        main_context,
        sizeof(RegionIdMap) + (1 << 16) * sizeof(pm_region_id));

    root->region_id_table = rim_offset;

    region_id_map = (RegionIdMap *)backend.read_object(main_context, rim_offset);
    region_id_map->current_ref_id = 0;
}

pm_region_id rim_get_reference_id(pm_region_id region_id)
{
    for (int i = 0; i <= region_id_map->current_ref_id; i++)
    {
        pm_region_id current_region_id = region_id_map->region_ids[i];
        if (current_region_id == region_id)
        {
            return i;
        }
    }
    return 0;
}

pm_region_reference_id rim_register_region(pm_region_id region_id)
{
    pm_region_reference_id new_ref_id = region_id_map->current_ref_id + 1;
    region_id_map->region_ids[new_ref_id] = region_id;
    return new_ref_id;
}