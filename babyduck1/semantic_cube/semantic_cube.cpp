#include "semantic_cube.h"

static const Type cube[2][2][8] = {
/*left = int */
 {
    /*right = int */ 
    { INT_T, INT_T, INT_T, FLOAT_T, BOOL_T, BOOL_T, BOOL_T, INT_T },
    /*right = float*/
    { FLOAT_T, FLOAT_T, FLOAT_T, FLOAT_T, BOOL_T, BOOL_T, BOOL_T, ERROR_T }
 },
/*left = float*/
 {
   /*right = int*/ 
   { FLOAT_T, FLOAT_T, FLOAT_T, FLOAT_T, BOOL_T, BOOL_T, BOOL_T, FLOAT_T },
    /*right = float*/ 
    { FLOAT_T, FLOAT_T, FLOAT_T, FLOAT_T, BOOL_T, BOOL_T, BOOL_T, FLOAT_T }
 }
};

Type check_types(Type left, Op op, Type right) {
    if (left > FLOAT_T || right > FLOAT_T) return ERROR_T;
    return cube[left][right][op];
};

const char *type_name(Type t) {
    switch (t) {
    case INT_T: return "int";
    case FLOAT_T: return "float";
    case BOOL_T: return "bool";
    case VOID_T: return "void";
    default: return "error";
 }
};