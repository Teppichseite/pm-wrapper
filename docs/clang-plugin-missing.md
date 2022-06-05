# Pm Wrapper Clang Plugin
1. Support most expression types
    - Addressof operator still missing
2. Support Library code
    - Provide clang plugin a path option, which indicates the source code location
    - On the main source code will be evaluated
2. Support recursive functions
    - If return value is a pointer, a Attribute is needed for the return value
3. Support Dynamic function calls
    - Allow to set Attributes on inline function defintions
    - If the address of a function is used without a call expression, show an error, that this function needs attributes for its input parameters
4. For cases which where the type is unkown, default to VM pointer
    - Print out a warning