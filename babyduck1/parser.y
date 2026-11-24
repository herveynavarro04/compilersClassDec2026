/* ============================================================
 * BabyDuck1 - Parser 
 * author: Hervey Navarro 
 * ============================================================ */

/* ===== SECTION 1: declarations ===== */
%{
#include <cstdio>
#include <cstdlib>
#include "semantics.h"
#include "quadruples.h"
int  yylex();                 
void yyerror(const char *s);  
extern int yylineno;          
%}

/* --- Detailed error messages ("unexpected X, expecting Y") --- */
%define parse.error verbose

/* --- Type of the semantic value (yylval) --- */
%union {
    char *sval;
}

/* --- Tokens (terminals) --- */
%token PROGRAM MAIN END VAR VOID INT FLOAT
%token IF ELSE WHILE DO PRINT READ RETURN
%token NE
%token <sval> ID CTE_INT CTE_FLOAT CTE_STRING

/* --- Start symbol --- */
%start program

%%

/* ===== SECTION 2: grammar rules ===== */

/* ---------- Program structure ---------- */

/* <Program> -> program id ; <DecVars> <DecFuncs> main <Body> end */
program
    : PROGRAM                   { np1_create_dirfunc(); }            /* 1 */
      ID                        { np2_add_program($3); }             /* 2 */
      ';'                       { npm1_goto_main(); }                /* M1 */
      dec_vars dec_funcs
      MAIN                      { npm2_fill_main(); }                /* M2 */
      body
      END                       { npm3_end_program(); np6_delete_dirfunc(); }            /* npm 3 & np6 */
    ;

/* ---------- Variable declarations ---------- */

/* <DecVars> -> <VARS> | ε */
dec_vars
    : vars
    | %empty
    ;

/* <VARS> -> var <VarList> */
vars
    : VAR                       { np3_ensure_vartable(); }           /* 3 */
      var_list
    ;

/* <VarList> -> <VarDecl> <VarList> | <VarDecl> */
var_list
    : var_list var_decl
    | var_decl
    ;

/* <VarDecl> -> <IdList> : <TYPE> ; */
var_decl
    : id_list ':' type ';'      { np5_add_vars(); }                  /* 5 */
    ;

/* <IdList> -> id <MoreIds> */
id_list
    : id_list ',' ID            { np5_queue_id($3); }                /* 5 */
    | ID                        { np5_queue_id($1); }                /* 5 */
    ;

/* <TYPE> -> int | float */
type
    : INT                       { np4_set_current_type(INT_T); }     /* 4 */
    | FLOAT                     { np4_set_current_type(FLOAT_T); }   /* 4 */
    ;

/* ---------- Function declarations ---------- */

/* <DecFuncs> -> <FUNCS> <DecFuncs> | ε */
dec_funcs
    : dec_funcs func
    | %empty
    ;

/* <FUNCS> -> <FuncType> id ( <Params> ) [ <DecVars> <Body> ] ; */
func
    :                           { np7_prepare_function(); }          /* 7 */
      func_type
      ID                        { np9_add_function($3); }            /* 9 */
      '('                       { np10_create_function_vartable(); } /* 10 */
      params ')'                { npf4_count_params(); }             /* F4 */
      '[' dec_vars              { npf5_count_locals();               /* F5 */
                                  npf6_set_start(); }                /* F6 */
      body ']' ';'              { npf7_end_function();               /* F7 */
                                  np12_delete_function_vartable(); } /* 12 */
    ;

/* <FuncType> -> void | <TYPE> */
func_type
    : VOID                      { np8_set_void(); }                  /* 8 */
    | type
    ;

/* <Params> -> <ParamList> | ε */
params
    : param_list
    | %empty
    ;

/* <ParamList> -> id : <TYPE> <MoreParams> */
param_list
    : param_list ',' ID ':' type { np11_add_param($3); }            /* 11 */
    | ID ':' type               { np11_add_param($1); }              /* 11 */
    ;

/* ---------- Body and statements ---------- */

/* <Body> -> { <Stmts> } */
body
    : '{' stmts '}'
    ;

/* <Stmts> -> <Stmts> <STATEMENT> | ε */
stmts
    : stmts statement
    | %empty
    ;

/* <STATEMENT> -> <ASSIGN> | <CONDITION> | <CYCLE> | <F_Call> ; | <Print>
 *              | <Read> | <Return> | [ <Stmts> ]                        */
statement
    : assign
    | condition
    | cycle
    | f_call ';'
    | print
    | read
    | return_stmt
    | '[' stmts ']'
    ;

/* <ASSIGN> -> id = <EXPRESSION> ; */
assign
    : ID '=' expression ';'     { npq12_assign($1); }                /* Q12 */
    ;

/* <CONDITION> -> if ( <EXPRESSION> ) <Body> <ElsePart> ; */
condition
    : IF '(' expression ')'     { npj1_if_condition(); }             /* J1 */
      body else_part ';'        { npj2_if_end(); }                   /* J2 */
    ;

