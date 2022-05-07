#include "stdlib.h"
#include <libpmemobj.h>

struct Node;

struct Node
{
    TOID(struct Node) *next;
    int value;
};

struct Foo
{
    int test;
};


int main(int argc, char const *argv[])
{
    PMEMobjpool *pool;
    TOID(struct Node) node;

    struct Node *node2 = (struct Node *)pm_alloc(sizeof(struct Node));
    if (node2 == NULL)
    {
        exit(1);
    }

    node2->value = 2;

    // After program restart the pointer is still completely valid
    node1->next = node2;

    return 0;
}
