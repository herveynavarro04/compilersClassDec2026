# Compiler Design & Implementation

Coursework for the Compilers module at Tecnológico de Monterrey, focused on designing and implementing a full compiler pipeline, from language design to execution on a virtual machine.

A compiler reads a program in a source language and translates it into an equivalent program in a target language. The process has two phases:

- **Analysis:** breaks the source program into its components, imposes a grammatical structure on them, and builds an intermediate representation.
- **Synthesis:** constructs the target program from that intermediate representation.

## Topics Covered

- **Language Design**
- **Lexical Analysis:** formal tools for pattern recognition (regular expressions, finite automata)
- **Syntax Analysis:** graphical and formal grammars; top-down and bottom-up parsing
- **Semantic Analysis:** static and dynamic semantics, symbol tables, and semantic cubes for type checking
- **Intermediate Code Generation:** parse-tree traversal generators and stack-machine-based generators
- **Virtual Machine:** execution of the generated intermediate code

## Why It Matters

The principles behind compiler construction apply well beyond compilers, to areas like text processing, DSLs, interpreters, and static analysis tools.