#include "linked_list.h"
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tqnode {
  char data;
  LL_NODES(struct tqnode, node) nodes;
};

struct fifo_root {
  LL_LIST(struct tqnode, list) list;
};

static void print_help(void) {
  printf("usage: fifo <pool> <option> [<type>]\n");
  printf("\tAvailable options:\n");
  printf("\tinsert, <character> Insert character into FIFO\n");
  printf("\tremove, Remove element from FIFO\n");
  printf("\tprint, Print all FIFO elements\n");
}

int main(int argc, char **argv) {

  if (argc < 3) {
    print_help();
    return 0;
  }

  PmRegionConfig main_region_config = {.file_path = argv[1],
                                       .root_size = sizeof(struct fifo_root)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  struct fifo_root *root = (struct fifo_root *)pm_get_root();
  typeof(root->list) *list = &root->list;

  if (strcmp(argv[2], "insert") == 0) {
    if (argc == 4) {
      struct tqnode *new_node =
          (struct tqnode *)pm_alloc(sizeof(struct tqnode));
      if (!new_node) {
        return 1;
      }
      new_node->data = *argv[3];
      LL_INSERT(list, new_node, nodes);
      printf("Added %c to FIFO\n", *argv[3]);
    } else {
      print_help();
    }
  } else if (strcmp(argv[2], "remove") == 0) {
    if (list->head == NULL) {
      printf("FIFO is empty\n");
    } else {
      LL_REMOVE_LAST(list, nodes);
      printf("Removed element from FIFO\n");
    }
  } else if (strcmp(argv[2], "print") == 0) {
    printf("Elements in FIFO:\n");
    LL_FOREACH(list, nodes, current_node,
               { printf("%c\t", current_node->data); });
    printf("\n");
  } else {
    print_help();
  }
  pm_close();
  return 0;
}