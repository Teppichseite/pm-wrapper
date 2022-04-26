#include "stdlib.h"

int main(int argc, char const *argv[])
{
    int array_size = 20;

    int *array = (int *)malloc(sizeof(int) * array_size);

    if (array == NULL)
    {
        exit(1);
    }

    for (int i = 0; i < array_size; i++)
    {
        array[i] = i;
    }

    free(array);
    return 0;
}
