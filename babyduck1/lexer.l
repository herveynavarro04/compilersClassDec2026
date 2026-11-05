/* ============================================================
 * BabyDuck1 - Lexer
 * author: Hervey Navarro 
 * ============================================================ */
/* ===== SECTION 1: definitions ===== */
%option noyywrap
%option yylineno
%option nounput
%option noinput

%{
#include <cstdio>
#include <cstdlib>
#include <string>
#include "parser.tab.h"   
%}

LETTER      [A-Za-z]
DIGIT       [0-9]
ID          {LETTER}({LETTER}|{DIGIT})*
CTE_INT     {DIGIT}+
CTE_FLOAT   {DIGIT}+"."{DIGIT}+
CTE_STRING  \"[^\"\n]*\"
WHITESPACE  [ \t\r]+

%%
 /* ===== SECTION 2: rules (pattern { action }) ===== */

 /* ---------- Reserved words (must come BEFORE id) ---------- */
"program"   { return PROGRAM; }
"main"      { return MAIN; }
"end"       { return END; }
"var"       { return VAR; }
"void"      { return VOID; }
"int"       { return INT; }
"float"      { return FLOAT; }
"if"        { return IF; }
"else"      { return ELSE; }
"while"     { return WHILE; }
"do"        { return DO; }
"print"     { return PRINT; }
"read"      { return READ; }
"return"    { return RETURN; }

 /* ---------- Comparison operators (opc) ---------- */
"!="        { return NE; }
"<"         { return '<'; }
">"         { return '>'; }

 /* ---------- Arithmetic and assignment operators (op) ---------- */
"+"         { return '+'; }
"-"         { return '-'; }
"*"         { return '*'; }
"/"         { return '/'; }
"="         { return '='; }

 /* ---------- Delimiters (del) ---------- */
";"         { return ';'; }
":"         { return ':'; }
","         { return ','; }
"("         { return '('; }
")"         { return ')'; }
"{"         { return '{'; }
"}"         { return '}'; }
"["         { return '['; }
"]"         { return ']'; }

 /* ---------- Constants and identifiers ---------- */
{CTE_FLOAT}  { yylval.sval = strdup(yytext); return CTE_FLOAT; }  /* before int */
{CTE_INT}    { yylval.sval = strdup(yytext); return CTE_INT; }
{CTE_STRING} { yylval.sval = strdup(yytext); return CTE_STRING; }
{ID}         { yylval.sval = strdup(yytext); return ID; }

 /* ---------- Ignored ---------- */
{WHITESPACE} { }
\n           { /* yylineno is incremented automatically */ }

 /* ---------- Anything else = lexical error ---------- */
.           {
              std::fprintf(stderr, "Lexical error (line %d): unrecognized character '%s'\n",
                           yylineno, yytext);
              std::exit(2);
            }
%%