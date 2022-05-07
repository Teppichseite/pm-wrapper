#include "../runtime/pm_wrapper.h"
#include "../runtime/context_map.h"
#include "../backends/pmdk_backend.h"

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

    // int data = 725;
    // pm_write_object(pm_root, (void *)vm_root, (char *)&data, sizeof(int));

    vm_root += 0;

    pm_close_reg(id);

    return 0;
}