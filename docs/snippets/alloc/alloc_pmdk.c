#include <libpmemobj.h>

int main(int argc, char const *argv[])
{
    PMEMobjpool *pool;

    PMEMoid oid = {};
    pmemobj_zalloc(pool, &oid, sizeof(int), 0);

    int *ptr = (int *)pmemobj_direct(oid);

    *ptr = 1;

    return 0;
}