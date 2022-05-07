#include <libpmemobj.h>

int main(int argc, char const *argv[])
{
    char *file_path = "./new_region";

    // Each region in PMDK is handled the same, there is no main region
    PMEMobjpool *pop1;

    if (file_exists(file_path) == 0)
    {
        pop1 = pmemobj_open(file_path, "key");
    }
    else
    {
        pop1 = pmemobj_create(file_path, "key", PMEMOBJ_MIN_POOL, 0666);
    }

    if (pop1 == NULL)
    {
        exit(1);
    }

    PMEMoid oid = {};
    pmemobj_zalloc(pop1, &oid, sizeof(int), 0);

    int *ptr = (int *)pmemobj_direct(oid);

    *ptr = 1;

    return 0;
}
