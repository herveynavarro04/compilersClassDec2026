#pragma once 
#include <string>
#include "semantic_cube.h"

enum Segment {
    GLOBAL = 0,
    LOCAL = 1,
    TEMP = 2,
    CONST = 3
};

int alloc_address(Segment seg, Type type);
void reset_local_memory();
int get_constant_address(const std::string &value, Type type);
Type type_of_address(int addr);
int segment_count(Segment seg, Type type);
void print_constants();