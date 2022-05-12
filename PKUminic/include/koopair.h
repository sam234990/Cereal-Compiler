#pragma once
#ifndef KOOPAIR_H
#define KOOPAIR_H

#include "koopa.h"
#include <string>
#include <map>

void koopa_ir_from_str(std::string irstr, std::ostream& outfile);


// 重载Visit，遍历访问每一种Koopa IR结构
void Visit(const koopa_raw_program_t &program, std::ostream& outfile);
void Visit(const koopa_raw_slice_t &slice, std::ostream& outfile);
void Visit_func(const koopa_raw_function_t &func, std::ostream& outfile);
void Visit_bblcok(const koopa_raw_basic_block_t &bb, std::ostream& outfile);
void Visit_val(const koopa_raw_value_t &value, std::ostream& outfile, std::map<koopa_raw_value_t, int> &map_reg);
void Visit_ret(const koopa_raw_return_t &ret, std::ostream& outfile, std::map<koopa_raw_value_t, int> &map_reg);
void Visit_int(const koopa_raw_integer_t &integer, std::ostream &outfile);
void Visit_binary(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg);
void Visit_bin_eq(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg);
void Visit_bin_double_reg(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg);

#endif