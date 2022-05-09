#include "stdlib.h"
#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

struct Node;

struct Node
{
    struct Node *next;
    int value;
};

int main(int argc, char const *argv[])
{
    uint32_t region_id;
    struct Node *node1 = (struct Node *)nvm_alloc(sizeof(struct Node), region_id);
    if (node1 == NULL)
    {
        exit(1);
    }

    node1->value = 1;

    struct Node *node2 = (struct Node *)nvm_alloc(sizeof(struct Node), region_id);
    if (node2 == NULL)
    {
        exit(1);
    }

    node2->value = 2;

    node1->next = node2;

    return 0;
}
