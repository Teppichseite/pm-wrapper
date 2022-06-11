#include <stdio.h>
#define PM __attribute__((pointer_type(1)))
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

int main(int argc, char *argv[]) {

  const char *command = argv[1];

  PmRegionConfig main_region_config = {.file_path = "./test",
                                       .root_size = sizeof(int)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  int *root = (int *)pm_get_root();

  if (command[0] == 's') {
    printf("inserteed \n");
    int value = 1;
    pm_write_object(root, pm_read_object(root), (char *)&value, sizeof(int));
  }

  if (command[0] == 'g') {
    printf("%i \n", *(int *)pm_read_object(root));
  }

  pm_close();

  return 0;
}