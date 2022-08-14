#include "pm_wrapper_multiregion.h"
#include "pmdk_multiregion.h"
#include <stdio.h>
#include <time.h>

double time_wrapper(void function(int), int count) {
  struct timespec t_start;
  struct timespec t_end;
  int iterations = 1;

  clock_gettime(CLOCK_MONOTONIC, &t_start);

  for (int i = 0; i < iterations; i++) {
    function(count);
  }

  clock_gettime(CLOCK_MONOTONIC, &t_end);

  return ((double)((t_end.tv_sec + t_end.tv_nsec * 1e-9)) -
          (t_start.tv_sec + t_start.tv_nsec * 1e-9)) /
         iterations;
}

void run_pmdk(int count) { pmdk_run(); }

void bench_pmdk() {
  pmdk_init("./pmdk_multi1", "./pmdk_multi2");
  double time = time_wrapper(run_pmdk, 0);
  printf("PMDK took %f seconds \n", time);
  pmdk_close();
}

void run_pm_wrapper(int count) { pm_store_run(); }

void bench_pm_wrapper() {
  pm_store_init("./pm_multi", "./pm_multi1", "./pm_multi2");
  double time = time_wrapper(run_pm_wrapper, 0);
  printf("PM Wrapper took %f seconds \n", time);
  pm_store_close();
}

int main() {
  //bench_pmdk();
  bench_pm_wrapper();
  return 0;
}