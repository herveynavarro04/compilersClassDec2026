#pragma once
#include <string>
#include "semantic_cube/semantic_cube.h"


/*Quadruple structure*/
struct Quadruple {
    Op oper;
    int left_op;
    int right_op;
    int result;
};

/*Needed neuralgic points to generate quadruples*/
void npq1_push_operand(const char *name);
void npq2_push_add_sub(Op op);
void npq3_push_mul_div(Op op);
void npq4_gen_add_sub();
void npq5_gen_mul_div();
void npq6_push_constant(const char *value, Type type);
void npq7_push_fake_bottom();
void npq8_pop_fake_bottom();
void npq9_push_rel_op(Op op);
void npq10_gen_rel();
void npq11_unary_minus();
void npq12_assign(const char *name);
void npq13_print_expression();
void npq14_print_string(const char *text);
void npq15_read(const char *name);

/*Quadruple print*/
void print_quadruples();

