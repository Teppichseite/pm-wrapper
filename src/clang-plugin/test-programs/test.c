#include <stdio.h>
#define PM __attribute__((pointer_type(1)))
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

struct A {
  int b;
};

int main(int argc, char *argv[]) {

  struct A *t = (struct A *)pm_get_root();

  int i = -1;

  return 0;
}