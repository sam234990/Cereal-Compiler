#pragma once
#ifndef KOOPAIR_H
#define KOOPAIR_H

#include "koopa.h"
#include<string>

void koopa_ir_from_str(std::string irstr, std::ostream& outfile);


// 重载Visit，遍历访问每一种Koopa IR结构
void Visit(const koopa_raw_program_t &program, std::ostream& outfile);
void Visit(const koopa_raw_slice_t &slice, std::ostream& outfile);
void Visit(const koopa_raw_function_t &func, std::ostream& outfile);
void Visit(const koopa_raw_basic_block_t &bb, std::ostream& outfile);
void Visit(const koopa_raw_value_t &value, std::ostream& outfile);
void Visit(const koopa_raw_return_t &ret, std::ostream& outfile);
void Visit(const koopa_raw_integer_t &integer, std::ostream& outfile);

#endif