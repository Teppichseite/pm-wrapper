#ifndef PMDK_BTREE_H
#define PMDK_BTREE_H
#include <libpmemobj.h>

int pmdk_init(char *path);

void pmdk_close();

void pmdk_btree_insert(int64_t key, const char *value);

const char *pmdk_btree_find(int64_t key);

void pmdk_btree_print();

#endif