#pragma once 

// Values are used as indexes into the semantic cube
enum Type {
    INT_T = 0,
    FLOAT_T = 1,
    BOOL_T = 2,
    ERROR_T = 3,
    VOID_T = 4        
};

enum Op {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
    LT = 4,
    GT = 5, 
    NE_OP = 6,
    ASSIGN = 7
};

// Returns the result type of (left op right), or ERROR_T if not allowed.
Type check_types(Type left, Op op, Type right);

// Returns types as strings for error messages
const char *type_name(Type t);