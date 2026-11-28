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

## Run a program with the VM

The compiler emits an `.obj` file (default `program.obj`, or whatever `-o` says)
that the Python VM in `vm.py` executes:

```bash
./build/babyduck tests/runtime/valid/10_recursion.bd -o program.obj
python3 vm.py program.obj
```

Programs that use `read(id)` consume lines from standard input, one value per
line:

```bash
./build/babyduck tests/runtime/valid/11_read.bd -o program.obj
printf '4\n7\n' | python3 vm.py program.obj
```

The VM exits with `0` on success, `3` if the `.obj` file cannot be opened, and
`6` on any runtime error (division by zero, reading an uninitialized variable,
bad input for `read`, etc.).

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
  statements/    control-flow statements (if, while) at the compile stage
  functions/     function declarations, calls, parameters, return
  runtime/       end-to-end execution: compile with babyduck, run with vm.py
    valid/         programs with expected output (checked by regex)
    invalid/       programs that compile but must fail at runtime (exit 6)
```

The runtime tests use the helper script `tests/run_program.sh`, which compiles
a `.bd` program into `build/runtime_obj/<name>.obj` and feeds it to `vm.py`
(optionally with piped stdin for programs that use `read`). To run only the
runtime phase:

```bash
cd build
ctest -R runtime_ --output-on-failure
```

Run the full suite:

```bash
cd build
ctest --output-on-failure
```

Each test is named `<phase>_<valid|invalid>_<file>`, so you can filter by
phase, e.g. `ctest -R quadruples_`.