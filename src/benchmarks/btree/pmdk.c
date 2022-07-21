// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2015-2017, Intel Corporation */

/*
 * btree.c -- implementation of persistent binary search tree
 */

#include <inttypes.h>
#include <libpmemobj.h>
#include <libpmemobj/atomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

POBJ_LAYOUT_BEGIN(btree);
POBJ_LAYOUT_ROOT(btree, struct btree);
POBJ_LAYOUT_TOID(btree, struct btree_node);
POBJ_LAYOUT_END(btree);

struct btree_node {
  int64_t key;
  TOID(struct btree_node) slots[2];
  char value[];
};

struct btree {
  TOID(struct btree_node) root;
};

static TOID(struct btree) btree;
static PMEMobjpool *pop;

/*
 * btree_insert -- inserts new element into the tree
 */
void pmdk_btree_insert(int64_t key, const char *value) {
  TOID(struct btree_node) *dst = &D_RW(btree)->root;

  while (!TOID_IS_NULL(*dst)) {
    dst = &D_RW(*dst)->slots[key > D_RO(*dst)->key];
  }

  int size = sizeof(struct btree_node) + strlen(value) + 1;
  pmemobj_zalloc(pop, (PMEMoid *)(dst), size, 0);
  D_RW(*dst)->key = key;
  memcpy(D_RW(*dst)->value, value, strlen(value) + 1);
}

/*
 * btree_find -- searches for key in the tree
 */
const char *pmdk_btree_find(int64_t key) {
  TOID(struct btree_node) node = D_RO(btree)->root;

  while (!TOID_IS_NULL(node)) {
    if (D_RO(node)->key == key)
      return D_RO(node)->value;
    else
      node = D_RO(node)->slots[key > D_RO(node)->key];
  }

  return NULL;
}

/*
 * btree_node_print -- prints content of the btree node
 */
void pmdk_btree_node_print(const TOID(struct btree_node) node) {
  printf("%" PRIu64 " %s\n", D_RO(node)->key, D_RO(node)->value);
}

/*
 * btree_foreach -- invoke callback for every node
 */
void pmdk_btree_foreach(const TOID(struct btree_node) node,
                        void (*cb)(const TOID(struct btree_node) node)) {
  if (TOID_IS_NULL(node))
    return;

  pmdk_btree_foreach(D_RO(node)->slots[0], cb);

  cb(node);

  pmdk_btree_foreach(D_RO(node)->slots[1], cb);
}

/*
 * btree_print -- initiates foreach node print
 */
void pmdk_btree_print() {
  pmdk_btree_foreach(D_RO(btree)->root, pmdk_btree_node_print);
}

int pmdk_init(char *path) {
  if (access(path, F_OK) != 0) {
    if ((pop = pmemobj_create(path, POBJ_LAYOUT_NAME(btree), PMEMOBJ_MIN_POOL,
                              0666)) == NULL) {
      perror("failed to create pool\n");
      return 1;
    }
  } else {
    if ((pop = pmemobj_open(path, POBJ_LAYOUT_NAME(btree))) == NULL) {
      perror("failed to open pool\n");
      return 1;
    }
  }

  btree = POBJ_ROOT(pop, struct btree);

  return 0;
}

void pmdk_close() { pmemobj_close(pop); }