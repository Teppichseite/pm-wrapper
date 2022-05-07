#include "context_map.h"
#include "./hashmap/hashmap.h"
#include <stdio.h>

static HASHMAP(pm_region_reference_id, PmBackendContext) context_map;

static size_t reference_id_hash(const pm_region_reference_id *id)
{
    return hashmap_hash_default(&id, sizeof(pm_region_reference_id *));
}

static int reference_id_cmp(const pm_region_reference_id *id1, const pm_region_reference_id *id2)
{
    return id1 == id2 ? 0 : 1;
}

void cm_init()
{
    hashmap_init(&context_map, reference_id_hash, reference_id_cmp);
}

PmBackendContext *cm_get_context(pm_region_reference_id reference_id)
{
    return hashmap_get(&context_map, (pm_region_reference_id *)(unsigned long)reference_id);
}

int cm_insert_context(pm_region_reference_id reference_id, PmBackendContext *context)
{
    hashmap_put(&context_map, (pm_region_reference_id *)(unsigned long)reference_id, context);
    return 0;
}

void cm_remove_context(pm_region_reference_id reference_id)
{
    hashmap_remove(&context_map, (pm_region_reference_id *)(unsigned long)reference_id);
}

void cm_free()
{
    hashmap_cleanup(&context_map);
}