#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "memory/memory.h"
#include "quadruples.h"


struct MemorySegment {
    std::vector<int> ints;
    std::vector<double> floats;
    std::vector<bool> bools;
    std::vector<std::string> strings;

    MemorySegment() = default;
    MemorySegment(int n_int, int n_float, int n_bool, int n_string)
        : ints(n_int, 0), floats(n_float, 0.0), bools(n_bool, false), strings(n_string) {}
};

struct Frame {
    MemorySegment locals;
    MemorySegment temps;
    int return_ip = -1;

    explicit Frame(const FuncRecord &f)
        : locals(f.local_vars[0], f.local_vars[1], 0, 0),
          temps(f.temp_vars[0], f.temp_vars[1], f.temp_vars[2], 0) {}
};