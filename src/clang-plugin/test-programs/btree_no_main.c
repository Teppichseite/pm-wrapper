#include <stdlib.h>
#define PM __attribute__((pointer_type(1)))
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

PM struct btree *btree;

/*
 * btree_insert -- inserts new element into the tree
 */
void btree_insert(int64_t key, char *value) {
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
char *btree_find(int64_t key) {
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
void btree_node_print(PM struct btree_node *node) {
  printf("%" PRIu64 " %s\n", node->key, node->value);
}

/*
 * btree_foreach -- invoke callback for every node
 */
void btree_foreach(struct btree_node *node, void (*cb)(struct btree_node *)) {
  if (node == NULL) {
    return;
  }

  btree_foreach(node->slots[0], cb);

  cb(node);

  btree_foreach(node->slots[1], cb);
}

/*
 * btree_print -- initiates foreach node print
 */
void btree_print() {
  btree_foreach(btree->root, btree_node_print);
}

int init(char *path) {
  PmRegionConfig main_region_config = {.file_path = path,
                                       .root_size = sizeof(struct btree)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  btree = (struct btree *)pm_get_root();

  return 0;
}

void close() { pm_close(); }

int main() {
  btree_insert(10, "hello");
  btree_find(10);
  btree_print();
  init("hello");
}
