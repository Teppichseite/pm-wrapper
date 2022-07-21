#include <libpmemobj.h>
#include <libpmemobj/base.h>
#include <unistd.h>

#define SIZE 1000

struct holder {
  int values[SIZE];
};

POBJ_LAYOUT_BEGIN(holder);
POBJ_LAYOUT_ROOT(holder, struct holder);
POBJ_LAYOUT_END(holder);

static PMEMobjpool *pop;
static TOID(struct holder) root;

int pmdk_init(const char *path) {
  if (access(path, F_OK) != 0) {
    if ((pop = pmemobj_create(path, POBJ_LAYOUT_NAME(holder), PMEMOBJ_MIN_POOL,
                              0666)) == NULL) {
      return 1;
    }
  } else {
    if ((pop = pmemobj_open(path, POBJ_LAYOUT_NAME(holder))) == NULL) {
      return 1;
    }
  }

  root = POBJ_ROOT(pop, struct holder);
  return 0;
}

void pmdk_run() {
  int rounds = 1;
  for (int i = 0; i < SIZE * rounds; i++) {
    // D_RW(root)->values[i % rounds] = i;
    pmemobj_memcpy_persist(pop, &D_RW(root)->values[i % rounds], &i,
                           sizeof(int));
  }
}

void pmdk_close() { pmemobj_close(pop); }