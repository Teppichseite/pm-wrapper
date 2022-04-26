#include "stdlib.h"

struct Node;

struct Node
{
    struct Node *next;
    int value;
};

int main(int argc, char const *argv[])
{
    struct Node *node1 = (struct Node *)pm_alloc(sizeof(struct Node));
    if (node1 == NULL)
    {
        exit(1);
    }

    node1->value = 1;

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
