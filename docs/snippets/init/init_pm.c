#include "../../../src/pm_wrapper.h"
#include "../../../src/pmdk_backend.h"

int main(int argc, char const *argv[])
{
    PmRegionConfig main_region_config = {
        .file_path = "./test",
        .root_size = sizeof(int)};

    PmWrapperConfig config = {
        .backend = &PMDK_BACKEND,
        .main_region_config = main_region_config};

    if (pm_init(config) != 0)
    {
        exit(1);
    };

    // returns a pm pointer
    int *root = (int *)pm_get_root();

    // pm pointer can be used like a normal pointer
    *root = 2;

    pm_close();
    return 0;
}
