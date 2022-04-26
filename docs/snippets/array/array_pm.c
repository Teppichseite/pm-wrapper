#include "stdlib.h"

int main(int argc, char const *argv[])
{
    int array_size = 20;

    int *array = (int *)pm_alloc(sizeof(int) * array_size);

    if (array == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < array_size; i++)
    {
        // Array access looks the same for PM pointers as for VM pointers
        array[i] = i;
    }

    pm_free(array);
    return 0;
}
