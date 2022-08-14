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
      sizeof(struct queue) + (sizeof(struct entry *) * nentries));

  q->capacity = nentries;

  return q;
}

/*
 * queue_nentries -- returns the number of entries
 */
static size_t queue_nentries(struct queue *queue) {
  return queue->back - queue->front;
}

/*
 * queue_enqueue -- allocates and inserts a new entry into the queue
 */
static int queue_enqueue(struct queue *queue, const char *data, size_t len) {
  if (queue->capacity - queue_nentries(queue) == 0)
    return -1; /* at capacity */

  /* back is never decreased, need to calculate the real position */
  size_t pos = queue->back % queue->capacity;

  int ret = 0;

  printf("inserting %zu: %s\n", pos, data);

  queue->back += 1;

  struct entry *entry = (struct entry *)pm_alloc(sizeof(struct entry) + len);
  entry->len = len;
  pm_write_object(&entry->data, (char *)data, len);
  queue->entries[pos] = entry;

  return ret;
}

/*
 * queue_dequeue - removes and frees the first element from the queue
 */
static int queue_dequeue(struct queue *queue) {
  if (queue_nentries(queue) == 0)
    return -1; /* no entries to remove */

  /* front is also never decreased */
  size_t pos = queue->front % queue->capacity;

  int ret = 0;

  printf("removing %zu: %s\n", pos, queue->entries[pos]->data);

  queue->front += 1;
  pm_free(queue->entries[pos]);

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
  exit(EXIT_FAILURE);
}

int size = 10000;

int pm_wrapper_init(char *path) {
  PmRegionConfig main_region_config = {.file_path = path,
                                       .root_size = sizeof(struct root)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  root = (struct root *)pm_get_root();
  if ((root->queue = queue_new(10)) != 0) {
    fail("failed to create a new queue");
  }
  return 0;
}
void pm_wrapper_run() {
  for (int i = 0; i < size; i++) {
    queue_enqueue(root->queue, "hello", 6);
  }
  for (int i = 0; i < size; i++) {
    queue_dequeue(root->queue);
  }
}
void pm_wrapper_close() { pm_wrapper_close(); }

int main(int argc, char *argv[]) {
  pm_wrapper_init("");
  pm_wrapper_run();
  pm_wrapper_close();
  return 0;
}