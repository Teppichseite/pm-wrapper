#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

#define SIZE 10000

struct holder {
  int values[SIZE];
};

static struct holder *root;

int pm_store_init(char *path) {
  PmRegionConfig main_region_config = {.file_path = path,
                                       .root_size = sizeof(struct holder)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  root = (struct holder *)pm_get_root();
}

void pm_store_run() {
  int rounds = 1000;
  for (int i = 0; i < SIZE * rounds; i++) {
    root->values[i % rounds] = i;
  }
}

void pm_store_close() { pm_close(); }

int main() {
  pm_store_init("t");
  pm_store_run();
  pm_store_close();
  return 0;
}