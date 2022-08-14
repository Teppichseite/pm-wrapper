// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2018, Intel Corporation */

/*
 * queue.c -- array based queue example
 */

#include <assert.h>
#include <errno.h>
#include <libpmemobj.h>
#include <libpmemobj/atomic_base.h>
#include <libpmemobj/base.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

POBJ_LAYOUT_BEGIN(queue);
POBJ_LAYOUT_ROOT(queue, struct root);
POBJ_LAYOUT_TOID(queue, struct entry);
POBJ_LAYOUT_TOID(queue, struct queue);
POBJ_LAYOUT_END(queue);

struct entry { /* queue entry that contains arbitrary data */
  size_t len;  /* length of the data buffer */
  char data[];
};

struct queue {  /* array-based queue container */
  size_t front; /* position of the first entry */
  size_t back;  /* position of the last entry */

  size_t capacity; /* size of the entries array */
  TOID(struct entry) entries[];
};

struct root {
  TOID(struct queue) queue;
};

static PMEMobjpool *pop;
static TOID(struct root) root;

/*
 * queue_new -- allocates a new queue container using the atomic API
 */
static int queue_new(PMEMobjpool *pop, TOID(struct queue) * q,
                     size_t nentries) {
  int size = sizeof(struct queue) + sizeof(TOID(struct entry)) * nentries;
  pmemobj_zalloc(pop, (PMEMoid *)(q), size, 0);
  D_RW(*q)->capacity = nentries;
  pmemobj_persist(pop, &D_RW(*q)->capacity, sizeof(size_t));
  return 0;
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
static int queue_enqueue(PMEMobjpool *pop, struct queue *queue,
                         const char *data, size_t len) {
  if (queue->capacity - queue_nentries(queue) == 0)
    return -1; /* at capacity */

  /* back is never decreased, need to calculate the real position */
  size_t pos = queue->back % queue->capacity;

  int ret = 0;

  //("inserting %zu: %s\n", pos, data);

  queue->back += 1;
  pmemobj_persist(pop, &queue->back, sizeof(size_t));

  /* now we can safely allocate and initialize the new entry */
  TOID(struct entry) entry = {};
  pmemobj_zalloc(pop, (PMEMoid *)(&entry), sizeof(struct entry) + len, 0);
  D_RW(entry)->len = len;
  pmemobj_persist(pop, &D_RW(entry)->len, sizeof(size_t));
  pmemobj_memcpy_persist(pop, D_RW(entry)->data, data, len);

  queue->entries[pos] = entry;
  pmemobj_persist(pop, &queue->entries[pos], sizeof(TOID(struct entry)));

  return ret;
}

/*
 * queue_dequeue - removes and frees the first element from the queue
 */
static int queue_dequeue(PMEMobjpool *pop, struct queue *queue) {
  if (queue_nentries(queue) == 0) {
    printf("none\n");
    return -1; /* no entries to remove */
  }

  /* front is also never decreased */
  size_t pos = queue->front % queue->capacity;

  int ret = 0;

  // printf("removing %zu: %s\n", pos, D_RO(queue->entries[pos])->data);

  queue->front += 1;
  pmemobj_persist(pop, &queue->front, sizeof(size_t));
  //pmemobj_free((PMEMoid *)(&queue->entries[pos]));
  return ret;
}

/*
 * queue_show -- prints all queue entries
 */
static void queue_show(PMEMobjpool *pop, struct queue *queue) {
  size_t nentries = queue_nentries(queue);
  printf("Entries %zu/%zu\n", nentries, queue->capacity);
  for (size_t i = queue->front; i < queue->back; ++i) {
    size_t pos = i % queue->capacity;
    printf("%zu: %s\n", pos, D_RO(queue->entries[pos])->data);
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

#define SIZE 5000

int pmdk_init(const char *path) {

  if (access(path, F_OK) != 0) {
    if ((pop = pmemobj_create(path, POBJ_LAYOUT_NAME(queue),
                              PMEMOBJ_MIN_POOL * 10, 0666)) == NULL) {
      perror("failed to create pool\n");
      return 1;
    }
  } else {
    if ((pop = pmemobj_open(path, POBJ_LAYOUT_NAME(queue))) == NULL) {
      perror("failed to open pool\n");
      return 1;
    }
  }

  root = POBJ_ROOT(pop, struct root);
  struct root *rootp = D_RW(root);

  if (queue_new(pop, &rootp->queue, SIZE) != 0) {
    fail("failed to create a new queue");
  }
  return 0;
}

void pmdk_run() {
  for (int i = 0; i < SIZE; i++) {
    queue_enqueue(pop, D_RW(D_RW(root)->queue), "hello", 6);
  }
  for (int i = 0; i < SIZE; i++) {
    queue_dequeue(pop, D_RW(D_RW(root)->queue));
  }
}

void pmdk_close() { pmemobj_close(pop); }