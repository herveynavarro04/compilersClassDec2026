#include <cstdio>
#include <cstring>
#include "quadruples.h"
#include "memory/memory.h"

/* ---------- Symbols defined by Flex and Bison ---------- */
extern FILE *yyin;       // input file the scanner reads from (Flex)
int yyparse();           // parser (Bison); calls yylex() internally

int main(int argc, char **argv) {
    bool show_quads = false;
    const char *file = nullptr;
    const char *obj_file = "program.obj";

    /* Read arguments: --quads, -o <output> and the input file (any order) */
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--quads") == 0) show_quads = true;
        else if (std::strcmp(argv[i], "-o") == 0 && i + 1 < argc) obj_file = argv[++i];
        else file = argv[i];
    }

    if (file) {
        yyin = std::fopen(file, "r");
        if (!yyin) { std::perror(file); return 3; }
    }

    if (yyparse() != 0) return 1;

    std::printf("ACCEPTED: the program is valid\n");
    if (show_quads) {
        print_quadruples();
        std::printf("\n");
        print_functions();
        std::printf("\n");
        print_constants();
    }

    FILE *out = std::fopen(obj_file, "w");
    if (!out) { std::perror(obj_file); return 5; }
    write_constants(out);
    write_obj(out);
    std::fclose(out);
    return 0;
}