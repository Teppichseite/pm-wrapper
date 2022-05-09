#include "stdlib.h"
#include <libpmemobj.h>

struct Node;

POBJ_LAYOUT_BEGIN(node_store);
POBJ_LAYOUT_ROOT(node_store, struct Node);
POBJ_LAYOUT_END(node_store);

struct Node
{
    TOID(struct Node)
    next;
    int value;
};

int main(int argc, char const *argv[])
{
    PMEMobjpool *pool;
    TOID(struct Node)
    node1;
    pmemobj_zalloc(pool, &node1, sizeof(struct Node), 0);

    D_RW(node1)->value = 1;

    TOID(struct Node)
    node2;
    pmemobj_zalloc(pool, &node2, sizeof(struct Node), 0);

    D_RW(node2)->value = 2;

    D_RW(node2)->next = node2;

    return 0;
}
