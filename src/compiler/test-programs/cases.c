#include <stdio.h>
#define PM __attribute__((pointer_type(1)))
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

struct A {
  int a;
  int b;
  int c[];
};

int main(int argc, char *argv[]) {

  int *int_t = (int *)pm_get_root();

  (*int_t) = 10;

  struct A *struct_t = (struct A *)pm_get_root();
  struct_t->b = 20;
  int *addr2 = struct_t->c;

  int *arr_t = (int *)pm_get_root();
  arr_t[10] = 30;

  int *var_I9DJygvdPB_0;
  int var_I9DJygvdPB_1;
  int var_I9DJygvdPB_2;
  ((var_I9DJygvdPB_0 = ((int *)((int_t) + 0))),
   (var_I9DJygvdPB_1 = *((int *)(pm_read_object(var_I9DJygvdPB_0)))),
   (var_I9DJygvdPB_2 = 10),
   pm_write_object(var_I9DJygvdPB_0, ((char *)(&var_I9DJygvdPB_2)),
                   sizeof(int)),
   var_I9DJygvdPB_2);

  struct A *struct_t = (struct A *)pm_get_root();
  int *var_I9DJygvdPB_3;
  int var_I9DJygvdPB_4;
  int var_I9DJygvdPB_5;
  ((var_I9DJygvdPB_3 = ((int *)(((char *)struct_t) + 4))),
   (var_I9DJygvdPB_4 = *((int *)(pm_read_object(var_I9DJygvdPB_3)))),
   (var_I9DJygvdPB_5 = 20),
   pm_write_object(var_I9DJygvdPB_3, ((char *)(&var_I9DJygvdPB_5)),
                   sizeof(int)),
   var_I9DJygvdPB_5);
  int *addr2 = ((int *)(((char *)(struct_t)) + 8));

  int *arr_t = (int *)pm_get_root();
  int *var_I9DJygvdPB_6;
  int var_I9DJygvdPB_7;
  int var_I9DJygvdPB_8;
  ((var_I9DJygvdPB_6 = ((int *)((arr_t) + 10))),
   (var_I9DJygvdPB_7 = *((int *)(pm_read_object(var_I9DJygvdPB_6)))),
   (var_I9DJygvdPB_8 = 30),
   pm_write_object(var_I9DJygvdPB_6, ((char *)(&var_I9DJygvdPB_8)),
                   sizeof(int)),
   var_I9DJygvdPB_8);

  return 0;
}