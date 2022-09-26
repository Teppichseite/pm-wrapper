// SPDX-License-Identifier: BSD-3-Clause
/* Copyright 2015-2019, Intel Corporation */

/* integer hash set implementation which uses only transaction APIs */

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "hashmap_internal.h"
#include <libpmemobj.h>

#include "../../../src/backends/pmdk_backend.h"
#include "../../runtime/pm_wrapper.h"

struct entry {
  uint64_t key;
  int value;

  /* next entry list pointer */
  struct entry *next;
};

struct buckets {
  /* number of buckets */
  size_t nbuckets;
  /* array of lists */
  struct entry *bucket[];
};

struct hashmap_tx {
  /* random number generator seed */
  uint32_t seed;

  /* hash function coefficients */
  uint32_t hash_fun_a;
  uint32_t hash_fun_b;
  uint64_t hash_fun_p;

  /* number of values inserted */
  uint64_t count;

  /* buckets */
  struct buckets *buckets;
};

/*
 * create_hashmap -- hashmap initializer
 */
static void create_hashmap(struct hashmap_tx *hashmap, uint32_t seed) {
  size_t len = INIT_BUCKETS_NUM;
  size_t sz = sizeof(struct buckets) + len * sizeof(struct entry *);

  hashmap->seed = seed;
  do {
    hashmap->hash_fun_a = (uint32_t)rand();
  } while (hashmap->hash_fun_a == 0);
  hashmap->hash_fun_b = (uint32_t)rand();
  hashmap->hash_fun_p = HASH_FUNC_COEFF_P;

  hashmap->buckets = (struct buckets *)pm_alloc(sizeof(struct buckets) * sz);
  if (!hashmap->buckets) {
    return;
  }

  hashmap->buckets->nbuckets = len;
}

/*
 * hash -- the simplest hashing function,
 * see https://en.wikipedia.org/wiki/Universal_hashing#Hashing_integers
 */
static uint64_t hash(struct hashmap_tx *hashmap, const struct buckets *buckets,
                     uint64_t value) {
  uint32_t a = hashmap->hash_fun_a;
  uint32_t b = hashmap->hash_fun_b;
  uint64_t p = hashmap->hash_fun_p;
  size_t len = buckets->nbuckets;

  return ((a * value + b) % p) % len;
}

/*
 * hm_tx_rebuild -- rebuilds the hashmap with a new number of buckets
 */
static void hm_tx_rebuild(struct hashmap_tx *hashmap, size_t new_len) {
  struct buckets *buckets_old = hashmap->buckets;

  if (new_len == 0)
    new_len = buckets_old->nbuckets;

  size_t sz_old =
      sizeof(struct buckets) + buckets_old->nbuckets * sizeof(struct entry *);
  size_t sz_new = sizeof(struct buckets) + new_len * sizeof(struct entry *);

  struct buckets *buckets_new =
      (struct buckets *)pm_alloc(sizeof(struct buckets) * sz_new);

  if (!buckets_new) {
    return;
  }

  buckets_new->nbuckets = new_len;

  for (size_t i = 0; i < buckets_old->nbuckets; ++i) {
    while (!buckets_old->bucket[i]) {
      struct entry *en = buckets_old->bucket[i];
      uint64_t h = hash(hashmap, buckets_new, en->key);

      buckets_old->bucket[i] = en->next;

      en->next = buckets_new->bucket[h];
      buckets_new->bucket[h] = en;
    }
  }

  hashmap->buckets = buckets_new;
  pm_free(buckets_old);
}

/*
 * hm_tx_insert -- inserts specified value into the hashmap,
 * returns:
 * - 0 if successful,
 * - 1 if value already existed,
 * - -1 if something bad happened
 */
int hm_tx_insert(struct hashmap_tx *hashmap, uint64_t key, int value) {
  struct buckets *buckets = hashmap->buckets;
  struct entry *var;

  uint64_t h = hash(hashmap, buckets, key);
  int num = 0;

  for (var = buckets->bucket[h]; var != NULL; var = var->next) {
    if (var->key == key)
      return 1;
    num++;
  }

  int ret = 0;

  struct entry *e = (struct entry *)pm_alloc(sizeof(struct entry));
  if (!e) {
    return -1;
  }
  e->key = key;
  e->value = value;
  e->next = buckets->bucket[h];
  buckets->bucket[h] = e;

  hashmap->count++;
  num++;

  if (ret)
    return ret;

  if (num > MAX_HASHSET_THRESHOLD ||
      (num > MIN_HASHSET_THRESHOLD && hashmap->count > 2 * buckets->nbuckets))
    hm_tx_rebuild(hashmap, buckets->nbuckets * 2);

  return 0;
}

/*
 * hm_tx_remove -- removes specified value from the hashmap,
 * returns:
 * - key's value if successful,
 * - OID_NULL if value didn't exist or if something bad happened
 */
