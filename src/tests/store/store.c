#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct entry { /* queue entry that contains arbitrary data */
  size_t len;  /* length of the data buffer */
  char data[];
};

struct queue {  /* array-based queue container */
  size_t front; /* position of the first entry */
  size_t back;  /* position of the last entry */

  size_t capacity; /* size of the entries array */
  struct entry *entries[];
};

struct root {
  struct queue *queue;
};

/*
 * queue_new -- allocates a new queue container using the atomic API
 */
static struct queue *queue_new(size_t nentries) {
  struct queue *q = (struct queue *)pm_calloc(
      sizeof(struct queue) + ((sizeof(struct entry *) * nentries)));

  size_t *var_zfoFYl3sPc_0;
  size_t var_zfoFYl3sPc_1;
  size_t var_zfoFYl3sPc_2;
  ((var_zfoFYl3sPc_0 = ((size_t *)(((char *)(q)) + (16)))),
   (var_zfoFYl3sPc_1 = *((size_t *)(pm_read_object(var_zfoFYl3sPc_0)))),
   (var_zfoFYl3sPc_2 = (nentries)),
   pm_write_object(var_zfoFYl3sPc_0, ((char *)(&var_zfoFYl3sPc_2)),
                   sizeof(size_t)),
   var_zfoFYl3sPc_2);

  return q;
}

/*
 * queue_nentries -- returns the number of entries
 */
static size_t queue_nentries(struct queue *queue) {
  return ((struct queue *)pm_read_object((queue)))->back -
         ((struct queue *)pm_read_object((queue)))->front;
}

/*
 * queue_enqueue -- allocates and inserts a new entry into the queue
 */
static int queue_enqueue(struct queue *queue, const char *data, size_t len) {
  if (((struct queue *)pm_read_object((queue)))->capacity -
          queue_nentries(queue) ==
      0)
    return -1; /* at capacity */

  /* back is never decreased, need to calculate the real position */
  size_t pos = ((struct queue *)pm_read_object((queue)))->back %
               ((struct queue *)pm_read_object((queue)))->capacity;

  int ret = 0;

  printf("inserting %zu: %s\n", pos, data);

  unsigned long *var_zfoFYl3sPc_6;
  unsigned long var_zfoFYl3sPc_7;
  unsigned long var_zfoFYl3sPc_8;
  ((var_zfoFYl3sPc_6 = ((unsigned long *)(((char *)(queue)) + (8)))),
   (var_zfoFYl3sPc_7 = *((unsigned long *)(pm_read_object(var_zfoFYl3sPc_6)))),
   (var_zfoFYl3sPc_8 = (var_zfoFYl3sPc_7 + 1)),
   pm_write_object(var_zfoFYl3sPc_6, ((char *)(&var_zfoFYl3sPc_8)),
                   sizeof(unsigned long)),
   var_zfoFYl3sPc_8);

  struct entry *entry = (struct entry *)pm_alloc(sizeof(struct entry) + len);
  size_t *var_zfoFYl3sPc_9;
  size_t var_zfoFYl3sPc_10;
  size_t var_zfoFYl3sPc_11;
  ((var_zfoFYl3sPc_9 = ((size_t *)(((char *)(entry)) + (0)))),
   (var_zfoFYl3sPc_10 = *((size_t *)(pm_read_object(var_zfoFYl3sPc_9)))),
   (var_zfoFYl3sPc_11 = (len)),
   pm_write_object(var_zfoFYl3sPc_9, ((char *)(&var_zfoFYl3sPc_11)),
                   sizeof(size_t)),
   var_zfoFYl3sPc_11);
  pm_write_object(((char *)(((char *)((entry))) + 8)), (char *)data, len);
  struct entry **var_zfoFYl3sPc_12;
  struct entry *var_zfoFYl3sPc_13;
  struct entry *var_zfoFYl3sPc_14;
  ((var_zfoFYl3sPc_12 =
        ((struct entry **)(((((struct entry **)(((char *)((queue))) + 24)))) +
                           (pos)))),
   (var_zfoFYl3sPc_13 =
        *((struct entry **)(pm_read_object(var_zfoFYl3sPc_12)))),
   (var_zfoFYl3sPc_14 = (entry)),
   pm_write_object(var_zfoFYl3sPc_12, ((char *)(&var_zfoFYl3sPc_14)),
                   sizeof(struct entry *)),
   var_zfoFYl3sPc_14);

  return ret;
}

/*
 * queue_dequeue - removes and frees the first element from the queue
 */
