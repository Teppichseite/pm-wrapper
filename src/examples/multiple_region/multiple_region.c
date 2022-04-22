#include "../../pm_wrapper.h"
#include "../../pmdk_backend.h"

int main(int argc, char *argv[])
{
    PmWrapperConfig config = {
        .backend = &PMDK_BACKEND,
        .main_region_config = {
            .file_path = "./test",
            .root_size = 1}};

    if (pm_init(config) != 0)
    {
        return 1;
    };

    PmRegionConfig region_config1 = {
        .file_path = "./test1",
        .root_size = sizeof(int)};

    pm_region_reference_id id = pm_init_reg(region_config1);

    // PM Pointer
    int *pm_root = (int *)pm_get_root_reg(id);

    // VM Pointer
    int *vm_root = (int *)pm_read_object(pm_root);

    int data = 2;

    // With PM Pointer
    pm_write_object(pm_root, &data, sizeof(data));

    // With VM Pointer
    pm_write_object(vm_root, &data, sizeof(data));

    // With VM Pointer without flushing
    *vm_root = 2;

    void *allocated = pm_alloc_reg(sizeof(long), id);

    pm_close_reg(id);
    return 0;
}