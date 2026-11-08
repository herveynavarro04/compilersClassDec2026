#include "semantics.h"
#include <cstdio>
#include <cstdlib>

extern int yylineno;

static FuncDirectory dir;
static std::string program_name;
static std::string current_func;
static std::vector<std::string> pending_ids;

// Error print 
static void semantic_error(const std::string &msg) {
    std::fprintf(stderr, "Semantic error (line %d): %s\n", yylineno, msg.c_str());
    std::exit(4);
};

// Inserts a variable into the CURRENT scope
static void add_variables(const std::string &name, Type type) {
    VarTable &table = dir[current_func].vars;
    if (table.count(name)) {
        semantic_error("variable '" + name + "' already declared in '" + current_func + "'");
    };
    table[name] = {type};
};

// NP1: create the directory and the global scope 
void np_start_program(const char *name) {
    program_name = name;
    current_func = name;
    dir[name] = {VOID_T, {}, {}};
};

// NP2: appends pending ids into the stack
void np_add_pending_id(const char *name) {
    pending_ids.push_back(name);
};

// NP3: insert every pending id with that type
void np_declare_pending_vars(int type) {
    for (const std::string &id: pending_ids) {
        add_variables(id, static_cast<Type>(type));  
    }
    pending_ids.clear();
};

void np_start_function(const char *name, int type) {
    if (dir.count(name))
        semantic_error("function '" + std::string(name) + "' already declared");
    if (dir[program_name].vars.count(name))
        semantic_error("'" + std::string(name) + "' is already a global variable");
    dir[name] = {static_cast<Type>(type), {}, {}};
    current_func = name;
};

