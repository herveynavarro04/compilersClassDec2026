/* ============================================================
 * BabyDuck1 - Parser 
 * author: Hervey Navarro 
 * ============================================================ */

/* ===== SECTION 1: declarations ===== */
%{
#include <cstdio>
#include <cstdlib>
#include "semantics.h"
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
      ';' dec_vars dec_funcs MAIN body
      END                       { np6_delete_dirfunc(); }            /* 6 */
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
      params ')' '[' dec_vars body ']' ';'
                                { np12_delete_function_vartable(); } /* 12 */
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
    : ID
      '='                       { np13_check_var_declared($1); }     /* 13 */
      expression ';'
    ;

/* <CONDITION> -> if ( <EXPRESSION> ) <Body> <ElsePart> ; */
condition
    : IF '(' expression ')' body else_part ';'
    ;

/* <ElsePart> -> else <Body> | ε */
else_part
    : ELSE body
    | %empty
    ;

/* <CYCLE> -> while ( <EXPRESSION> ) do <Body> ; */
cycle
    : WHILE '(' expression ')' DO body ';'
    ;

/* <Print> -> print ( <PrintList> ) ; */
print
    : PRINT '(' print_list ')' ';'
    ;

/* <PrintList> -> <PrintItem> <MorePrint> */
print_list
    : print_list ',' print_item
    | print_item
    ;

/* <PrintItem> -> <EXPRESSION> | cte_string */
print_item
    : expression
    | CTE_STRING
    ;

/* <Read> -> read ( id ) ; */
read
    : READ '(' ID ')' ';'       { np13_check_var_declared($3); }     /* 13 */
    ;

/* <Return> -> return <EXPRESSION> ; */
return_stmt
    : RETURN expression ';'
    ;

/* ---------- Expressions (one level per precedence) ---------- */

/* <EXPRESSION> -> <EXP> | <EXP> <RelOp> <EXP> */
expression
    : exp
    | exp '<' exp
    | exp '>' exp
    | exp NE  exp
    ;

/* <EXP> -> <TERM> <MoreTerms>      (+, -) */
exp
    : exp '+' term
    | exp '-' term
    | term
    ;

/* <TERM> -> <FACTOR> <MoreFactors> (*, /) */
term
    : term '*' factor
    | term '/' factor
    | factor
    ;

/* <FACTOR> -> ( <EXPRESSION> ) | <Sign> <FactorValue> | <F_Call> */
factor
    : '(' expression ')'
    | '+' factor_value
    | '-' factor_value
    | factor_value
    | f_call
    ;

/* <FactorValue> -> id | <CTE> */
factor_value
    : ID                        { np13_check_var_declared($1); }     /* 13 */
    | cte
    ;

/* <CTE> -> cte_int | cte_float */
cte
    : CTE_INT
    | CTE_FLOAT
    ;

/* ---------- Function calls ---------- */

/* <F_Call> -> id ( <Args> ) */
f_call
    : ID                       
      '(' args ')'
    ;

/* <Args> -> <ArgList> | ε */
args
    : arg_list
    | %empty
    ;

/* <ArgList> -> <EXPRESSION> <MoreArgs> */
arg_list
    : arg_list ',' expression
    | expression
    ;

%%

/* ===== SECTION 3: C++ code ===== */

void yyerror(const char *s) {
    std::fprintf(stderr, "Syntax error (line %d): %s\n", yylineno, s);
}