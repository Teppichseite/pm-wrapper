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

static struct root *root;

/*
 * queue_new -- allocates a new queue container using the atomic API
 */
static struct queue *queue_new(size_t nentries) {
  struct queue *q = (struct queue *)pm_calloc(
      sizeof(struct queue) + ((sizeof(struct entry *) * nentries)));

  size_t *var_dQiaJ8UVEb_0;
  size_t var_dQiaJ8UVEb_1;
  size_t var_dQiaJ8UVEb_2;
  ((var_dQiaJ8UVEb_0 = ((size_t *)(((char *)(q)) + (16)))),
   (var_dQiaJ8UVEb_1 = *((size_t *)(pm_read_object(var_dQiaJ8UVEb_0)))),
   (var_dQiaJ8UVEb_2 = (nentries)),
   pm_write_object(var_dQiaJ8UVEb_0, ((char *)(&var_dQiaJ8UVEb_2)),
                   sizeof(size_t)),
   var_dQiaJ8UVEb_2);

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

  // printf("inserting %zu: %s\n",pos,data);

  unsigned long *var_dQiaJ8UVEb_6;
  unsigned long var_dQiaJ8UVEb_7;
  unsigned long var_dQiaJ8UVEb_8;
  ((var_dQiaJ8UVEb_6 = ((unsigned long *)(((char *)(queue)) + (8)))),
   (var_dQiaJ8UVEb_7 = *((unsigned long *)(pm_read_object(var_dQiaJ8UVEb_6)))),
   (var_dQiaJ8UVEb_8 = (var_dQiaJ8UVEb_7 + 1)),
   pm_write_object(var_dQiaJ8UVEb_6, ((char *)(&var_dQiaJ8UVEb_8)),
                   sizeof(unsigned long)),
   var_dQiaJ8UVEb_8);

  struct entry *entry = (struct entry *)pm_alloc(sizeof(struct entry) + len);
  size_t *var_dQiaJ8UVEb_9;
  size_t var_dQiaJ8UVEb_10;
  size_t var_dQiaJ8UVEb_11;
  ((var_dQiaJ8UVEb_9 = ((size_t *)(((char *)(entry)) + (0)))),
   (var_dQiaJ8UVEb_10 = *((size_t *)(pm_read_object(var_dQiaJ8UVEb_9)))),
   (var_dQiaJ8UVEb_11 = (len)),
   pm_write_object(var_dQiaJ8UVEb_9, ((char *)(&var_dQiaJ8UVEb_11)),
                   sizeof(size_t)),
   var_dQiaJ8UVEb_11);
  pm_write_object(((char *)(((char *)((entry))) + 8)), (char *)data, len);
  struct entry **var_dQiaJ8UVEb_12;
  struct entry *var_dQiaJ8UVEb_13;
  struct entry *var_dQiaJ8UVEb_14;
  ((var_dQiaJ8UVEb_12 =
        ((struct entry **)(((((struct entry **)(((char *)((queue))) + 24)))) +
                           (pos)))),
   (var_dQiaJ8UVEb_13 =
        *((struct entry **)(pm_read_object(var_dQiaJ8UVEb_12)))),
   (var_dQiaJ8UVEb_14 = (entry)),
   pm_write_object(var_dQiaJ8UVEb_12, ((char *)(&var_dQiaJ8UVEb_14)),
                   sizeof(struct entry *)),
   var_dQiaJ8UVEb_14);

  return ret;
}

/*
 * queue_dequeue - removes and frees the first element from the queue
 */
static int queue_dequeue(struct queue *queue) {
  if (queue_nentries(queue) == 0) {
    printf("size\n");
    return -1; /* no entries to remove */
  }

  /* front is also never decreased */
  size_t pos = ((struct queue *)pm_read_object((queue)))->front %
               ((struct queue *)pm_read_object((queue)))->capacity;

  int ret = 0;

  // printf("removing %zu: %s\n",pos,((char
  // *)pm_read_object((((char*)(((char*)((((struct entry
  // **)pm_read_object((((struct entry
  // **)(((char*)((queue)))+24)))))[pos])))+8))))));

  unsigned long *var_dQiaJ8UVEb_15;
  unsigned long var_dQiaJ8UVEb_16;
  unsigned long var_dQiaJ8UVEb_17;
  ((var_dQiaJ8UVEb_15 = ((unsigned long *)(((char *)(queue)) + (0)))),
   (var_dQiaJ8UVEb_16 =
        *((unsigned long *)(pm_read_object(var_dQiaJ8UVEb_15)))),
   (var_dQiaJ8UVEb_17 = (var_dQiaJ8UVEb_16 + 1)),
   pm_write_object(var_dQiaJ8UVEb_15, ((char *)(&var_dQiaJ8UVEb_17)),
                   sizeof(unsigned long)),
   var_dQiaJ8UVEb_17);
  //pm_free(((void *)pm_read_object((((struct entry **)pm_read_object(
  //    (((struct entry **)(((char *)((queue))) + 24)))))[pos]))));

  return ret;
}

/*
 * queue_show -- prints all queue entries
 */
static void queue_show(struct queue *queue) {
  size_t nentries = queue_nentries(queue);
  printf("Entries %zu/%zu\n", nentries, queue->capacity);
  for (size_t i = queue->front; i < queue->back; ++i) {
    size_t pos = i % queue->capacity;
    printf("%zu: %s\n", pos, queue->entries[pos]->data);
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

int size = 5000;

int pm_wrapper_init(char *path) {
  PmRegionConfig main_region_config = {.file_path = path,
                                       .root_size = sizeof(struct root)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  root = (struct root *)pm_get_root();
  struct queue **var_dQiaJ8UVEb_3;
  struct queue *var_dQiaJ8UVEb_4;
  struct queue *var_dQiaJ8UVEb_5;
  if (((((var_dQiaJ8UVEb_3 = ((struct queue **)(((char *)(root)) + (0)))),
         (var_dQiaJ8UVEb_4 =
              *((struct queue **)(pm_read_object(var_dQiaJ8UVEb_3)))),
         (var_dQiaJ8UVEb_5 = (queue_new(size))),
         pm_write_object(var_dQiaJ8UVEb_3, ((char *)(&var_dQiaJ8UVEb_5)),
                         sizeof(struct queue *)),
         var_dQiaJ8UVEb_5))) == NULL) {
    fail("failed to create a new queue");
  }
  return 0;
}
void pm_wrapper_run() {
  for (int i = 0; i < size; i++) {
    queue_enqueue(((struct root *)pm_read_object((root)))->queue, "hello", 6);
  }
  for (int i = 0; i < size; i++) {
    queue_dequeue(((struct root *)pm_read_object((root)))->queue);
  }
}
void pm_wrapper_close() { pm_close(); }