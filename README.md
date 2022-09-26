# pm-wrapper

This library creates an additional layer for persistent memory applications.
Developers can write one program and can switch out the pm library in the background.

## Required dependencies
- make
- docker
- clang-13

## Build compiler
- Run `make build_compiler`
- Compiler is located at `src/build/PmWrapperCompiler`

## Run runtime tests
- Run `make build_docker`
- Run `make run_docker`

## Backends
Currently there are two backend implementations:
- PMDK (https://pmem.io/pmdk/)
- Atlas HP (https://github.com/HewlettPackard/Atlas)