static int queue_dequeue(struct queue *queue) {
  if (queue_nentries(queue) == 0)
    return -1; /* no entries to remove */

  /* front is also never decreased */
  size_t pos = ((struct queue *)pm_read_object((queue)))->front %
               ((struct queue *)pm_read_object((queue)))->capacity;

  int ret = 0;

  printf(
      "removing %zu: %s\n", pos,
      ((char *)pm_read_object((
          ((char *)(((char *)((((struct entry **)pm_read_object((((
                        struct entry **)(((char *)((queue))) + 24)))))[pos]))) +
                    8))))));

  unsigned long *var_zfoFYl3sPc_15;
  unsigned long var_zfoFYl3sPc_16;
  unsigned long var_zfoFYl3sPc_17;
  ((var_zfoFYl3sPc_15 = ((unsigned long *)(((char *)(queue)) + (0)))),
   (var_zfoFYl3sPc_16 =
        *((unsigned long *)(pm_read_object(var_zfoFYl3sPc_15)))),
   (var_zfoFYl3sPc_17 = (var_zfoFYl3sPc_16 + 1)),
   pm_write_object(var_zfoFYl3sPc_15, ((char *)(&var_zfoFYl3sPc_17)),
                   sizeof(unsigned long)),
   var_zfoFYl3sPc_17);
  pm_free(((void *)pm_read_object((((struct entry **)pm_read_object(
      (((struct entry **)(((char *)((queue))) + 24)))))[pos]))));

  return ret;
}

/*
 * queue_show -- prints all queue entries
 */
static void queue_show(struct queue *queue) {
  size_t nentries = queue_nentries(queue);
  printf("Entries %zu/%zu\n", nentries,
         ((struct queue *)pm_read_object((queue)))->capacity);
  for (size_t i = ((struct queue *)pm_read_object((queue)))->front;
       i < ((struct queue *)pm_read_object((queue)))->back; ++i) {
    size_t pos = i % ((struct queue *)pm_read_object((queue)))->capacity;
    printf(
        "%zu: %s\n", pos,
        ((char *)pm_read_object((((
            char *)(((char *)((((struct entry **)pm_read_object((((
                        struct entry **)(((char *)((queue))) + 24)))))[pos]))) +
                    8))))));
  }
}

/* available queue operations */
enum queue_op {
  UNKNOWN_QUEUE_OP,
  QUEUE_NEW,
  QUEUE_ENQUEUE,
  QUEUE_DEQUEUE,
  QUEUE_SHOW,

  MAX_QUEUE_OP,
};

/* queue operations strings */
static const char *ops_str[MAX_QUEUE_OP] = {"", "new", "enqueue", "dequeue",
                                            "show"};

/*
 * parse_queue_op -- parses the operation string and returns matching queue_op
 */
static enum queue_op queue_op_parse(const char *str) {
  for (int i = 0; i < MAX_QUEUE_OP; ++i)
    if (strcmp(str, ops_str[i]) == 0)
      return (enum queue_op)i;

  return UNKNOWN_QUEUE_OP;
}

/*
 * fail -- helper function to exit the application in the event of an error
 */
static void __attribute__((noreturn)) /* this function terminates */
fail(const char *msg) {
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

int main(int argc, char *argv[]) {
  enum queue_op op;
  if (argc < 3 || ((op = queue_op_parse(argv[2]))) == UNKNOWN_QUEUE_OP)
    fail("usage: file-name [new <n>|show|enqueue <data>|dequeue]");

  PmRegionConfig main_region_config = {.file_path = argv[1],
                                       .root_size = sizeof(struct root)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  struct root *rootp = (struct root *)pm_get_root();

  size_t capacity;

  switch (op) {
  case QUEUE_NEW:
    if (argc != 4)
      fail("missing size of the queue");

    char *end;
    ((*__errno_location())) = 0;
    capacity = strtoull(argv[3], &end, 0);
    if (((*__errno_location())) == 34 || *end != '\x00')
      fail("invalid size of the queue");

    struct queue **var_zfoFYl3sPc_3;
    struct queue *var_zfoFYl3sPc_4;
    struct queue *var_zfoFYl3sPc_5;
    if (((((var_zfoFYl3sPc_3 = ((struct queue **)(((char *)(rootp)) + (0)))),
           (var_zfoFYl3sPc_4 =
                *((struct queue **)(pm_read_object(var_zfoFYl3sPc_3)))),
           (var_zfoFYl3sPc_5 = (queue_new(capacity))),
           pm_write_object(var_zfoFYl3sPc_3, ((char *)(&var_zfoFYl3sPc_5)),
                           sizeof(struct queue *)),
           var_zfoFYl3sPc_5))) == ((NULL)))
      fail("failed to create a new queue");
    break;
  case QUEUE_ENQUEUE:
    if (argc != 4)
      fail("missing new entry data");

    if (((struct root *)pm_read_object((rootp)))->queue == ((NULL)))
      fail("queue must exist");

    if (queue_enqueue(((struct root *)pm_read_object((rootp)))->queue, argv[3],
                      strlen(argv[3]) + 1) != 0)
      fail("failed to insert new entry");
    break;
  case QUEUE_DEQUEUE:
    if (((struct root *)pm_read_object((rootp)))->queue == ((NULL)))
      fail("queue must exist");

    if (queue_dequeue(((struct root *)pm_read_object((rootp)))->queue) != 0)
      fail("failed to remove entry");
    break;
  case QUEUE_SHOW:
    if (((struct root *)pm_read_object((rootp)))->queue == ((NULL)))
      fail("queue must exist");

    queue_show(((struct root *)pm_read_object((rootp)))->queue);
    break;
  default:
    break;
  }

  pm_close();

  return 0;
}