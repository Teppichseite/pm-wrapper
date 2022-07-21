#include "pm_wrapper_btree.h"
#include "pmdk.h"
#include <stdio.h>
#include <time.h>

#define SAMPLE_COUNT 15000

double time_wrapper(void function(int), int count) {
  struct timespec t_start;
  struct timespec t_end;
  int iterations = 3;

  clock_gettime(CLOCK_MONOTONIC, &t_start);

  for (int i = 0; i < iterations; i++) {
    function(count);
  }

  clock_gettime(CLOCK_MONOTONIC, &t_end);

  return ((double)((t_end.tv_sec + t_end.tv_nsec * 1e-9)) -
          (t_start.tv_sec + t_start.tv_nsec * 1e-9)) /
         iterations;
}

void run_pmdk(int count) {
  for (int i = 0; i < count; i++) {
    pmdk_btree_insert(i, "test");
  }
  for (int i = 0; i < count; i++) {
    const char *value = pmdk_btree_find(i);
  }
}

void run_pm_wrapper(int count) {
  for (int i = 0; i < count; i++) {
    btree_insert(i, "test");
  }
  for (int i = 0; i < count; i++) {
    const char *value = btree_find(i);
  }
}

void bench_pmdk(int count) {
  pmdk_init("./btree_pmdk");
  double time = time_wrapper(run_pmdk, count);
  printf("PMDK: %i inserts and %i lookups took %f seconds \n", count, count,
         time);
  pmdk_close();
}

void bench_pm_wrapper(int count) {
  p_init("./btree_pm_wrapper");
  double time = time_wrapper(run_pm_wrapper, count);
  printf("pm-wrapper: %i inserts and %i lookups took %f seconds \n", count,
         count, time);
  p_close();
}

int main() {
  bench_pmdk(100);
  bench_pm_wrapper(100);

  bench_pmdk(500);
  bench_pm_wrapper(500);

  bench_pmdk(1000);
  bench_pm_wrapper(1000);

  bench_pmdk(1500);
  bench_pm_wrapper(1500);
  return 0;
}