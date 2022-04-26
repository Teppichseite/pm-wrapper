#include "stdlib.h"

void modify_data(int *ptr)
{
    *ptr = 1;
}

int main(int argc, char const *argv[])
{
    int *ptr = (int *)malloc(sizeof(int));

    if (ptr == NULL)
    {
        exit(1);
    }

    modify_data(ptr);

    free(ptr);
    return 0;
}
