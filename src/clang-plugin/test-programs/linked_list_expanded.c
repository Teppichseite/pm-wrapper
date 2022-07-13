#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include "linked_list.h"
#include <cstddef>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tqnode {
  char data;
  struct {
    struct tqnode *prev;
    struct tqnode *next;
  } nodes;
};

struct fifo_root {
  struct {
    struct tqnode *head;
    struct tqnode *tail;
  } list;
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
      do {
        if (!new_node) {
          break;
        }
        typeof(new_node) head = (list)->head;
        (&new_node->nodes)->next = head;
        (&new_node->nodes)->prev = ((void *)0);
        (list)->head = new_node;
        if (!head) {
          (list)->tail = new_node;
        } else {
          (&new_node->nodes)->prev = new_node;
        }
      } while (0);
      printf("Added %c to FIFO\n", *argv[3]);
    } else {
      print_help();
    }
  } else if (strcmp(argv[2], "remove") == 0) {
    if (list->head) {
      printf("FIFO is empty\n");
    } else {
      do {
        typeof((list)->tail) tail = (list)->tail;
        if (!(&tail->nodes)->prev) {
          (list)->tail = ((void *)0);
          (list)->head = ((void *)0);
          pm_free(tail);
          break;
        }
        typeof((list)->tail) new_tail = (&tail->nodes)->prev;
        (&new_tail->nodes)->next = ((void *)0);
        (list)->tail = new_tail;
        pm_free(tail);
      } while (0);
      printf("Removed element from FIFO\n");
    }
  } else if (strcmp(argv[2], "print") == 0) {
    printf("Elements in FIFO:\n");
    do {
      typeof((list)->tail) current_node = (list)->head;
      while ((current_node) != ((void *)0)) {
        { printf("%c\t", current_node->data); };
        (current_node) = (&current_node->nodes)->next;
      }
    } while (0);

    printf("\n");
  } else {
    print_help();
  }
  pm_close();
  return 0;
}