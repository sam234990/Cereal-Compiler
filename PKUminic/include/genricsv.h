#pragma once
#ifndef KOOPAIR_H
#define KOOPAIR_H

#include "koopa.h"
#include "../include/symbol.h"
#include <string>
#include <map>

void koopa_ir_from_str(std::string irstr, std::ostream &outfile, symbol::kirinfo &kirinfo);

// 重载Visit，遍历访问每一种Koopa IR结构
void Visit(const koopa_raw_program_t &program, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit(const koopa_raw_slice_t &slice, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_func(const koopa_raw_function_t &func, std::ostream &outfile, symbol::kirinfo &kirinfo);

void func_prologue(const koopa_raw_slice_t &slice, std::ostream &outfile, symbol::kirinfo &kirinfo);
void func_params_reg(const koopa_raw_slice_t &params, std::ostream &outfile, symbol::kirinfo &kirinfo);


void Visit_bblcok(const koopa_raw_basic_block_t &bb, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_val(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);

// 各种指令
void Visit_ret(const koopa_raw_return_t &ret, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_int(const koopa_raw_integer_t &integer, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_alloc(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_load(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_store(const koopa_raw_store_t &store, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_jump(const koopa_raw_jump_t &jump, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_branch(const koopa_raw_branch_t &branch, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_call(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_global_alloc(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void visit_aggregate(const koopa_raw_value_t &aggregate, std::ostream &outfile, symbol::kirinfo &kirinfo);
void visit_getelemptr(const koopa_raw_value_t &getelemptr, std::ostream &outfile, symbol::kirinfo &kirinfo);
void visit_getptr(const koopa_raw_value_t &getptr, std::ostream &outfile, symbol::kirinfo &kirinfo);



//二元运算
void Visit_binary(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_bin_cond(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);
void Visit_bin_double_reg(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo);

// 特殊方法
void handle_left_right_reg(const koopa_raw_value_t &value, std::ostream &outfile, std::string &leftreg, std::string &rightreg,
                           std::string &eqregister, symbol::kirinfo &kirinfo);
std::string get_reg_(const koopa_raw_value_t &value, symbol::kirinfo &kirinfo);

#endif