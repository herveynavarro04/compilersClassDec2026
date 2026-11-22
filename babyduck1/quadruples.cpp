#include "quadruples.h"
#include "memory/memory.h"
#include "semantics.h"
#include <stack>
#include <vector>
#include <cstdio>

static std::stack<int> stack_operands;
static std::stack<int> stack_jumps;
static std::stack<Type> stack_types;
static std::stack<Op> stack_operators;
static std::vector<Quadruple> quadruples;

static const int EMPTY = -1;

static int next_quad() {
    return static_cast<int>(quadruples.size());
}

static void fill(int quad, int target) {
    quadruples[quad].result = target;
}

static int new_temp(Type type) {
    return alloc_address(TEMP, type);
}

static void print_field(int value) {
    if (value == EMPTY) std::printf("%-8s ", "_");
    else                std::printf("%-8d ", value);
}

static void gen_quadruple() {
    Op op = stack_operators.top();
    stack_operators.pop();

    int right_operand = stack_operands.top();
    stack_operands.pop();
    Type right_type = stack_types.top();
    stack_types.pop();

    int left_operand = stack_operands.top();
    stack_operands.pop();
    Type left_type = stack_types.top();
    stack_types.pop();

    Type type_res = check_types(left_type, op, right_type);
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: ") + type_name(left_type)
                       + " " + op_symbol(op) + " " + type_name(right_type));
    }

    int temp = new_temp(type_res);
    quadruples.push_back(Quadruple{ op, left_operand, right_operand, temp });
    stack_operands.push(temp);
    stack_types.push(type_res);
}

void print_quadruples() {
    std::printf("%3s  %-6s %-8s %-8s %-8s\n", "#", "op", "left", "right", "result");
    for (size_t i = 0; i < quadruples.size(); ++i) {
        const Quadruple &q = quadruples[i];
        std::printf("%3zu  %-6s ", i + 1, op_symbol(q.oper));
        print_field(q.left_op);
        print_field(q.right_op);
        if ((q.oper == GOTO || q.oper == GOTOF) && q.result != EMPTY) {
            std::printf("%-8d ", q.result + 1);
        } else {
            print_field(q.result);
        }
        std::printf("\n");
    }
}

void npq1_push_operand(const char *name) {
    VarInfo var = var_type(name);
    stack_operands.push(var.address);
    stack_types.push(var.type);
}

void npq2_push_add_sub(Op op) {
    if (op != ADD && op != SUB) {
        internal_error("npq2 received an operator other than + or -");
    }
    stack_operators.push(op);
}

void npq3_push_mul_div(Op op) {
    if (op != MUL && op != DIV) {
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

void npq6_push_constant(const char *value, Type type) {
    stack_operands.push(get_constant_address(value, type));
    stack_types.push(type);
}

void npq7_push_fake_bottom() {
    stack_operators.push(FAKE_BOTTOM);
}

void npq8_pop_fake_bottom() {
    if (stack_operators.empty() || stack_operators.top() != FAKE_BOTTOM) {
        internal_error("npq8 received an operator other than (");
    }
    stack_operators.pop();
}

void npq9_push_rel_op(Op op) {
    if (op != GT && op != LT && op != NE_OP) {
        internal_error("npq9 received an operator other than >, <, !=");
    }
    stack_operators.push(op);
}

void npq10_gen_rel() {
    if (stack_operators.empty()) {
        return;
    }
    Op op = stack_operators.top();
    if (op != GT && op != LT && op != NE_OP) {
        return;
    }
    gen_quadruple();
}

void npq11_unary_minus() {
    int operand = stack_operands.top();
    stack_operands.pop();
    Type type = stack_types.top();
    stack_types.pop();

    Type type_res = check_types(INT_T, SUB, type);
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: unary - on ") + type_name(type));
    }

    int zero = get_constant_address("0", INT_T);
    int temp = new_temp(type_res);
    quadruples.push_back(Quadruple{ SUB, zero, operand, temp });

    stack_operands.push(temp);
    stack_types.push(type_res);
}

void npq12_assign(const char *name) {
    int right_operand = stack_operands.top();
    stack_operands.pop();
    Type right_type = stack_types.top();
    stack_types.pop();

    VarInfo var = var_type(name);

    Type type_res = check_types(var.type, ASSIGN, right_type);
    if (type_res == ERROR_T) {
        semantic_error(std::string("type mismatch: cannot assign ") + type_name(right_type)
                       + " to " + type_name(var.type) + " '" + name + "'");
    }

    quadruples.push_back(Quadruple{ ASSIGN, right_operand, EMPTY, var.address });
}

void npq13_print_expression() {
    int operand = stack_operands.top();
    stack_operands.pop();
    stack_types.pop();

    quadruples.push_back(Quadruple{ OP_PRINT, operand, EMPTY, EMPTY });
}

void npq14_print_string(const char *text) {
    int addr = get_constant_address(text, STRING_T);
    quadruples.push_back(Quadruple{ OP_PRINT, addr, EMPTY, EMPTY });
}

void npq15_read(const char *name) {
    VarInfo var = var_type(name);
    quadruples.push_back(Quadruple{ OP_READ, EMPTY, EMPTY, var.address });
}

static void gen_gotof() {
    Type type = stack_types.top();
    stack_types.pop();
    if (type != BOOL_T) {
        semantic_error(std::string("type mismatch: condition must be bool, got ") + type_name(type));
    }
    int result = stack_operands.top();
    stack_operands.pop();

    quadruples.push_back(Quadruple{ GOTOF, result, EMPTY, EMPTY });
    stack_jumps.push(next_quad() - 1);
}

void npj1_if_condition() {
    gen_gotof();
}

void npj2_if_end() {
    int end = stack_jumps.top();
    stack_jumps.pop();
    fill(end, next_quad());
}

void npj3_else() {
    quadruples.push_back(Quadruple{ GOTO, EMPTY, EMPTY, EMPTY });
    int false_jump = stack_jumps.top();
    stack_jumps.pop();
    stack_jumps.push(next_quad() - 1);
    fill(false_jump, next_quad());
}

void npj4_while_start() {
    stack_jumps.push(next_quad());
}

void npj5_while_condition() {
    gen_gotof();
}

void npj6_while_end() {
    int end = stack_jumps.top();
    stack_jumps.pop();
    int ret = stack_jumps.top();
    stack_jumps.pop();

    quadruples.push_back(Quadruple{ GOTO, EMPTY, EMPTY, ret });
    fill(end, next_quad());
}