#include <libpmemobj.h>
#include <libpmemobj/base.h>
#include <unistd.h>

#define SIZE 10000

struct holder {
  int values[SIZE];
};

POBJ_LAYOUT_BEGIN(holder);
POBJ_LAYOUT_ROOT(holder, struct holder);
POBJ_LAYOUT_END(holder);

static PMEMobjpool *pop1;
static TOID(struct holder) root1;

static PMEMobjpool *pop2;
static TOID(struct holder) root2;

int pmdk_init(const char *path1, const char *path2) {
  if (access(path1, F_OK) != 0) {
    if ((pop1 = pmemobj_create(path1, POBJ_LAYOUT_NAME(holder),
                               PMEMOBJ_MIN_POOL, 0666)) == NULL) {
      return 1;
    }
  } else {
    if ((pop1 = pmemobj_open(path1, POBJ_LAYOUT_NAME(holder))) == NULL) {
      return 1;
    }
  }

  if (access(path2, F_OK) != 0) {
    if ((pop2 = pmemobj_create(path2, POBJ_LAYOUT_NAME(holder),
                               PMEMOBJ_MIN_POOL, 0666)) == NULL) {
      return 1;
    }
  } else {
    if ((pop2 = pmemobj_open(path2, POBJ_LAYOUT_NAME(holder))) == NULL) {
      return 1;
    }
  }

  root1 = POBJ_ROOT(pop1, struct holder);
  root2 = POBJ_ROOT(pop2, struct holder);
  return 0;
}

void pmdk_run() {
  int rounds = 1;
  for (int i = 0; i < SIZE * rounds; i++) {
    D_RW(root1)->values[i % rounds] = i;
    pmemobj_persist(pop1, &D_RW(root1)->values[i % rounds], sizeof(int));
    D_RW(root2)->values[i % rounds] = i;
    pmemobj_persist(pop2, &D_RW(root2)->values[i % rounds], sizeof(int));
  }
}

void pmdk_close() { pmemobj_close(pop1); }