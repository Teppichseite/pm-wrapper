CC = clang-13
CFLAGS = -Wall -g -O3 -fno-omit-frame-pointer -pthread -lm -pthread -lstdc++
PM_WRAPPER_RUNTIME_FILES = ../runtime/pm_wrapper.c ../runtime/region_id_map.c ../runtime/context_map.c ../runtime/hashmap/hashmap.c
PM_PMDK_BACKEND = ../backends/pmdk_backend.c

btree: ./bin/btree
store: ./bin/store
queue: ./bin/queue
multi: ./bin/multi

./bin/btree: ./btree/main.c ./btree/pmdk.c ./btree/pm_wrapper_btree.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) $(CFLAGS) ./btree/main.c ./btree/pmdk.c ./btree/pm_wrapper_btree.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND) -lpmemobj -lpmem -o ./bin/btree
 
./bin/store: ./store/main.c ./store/pmdk_store.c ./store/pm_wrapper_store.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) $(CFLAGS) ./store/main.c ./store/pmdk_store.c ./store/pm_wrapper_store.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND) -lpmemobj -lpmem -o ./bin/store
	
./bin/queue: ./queue/main.c ./queue/pmdk_queue.c ./queue/pm_wrapper_queue.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) $(CFLAGS) ./queue/main.c ./queue/pmdk_queue.c ./queue/pm_wrapper_queue.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND) -lpmemobj -lpmem -o ./bin/queue
	
./bin/multi: ./multiregion/main.c ./multiregion/pmdk_multiregion.c ./multiregion/pm_wrapper_multiregion.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) $(CFLAGS) ./multiregion/main.c ./multiregion/pmdk_multiregion.c ./multiregion/pm_wrapper_multiregion.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND) -lpmemobj -lpmem -o ./bin/multi

clean:
	rm ./bin/*