/* <ElsePart> -> else <Body> | ε */
else_part
    : ELSE                      { npj3_else(); }                     /* J3 */
      body
    | %empty
    ;

/* <CYCLE> -> while ( <EXPRESSION> ) do <Body> ; */
cycle
    : WHILE                     { npj4_while_start(); }              /* J4 */
      '(' expression ')'        { npj5_while_condition(); }          /* J5 */
      DO body ';'               { npj6_while_end(); }                /* J6 */
    ;

/* <Print> -> print ( <PrintList> ) ; */
print
    : PRINT '(' print_list ')' ';' { npq1_print_end(); }             /* Q16 */
    ;

/* <PrintList> -> <PrintItem> <MorePrint> */
print_list
    : print_list ',' print_item
    | print_item
    ;

/* <PrintItem> -> <EXPRESSION> | cte_string */
print_item
    : expression                { npq13_print_expression(); }        /* Q13 */
    | CTE_STRING                { npq14_print_string($1); }          /* Q14 */
    ;

/* <Read> -> read ( id ) ; */
read
    : READ '(' ID ')' ';'       { npq15_read($3); }                  /* Q15 */
    ;

/* <Return> -> return <EXPRESSION> ; */
return_stmt
    : RETURN expression ';'     { npr1_return(); }                   /* R1 */
    ;

/* ---------- Expressions (one level per precedence) ---------- */

/* <EXPRESSION> -> <EXP> | <EXP> <RelOp> <EXP> */
expression
    : exp
    | exp '<'                   { npq9_push_rel_op(LT); }            /* Q9  */
      exp                       { npq10_gen_rel(); }                 /* Q10 */
    | exp '>'                   { npq9_push_rel_op(GT); }            /* Q9  */
      exp                       { npq10_gen_rel(); }                 /* Q10 */
    | exp NE                    { npq9_push_rel_op(NE_OP); }         /* Q9  */
      exp                       { npq10_gen_rel(); }                 /* Q10 */
    ;

/* <EXP> -> <TERM> <MoreTerms>      (+, -) */
exp
    : exp '+'                   { npq2_push_add_sub(ADD); }          /* Q2 */
      term                      { npq4_gen_add_sub(); }              /* Q4 */
    | exp '-'                   { npq2_push_add_sub(SUB); }          /* Q2 */
      term                      { npq4_gen_add_sub(); }              /* Q4 */
    | term                      { npq4_gen_add_sub(); }              /* Q4 */
    ;

/* <TERM> -> <FACTOR> <MoreFactors> (*, /) */
term
    : term '*'                  { npq3_push_mul_div(MUL); }          /* Q3 */
      factor                    { npq5_gen_mul_div(); }              /* Q5 */
    | term '/'                  { npq3_push_mul_div(DIV); }          /* Q3 */
      factor                    { npq5_gen_mul_div(); }              /* Q5 */
    | factor                    { npq5_gen_mul_div(); }              /* Q5 */
    ;

/* <FACTOR> -> ( <EXPRESSION> ) | <Sign> <FactorValue> | <F_Call> */
factor
    : '('                       { npq7_push_fake_bottom(); }         /* Q7 */
      expression
      ')'                       { npq8_pop_fake_bottom(); }          /* Q8 */
    | '+' factor_value
    | '-' factor_value          { npq11_unary_minus(); }             /* Q11 */
    | factor_value
    | f_call                    { npc7_return_value(); }             /* C7 */
    ;

/* <FactorValue> -> id | <CTE> */
factor_value
    : ID                        { npq1_push_operand($1); }           /* Q1 */
    | cte
    ;

/* <CTE> -> cte_int | cte_float */
cte
    : CTE_INT                   { npq6_push_constant($1, INT_T); }   /* Q6 */
    | CTE_FLOAT                 { npq6_push_constant($1, FLOAT_T); } /* Q6 */
    ;

/* ---------- Function calls ---------- */

/* <F_Call> -> id ( <Args> ) */
f_call
    : ID                        { npc1_verify_function($1); }        /* C1 */
      '('                       { npc2_era(); }                      /* C2 */
      args
      ')'                       { npc5_check_args();                 /* C5 */
                                  npc6_gosub(); }                    /* C6 */
    ;

/* <Args> -> <ArgList> | ε */
args
    : arg_list
    | %empty
    ;

/* <ArgList> -> <EXPRESSION> <MoreArgs> */
arg_list
    : arg_list ','              { npc4_next_param(); }               /* C4 */
      expression                { npc3_param(); }                    /* C3 */
    | expression                { npc3_param(); }                    /* C3 */
    ;

%%

/* ===== SECTION 3: C++ code ===== */

void yyerror(const char *s) {
    std::fprintf(stderr, "Syntax error (line %d): %s\n", yylineno, s);
}