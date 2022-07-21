#ifndef PM_WRAPPER_BTREE_H
#define PM_WRAPPER_BTREE_H
#include <stdint.h>

int p_init(char *path);

void p_close();

void btree_insert(int64_t key, const char *value);

const char *btree_find(int64_t key);

void btree_print();

#endif