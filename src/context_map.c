#include "context_map.h"
#include "./hashmap/hashmap.h"

static HASHMAP(pm_region_reference_id, PmBackendContext) context_map;

size_t reference_id_hash(const pm_region_reference_id *id1)
{
    return hashmap_hash_default(id1, sizeof(pm_region_reference_id));
}

int reference_id_cmp(const pm_region_reference_id *id1, const pm_region_reference_id *id2)
{
    return *id1 == *id2 ? 0 : 1;
}

void init_context_map()
{
    hashmap_init(&context_map, reference_id_hash, reference_id_cmp);
};

PmBackendContext *get_context(pm_region_reference_id reference_id)
{
    PmBackendContext *context = hashmap_get(&context_map, &reference_id);
    return context;
};

void insert_context(pm_region_reference_id reference_id, PmBackendContext *context)
{
    hashmap_put(&context_map, &reference_id, context);
};

void remove_context(pm_region_reference_id reference_id)
{
    hashmap_remove(&context_map, &reference_id);
};