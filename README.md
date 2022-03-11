# pm-wrapper

This library creates an additional layer for persistent memory applications.
Developers can write one program and can switch out the pm library in the background.

## Required dependencies
- make
- clang 
    - you can also adjust the CC variable in the Makefile
- libpmemobj

## Documentation
File `./src/pm_wrapper.h` explains all important wrapper functions.

## Examples
There are two versions of a btree program:
- `./src/examples/btree_direct.c` uses libmemobj directly
- `./src/examples/btree_wrapper.c` uses the pm wrapper
- run `make` to build the programs


## Backend examples
File `pmdk_backend.c` implements an example backend with libmemobj.

