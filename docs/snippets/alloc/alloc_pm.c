#include "stdlib.h"

int main(int argc, char const *argv[])
{
    // Returns a PM pointer
    int *ptr = (int *)pm_alloc(sizeof(int));

    if (ptr == NULL)
    {
        exit(1);
    }

    // PM pointer can be accessed like a normal pointer
    *ptr = 1;

    pm_free(ptr);
    return 0;
}
