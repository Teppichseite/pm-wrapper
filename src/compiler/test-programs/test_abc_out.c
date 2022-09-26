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

/*
 * btree_insert -- inserts new element into the tree
 */
static void btree_insert(struct btree *btree, int64_t key, char *value) {
  struct btree_node **dst = ((struct btree_node **)(((char*)((btree)))+0));

  while (*((struct btree_node **)pm_read_object((dst)))!=((NULL))) {
    dst=((((struct btree_node **)(((char*)((((*((struct btree_node **)pm_read_object((dst))))))))+8)))+(key>((struct btree_node *)pm_read_object((((*((struct btree_node **)pm_read_object((dst))))))))->key));
  }

  struct btree_node *new_node = (struct btree_node *)pm_alloc(sizeof(struct btree_node *)+strlen(value)+1);

  int64_t* var_jeyPXDYwnc_3;int64_t var_jeyPXDYwnc_4;int64_t var_jeyPXDYwnc_5;((var_jeyPXDYwnc_3 = ((int64_t*)(((char*)(new_node)) + (0)))),(var_jeyPXDYwnc_4 = *((int64_t*)(pm_read_object(var_jeyPXDYwnc_3)))),(var_jeyPXDYwnc_5 = (key)),pm_write_object(var_jeyPXDYwnc_3,((char*)(&var_jeyPXDYwnc_5)),sizeof(int64_t)),var_jeyPXDYwnc_5);
  pm_write_object(((char*)(((char*)((new_node)))+24)),value,strlen(value)+1);

  struct btree_node ** var_jeyPXDYwnc_6;struct btree_node * var_jeyPXDYwnc_7;struct btree_node * var_jeyPXDYwnc_8;((var_jeyPXDYwnc_6 = ((struct btree_node **)(((dst)) + (0)))),(var_jeyPXDYwnc_7 = *((struct btree_node **)(pm_read_object(var_jeyPXDYwnc_6)))),(var_jeyPXDYwnc_8 = (new_node)),pm_write_object(var_jeyPXDYwnc_6,((char*)(&var_jeyPXDYwnc_8)),sizeof(struct btree_node *)),var_jeyPXDYwnc_8);
}

/*
 * btree_find -- searches for key in the tree
 */
static char *btree_find(struct btree *btree, int64_t key) {
  struct btree_node *node = ((struct btree *)pm_read_object((btree)))->root;

  while (node!=((NULL))) {
    if (((struct btree_node *)pm_read_object((node)))->key==key)
      return ((char*)(((char*)((node)))+24));
    else
      node=((struct btree_node **)pm_read_object((((struct btree_node **)(((char*)((node)))+8)))))[key>((struct btree_node *)pm_read_object((node)))->key];
  }

  return NULL;
}

/*
 * btree_node_print -- prints content of the btree node
 */
static void btree_node_print(PM struct btree_node *node) {
  printf("%lu %s\n",((struct btree_node *)pm_read_object((node)))->key,((char *)pm_read_object((((char*)(((char*)((node)))+24))))));
}

/*
 * btree_foreach -- invoke callback for every node
 */
static void btree_foreach(struct btree *btree, struct btree_node *node,
                          void (*cb)(struct btree_node *)) {
  if (node==((NULL))) {
    return;
  }

  long* var_jeyPXDYwnc_0;long var_jeyPXDYwnc_1;long var_jeyPXDYwnc_2;((var_jeyPXDYwnc_0 = ((long*)(((char*)(node)) + (0)))),(var_jeyPXDYwnc_1 = *((long*)(pm_read_object(var_jeyPXDYwnc_0)))),(var_jeyPXDYwnc_2 = (var_jeyPXDYwnc_1+10)),pm_write_object(var_jeyPXDYwnc_0,((char*)(&var_jeyPXDYwnc_2)),sizeof(long)),var_jeyPXDYwnc_2);

  btree_foreach(btree,((struct btree_node **)pm_read_object((((struct btree_node **)(((char*)((node)))+8)))))[0],cb);

  cb(node);

  btree_foreach(btree,((struct btree_node **)pm_read_object((((struct btree_node **)(((char*)((node)))+8)))))[1],cb);
}

/*
 * btree_print -- initiates foreach node print
 */
static void btree_print(struct btree *btree) {
  btree_foreach(btree,((struct btree *)pm_read_object((btree)))->root,btree_node_print);
}

int main(int argc, char *argv[]) {
  if (argc<3) {
    printf("usage: %s file-name [p|i|f] [key] [value] \n",argv[0]);
    return 1;
  }

  char *path = argv[1];

  PmRegionConfig main_region_config = {.file_path="./test",.root_size=sizeof(struct btree)};

  PmWrapperConfig config = {.backend=&PMDK_BACKEND,.main_region_config=main_region_config};

  if (pm_init(config)!=0) {
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
    key=atoll(argv[3]);
    value=argv[4];
    btree_insert(btree,key,value);
    break;
  case 'f':
    key=atoll(argv[3]);
    char *found_value;
    if (((found_value=btree_find(btree,key)))!=((NULL)))
      printf("%s\n",((char *)pm_read_object((found_value))));
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