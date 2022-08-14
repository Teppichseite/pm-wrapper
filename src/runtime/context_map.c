#include "context_map.h"
#include "./hashmap/hashmap.h"
#include "pm_wrapper.h"
#include <stdio.h>
#include <stdlib.h>

static HASHMAP(pm_region_reference_id, PmBackendContext) context_map;

static PmBackendContext *main_context = NULL;

static PmBackendContext **contexts;

static size_t reference_id_hash(const pm_region_reference_id *id) {
  return hashmap_hash_default(&id, sizeof(pm_region_reference_id *));
}

static int reference_id_cmp(const pm_region_reference_id *id1,
                            const pm_region_reference_id *id2) {
  return id1 == id2 ? 0 : 1;
}

void cm_init() {
  hashmap_init(&context_map, reference_id_hash, reference_id_cmp);
  contexts =
      (PmBackendContext **)malloc((2 << 16) * sizeof(PmBackendContext *));
}

PmBackendContext *cm_get_context(pm_region_reference_id reference_id) {
  if (reference_id == MAIN_REGION_REFERENCE_ID && main_context) {
    return main_context;
  }

  return contexts[reference_id];
}

int cm_insert_context(pm_region_reference_id reference_id,
                      PmBackendContext *context) {

  if (reference_id == MAIN_REGION_REFERENCE_ID) {
    main_context = context;
  }

  contexts[reference_id] = context;
  return 0;
}

void cm_remove_context(pm_region_reference_id reference_id) {}

void cm_free() { free(contexts); }