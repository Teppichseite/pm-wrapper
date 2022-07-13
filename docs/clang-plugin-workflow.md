# Clang Plugin Workflow

1. Load the clang plugin via -Xclang args
2. Get AST of current input files
2. Perform pointer type evaluation
3. Emit errors/warnings
4. Emit pointer type source locations (for editor)
5. Stop workflow if there are errors
6. Write AST as serialized c program to temporarily file
    1. All macros of the c prorams are expanded
7. Reperform pointer type evaluation on AST of temporarily file
8. Perform rewriting on a buffer of the temporarily file
9. Write rewritten buffer to a new temporarily file
10. Perform a new compiler invocation for the new temporarily file
