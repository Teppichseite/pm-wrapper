#include <libpmemobj.h>

void modify_data(PMEMoid pm_ptr)
{
    int *ptr = (int *)pmemobj_direct(pm_ptr);
    *ptr = 1;
}

// Alternative way
void modify_data_oid(PMEMoid oid)
{
    int *ptr = (int *)pmemobj_direct(oid);
    *ptr = 1;
}

int main(int argc, char const *argv[])
{
    PMEMobjpool *pool;

    PMEMoid oid = {};
    pmemobj_zalloc(pool, &oid, sizeof(int), 0);

    modify_data(oid);

    return 0;
}
