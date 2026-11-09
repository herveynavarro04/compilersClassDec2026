#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
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
    std::unique_ptr<VarTable> vars;
};
using FuncDirectory = std::unordered_map<std::string, FuncInfo>;

/*Neuralgic point actions*/
void np1_create_dirfunc();                     
void np2_add_program(const char *name);        
void np3_ensure_vartable();                    
void np4_set_current_type(int type);           
void np5_queue_id(const char *name);           
void np5_add_vars();                           
void np6_delete_dirfunc();                     
void np7_prepare_function();                   
void np8_set_void();                           
void np9_add_function(const char *name);       
void np10_create_function_vartable();          
void np11_add_param(const char *name);         
void np12_delete_function_vartable();
void np13_check_var_declared(const char *name);