#include "stdlib.h"
#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

struct Node
{
    int value1;
    int value2;
};

int main(int argc, char const *argv[])
{
    uint32_t region_id;
    struct Node *node = (struct Node *)nvm_alloc(sizeof(struct Node), region_id);
    if (node == NULL)
    {
        exit(1);
    }

    NVM_BEGIN_DURABLE();

    node->value1 = 2;
    node->value2 = 3;

    NVM_END_DURABLE();

    return 0;
}
