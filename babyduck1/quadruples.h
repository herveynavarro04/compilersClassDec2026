#pragma once
#include <string>
#include <vector>
#include "semantic_cube/semantic_cube.h"


/*Quadruple structure*/
struct Quadruple {
    Op oper;
    int left_op;
    int right_op;
    int result;
};

struct FuncRecord {
    std::string name;
    int start_quad;
    int num_params;
    int local_vars[2];
    int temp_vars[3];
    int return_address;
    std::vector<int> param_addresses;
};

const std::vector<Quadruple> &get_quadruples();
const std::vector<Quadruple> &get_functions();
const std::vector<Quadruple> &get_main_record();

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
void npq16_print_end();

/*Neuralgic points for conditional and loop statements*/
void npj1_if_condition();
void npj2_if_end();
void npj3_else();
void npj4_while_start();
void npj5_while_condition();
void npj6_while_end();

/*Neuralgic points for the program entry (jump to main)*/
void npm1_goto_main();
void npm2_fill_main();

/*Neuralgic points for function declaration*/
void npf4_count_params();
void npf5_count_locals();
void npf6_set_start();
void npf7_end_function();
void npr1_return();

/*Neuralgic points for function calls*/
void npc1_verify_function(const char *name);
void npc2_era();
void npc3_param();
void npc4_next_param();
void npc5_check_args();
void npc6_gosub();
void npc7_return_value();

/*Quadruple print*/
void print_quadruples();
void print_functions();

/*Global count*/
int get_global_count(Type type);