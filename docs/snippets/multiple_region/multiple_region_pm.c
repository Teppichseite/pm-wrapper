#include "../../../src/pm_wrapper.h"
#include "../../../src/pmdk_backend.h"

int main(int argc, char const *argv[])
{
    PmRegionConfig second_region_config = {
        .file_path = "./test",
        .root_size = sizeof(int)};

    // id identifies the region
    pm_region_reference_id id = pm_init_reg(second_region_config);
    if (id == 0)
    {
        exit(1);
    }

    // Returns a pm pointer with a region reference id included
    int *ptr = (int *)pm_alloc_reg(sizeof(int), id);

    *ptr = 1;

    pm_close_reg(id);
    return 0;
}
