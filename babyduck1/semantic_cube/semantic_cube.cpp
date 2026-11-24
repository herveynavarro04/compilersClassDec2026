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
    if (left > FLOAT_T || right > FLOAT_T || op > ASSIGN) return ERROR_T;
    return cube[left][right][op];
};

const char *type_name(Type t) {
    switch (t) {
    case INT_T: return "int";
    case FLOAT_T: return "float";
    case BOOL_T: return "bool";
    case VOID_T: return "void";
    case STRING_T: return "string";
    default: return "error";
 }
}

const char *op_symbol(Op op) {
   switch (op) {
       case ADD:    return "+";
       case SUB:    return "-";
       case MUL:    return "*";
       case DIV:    return "/";
       case LT:     return "<";
       case GT:     return ">";
       case NE_OP:  return "!=";
       case ASSIGN: return "=";
       case FAKE_BOTTOM: return "(";
       case OP_PRINT: return "print";
       case OP_READ: return "read";
       case GOTO: return "goto";
       case GOTOF: return "gotof";
       case ERA: return "era";
       case PARAM: return "param";
       case GOSUB: return "gosub";
       case ENDFUNC: return "endfunc";
       case OP_RETURN: return "return";
       case OP_PRINTLN: return "println";
       case OP_END: return "end";
       default:     return "?";
   }
}