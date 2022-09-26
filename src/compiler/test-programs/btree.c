#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

struct btree_node {
  int64_t key;
  struct btree_node *slots[2];
  char value[];
};

struct btree {
  struct btree_node *root;
};

/*
 * btree_insert -- inserts new element into the tree
 */
static void btree_insert(struct btree *btree, int64_t key, char *value) {
  struct btree_node **dst = &btree->root;

  while (*dst != NULL) {
    dst = &(*dst)->slots[key > (*dst)->key];
  }

  struct btree_node *new_node = (struct btree_node *)pm_alloc(
      sizeof(struct btree_node *) + strlen(value) + 1);

  new_node->key = key;
  pm_write_object(&new_node->value, value, strlen(value) + 1);

  *dst = new_node;
}

/*
 * btree_find -- searches for key in the tree
 */
static char *btree_find(struct btree *btree, int64_t key) {
  struct btree_node *node = btree->root;

  while (node != NULL) {
    if (node->key == key)
      return node->value;
    else
      node = node->slots[key > node->key];
  }

  return NULL;
}

/*
 * btree_node_print -- prints content of the btree node
 */
static void btree_node_print(PM struct btree_node *node) {
  printf("%" PRIu64 " %s\n", node->key, node->value);
}

/*
 * btree_foreach -- invoke callback for every node
 */
static void btree_foreach(struct btree *btree, struct btree_node *node,
                          void (*cb)(struct btree_node *)) {
  if (node == NULL) {
    return;
  }

  btree_foreach(btree, node->slots[0], cb);

  cb(node);

  btree_foreach(btree, node->slots[1], cb);
}

/*
 * btree_print -- initiates foreach node print
 */
static void btree_print(struct btree *btree) {
  btree_foreach(btree, btree->root, btree_node_print);
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s file-name [p|i|f] [key] [value] \n", argv[0]);
    return 1;
  }

  char *path = argv[1];

  PmRegionConfig main_region_config = {.file_path = "./test",
                                       .root_size = sizeof(struct btree)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  struct btree *btree = (struct btree *)pm_get_root();

  char op = argv[2][0];
  int64_t key;
  char *value;

  switch (op) {
  case 'p':
    btree_print(btree);
    break;
  case 'i':
    key = atoll(argv[3]);
    value = argv[4];
    btree_insert(btree, key, value);
    break;
  case 'f':
    key = atoll(argv[3]);
    char *found_value;
    if ((found_value = btree_find(btree, key)) != NULL)
      printf("%s\n", found_value);
    else
      printf("not found\n");
    break;
  default:
    printf("invalid operation\n");
    break;
  }

  pm_close();

  return 0;
}