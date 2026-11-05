/* ============================================================
 * BabyDuck1 - Parser 
 * author: Hervey Navarro 
 * ============================================================ */

/* ===== SECTION 1: declarations ===== */
%{
#include <cstdio>
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
    : PROGRAM ID ';' dec_vars dec_funcs MAIN body END
    ;

/* ---------- Variable declarations ---------- */

/* <DecVars> -> <VARS> | ε */
dec_vars
    : vars
    | %empty
    ;

/* <VARS> -> var <VarList> */
vars
    : VAR var_list
    ;

/* <VarList> -> <VarDecl> <VarList> | <VarDecl> */
var_list
    : var_list var_decl
    | var_decl
    ;

/* <VarDecl> -> <IdList> : <TYPE> ; */
var_decl
    : id_list ':' type ';'
    ;

/* <IdList> -> id <MoreIds> */
id_list
    : id_list ',' ID
    | ID
    ;

/* <TYPE> -> int | float */
type
    : INT
    | FLOAT
    ;

/* ---------- Function declarations ---------- */

/* <DecFuncs> -> <FUNCS> <DecFuncs> | ε */
dec_funcs
    : dec_funcs func
    | %empty
    ;

/* <FUNCS> -> <FuncType> id ( <Params> ) [ <DecVars> <Body> ] ; */
func
    : func_type ID '(' params ')' '[' dec_vars body ']' ';'
    ;

/* <FuncType> -> void | <TYPE> */
func_type
    : VOID
    | type
    ;

/* <Params> -> <ParamList> | ε */
params
    : param_list
    | %empty
    ;

/* <ParamList> -> id : <TYPE> <MoreParams> */
param_list
    : param_list ',' ID ':' type
    | ID ':' type
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
    : ID '=' expression ';'
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
    : READ '(' ID ')' ';'
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
    : ID
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
    : ID '(' args ')'
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