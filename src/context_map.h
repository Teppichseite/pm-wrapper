#include "pm_wrapper.h"
#include <stdbool.h>

void init_context_map();

PmBackendContext *get_context(pm_region_reference_id reference_id);

void insert_context(pm_region_reference_id reference_id, PmBackendContext *context);

void remove_context(pm_region_reference_id reference_id);