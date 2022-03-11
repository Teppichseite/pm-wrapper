CC = clang
CFLAGS = -Wall -Werror -g -lpmemobj -lpmem -pthread -lm -pthread
PM_WRAPPER_FILES = ./src/pm_wrapper.c ./src/pmdk_backend.c

all: ./bin/btree_wrapper ./bin/btree_direct

./bin/btree_wrapper: ./src/examples/btree_wrapper.c $(PM_WRAPPER_FILES)
	mkdir ./bin -p
	$(CC) ./src/examples/btree_wrapper.c $(PM_WRAPPER_FILES) $(CFLAGS) -o ./bin/btree_wrapper

./bin/btree_direct: ./src/examples/btree_direct.c $(PM_WRAPPER_FILES)
	mkdir ./bin -p
	$(CC) ./src/examples/btree_direct.c $(PM_WRAPPER_FILES) $(CFLAGS) -o ./bin/btree_direct

clean:
	rm ./bin/*




