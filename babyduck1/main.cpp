#include <cstdio>

/* ---------- Symbols defined by Flex and Bison ---------- */
extern FILE *yyin;       // input file the scanner reads from (Flex)
int yyparse();           // parser (Bison); calls yylex() internally

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = std::fopen(argv[1], "r");
        if (!yyin) { std::perror(argv[1]); return 3; }
    }
    if (yyparse() == 0) {              
        std::printf("ACCEPTED: the program is valid\n");
        return 0;
    }
    return 1;                           
}