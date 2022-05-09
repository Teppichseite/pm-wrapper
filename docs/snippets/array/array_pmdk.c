#include "stdlib.h"
#include <libpmemobj.h>

int main(int argc, char const *argv[])
{
    int array_size = 20;

    PMEMobjpool *pool;

    PMEMoid oid = {};
    pmemobj_zalloc(pool, &oid, sizeof(int), 0);

    int *array = (int *)pmemobj_direct(oid);

    for (int i = 0; i < array_size; i++)
    {
        array[i] = i;
    }

    return 0;
}
