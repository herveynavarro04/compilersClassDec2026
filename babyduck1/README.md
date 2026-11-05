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
./build/babyduck tests/valid/04_functions.bd
```

## Test

```bash
cd build
ctest --output-on-failure
```