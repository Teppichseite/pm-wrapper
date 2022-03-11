/*
 * btree.c -- implementation of persistent binary search tree
 */

#include "../ex_common.h"
#include <stddef.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "../pm_wrapper.h"
#include "../pmdk_backend.h"

struct btree_node
{
    int64_t key;
    struct btree_node *slots[2];
    char value[];
};

struct btree
{
    struct btree_node *root;
};

static void
btree_insert(struct btree *tree, int64_t key, char *value)
{
    struct btree_node node = {
        .key = key,
        .slots[0] = NULL,
        .slots[1] = NULL};

    size_t value_size = strlen(value) + 1;
    size_t total_size = sizeof(node) + value_size;

    struct btree_node *node_addr = (struct btree_node *)pm_alloc(total_size);

    PM_W(PM_R(node_addr), &node, sizeof(node));
    PM_W(&PM_R(node_addr)->value, value, value_size);

    struct btree_node **dst = &PM_R(tree)->root;
    while (*dst != NULL)
    {
        dst = &PM_R(*dst)->slots[key > PM_R(*dst)->key];
    }

    PM_W(dst, &node_addr, sizeof(node_addr));
}

/*
 * btree_find -- searches for key in the tree
 */
static const char *
btree_find(struct btree *tree, int64_t key)
{
    struct btree_node *node = PM_R(tree)->root;

    while (node != NULL)
    {
        int64_t node_key = PM_R(node)->key;
        if (node_key == key)
            return PM_R(node)->value;
        else
            node = PM_R(node)->slots[key > node_key];
    }

    return NULL;
}

/*
 * btree_foreach -- invoke callback for every node
 */
static void
btree_foreach(struct btree_node *node,
              void (*cb)(struct btree_node *node))
{

    if (node == NULL)
    {
        return;
    }

    btree_foreach(PM_R(node)->slots[0], cb);

    cb(node);

    btree_foreach(PM_R(node)->slots[1], cb);
}

/*
 * btree_node_print -- prints content of the btree node
 */
static void
btree_node_print(struct btree_node *node)
{
    printf("%" PRIu64 " %s\n", PM_R(node)->key, PM_R(node)->value);
}

/*
 * btree_print -- initiates foreach node print
 */
static void
btree_print(struct btree *tree)
{
    btree_foreach(PM_R(tree)->root, btree_node_print);
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("Wrong usage!\n");
        return 1;
    }

    char *path = argv[1];

    PmWrapperConfig config = {
        .key = "btree",
        .file_path = path,
        .root_size = sizeof(struct btree)};

    if (pm_init(config, PMDK_BACKEND) != 0)
    {
        return 1;
    };

    const char op = argv[2][0];

    struct btree *root = (struct btree *)pm_get_root();

    switch (op)
    {
    case 'p':
    {
        btree_print(root);
    }
    break;
    case 'i':
    {
        int64_t key = atoll(argv[3]);
        char *value = argv[4];
        btree_insert(root, key, value);
    }
    break;
    case 'f':
    {
        int64_t key = atoll(argv[3]);
        const char *value;
        if ((value = btree_find(root, key)) != NULL)
            printf("%s\n", value);
        else
            printf("not found\n");
    }
    break;
    default:
        printf("invalid operation\n");
        break;
    }

    pm_close();

    return 0;
}