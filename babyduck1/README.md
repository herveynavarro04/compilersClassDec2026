# BabyDuck

## Requirements

- C++17 compiler
- CMake ≥ 3.16
- Bison ≥ 3.0
- Flex ≥ 2.6

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Run

```bash
./build/babyduck tests/syntax/valid/03_functions.bd
```

To print the generated quadruples for a program, pass `--quads`:

```bash
./build/babyduck --quads tests/quadruples/valid/01_variables_and_expressions.bd
```

## Tests

Tests live under `tests/` and are organized by compiler phase. Each phase has
its own `valid/` (programs that must be accepted, exit 0) and `invalid/`
(programs that must be rejected, exit != 0) folder.

```
tests/
  syntax/        lexical and syntactic rules
    valid/
    invalid/
  semantics/     function directory, variable tables, semantic cube
    valid/
    invalid/
  quadruples/    quadruple generation for expressions and linear statements
    valid/
    invalid/
```

Run the full suite:

```bash
cd build
ctest --output-on-failure
```

Each test is named `<phase>_<valid|invalid>_<file>`, so you can filter by
phase, e.g. `ctest -R quadruples_`.