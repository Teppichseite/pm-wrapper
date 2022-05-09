CC = clang
CFLAGS = -Wall -Werror -g -pthread -lm -pthread -lstdc++
PM_WRAPPER_RUNTIME_FILES = ./src/runtime/pm_wrapper.c ./src/runtime/region_id_map.c ./src/runtime/context_map.c ./src/runtime/hashmap/hashmap.c
PM_WRAPPER_TEST_FILES = ./src/tests/multiple_region/multiple_region_test.c
PM_PMDK_BACKEND = ./src/backends/pmdk_backend.c -lpmemobj -lpmem
PM_ATLAS_BACKEND = ./src/backends/atlas_backend.c ../Atlas/runtime/Atlas-Build/lib/libatlas.a

pmdk: ./bin/multiple_region_pmdk
atlas: ./bin/multiple_region_atlas

./bin/multiple_region_pmdk: ./src/tests/multiple_region/multiple_region_pmdk.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_PMDK_BACKEND)
	mkdir ./bin -p
	$(CC) ./src/tests/multiple_region/multiple_region_pmdk.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_WRAPPER_TEST_FILES) $(PM_PMDK_BACKEND) $(CFLAGS) -o ./bin/multiple_region_pmdk

./bin/multiple_region_atlas: ./src/tests/multiple_region/multiple_region_atlas.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_ATLAS_BACKEND)
	mkdir ./bin -p
	$(CC) ./src/tests/multiple_region/multiple_region_atlas.c $(PM_WRAPPER_RUNTIME_FILES) $(PM_WRAPPER_TEST_FILES) $(PM_ATLAS_BACKEND) $(CFLAGS) -o ./bin/multiple_region_atlas

clean:
	rm ./bin/*

build_docker:
	docker build . -t test-atlas

run_docker:
	docker run -t test-atlas


