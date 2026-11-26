#include "memory.h"
#include "semantic_cube/semantic_cube.h"
#include "semantics.h"
#include <unordered_map>
#include <map>
#include <cstdio>

const int SEGMENT_SIZE = 1000;

static const char *segment_name[4] = { "global", "local", "temporary", "constant" };

const int base[4][6] = {
    //INT_T, FLOAT_T, BOOL_T, ERROR_T, VOID_T, STRING_T
    {1000, 2000, -1, -1, -1, -1}, // Global
    {3000, 4000, -1, -1, -1, -1}, // Local
    {5000, 6000, 7000, -1, -1, -1}, // TEMP
    {8000, 9000, -1, -1, -1, 10000} // CONST
};

static int seg_counter[4][6] = {};

static std::unordered_map<std::string, int> constant_table;

int alloc_address(Segment seg, Type type) {
    int var_seg = base[seg][type];

    if (var_seg == - 1) {
        internal_error("no memory segment for " + std::string(segment_name[seg])
               + " " + type_name(type) + " values");
    }

    if (seg_counter[seg][type] >= SEGMENT_SIZE) {
        semantic_error("out of memory: too many " + std::string(segment_name[seg])
               + " " + type_name(type) + " values");
    }

    int addr = var_seg + seg_counter[seg][type];
    seg_counter[seg][type]++;

    return addr;
}

void reset_local_memory() {
    for (int i= 0; i <= 5; i++) {
        seg_counter[LOCAL][i] = 0;
        seg_counter[TEMP][i] = 0;
    }
}

int get_constant_address(const std::string &value, Type type){
    auto it  = constant_table.find(value);
    if (it != constant_table.end()) {
        return it->second;
    }

    int addr = alloc_address(CONST, type);
    constant_table[value] = addr;
    
    return addr;
}

Type type_of_address(int addr) {
    for (int s = 0; s < 4; s++) {
        for (int t = 0; t < 6; t++) {
            int start = base[s][t];
            if (start != -1 && addr >= start && addr < start + SEGMENT_SIZE) {
                return static_cast<Type>(t);
            }
        }
    }
    return ERROR_T;
}

void print_constants() {
    std::map<int, std::string> by_address;
    for (const auto &entry : constant_table) {
        by_address[entry.second] = entry.first;
    }
    std::printf("%-8s %s\n", "address", "value");
    for (const auto &entry : by_address) {
        std::printf("%-8d %s\n", entry.first, entry.second.c_str());
    }
}

int segment_count(Segment seg, Type type) {
    return seg_counter[seg][type];
}

Type type_of_address(int addr) {
    for (int s = 0; s < 4; s++) {
        for (int t = 0; t < 6; t++) {
            int start = base[s][t];
            if (start != -1 && addr >= start && addr < start + SEGMENT_SIZE) {
                return static_cast<Type>(t);
            }
        }
    }
    return ERROR_T;
}