#pragma once
#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include "semantic_cube/semantic_cube.h"

/*Variable table*/
struct VarInfo {
    Type type;
    int address;
};
using VarTable = std::unordered_map<std::string, VarInfo>;

/*Function dir*/
struct FuncInfo {
    Type returnType;
    std::vector<Type> params;
    std::unique_ptr<VarTable> vars;
    int num_params = 0;
    int start_quad = -1;
    int local_vars[2] = {};
    int temp_vars[3] = {};
    int return_address = -1;
    bool has_return = false;
    std::vector<int> param_addresses;
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

/*Function lookup*/
FuncInfo &find_function(const char *name);
FuncInfo &current_function();
const std::string &current_function_name();
bool in_main_scope();

/*Look for a variable (type and address)*/
VarInfo var_type(const char *name);

/*error logs*/
[[noreturn]] void semantic_error(const std::string &msg);
[[noreturn]] void internal_error(const std::string &msg);