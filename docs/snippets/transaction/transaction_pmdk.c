#include "stdlib.h"
#include <libpmemobj.h>

struct Node
{
    int value1;
    int value2;
};

POBJ_LAYOUT_BEGIN(node_store);
POBJ_LAYOUT_ROOT(node_store, struct Node);
POBJ_LAYOUT_END(node_store);

int main(int argc, char const *argv[])
{
    PMEMobjpool *pool;
    TOID(struct Node)
    node;
    pmemobj_zalloc(pool, &node, sizeof(struct Node), 0);

    TX_BEGIN(pool)
    {
        struct Node new_node = {
            .value1 = 2,
            .value2 = 3};
        TX_MEMCPY(D_RW(node), &new_node, sizeof(new_node));
    }
    TX_END

    return 0;
}
