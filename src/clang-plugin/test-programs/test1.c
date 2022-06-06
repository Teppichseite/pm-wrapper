#define PM __attribute__((pointer_type(1)))
#include "../../runtime/pm_wrapper.h"
#include <stdio.h>
#include <stdlib.h>

int *p;

int *test(int i) {
  if (i == 10) {
    return NULL;
  }

  return (int *)pm_get_root();
}

int main() {
  int *v = test(10);
  int *k = v;
  p = k;
  return 0;
}