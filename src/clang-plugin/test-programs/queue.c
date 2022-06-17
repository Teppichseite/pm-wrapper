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

int main(int argc, char *argv[]) {
  enum queue_op op;
  if (argc < 3 || (op = queue_op_parse(argv[2])) == UNKNOWN_QUEUE_OP)
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
    errno = 0;
    capacity = strtoull(argv[3], &end, 0);
    if (errno == ERANGE || *end != '\0')
      fail("invalid size of the queue");

    if ((rootp->queue = queue_new(capacity)) == NULL)
      fail("failed to create a new queue");
    break;
  case QUEUE_ENQUEUE:
    if (argc != 4)
      fail("missing new entry data");

    if (rootp->queue == NULL)
      fail("queue must exist");

    if (queue_enqueue(rootp->queue, argv[3], strlen(argv[3]) + 1) != 0)
      fail("failed to insert new entry");
    break;
  case QUEUE_DEQUEUE:
    if (rootp->queue == NULL)
      fail("queue must exist");

    if (queue_dequeue(rootp->queue) != 0)
      fail("failed to remove entry");
    break;
  case QUEUE_SHOW:
    if (rootp->queue == NULL)
      fail("queue must exist");

    queue_show(rootp->queue);
    break;
  default:
    break;
  }

  pm_close();

  return 0;
}