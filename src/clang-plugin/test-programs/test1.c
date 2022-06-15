#include <stdio.h>
#define PM __attribute__((pointer_type(1)))
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

struct Test {
  int a;
  int b;
};

int main(int argc, char *argv[]) {

  const char *command = argv[1];

  PmRegionConfig main_region_config = {.file_path = "./test",
                                       .root_size = sizeof(struct Test)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  struct Test *root = (struct Test *)pm_get_root();

  if (command[0] == 'g') {
    printf("%i", root->b += 10);
  }

  pm_close();

  return 0;
}