// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2015-2019, Intel Corporation */

/*
 * pi.c -- example usage of user lists
 *
 * Calculates pi number with multiple threads using Leibniz formula.
 */
#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"
#include "linked_list.h"
#include <assert.h>
#include <ex_common.h>
#include <inttypes.h>
#include <libpmemobj.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#define PM __attribute__((pointer_type(1)))
#define VM __attribute__((pointer_type(2)))

struct pi_task_proto {
  uint64_t start;
  uint64_t stop;
  long double result;
};

struct pi_task {
  struct pi_task_proto proto;
  LL_NODES(struct pi_task, todo_node) todo;
  LL_NODES(struct pi_task, done_node) done;
};

struct pi {
  LL_LIST(struct pi_task, todo_list) todo;
  LL_LIST(struct pi_task, done_list) done;
};

/*
 * calc_pi -- worker for pi calculation
 */
VM static void *calc_pi(void *arg) {
  struct pi *root = (struct pi *)pm_get_root();
  struct pi_task *task = *(struct pi_task **)arg;

  long double result = 0;
  for (uint64_t i = task->proto.start; i < task->proto.stop; ++i) {
    result += (pow(-1, (double)i) / (2 * i + 1));
  }
  task->proto.result = result;

  return NULL;
}

/*
 * calc_pi_mt -- calculate all the pending to-do tasks
 */
static void calc_pi_mt(void) {
  struct pi *root = (struct pi *)pm_get_root();

  int pending = 0;
  LL_FOREACH(root->todo, todo, item, { pending++; });

  if (pending == 0)
    return;

  int i = 0;
  struct pi_task *tasks =
      (struct pi_task *)malloc(sizeof(struct pi_task) * pending);
  if (tasks == NULL) {
    fprintf(stderr, "failed to allocate tasks\n");
    return;
  }

  LL_FOREACH(root->todo, todo, item, { tasks[i++] = item; });

  VM pthread_t workers[pending];
  for (i = 0; i < pending; ++i)
    if (pthread_create(&workers[i], NULL, calc_pi, &tasks[i]) != 0)
      break;

  for (i = i - 1; i >= 0; --i)
    pthread_join(workers[i], NULL);

  free(tasks);
}

/*
 * prep_todo_list -- create tasks to be done
 */
static int prep_todo_list(int threads, int ops) {
  struct pi *root = (struct pi *)pm_get_root();

  if (!(&root->todo)->head)
    return -1;

  int ops_per_thread = ops / threads;
  uint64_t last = 0; /* last calculated denominator */

  LL_FOREACH(root->done, done, item, {
    if (last < item->proto.stop)
      last = item->proto.stop;
  });

  int i;
  for (i = 0; i < threads; ++i) {
    uint64_t start = last + (i * ops_per_thread);
    struct pi_task_proto proto;
    proto.start = start;
    proto.stop = start + ops_per_thread;
    proto.result = 0;

    struct pi_task *task = (struct pi_task *)pm_alloc(sizeof(struct pi_task));
    if (!task) {
      return 1;
    }

    task->proto = proto;

    LL_INSERT(root->todo, todo, todo);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("usage: %s file-name "
           "[print|done|todo|finish|calc <# of threads> <ops>]\n",
           argv[0]);
    return 1;
  }

  PmRegionConfig main_region_config = {.file_path = argv[1],
                                       .root_size = sizeof(struct pi)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  struct pi *root = (struct pi *)pm_get_root();

  char op = argv[2][0];
  switch (op) {
  case 'p': { /* print pi */
    long double pi_val = 0;
    LL_FOREACH(root->done, done, item, { pi_val += item->proto.result; });

    printf("pi: %.10Lf\n", pi_val * 4);
  } break;
  case 'd': { /* print done list */
    LL_FOREACH(root->done, done, item, {
      printf("(%" PRIu64 " - %" PRIu64 ") = %.10Lf\n", item->proto.start,
             item->proto.stop, item->proto.result);
    });
  } break;
  case 't': { /* print to-do list */
    LL_FOREACH(root->todo, todo, item, {
      printf("(%" PRIu64 " - %" PRIu64 ") = %.10Lf\n", item->proto.start,
             item->proto.stop, item->proto.result);
    });
  } break;
  case 'c': { /* calculate pi */
    if (argc < 5) {
      printf("usage: %s file-name "
             "calc <# of threads> <ops>\n",
             argv[0]);
      return 1;
    }
    int threads = atoi(argv[3]);
    int ops = atoi(argv[4]);
    assert((threads > 0) && (ops > 0));
    if (prep_todo_list(threads, ops) == -1)
      printf("pending todo tasks\n");
    else
      calc_pi_mt();
  } break;
  case 'f': { /* finish to-do tasks */
    calc_pi_mt();
  } break;
  }

  pm_close();

  return 0;
}