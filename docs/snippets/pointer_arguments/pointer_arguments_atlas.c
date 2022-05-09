#include "stdlib.h"
#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

void modify_data(int *ptr)
{
    *ptr = 1;
}

int main(int argc, char const *argv[])
{
    uint32_t region_id;

    int *ptr = (int *)nvm_alloc(sizeof(int), region_id);

    if (ptr == NULL)
    {
        exit(1);
    }

    modify_data(ptr);

    free(ptr);
    return 0;
}
