#include "pm_wrapper.h"
#include <stdbool.h>

void cm_init();

PmBackendContext *cm_get_context(pm_region_reference_id reference_id);

void cm_insert_context(pm_region_reference_id reference_id, PmBackendContext *context);

void cm_remove_context(pm_region_reference_id reference_id);

void cm_free();