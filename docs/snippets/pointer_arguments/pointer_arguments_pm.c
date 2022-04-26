#include "stdlib.h"


void modify_data(int *ptr)
{
    // it will be detected that ptr is a PM pointer
    *ptr = 1;
}

int main(int argc, char const *argv[])
{

    int *ptr = (int *)pm_alloc(sizeof(int));

    if (ptr == NULL)
    {
        exit(1);
    }

    modify_data(ptr);

    pm_free(ptr);
    return 0;
}
