#include "stdlib.h"

#define TX

struct Node
{
    int value1;
    int value2;
};

int main(int argc, char const *argv[])
{
    struct Node *node = (struct Node *)pm_alloc(sizeof(struct Node));
    if (node == NULL)
    {
        exit(1);
    }

    TX
    {
        // TBD: Changed address have to be logged
        // In the optimal case it happens automatically
        node->value1 = 2;
        node->value2 = 3;
    }

    return 0;
}
