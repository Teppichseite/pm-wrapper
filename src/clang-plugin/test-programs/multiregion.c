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

  pm_region_reference_id ref2 = pm_init_reg(region_config1);

  root1 = (struct holder *)pm_get_root_reg(ref1);
  root2 = (struct holder *)pm_get_root_reg(ref2);
}

void pm_store_run() {
  int rounds = 1000;
  for (int i = 0; i < SIZE * rounds; i++) {
    root1->values[i % rounds] = i;
    root2->values[i % rounds] = i;
  }
}

void pm_store_close() { pm_close(); }

int main() {
  pm_store_init("t", "t", "t");
  pm_store_run();
  pm_store_close();
  return 0;
}