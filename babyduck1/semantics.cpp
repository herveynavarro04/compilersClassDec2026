#include "semantics.h"
#include <cstdio>
#include <cstdlib>

extern int yylineno;

static std::unique_ptr<FuncDirectory> dir;
static std::string program_name;
static std::string current_func;
static Type current_type = ERROR_T;
static std::vector<std::string> pending_ids;

static void semantic_error(const std::string &msg) {
    std::fprintf(stderr, "Semantic error (line %d): %s\n", yylineno, msg.c_str());
    std::exit(4);
};

static void add_to_current_vartable(const std::string &name) {
    VarTable &table = *(*dir)[current_func].vars;
    if (table.count(name)){
        semantic_error("multiple declaration of '" + name + "' in '" + current_func + "'");
    }
    table[name] = {current_type};
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
};

void np4_set_current_type(int type) {
    current_type = static_cast<Type>(type);
};

void np5_queue_id(const char *name) {
    pending_ids.push_back(name);
};

void np5_add_vars() {
    for (const std::string &id : pending_ids) {
        add_to_current_vartable(id);
    }
    pending_ids.clear();
};

void np6_delete_dirfunc() {
    dir.reset();
};

void np7_prepare_function() {
    current_type = ERROR_T;
};

void np8_set_void() {
    current_type = VOID_T;
};

void np9_add_function(const char *name) {
    if (dir->count(name)) {
        semantic_error("multiple declaration of function '" + std::string(name) + "'");
    }
    FuncInfo &global = (*dir)[program_name];
    if (global.vars && global.vars->count(name)) {
        semantic_error("'" + std::string(name) + "' is already a global variable");
    }
    (*dir)[name].returnType = current_type;
    current_func = name;
};

void np10_create_function_vartable() {
    (*dir)[current_func].vars = std::make_unique<VarTable>();
};

void np11_add_param(const char *name) {
    add_to_current_vartable(name);
    (*dir)[current_func].params.push_back(current_type);
};

void np12_delete_function_vartable() {
    FuncInfo &f = (*dir)[current_func];
    f.vars.reset();
    current_func = program_name;
};

void np13_check_var_declared(const char *name) {
    const VarTable *local = (*dir)[current_func].vars.get();
    const VarTable *global = (*dir)[program_name].vars.get();
    if ((local && local->count(name)) || (global && global->count(name))) {
        return;
    }
    semantic_error("undeclared variable '" + std::string(name) + "'");
};