#include "../../runtime/context_map.h"
#include "../../runtime/pm_wrapper.h"
#include <stddef.h>
#include <stdio.h>

static int data1 = 300;
static int data2 = 400;

struct TestStruct {
  int value;
  int *ptr;
};

int first_run(PmBackend *backend) {
  PmWrapperConfig config = {
      .backend = backend,
      .main_region_config = {.file_path = "test1", .root_size = 1}};

  if (pm_init(config) != 0) {
    return 1;
  };

  PmRegionConfig region_config1 = {.file_path = "test2",
                                   .root_size = sizeof(struct TestStruct)};

  pm_region_reference_id id = pm_init_reg(region_config1);

  struct TestStruct *pm_root = (struct TestStruct *)pm_get_root_reg(id);
  struct TestStruct *vm_root = (struct TestStruct *)pm_read_object(pm_root);

  pm_write_object(((char *)pm_root) + offsetof(struct TestStruct, value),
                  (char *)&data1, sizeof(int));

  int *pm_ptr = (int *)pm_alloc_reg(sizeof(int), id);

  pm_write_object(((char *)pm_root) + offsetof(struct TestStruct, ptr),
                  (char *)&pm_ptr, sizeof(pm_ptr));

  int *vm_ptr = (int *)pm_read_object(pm_ptr);

  pm_write_object(pm_ptr, (char *)&data2, sizeof(int));

  pm_close_reg(id);
  pm_close();
  return 0;
}

int second_run(PmBackend *backend) {
  PmWrapperConfig config = {
      .backend = backend,
      .main_region_config = {.file_path = "test1", .root_size = 1}};

  if (pm_init(config) != 0) {
    return 1;
  };

  PmRegionConfig region_config1 = {.file_path = "test2",
                                   .root_size = sizeof(struct TestStruct)};

  pm_region_reference_id id = pm_init_reg(region_config1);

  struct TestStruct *pm_root = (struct TestStruct *)pm_get_root_reg(id);
  struct TestStruct *vm_root = (struct TestStruct *)pm_read_object(pm_root);
  int *vm_ptr = (int *)pm_read_object(vm_root->ptr);
  if (vm_root->value == data1) {
    printf("\x1B[32mTest 1 succeeded \x1B[0m\n");
  } else {
    printf("\x1B[31mTest 1 failed \x1B[0m\n");
    return 1;
  }

  if (*vm_ptr == data2) {
    printf("\x1B[32mTest 2 succeeded \x1B[0m\n");
  } else {
    printf("\x1B[31mTest 2 failed \x1B[0m\n");
    return 1;
  }

  pm_close_reg(id);
  pm_close();
  return 0;
}

int multiple_region_test(PmBackend *backend) {

  if (first_run(backend)) {
    return 1;
  };

  if (second_run(backend)) {
    return 1;
  };

  return 0;
}