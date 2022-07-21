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
  struct btree_node **dst = ((struct btree_node **)(((char *)((btree))) + 0));

  while (*((struct btree_node **)pm_read_object((dst))) != ((NULL))) {
    dst = ((((struct btree_node **)(((char *)(((
                                        (*((struct btree_node **)pm_read_object(
                                            (dst)))))))) +
                                    8))) +
           (key > ((struct btree_node *)pm_read_object(
                       (((*((struct btree_node **)pm_read_object((dst))))))))
                      ->key));
  }

  struct btree_node *new_node = (struct btree_node *)pm_alloc(
      sizeof(struct btree_node *) + strlen(value) + 1);

  int64_t *var_RtL0daEPvS_0;
  int64_t var_RtL0daEPvS_1;
  int64_t var_RtL0daEPvS_2;
  ((var_RtL0daEPvS_0 = ((int64_t *)(((char *)(new_node)) + (0)))),
   (var_RtL0daEPvS_1 = *((int64_t *)(pm_read_object(var_RtL0daEPvS_0)))),
   (var_RtL0daEPvS_2 = (key)),
   pm_write_object(var_RtL0daEPvS_0, ((char *)(&var_RtL0daEPvS_2)),
                   sizeof(int64_t)),
   var_RtL0daEPvS_2);
  pm_write_object(((char *)(((char *)((new_node))) + 24)), value,
                  strlen(value) + 1);

  struct btree_node **var_RtL0daEPvS_3;
  struct btree_node *var_RtL0daEPvS_4;
  struct btree_node *var_RtL0daEPvS_5;
  ((var_RtL0daEPvS_3 = ((struct btree_node **)(((dst)) + (0)))),
   (var_RtL0daEPvS_4 =
        *((struct btree_node **)(pm_read_object(var_RtL0daEPvS_3)))),
   (var_RtL0daEPvS_5 = (new_node)),
   pm_write_object(var_RtL0daEPvS_3, ((char *)(&var_RtL0daEPvS_5)),
                   sizeof(struct btree_node *)),
   var_RtL0daEPvS_5);
}

/*
 * btree_find -- searches for key in the tree
 */
char *btree_find(int64_t key) {
  struct btree_node *node = ((struct btree *)pm_read_object((btree)))->root;

  while (node != ((NULL))) {
    if (((struct btree_node *)pm_read_object((node)))->key == key)
      return ((char *)(((char *)((node))) + 24));
    else
      node = ((struct btree_node **)pm_read_object(
          (((struct btree_node **)(((char *)((node))) + 8)))))
          [key > ((struct btree_node *)pm_read_object((node)))->key];
  }

  return NULL;
}

/*
 * btree_node_print -- prints content of the btree node
 */
void btree_node_print(PM struct btree_node *node) {
  printf("%lu %s\n", ((struct btree_node *)pm_read_object((node)))->key,
         ((char *)pm_read_object((((char *)(((char *)((node))) + 24))))));
}

/*
 * btree_foreach -- invoke callback for every node
 */
void btree_foreach(struct btree_node *node, void (*cb)(struct btree_node *)) {
  if (node == ((NULL))) {
    return;
  }

  btree_foreach(((struct btree_node **)pm_read_object(
                    (((struct btree_node **)(((char *)((node))) + 8)))))[0],
                cb);

  cb(node);

  btree_foreach(((struct btree_node **)pm_read_object(
                    (((struct btree_node **)(((char *)((node))) + 8)))))[1],
                cb);
}

/*
 * btree_print -- initiates foreach node print
 */
void btree_print() {
  btree_foreach(((struct btree *)pm_read_object((btree)))->root,
                btree_node_print);
}

int p_init(char *path) {
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

void p_close() { pm_close(); }