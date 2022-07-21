1. Introduction
- New type of memory: Persistent memory
- Allows programs like redis to be faster
- Due to the persistency it requires addtional effort to create a safe program
- Programs are heavily depending on one library
- Existing programs are hard to port to one of these libraries
- Existing libraries lack in multiple dimensions in terms of usability

2. Background
- Quick introduction how persistent memory usually works
    - explanation of Memory mapped file

3. Related work
- PMDK/Libpmemobj
    - How the pointer logic works
    - Supports relocateable pointers
    - Not very readable
- Atlas
    - Directly operates on mapped memory
    - No relocateable pointers
- Relocateable pointers paper:
    - Shows how to store a fat pointer as a thin pointer
    - Uses runtime checks to insert PM pointer operations
- go-pmem
    - Directly support persistent memory pointers within the go compiler

4. Solution to these problems:
    1. Not depending on one single persistent memory library
    2. You can write very readable code which looks like usual c code
    3. It still allows enough freedom to the developer to interact with the characteristics of persistent memory
    4. Showcase of design...



