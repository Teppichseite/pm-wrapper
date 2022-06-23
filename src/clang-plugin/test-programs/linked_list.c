#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct fifo_root {
  struct tqnode *head;
  struct tqnode *tail;
};

struct tqnode {
  char data;
  struct tqnode *prev;
  struct tqnode *next;
};

static void print_help(void) {
  printf("usage: fifo <pool> <option> [<type>]\n");
  printf("\tAvailable options:\n");
  printf("\tinsert, <character> Insert character into FIFO\n");
  printf("\tremove, Remove element from FIFO\n");
  printf("\tprint, Print all FIFO elements\n");
}

void insert_head(struct fifo_root *root, char data) {
  struct tqnode *head = root->head;
  struct tqnode *new_node = (struct tqnode *)pm_alloc(sizeof(struct tqnode));
  if (!new_node) {
    return;
  }
  new_node->data = data;
  new_node->next = head;
  new_node->prev = NULL;
  root->head = new_node;

  if (!head) {
    root->tail = new_node;
  } else {
    head->prev = new_node;
  }
}

void remove_last(struct fifo_root *root) {
  struct tqnode *tail = root->tail;
  if (!tail->prev) {
    root->tail = NULL;
    root->head = NULL;
    pm_free(tail);
    return;
  }

  struct tqnode *new_tail = tail->prev;
  new_tail->next = NULL;
  root->tail = new_tail;
  pm_free(tail);
}

void print_nodes(struct tqnode *tnd) {
  if (!tnd) {
    return;
  }
  printf("%c\t", tnd->data);
  print_nodes(tnd->next);
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

  if (strcmp(argv[2], "insert") == 0) {
    if (argc == 4) {
      insert_head(root, *argv[3]);
      printf("Added %c to FIFO\n", *argv[3]);
    } else {
      print_help();
    }
  } else if (strcmp(argv[2], "remove") == 0) {
    if (!root->head) {
      printf("FIFO is empty\n");
    } else {
      remove_last(root);
      printf("Removed element from FIFO\n");
    }
  } else if (strcmp(argv[2], "print") == 0) {
    printf("Elements in FIFO:\n");
    print_nodes(root->head);
    printf("\n");
  } else {
    print_help();
  }
  pm_close();
  return 0;
}