CC = clang
CFLAGS = -Wall -Werror -g -lpmemobj -lpmem -pthread -lm -pthread
PM_WRAPPER_RUNTIME_FILES = ./src/runtime/pm_wrapper.c ./src/runtime/region_id_map.c ./src/runtime/context_map.c ./src/runtime/hashmap/hashmap.c
PM_PMDK_BACKEND = ./src/backends/pmdk_backend.c


all: ./bin/multiple_region

./bin/multiple_region: ./src/examples/multiple_region.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) ./src/examples/multiple_region.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND) $(CFLAGS) -o ./bin/multiple_region

clean:
	rm ./bin/*




