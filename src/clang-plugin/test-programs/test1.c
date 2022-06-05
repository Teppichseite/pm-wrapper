#define PM __attribute__((pointer_type(1)))
#include "../../runtime/pm_wrapper.h"
#include <stdlib.h>

int *test(int *ptr) { return ptr; }

int main() {
  test(0);
  int *(*init)(int *p) = test;
  int *ptr = init(0);
}