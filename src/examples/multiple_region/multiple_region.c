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
        .root_size = 1};

    pm_region_reference_id id = pm_init_reg(region_config1);

    void *root = pm_get_root_reg(id);

    root++;
    return 0;
}