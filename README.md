# pm-wrapper

This library creates an additional layer for persistent memory applications.
Developers can write one program and can switch out the pm library in the background.

## Required dependencies
- make
- docker

## Run Tests
- Run `make build_docker`
- Run `make run_docker`

## Backends
Currently there are two backend implementations:
- PMDK (https://pmem.io/pmdk/)
- Atlas HP (https://github.com/HewlettPackard/Atlas)

