#include <string>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <map>
#include <cassert>
#include "../include/genricsv.h"
#include "koopa.h"

using namespace std;

// 访问binary指令
void Visit_binary(const koopa_raw_value_t &value, ostream &outfile, symbol::kirinfo &kirinfo)
{
    // lv4中每个二元指令都能将结果存入，因此每次寄存器都是可以从0开始使用
    kirinfo.register_num = 0; // 刷新寄存器值

    // 根据指令类型判断后续需要如何访问
    const auto &binary = value->kind.data.binary;
    // 根据运算符类型判断后续如何翻译
    switch (binary.op)
    {
    case KOOPA_RBO_NOT_EQ:
        outfile << "  # or\n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_EQ:
        outfile << "  # eq\n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_OR:
        outfile << "\t #ne \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_GT:
        outfile << "\t #gt \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_LT:
        outfile << "\t #lt \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_GE:
        outfile << "\t #ge \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_LE:
        outfile << "\t #le \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_AND:
        outfile << "\t #and \n";
        Visit_bin_cond(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_ADD:
        outfile << "\t # add\n";
        Visit_bin_double_reg(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_SUB:
        outfile << "\t # sub\n";
        Visit_bin_double_reg(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_MUL:
        outfile << "\t # mul\n";
        Visit_bin_double_reg(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_DIV:
        outfile << "\t # div\n";
        Visit_bin_double_reg(value, outfile, kirinfo);
        break;
    case KOOPA_RBO_MOD:
        outfile << "\t # mod\n";
        Visit_bin_double_reg(value, outfile, kirinfo);
        break;
    default:
        // 其他类型暂时遇不到
        assert(false);
    }
    kirinfo.regmap.erase(kirinfo.regmap.begin(), kirinfo.regmap.end()); //一条运算指令处理完之后清空当前regmap
    return;
}

void Visit_bin_cond(const koopa_raw_value_t &value, ostream &outfile, symbol::kirinfo &kirinfo)
{ // 访问 cond 指令
    cout << " cond 指令" << endl;
    const auto &binary = value->kind.data.binary;

    string leftreg, rightreg, eqregister;           // 左右节点值
    kirinfo.regmap[value] = kirinfo.register_num++; // 为当前指令分配一个寄存器
    eqregister = get_reg_(value, kirinfo);          //当前指令的寄存器号

    handle_left_right_reg(value, outfile, leftreg, rightreg, eqregister, kirinfo);

    switch (binary.op) // 根据指令类型判断当前指令的运算符
    {
    case KOOPA_RBO_EQ:
        //使用xor和seqz指令完成 等值 判断
        outfile << "  xor\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        outfile << "  seqz\t" + eqregister + ", " + eqregister + '\n';
        break;
    case KOOPA_RBO_NOT_EQ:
        //使用xor和snez指令完成 不等 判断
        outfile << "  xor\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        outfile << "  snez\t" + eqregister + ", " + eqregister + '\n';
        break;
    case KOOPA_RBO_OR:
        //使用or和snez指令完成 或 判断
        outfile << "  or\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        outfile << "  snez\t" + eqregister + ", " + eqregister + '\n';
        break;
    case KOOPA_RBO_GT:
        //大于使用 slt 指令,交换两操作数位置,一条语句直接结束
        outfile << "  slt\t" + eqregister + ", " + rightreg + ", " + leftreg + '\n';
        break;
    case KOOPA_RBO_LT:
        //小于使用 slt 指令,一条语句直接结束
        outfile << "  slt\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        break;
    case KOOPA_RBO_GE:
        //>= 大于等于 先判断反命题: slt 判断 左 < 右, 再用异或 '1' 得到原命题
        outfile << "  slt\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        outfile << "  xori\t" + eqregister + ", " + eqregister + ", 1\n";
        break;
    case KOOPA_RBO_LE:
        //<= 小于等于,交换量操作数后, 与上面操作一致
        outfile << "  slt\t" + eqregister + ", " + rightreg + ", " + leftreg + '\n';
        outfile << "  xori\t" + eqregister + ", " + eqregister + ", 1\n";
        break;
    case KOOPA_RBO_AND:
        // and使用三条指令
        outfile << "  snez\t" + leftreg + ", " + leftreg + '\n';
        outfile << "  snez\t" + rightreg + ", " + rightreg + '\n';
        outfile << "  and\t" + eqregister + ", " + leftreg + ", " + rightreg + '\n';
        break;
    default: // 其他类型暂时遇不到
        assert(false);
    }
    // 2.将计算的结果存入
    string instr_stack = kirinfo.find_value_in_stack(outfile, value); 
    outfile << "  sw\t" + eqregister + ", " + instr_stack << endl;

    return;
}

void Visit_bin_double_reg(const koopa_raw_value_t &value, ostream &outfile, symbol::kirinfo &kirinfo)
{
    // cout << " 二者均为寄存器的指令" << endl;
    const auto &binary = value->kind.data.binary;
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
    string leftreg, rightreg;                       // 左右节点值
    kirinfo.regmap[value] = kirinfo.register_num++; // 为当前指令分配一个寄存器
    string eqregister = get_reg_(value, kirinfo);   // 定义当前指令的寄存器

    handle_left_right_reg(value, outfile, leftreg, rightreg, eqregister, kirinfo);

    outfile << "  " + bin_op + '\t' + eqregister + ", " + leftreg + ", " + rightreg + "\n";
    // 2.将计算的结果存入
    string instr_stack = kirinfo.find_value_in_stack(outfile, value);
    outfile << "  sw\t" + eqregister + ", " + instr_stack << endl;
}

void handle_left_right_reg(const koopa_raw_value_t &value, ostream &outfile, string &leftreg, string &rightreg, string &eqregister,
                           symbol::kirinfo &kirinfo)
{
    const auto &binary = value->kind.data.binary;
    if (binary.lhs->kind.tag == KOOPA_RVT_INTEGER)
    { //先处理左节点,左节点为int值
        if (binary.lhs->kind.data.integer.value == 0)
        { //左节点为0
            leftreg = "x0";
        }
        else
        { //左节点为非0 int, li到当前寄存器
            outfile << "  li\t" << eqregister << ", ";
            Visit_val(binary.lhs, outfile, kirinfo);
            outfile << endl;
            leftreg = eqregister;
        }
    }
    else
    {
        assert(binary.lhs->kind.tag != KOOPA_RVT_INTEGER); // assert左节点不是int值
        //用 lw  stack 中对应的值 到当前寄存器
        string lhs_stack = kirinfo.find_value_in_stack(outfile, binary.lhs);
        outfile << "  lw\t" + eqregister + ", " + lhs_stack << endl;

        leftreg = eqregister; // 获取左边的寄存器
    }
    if (binary.rhs->kind.tag == KOOPA_RVT_INTEGER)
    { //处理右节点,右节点为int值且为0
        if (binary.rhs->kind.data.integer.value == 0)
        { //右节点为0
            rightreg = "x0";
        }
        else
        {                                                   //右节点为非0 int, li到当前寄存器
            kirinfo.regmap[value] = kirinfo.register_num++; // 为当前指令再分配一个寄存器
            eqregister = get_reg_(value, kirinfo);          // 定义当前指令的寄存器
            outfile << "  li\t" << eqregister << ", ";
            Visit_val(binary.rhs, outfile, kirinfo);
            outfile << endl;
            rightreg = eqregister;
        }
    }
    else
    {
        assert(binary.rhs->kind.tag != KOOPA_RVT_INTEGER); // assert左右节点不是int值
        kirinfo.regmap[value] = kirinfo.register_num++; // 为当前指令再分配一个寄存器
        eqregister = get_reg_(value, kirinfo);          
        
        //用 lw  stack 中对应的值 到当前寄存器
        string rhs_stack = kirinfo.find_value_in_stack(outfile, binary.rhs);
        outfile << "  lw\t" + eqregister + ", " + rhs_stack << endl;
        
        rightreg = eqregister;                          // 获取右边的寄存器

    }
}

string get_reg_(const koopa_raw_value_t &value, symbol::kirinfo &kirinfo)
{

    if (kirinfo.regmap[value] > 6)
    {                                                      //当t0~t6用完时
        return "a" + to_string(kirinfo.regmap[value] - 7); // 用a0~a7
    }
    else
    {
        return "t" + to_string(kirinfo.regmap[value]); // 获得当前指令的寄存器
    }
}

