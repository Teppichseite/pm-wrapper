# pm-wrapper

This library creates an additional layer for persistent memory applications.
Developers can write one program and can switch out the pm library in the background.

## Required dependencies
- make
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



