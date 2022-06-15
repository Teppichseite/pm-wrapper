#include <stdio.h>
#define PM __attribute__((pointer_type(1)))
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

struct Test {
  int a;
  int b[];
};

int main(int argc, char *argv[]) {

  struct Test *root = (struct Test *)pm_get_root();

  int a = root->b[0];

  return 0;
}