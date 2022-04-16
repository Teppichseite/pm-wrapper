CC = clang
CFLAGS = -Wall -Werror -g -lpmemobj -lpmem -pthread -lm -pthread
PM_WRAPPER_FILES = ./src/pm_wrapper.c ./src/pmdk_backend.c ./src/region_id_map.c ./src/context_map.c ./src/hashmap/hashmap.c

all: ./bin/multiple_region

./bin/multiple_region: ./src/examples/multiple_region/multiple_region.c $(PM_WRAPPER_FILES)
	mkdir ./bin -p
	$(CC) ./src/examples/multiple_region/multiple_region.c $(PM_WRAPPER_FILES) $(CFLAGS) -o ./bin/multiple_region

clean:
	rm ./bin/*




