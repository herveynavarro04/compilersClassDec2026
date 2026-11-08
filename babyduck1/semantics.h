#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "semantic_cube/semantic_cube.h"

/*Variable table*/
struct VarInfo {
    Type type;
};
using VarTable = std::unordered_map<std::string, VarInfo>;

/*Function dir*/
struct FuncInfo {
    Type returnType;
    std::vector<Type> params;
    VarTable vars;
};
using FuncDirectory = std::unordered_map<std::string, FuncInfo>;

/*Neuralgic point actions*/
void np_start_program(const char *name);              
void np_add_pending_id(const char *name);             
void np_declare_pending_vars(int type);               
void np_start_function(const char *name, int type);   
void np_add_param(const char *name, int type);        
void np_end_function();                               
void np_start_main();                                 
void np_check_var_exists(const char *name);           
void np_check_func_exists(const char *name);          

/*Debug*/
void print_dir();