int hm_tx_remove(struct hashmap_tx *hashmap, uint64_t key) {
  struct buckets *buckets = hashmap->buckets;
  struct entry *var, *prev = NULL;

  uint64_t h = hash(hashmap, buckets, key);
  for (var = buckets->bucket[h]; var != NULL; prev = var, var = var->next) {
    if (var->key == key) {
      break;
    }
  }

  if (var == NULL) {
    return 0;
  }
  int ret = 0;

  int retoid = var->value;

  if (prev == NULL) {
    buckets->bucket[h] = var->next;

  } else {
    prev->next = var->next;
  }
  hashmap->count--;
  pm_free(var);

  if (ret) {
    return 0;
  }

  if (hashmap->count < buckets->nbuckets) {
    hm_tx_rebuild(hashmap, buckets->nbuckets / 2);
  }

  return retoid;
}

/*
 * hm_tx_foreach -- prints all values from the hashmap
 */
/*int hm_tx_foreach(struct hashmap_tx *hashmap,
                  int (*cb)(uint64_t key, PMEMoid value, void *arg),
                  void *arg) {
  TOID(struct buckets) buckets = D_RO(hashmap)->buckets;
  TOID(struct entry) var;

  int ret = 0;
  for (size_t i = 0; i < D_RO(buckets)->nbuckets; ++i) {
    if (TOID_IS_NULL(D_RO(buckets)->bucket[i]))
      continue;

    for (var = D_RO(buckets)->bucket[i]; !TOID_IS_NULL(var);
         var = D_RO(var)->next) {
      ret = cb(D_RO(var)->key, D_RO(var)->value, arg);
      if (ret)
        break;
    }
  }

  return ret;
}*/

/*
 * hm_tx_debug -- prints complete hashmap state
 */
static void hm_tx_debug(struct hashmap_tx *hashmap) {
  struct buckets *buckets = hashmap->buckets;
  struct entry *var;

  printf("a: %u b: %u p: %" PRIu64 "\n", hashmap->hash_fun_a,
         hashmap->hash_fun_b, hashmap->hash_fun_p);
  printf("count: %" PRIu64 ", buckets: %zu\n", hashmap->count,
         buckets->nbuckets);

  for (size_t i = 0; i < buckets->nbuckets; ++i) {
    if (buckets->bucket[i] != NULL)
      continue;

    int num = 0;
    printf("%zu: ", i);
    for (var = buckets->bucket[i]; var != NULL; var = var->next) {
      printf("%" PRIu64 " ", var->key);
      num++;
    }
    printf("(%d)\n", num);
  }
}

/*
 * hm_tx_get -- checks whether specified value is in the hashmap
 */
int hm_tx_get(struct hashmap_tx *hashmap, uint64_t key) {
  struct buckets *buckets = hashmap->buckets;
  struct entry *var;

  uint64_t h = hash(hashmap, buckets, key);

  for (var = buckets->bucket[h]; var != NULL; var = var->next)
    if (var->key == key)
      return var->value;

  return 0;
}

/*
 * hm_tx_lookup -- checks whether specified value exists
 */
int hm_tx_lookup(struct hashmap_tx *hashmap, uint64_t key) {
  struct buckets *buckets = hashmap->buckets;
  struct entry *var;

  uint64_t h = hash(hashmap, buckets, key);

  for (var = buckets->bucket[h]; var != NULL; var = var->next)
    if (var->key == key)
      return 1;

  return 0;
}

/*
 * hm_tx_count -- returns number of elements
 */
size_t hm_tx_count(struct hashmap_tx *hashmap) { return hashmap->count; }

/*
 * hm_tx_init -- recovers hashmap state, called after pmemobj_open
 */
int hm_tx_init(struct hashmap_tx *hashmap) {
  srand(hashmap->seed);
  return 0;
}

/*
 * hm_tx_cmd -- execute cmd for hashmap
 */
/*int hm_tx_cmd(PMEMobjpool *pop, TOID(struct hashmap_tx) hashmap, unsigned cmd,
              uint64_t arg) {
  switch (cmd) {
  case HASHMAP_CMD_REBUILD:
    hm_tx_rebuild(pop, hashmap, arg);
    return 0;
  case HASHMAP_CMD_DEBUG:
    if (!arg)
      return -EINVAL;
    hm_tx_debug(pop, hashmap, (FILE *)arg);
    return 0;
  default:
    return -EINVAL;
  }
}*/

int main(int argc, char **argv) {

  if (argc < 3) {
    return 0;
  }

  PmRegionConfig main_region_config = {.file_path = argv[1],
                                       .root_size = sizeof(struct hashmap_tx)};

  PmWrapperConfig config = {.backend = &PMDK_BACKEND,
                            .main_region_config = main_region_config};

  if (pm_init(config) != 0) {
    return 1;
  };

  struct hashmap_tx *root = (struct hashmap_tx *)pm_get_root();
  create_hashmap(root, 234);

  int a = 5;
  hm_tx_insert(root, 10, 5);
  hm_tx_insert(root, 20, 10);

  hm_tx_insert(root, 30, 50);
  hm_tx_insert(root, 40, 90);

  //hm_tx_remove(root, 30);

  hm_tx_debug(root);

  pm_close();
  return 0;
}