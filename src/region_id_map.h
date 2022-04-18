#include "pm_wrapper.h"
#include <stdbool.h>

void rim_init(PmBackend backend, PmBackendContext *main_context, bool created);

pm_region_reference_id rim_get_reference_id(pm_region_id region_id);

pm_region_reference_id rim_register_region(pm_region_id region_id);