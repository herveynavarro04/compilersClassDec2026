#include "memory/memory.h"
#include "semantics.h"
#include <cstdio>
#include <cstdlib>

extern int yylineno;

static std::unique_ptr<FuncDirectory> dir;
static std::string program_name;
static std::string current_func;
static Type current_type = ERROR_T;
static std::vector<std::string> pending_ids;

void semantic_error(const std::string &msg) {
    std::fprintf(stderr, "Semantic error (line %d): %s\n", yylineno, msg.c_str());
    std::exit(4);
}

void internal_error(const std::string &msg) {
    std::fprintf(stderr, "Internal compiler error (line %d): %s\n", yylineno, msg.c_str());
    std::exit(5);
}

static void add_to_current_vartable(const std::string &name) {
    VarTable &table = *(*dir)[current_func].vars;
    if (table.count(name)){
        semantic_error("multiple declaration of '" + name + "' in '" + current_func + "'");
    }
    Segment seg = (current_func == program_name) ? GLOBAL : LOCAL;
    table[name] = {current_type, alloc_address(seg, current_type)};
}

VarInfo var_type(const char *name) {
    FuncInfo &f = (*dir)[current_func];
    if (f.vars) {
        auto curr_var = f.vars->find(name);
        if (curr_var != f.vars->end()) {
            return curr_var->second;
        }
    }
    FuncInfo &global = (*dir)[program_name];
    if (global.vars) {
        auto curr_var = global.vars->find(name);
        if (curr_var != global.vars->end()) {
            return curr_var->second;
        }
    }
    semantic_error("variable '" + std::string(name) + "' not declared");
}

void np1_create_dirfunc() {
    dir = std::make_unique<FuncDirectory>();
}

void np2_add_program(const char *name) {
    program_name = name;
    current_func = name;
    (*dir)[name].returnType = VOID_T;
}

void np3_ensure_vartable() {
    FuncInfo &f = (*dir)[current_func];
    if (!f.vars) {
        f.vars = std::make_unique<VarTable>();
    }
}

void np4_set_current_type(int type) {
    current_type = static_cast<Type>(type);
}

void np5_queue_id(const char *name) {
    pending_ids.push_back(name);
}

void np5_add_vars() {
    for (const std::string &id : pending_ids) {
        add_to_current_vartable(id);
    }
    pending_ids.clear();
}

void np6_delete_dirfunc() {
    dir.reset();
}

void np7_prepare_function() {
    current_type = ERROR_T;
}

void np8_set_void() {
    current_type = VOID_T;
}

void np9_add_function(const char *name) {
    if (dir->count(name)) {
        semantic_error("multiple declaration of function '" + std::string(name) + "'");
    }
    FuncInfo &global = (*dir)[program_name];
    if (global.vars && global.vars->count(name)) {
        semantic_error("'" + std::string(name) + "' is already a global variable");
    }
    FuncInfo &f = (*dir)[name];
    f.returnType = current_type;
    if (current_type != VOID_T) {
        f.return_address = alloc_address(GLOBAL, current_type);
    }
    current_func = name;
}

void np10_create_function_vartable() {
    reset_local_memory();
    (*dir)[current_func].vars = std::make_unique<VarTable>();
}

void np11_add_param(const char *name) {
    add_to_current_vartable(name);
    (*dir)[current_func].params.push_back(current_type);
}

void np12_delete_function_vartable() {
    FuncInfo &f = (*dir)[current_func];
    f.vars.reset();
    reset_local_memory();
    current_func = program_name;
}

FuncInfo &find_function(const char *name) {
    auto f = dir->find(name);
    if (f == dir->end() || name == program_name) {
        semantic_error("function '" + std::string(name) + "' not declared");
    }
    return f->second;
}

FuncInfo &current_function() {
    return (*dir)[current_func];
}

const std::string &current_function_name() {
    return current_func;
}

bool in_main_scope() {
    return current_func == program_name;
}