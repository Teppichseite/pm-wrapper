#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

int main(int argc, char const *argv[])
{
    uint32_t id;

    int array_size = 20;

    int *array = (int *)nvm_alloc(sizeof(int) * array_size, id);

    if (array == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < array_size; i++)
    {
        array[i] = i;
    }

    return 0;
}