#include "../src/ex_common.h"
#include <libpmemobj.h>

int main(int argc, char const *argv[])
{
    char *file_path = "./test";
    PMEMobjpool *pop;

    if (file_exists(file_path) == 0)
    {
        pop = pmemobj_open(file_path, "key");
    }
    else
    {
        pop = pmemobj_create(file_path, "key", PMEMOBJ_MIN_POOL, 0666);
    }

    if (pop == NULL)
    {
        exit(1);
    }

    pmemobj_close(pop);
    return 0;
}