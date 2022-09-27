# pm-wrapper
Persistent memory allows programs to perform operations on non volatile memory
which has lower latency than NAND SSDs. The programmer can interact with this
kind of memory via different libraries. Persistent memory access is very often not
straightforward compared to regular volatile memory access. pm-wrapper introduces
a new programming model for persistent memory. The programmer can implement
persistent memory operations with an existing library as backend. An additional
layer allows an application to seamlessly adopt and interact with the backend for its
persistent memory accesses in a similar fashion with the regular volatile heap memory
accesses. This project was part of a bachelor thesis at the Technical University of Munich. 
You can read more about persistent memory and the design of pm-wrapper in the [report](https://github.com/TUM-DSE/research-work-archive/blob/main/archive/2022/summer/docs/bsc_werndle_programming_model_for_hybrid_persistent_memory_systems.pdf) itself.

## Required dependencies
- make
- cmake
- docker
- clang-13

## Build compiler
- Run `make build_compiler`
- Compiler is located at `./bin/PmWrapperCompiler`

## Run runtime tests
- Run `make build_docker`
- Run `make run_docker`

## Run example
- Run `make queue`
- Run `./bin/queue test-file new 10`
- Run `./bin/queue enqueue 20`
- Run `./bin/queue show`

## Compiler interface
- The compiler will generate an object file from the input source file
- Usage: `PM_WRAPPER_OUTPUT_PATH=./out/file ./bin/PmWrapperCompiler ./program.c`
- Checkout the build command for `./src/compiler/build/queue.o` in the makefile

## Backends
Currently there are two backend implementations:
- PMDK (https://pmem.io/pmdk/)
- Atlas HP (https://github.com/HewlettPackard/Atlas)

## VSCode extension
- Run `make build_vscode_extension`
- Extension will be located at `./bin/pmwrappersense.vsix`
- Run `code --install-extension ./bin/pmwrappersense.vsix`



