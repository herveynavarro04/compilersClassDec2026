#include "quadruples.h"
#include "semantics.h"
#include <stack>
#include <vector>
#include <cstdio>

static std::stack<std::string> stack_operands;
static std::stack<Type> stack_types;
static std::stack<Op> stack_operators;
static std::vector<Quadruple> quadruples;

static int temp_counter = 0;

static std::string new_temp(){
    temp_counter++;
    return "t" + std::to_string(temp_counter);
}

static const char *field(const std::string &s) {
    return s.empty() ? "_" : s.c_str();
}

static void gen_quadruple() {
    Op op = stack_operators.top();
    stack_operators.pop();                                  

    std::string right_operand = stack_operands.top();
    stack_operands.pop();
    Type right_type = stack_types.top();
    stack_types.pop();

    std::string left_operand = stack_operands.top();
    stack_operands.pop();
    Type left_type = stack_types.top();
    stack_types.pop();

    Type type_res = check_types(left_type, op, right_type); 
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: ") + type_name(left_type)
                       + " " + op_symbol(op) + " " + type_name(right_type));
    }
    
    std::string temp = new_temp();
    quadruples.push_back( Quadruple { op, left_operand, right_operand, temp } );
    stack_operands.push(temp);
    stack_types.push(type_res);
}

void print_quadruples() {
    std::printf("%3s  %-6s %-8s %-8s %-8s\n", "#", "op", "left", "right", "result");
    for (size_t i = 0; i < quadruples.size(); ++i) {
        const Quadruple &q = quadruples[i];
        std::printf("%3zu  %-6s %-8s %-8s %-8s\n",
                    i + 1,
                    op_symbol(q.oper),
                    field(q.left_op),
                    field(q.right_op),
                    field(q.result));
    }
}

void npq1_push_operand(const char *name){
    Type type = var_type(name);
    stack_operands.push(name);
    stack_types.push(type);
}

void npq2_push_add_sub(Op op){
    if (op != ADD && op != SUB){
        internal_error("npq2 received an operator other than + or -");
    }
    stack_operators.push(op);
}

void npq3_push_mul_div(Op op){
    if (op != MUL && op != DIV){
        internal_error("npq3 received an operator other than * or /");
    }
    stack_operators.push(op);
}

void npq4_gen_add_sub() {
    if (stack_operators.empty()) {
        return;
    }
    Op op = stack_operators.top();
    if (op != ADD && op != SUB) {
        return;
    }
    gen_quadruple();
}

void npq5_gen_mul_div() {
    if (stack_operators.empty()) {
        return;
    }
    Op op = stack_operators.top();
    if (op != MUL && op != DIV) {
        return;
    }
    gen_quadruple();
}

void npq6_push_constant(const char *value, Type type){
    stack_operands.push(value);
    stack_types.push(type);
}

void npq7_push_fake_bottom(){
    stack_operators.push(FAKE_BOTTOM);
}

void npq8_pop_fake_bottom(){
    if (stack_operators.empty() || stack_operators.top() != FAKE_BOTTOM){
        internal_error("npq8 received an operator other than (");
    }
    stack_operators.pop();
}

void npq9_push_rel_op(Op op){
    if (op != GT && op != LT && op != NE_OP){
        internal_error("npq9 received an operator other than >, <, !=");
    }
    stack_operators.push(op);
}

void npq10_gen_rel(){
    if (stack_operators.empty()) {
        return;
    }
    Op op = stack_operators.top();
    if (op != GT && op != LT && op != NE_OP) {
        return;
    }
    gen_quadruple();
}

void npq11_unary_minus(){
    std::string operand = stack_operands.top();
    stack_operands.pop();

    Type type = stack_types.top();
    stack_types.pop();

    Type type_res = check_types(INT_T, SUB, type); 
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: unary - on ") + type_name(type));
    }

    std::string temp = new_temp();
    quadruples.push_back(Quadruple { SUB, "0", operand, temp});

    stack_operands.push(temp);
    stack_types.push(type_res);
}

void npq12_assign(const char *name) {
    std::string right_operand = stack_operands.top();
    stack_operands.pop();
    Type right_type = stack_types.top();
    stack_types.pop();

    Type left_type = var_type(name);

    Type type_res = check_types(left_type, ASSIGN, right_type);
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: cannot assign ") + type_name(right_type)
                       + " to " + type_name(left_type) + " '" + name + "'");
    }

    quadruples.push_back(Quadruple{ ASSIGN, right_operand, "", name });
}

void npq13_print_expression(){
    std::string operand = stack_operands.top();
    stack_operands.pop();    
    stack_types.pop();

    quadruples.push_back(Quadruple{ OP_PRINT, operand, "", "" });

}

void npq14_print_string(const char *text){    
    quadruples.push_back(Quadruple{ OP_PRINT, text, "", "" });
}

void npq15_read(const char *name){
    var_type(name);
    quadruples.push_back(Quadruple{ OP_READ, "", "", name });
}
