#include "region_id_map.h"

#define REGION_ID_MAP_ENTRY_COUNT (0x1FFFF)

#define BACKEND_WRITE(field, value) backend.write_object( \
    main_context,                                         \
    &(field),                                             \
    (char *)(&(value)),                                   \
    sizeof(value));

/**
 * @brief Simple hashtable which uses linear probing
 */

typedef struct RegionIdMapEntry
{
    pm_region_id key;
    pm_region_reference_id value;
} RegionIdMapEntry;

typedef struct RegionIdMap
{
    pm_region_reference_id current_ref_id;
    RegionIdMapEntry entries[];
} RegionIdMap;

static PmBackend backend;
static PmBackendContext *main_context;

static RegionIdMap *region_id_map;

static size_t hash_key(pm_region_id key)
{
    return key % REGION_ID_MAP_ENTRY_COUNT;
}

void rim_init(PmBackend pm_backend, PmBackendContext *pm_main_context, bool created)
{
    backend = pm_backend;
    main_context = pm_main_context;

    PmMainRegionRoot *root = (PmMainRegionRoot *)backend.read_object(
        main_context,
        backend.get_root(main_context));

    if (!created)
    {
        region_id_map = (RegionIdMap *)backend.read_object(main_context, root->region_id_table);
        return;
    }

    pm_region_offset rim_offset = backend.calloc(
        main_context,
        sizeof(RegionIdMap) + REGION_ID_MAP_ENTRY_COUNT * sizeof(RegionIdMapEntry));

    region_id_map = (RegionIdMap *)backend.read_object(main_context, rim_offset);

    // TODO: Use transaction
    pm_region_reference_id initial_ref_id = 0;
    BACKEND_WRITE(root->region_id_table, rim_offset);
    BACKEND_WRITE(region_id_map->current_ref_id, initial_ref_id);

    rim_register_region(pm_main_context->id);
}

pm_region_reference_id rim_get_reference_id(pm_region_id region_id)
{
    size_t hash = hash_key(region_id);
    while (true)
    {
        RegionIdMapEntry *entry = &region_id_map->entries[hash];

        if (entry->value == 0)
        {
            return 0;
        }

        if (entry->key == region_id)
        {
            return entry->value;
        }

        hash = (hash + 1) % REGION_ID_MAP_ENTRY_COUNT;
    }

    return 0;
}

pm_region_reference_id rim_register_region(pm_region_id region_id)
{
    size_t hash = hash_key(region_id);
    while (true)
    {
        RegionIdMapEntry *entry = &region_id_map->entries[hash];

        if (entry->value == 0)
        {
            // TODO: Use transaction
            pm_region_reference_id new_ref_id = region_id_map->current_ref_id + 1;
            BACKEND_WRITE(region_id_map->current_ref_id, new_ref_id);
            BACKEND_WRITE(entry->key, region_id);
            BACKEND_WRITE(entry->value, new_ref_id);

            return region_id_map->current_ref_id;
        }

        hash = (hash + 1) % REGION_ID_MAP_ENTRY_COUNT;
    }

    return 0;
}