#include "stdlib.h"

int main(int argc, char const *argv[])
{
    int *ptr = (int *)malloc(sizeof(int));

    if (ptr == NULL)
    {
        exit(1);
    }

    *ptr = 1;

    free(ptr);
    return 0;
}
