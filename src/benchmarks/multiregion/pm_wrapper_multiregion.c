#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

#define SIZE 10000

struct holder {
  int values[SIZE];
};

static struct holder *root1;
static struct holder *root2;

int pm_store_init(char *path, char *path1, char *path2) {
  PmRegionConfig main_region_config = {.file_path = path,
                                       .root_size = sizeof(struct holder)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  PmRegionConfig region_config1 = {.file_path = path1,
                                   .root_size = sizeof(struct holder)};

  pm_region_reference_id ref1 = pm_init_reg(region_config1);

  PmRegionConfig region_config2 = {.file_path = path2,
                                   .root_size = sizeof(struct holder)};

  pm_region_reference_id ref2 = pm_init_reg(region_config2);

  root1 = (struct holder *)pm_get_root_reg(ref1);
  root2 = (struct holder *)pm_get_root_reg(ref2);
}

void pm_store_run() {
  int rounds = 1;
  for (int i = 0; i < SIZE * rounds; i++) {
    int *var_HMFQV3X2Ep_0;
    int var_HMFQV3X2Ep_1;
    int var_HMFQV3X2Ep_2;
    ((var_HMFQV3X2Ep_0 =
          ((int *)(((((int *)(((char *)((root1))) + 0)))) + (i % rounds)))),
     (var_HMFQV3X2Ep_1 = *((int *)(pm_read_object(var_HMFQV3X2Ep_0)))),
     (var_HMFQV3X2Ep_2 = (i)),
     pm_write_object(var_HMFQV3X2Ep_0, ((char *)(&var_HMFQV3X2Ep_2)),
                     sizeof(int)),
     var_HMFQV3X2Ep_2);
    int *var_HMFQV3X2Ep_3;
    int var_HMFQV3X2Ep_4;
    int var_HMFQV3X2Ep_5;
    ((var_HMFQV3X2Ep_3 =
          ((int *)(((((int *)(((char *)((root2))) + 0)))) + (i % rounds)))),
     (var_HMFQV3X2Ep_4 = *((int *)(pm_read_object(var_HMFQV3X2Ep_3)))),
     (var_HMFQV3X2Ep_5 = (i)),
     pm_write_object(var_HMFQV3X2Ep_3, ((char *)(&var_HMFQV3X2Ep_5)),
                     sizeof(int)),
     var_HMFQV3X2Ep_5);
  }
}

void pm_store_close() { pm_close(); }