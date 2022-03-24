## PM Wrapper goals
1. Primary: Abstraction without leaky concepts
    - So backend can be switched easily
2. Secondary: More transparent usage of persistant memory
3. (Turn existing vm programs into PM programs)

## Pointer handling
1. There are two types of pointers:
    - VM Pointers: Virtual memory pointer which can be dereferenced
    - PM pointers: Offset within the persistent memory region, can be converted into a VM Pointer with PM_R
2. PM pointers structure:
    1. "Fat pointer"
        - First 8 bytes represent the region id
        - Last 8 bytes represent the region offset
        - Would use an explicit datatype, which does not contain type of the data the pointer is pointing to
        - Similar to PMDK `PMEMoid`
    2. "Normal pointer"
        - 8 byte pointer which contains the region id and pointer offset
        - Type would be a normal pointer type: `data_type *ptr`
        - Datastructures can be reused for vm memory allocations
        - Using normal pointers have the downside that you have to follow the programs logic to find out if a pointer is a PM pointer or a VM pointer
    3. Clang plugin could solve the problem with "Normal pointers"
        - Plugin figures how pointers should be used
        - Static approach
            - Compiler cannot always guess which kind of pointer a value is
            - Attributes <- solution for now
        - Dynamic appraoch
            - Compiler could insert dynamic instruction by checking for specific bits in the pointer
            - Has the downside that it impacts runtime performance

## Flushing and Transaction managment
1. Auto flushing 
    - Each write operations performs an auto flush
    - Helps to easily test programs out of the box
2. Transactions have to be explicitly handled specified
    - with TX_START and TX_END

## Region managment 
1. One primary pool
    - Opened at beginning of program
    - Closed at stop of program
2. Several secondary pools
    - Initialized with `pm_init_sec(PmWrapperConfig config)`
    - Opened and closed anytime during the program
3. Region identification
    - Pm pointer length of `k` bits
    - First `n` (`n` < `k`>) bits represent the region number
    - Last `k - n` bits represent the offset in the region
    - PM Wrapper maps region identification to a real region number
    - Save table in region

## Version managment
1. What if you upgrade your datastructures with new fields or changed fields?

