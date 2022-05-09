# Pm Wrapper Snippets Draft
Here are some example how the PM wrapper looks like from a user perspective.
- Suffix `pm` means it uses the `pm` wrapper library
- Suffix `vm` means it uses regular heap allocation
- Suffix `pdmk` mean it uses directly the PMDK library
- Suffix `atlas` mean it uses directly the ATLAS library

## Init
- Shows how to initialize and close the pm-wrapper library
- These steps will be omitted in the following examples
- PM, PMDK and Atlas example

## Alloc
- Shows how to allocate data
- VM, PM, PMDK and Atlas example

## Array
- Shows how to allocate an array 
- Shows how to iterate through an array
- Shows how to write to an array
- PM, PMDK and Atlas example

## Pointer arguments
- Shows how to pass a pointer to a function
- PM, PMDK and Atlas example

## Store pointer
- Shows how to store a pointer in allocated memory
- PM, PMDK and Atlas example

## Multiple region
- Shows how to work with multiple regions
- PM only example, since for Atlas and PMDK this task is the same as for the init task

## Transactions
- Shows how to work with transactions
- PM, PMDK and Atlas example