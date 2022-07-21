#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

#define SIZE 1000

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
  int rounds = 1;
  for (int i = 0; i < 1000 * rounds; i++) {
    int *var_al9RW0qoG7_0;
    int var_al9RW0qoG7_1;
    int var_al9RW0qoG7_2;
    ((var_al9RW0qoG7_0 =
          ((int *)(((((int *)(((char *)((root))) + 0)))) + (i % rounds)))),
     (var_al9RW0qoG7_1 = *((int *)(pm_read_object(var_al9RW0qoG7_0)))),
     (var_al9RW0qoG7_2 = (i)),
     pm_write_object(var_al9RW0qoG7_0, ((char *)(&var_al9RW0qoG7_2)),
                     sizeof(int)),
     var_al9RW0qoG7_2);
  }
}

void pm_store_close() { pm_close(); }