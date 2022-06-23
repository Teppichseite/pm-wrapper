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
  struct tqnode *head = ((struct fifo_root *)pm_read_object((root)))->head;
  struct tqnode *new_node = (struct tqnode *)pm_alloc(sizeof(struct tqnode));
  if (!new_node) {
    return;
  }
  char *var_2hFLos4f8S_0;
  char var_2hFLos4f8S_1;
  char var_2hFLos4f8S_2;
  ((var_2hFLos4f8S_0 = ((char *)(((char *)(new_node)) + (0)))),
   (var_2hFLos4f8S_1 = *((char *)(pm_read_object(var_2hFLos4f8S_0)))),
   (var_2hFLos4f8S_2 = (data)),
   pm_write_object(var_2hFLos4f8S_0, ((char *)(&var_2hFLos4f8S_2)),
                   sizeof(char)),
   var_2hFLos4f8S_2);
  struct tqnode **var_2hFLos4f8S_3;
  struct tqnode *var_2hFLos4f8S_4;
  struct tqnode *var_2hFLos4f8S_5;
  ((var_2hFLos4f8S_3 = ((struct tqnode **)(((char *)(new_node)) + (16)))),
   (var_2hFLos4f8S_4 = *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_3)))),
   (var_2hFLos4f8S_5 = (head)),
   pm_write_object(var_2hFLos4f8S_3, ((char *)(&var_2hFLos4f8S_5)),
                   sizeof(struct tqnode *)),
   var_2hFLos4f8S_5);
  struct tqnode **var_2hFLos4f8S_6;
  struct tqnode *var_2hFLos4f8S_7;
  struct tqnode *var_2hFLos4f8S_8;
  ((var_2hFLos4f8S_6 = ((struct tqnode **)(((char *)(new_node)) + (8)))),
   (var_2hFLos4f8S_7 = *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_6)))),
   (var_2hFLos4f8S_8 = (NULL)),
   pm_write_object(var_2hFLos4f8S_6, ((char *)(&var_2hFLos4f8S_8)),
                   sizeof(struct tqnode *)),
   var_2hFLos4f8S_8);
  struct tqnode **var_2hFLos4f8S_9;
  struct tqnode *var_2hFLos4f8S_10;
  struct tqnode *var_2hFLos4f8S_11;
  ((var_2hFLos4f8S_9 = ((struct tqnode **)(((char *)(root)) + (0)))),
   (var_2hFLos4f8S_10 =
        *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_9)))),
   (var_2hFLos4f8S_11 = (new_node)),
   pm_write_object(var_2hFLos4f8S_9, ((char *)(&var_2hFLos4f8S_11)),
                   sizeof(struct tqnode *)),
   var_2hFLos4f8S_11);

  if (!head) {
    struct tqnode **var_2hFLos4f8S_12;
    struct tqnode *var_2hFLos4f8S_13;
    struct tqnode *var_2hFLos4f8S_14;
    ((var_2hFLos4f8S_12 = ((struct tqnode **)(((char *)(root)) + (8)))),
     (var_2hFLos4f8S_13 =
          *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_12)))),
     (var_2hFLos4f8S_14 = (new_node)),
     pm_write_object(var_2hFLos4f8S_12, ((char *)(&var_2hFLos4f8S_14)),
                     sizeof(struct tqnode *)),
     var_2hFLos4f8S_14);
  } else {
    struct tqnode **var_2hFLos4f8S_15;
    struct tqnode *var_2hFLos4f8S_16;
    struct tqnode *var_2hFLos4f8S_17;
    ((var_2hFLos4f8S_15 = ((struct tqnode **)(((char *)(head)) + (8)))),
     (var_2hFLos4f8S_16 =
          *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_15)))),
     (var_2hFLos4f8S_17 = (new_node)),
     pm_write_object(var_2hFLos4f8S_15, ((char *)(&var_2hFLos4f8S_17)),
                     sizeof(struct tqnode *)),
     var_2hFLos4f8S_17);
  }
}

void remove_last(struct fifo_root *root) {
  struct tqnode *tail = ((struct fifo_root *)pm_read_object((root)))->tail;
  if (!((struct tqnode *)pm_read_object((tail)))->prev) {
    struct tqnode **var_2hFLos4f8S_18;
    struct tqnode *var_2hFLos4f8S_19;
    struct tqnode *var_2hFLos4f8S_20;
    ((var_2hFLos4f8S_18 = ((struct tqnode **)(((char *)(root)) + (8)))),
     (var_2hFLos4f8S_19 =
          *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_18)))),
     (var_2hFLos4f8S_20 = (NULL)),
     pm_write_object(var_2hFLos4f8S_18, ((char *)(&var_2hFLos4f8S_20)),
                     sizeof(struct tqnode *)),
     var_2hFLos4f8S_20);
    struct tqnode **var_2hFLos4f8S_21;
    struct tqnode *var_2hFLos4f8S_22;
    struct tqnode *var_2hFLos4f8S_23;
    ((var_2hFLos4f8S_21 = ((struct tqnode **)(((char *)(root)) + (0)))),
     (var_2hFLos4f8S_22 =
          *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_21)))),
     (var_2hFLos4f8S_23 = (NULL)),
     pm_write_object(var_2hFLos4f8S_21, ((char *)(&var_2hFLos4f8S_23)),
                     sizeof(struct tqnode *)),
     var_2hFLos4f8S_23);
    pm_free(((void *)pm_read_object((tail))));
    return;
  }

  struct tqnode *new_tail = ((struct tqnode *)pm_read_object((tail)))->prev;
  struct tqnode **var_2hFLos4f8S_24;
  struct tqnode *var_2hFLos4f8S_25;
  struct tqnode *var_2hFLos4f8S_26;
  ((var_2hFLos4f8S_24 = ((struct tqnode **)(((char *)(new_tail)) + (16)))),
   (var_2hFLos4f8S_25 =
        *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_24)))),
   (var_2hFLos4f8S_26 = (NULL)),
   pm_write_object(var_2hFLos4f8S_24, ((char *)(&var_2hFLos4f8S_26)),
                   sizeof(struct tqnode *)),
   var_2hFLos4f8S_26);
  struct tqnode **var_2hFLos4f8S_27;
  struct tqnode *var_2hFLos4f8S_28;
  struct tqnode *var_2hFLos4f8S_29;
  ((var_2hFLos4f8S_27 = ((struct tqnode **)(((char *)(root)) + (8)))),
   (var_2hFLos4f8S_28 =
        *((struct tqnode **)(pm_read_object(var_2hFLos4f8S_27)))),
   (var_2hFLos4f8S_29 = (new_tail)),
   pm_write_object(var_2hFLos4f8S_27, ((char *)(&var_2hFLos4f8S_29)),
                   sizeof(struct tqnode *)),
   var_2hFLos4f8S_29);
  pm_free(((void *)pm_read_object((tail))));
}

void print_nodes(struct tqnode *tnd) {
  if (!tnd) {
    return;
  }
  printf("%c\t", ((struct tqnode *)pm_read_object((tnd)))->data);
  print_nodes(((struct tqnode *)pm_read_object((tnd)))->next);
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
    if (!((struct fifo_root *)pm_read_object((root)))->head) {
      printf("FIFO is empty\n");
    } else {
      remove_last(root);
      printf("Removed element from FIFO\n");
    }
  } else if (strcmp(argv[2], "print") == 0) {
    printf("Elements in FIFO:\n");
    print_nodes(((struct fifo_root *)pm_read_object((root)))->head);
    printf("\n");
  } else {
    print_help();
  }
  pm_close();
  return 0;
}