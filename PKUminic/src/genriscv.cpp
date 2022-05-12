#include <string>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <map>
#include <cassert>
#include "../include/koopair.h"
#include "koopa.h"

using namespace std;

// 访问binary指令
void Visit_binary(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg)
{
    // 根据指令类型判断后续需要如何访问
    const auto &kind = value->kind;
    const auto binary = kind.data.binary;
    // 根据运算符类型判断后续如何翻译
    switch (binary.op)
    {
    case KOOPA_RBO_EQ:
        // outfile << "  # eq\n";
        Visit_bin_eq(value, outfile, register_num, map_reg);
        break;
    case KOOPA_RBO_ADD:
        // outfile << "  # add\n";
        Visit_bin_double_reg(value, outfile, register_num, map_reg);
        break;
    case KOOPA_RBO_SUB:
        // outfile << "  # sub\n";
        Visit_bin_double_reg(value, outfile, register_num, map_reg);
        break;
    case KOOPA_RBO_MUL:
        // outfile << "  # mul\n";
        Visit_bin_double_reg(value, outfile, register_num, map_reg);
        break;
    case KOOPA_RBO_DIV:
        // outfile << "  # div\n";
        Visit_bin_double_reg(value, outfile, register_num, map_reg);
        break;
    case KOOPA_RBO_MOD:
        // outfile << "  # mod\n";
        Visit_bin_double_reg(value, outfile, register_num, map_reg);
        break;
    default:
        // 其他类型暂时遇不到
        assert(false);
    }
    return;
}

void Visit_bin_eq(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg)
{ // 访问 eq 指令
    cout << " eq 指令" << endl;
    const auto &kind = value->kind;
    const auto binary = kind.data.binary;
    string leftreg, rightreg;                            // 左右节点值
    map_reg[value] = register_num++;                     // 为当前指令分配一个寄存器
    string eqregister = "t" + to_string(map_reg[value]); // 定义当前指令的寄存器
    if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER)
    { //先处理左节点,左节点为int值
        if (binary.lhs->kind.data.integer.value != 0)
        { //左节点为非0结点, 用li指令到当前寄存器
            outfile << "  li\t" << eqregister << ", ";
            Visit_val(binary.lhs, outfile, map_reg);
            outfile << endl;
            leftreg = eqregister;
        }
        else
        { //左节点为0值
            leftreg = "x0";
        }
    }
    else
    {
        leftreg = "t" + to_string(map_reg[binary.lhs]); //获取左边的寄存器
    }
    if (binary.rhs->kind.tag == KOOPA_RVT_INTEGER)
    { //处理右节点,右节点为int值
        if (binary.rhs->kind.data.integer.value != 0)
        { //右节点为非0结点, 用xori立即数指令
            outfile << "  xori\t" + eqregister + ", " + leftreg + ", ";
            Visit_val(binary.rhs, outfile, map_reg);
            outfile << endl;
        }
        else
        { //右节点为0值, 用x0
            outfile << "  xor\t" + eqregister + ", " + leftreg + ", x0" + '\n';
        }
    }
    else
    {
        rightreg = "t" + to_string(map_reg[binary.lhs]); //获取右边的寄存器
        outfile << "  xor\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
    }
    outfile << "  seqz\t" + eqregister + ", " + eqregister + '\n';
    return;
}

void Visit_bin_double_reg(const koopa_raw_value_t &value, std::ostream &outfile, int &register_num, std::map<koopa_raw_value_t, int> &map_reg)
{
    cout << " 二者均为寄存器的指令" << endl;
    const auto &kind = value->kind;
    const auto binary = kind.data.binary;
    string bin_op; //当前指令的运算符
    switch (binary.op)
    { // 根据指令类型判断当前指令的运算符
    case KOOPA_RBO_ADD:
        bin_op = "add";
        break;
    case KOOPA_RBO_SUB:
        bin_op = "sub";
        break;
    case KOOPA_RBO_MUL:
        bin_op = "mul";
        break;
    case KOOPA_RBO_DIV:
        bin_op = "div";
        break;
    case KOOPA_RBO_MOD:
        bin_op = "rem";
        break;
    default: // 其他类型暂时遇不到
        assert(false);
    }
    string leftreg, rightreg;                            // 左右节点值
    map_reg[value] = register_num++;                     // 为当前指令分配一个寄存器
    string eqregister = "t" + to_string(map_reg[value]); // 定义当前指令的寄存器

    if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER)
    { //先处理左节点,左节点为int值
        if (binary.lhs->kind.data.integer.value == 0)
        { //左节点为0
            leftreg = "x0";
        }
        else
        { //左节点为非0 int, li到当前寄存器
            outfile << "  li\t" << eqregister << ", ";
            Visit_val(binary.lhs, outfile, map_reg);
            outfile << endl;
            leftreg = eqregister;
        }
    }
    else
    {
        assert(binary.lhs->kind.tag != KOOPA_RVT_INTEGER); // assert左右节点不是int值
        leftreg = "t" + to_string(map_reg[binary.lhs]);    //获取左边的寄存器
    }

    if (binary.rhs->kind.tag == KOOPA_RVT_INTEGER)
    { //处理右节点,右节点为int值且为0
        if (binary.rhs->kind.data.integer.value == 0)
        { //右节点为0
            rightreg = "x0";
        }
        else
        { //右节点为非0 int, li到当前寄存器
            map_reg[value] = register_num++;              // 为当前指令再分配一个寄存器
            eqregister = "t" + to_string(map_reg[value]); // 定义当前指令的寄存器
            outfile << "  li\t" << eqregister << ", ";
            Visit_val(binary.rhs, outfile, map_reg);
            outfile << endl;
            rightreg = eqregister;
        }
    }
    else
    {
        assert(binary.rhs->kind.tag != KOOPA_RVT_INTEGER); // assert左右节点不是int值
        rightreg = "t" + to_string(map_reg[binary.rhs]);   //获取右边的寄存器
    }
    outfile << "  " + bin_op + '\t' + eqregister + ", " + leftreg + ", " + rightreg + "\n";
}
