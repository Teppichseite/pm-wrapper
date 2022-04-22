#include "context_map.h"
#include "./hashmap/hashmap.h"
#include "./vector/vector.h"

#define CVECTOR_LOGARITHMIC_GROWTH

static HASHMAP(pm_region_reference_id, PmBackendContext) context_map;
static cvector_vector_type(PmBackendContext *) sorted_contexts;

static size_t reference_id_hash(const pm_region_reference_id *id)
{
    return hashmap_hash_default(id, sizeof(pm_region_reference_id));
}

static int reference_id_cmp(const pm_region_reference_id *id1, const pm_region_reference_id *id2)
{
    return *id1 == *id2 ? 0 : 1;
}

void cm_init()
{
    hashmap_init(&context_map, reference_id_hash, reference_id_cmp);
}

PmBackendContext *cm_get_context(pm_region_reference_id reference_id)
{
    return hashmap_get(&context_map, &reference_id);
}

PmBackendContext *cm_get_context_by_vm_ptr(void *ptr)
{
    int right = cvector_size(sorted_contexts) - 1;
    int left = 0;
    while (true)
    {
        int middle = left + ((right - left) / 2);
        PmBackendContext *context = sorted_contexts[middle];
        void *first_address = context->first_address;
        void *last_address = (void *)((char *)(context->first_address) + context->region_config.size);

        if (ptr < first_address)
        {
            right = middle;
            continue;
        }

        if (ptr > last_address)
        {
            left = middle;
            continue;
        }

        return context;
    }
    return NULL;
}

int compare_contexts(const void *c1, const void *c2)
{
    PmBackendContext *context1 = (PmBackendContext *)c1;
    PmBackendContext *context2 = (PmBackendContext *)c2;

    return context1->first_address - context2->first_address;
}

int cm_insert_context(pm_region_reference_id reference_id, PmBackendContext *context)
{
    hashmap_put(&context_map, &reference_id, context);
    cvector_push_back(sorted_contexts, context);
    qsort(sorted_contexts, cvector_size(sorted_contexts), sizeof(PmBackendContext *), compare_contexts);
    return 0;
}

void cm_remove_context(pm_region_reference_id reference_id)
{
    hashmap_remove(&context_map, &reference_id);
    for (int i = 0; i < cvector_size(sorted_contexts); i++)
    {
        if (sorted_contexts[i]->reference_id != reference_id)
        {
            continue;
        }

        cvector_erase(sorted_contexts, i);
    }
}

void cm_free()
{
    cvector_free(sorted_contexts);
    hashmap_cleanup(&context_map);
}