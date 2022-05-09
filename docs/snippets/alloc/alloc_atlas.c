#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

int main(int argc, char const *argv[])
{
    uint32_t region_id;

    int *ptr = (int *)nvm_alloc(sizeof(int), region_id);

    if (ptr == NULL)
    {
        exit(1);
    }

    *ptr = 1;

    return 0;
}