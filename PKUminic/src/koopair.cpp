#include <string>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <map>
#include "../include/koopair.h"
#include "koopa.h"

using namespace std;

int register_num = 0;
map<koopa_raw_value_t, int> regmap;

void koopa_ir_from_str(string irstr, std::ostream &outfile)
{
    const char *str = irstr.c_str();

    // 解析字符串 str, 得到 Koopa IR 程序
    koopa_program_t program;
    koopa_error_code_t ret = koopa_parse_from_string(str, &program);
    assert(ret == KOOPA_EC_SUCCESS); // 确保解析时没有出错
    // 创建一个 raw program builder, 用来构建 raw program
    koopa_raw_program_builder_t builder = koopa_new_raw_program_builder();
    // 将 Koopa IR 程序转换为 raw program
    koopa_raw_program_t raw = koopa_build_raw_program(builder, program);
    // 将 Koopa IR 程序输出到stdout中
    koopa_dump_to_stdout(program);

    // 释放 Koopa IR 程序占用的内存
    koopa_delete_program(program);

    // 处理 raw program
    // 遍历raw
    Visit(raw, outfile);

    // 处理完成, 释放 raw program builder 占用的内存
    // 注意, raw program 中所有的指针指向的内存均为 raw program builder 的内存
    // 所以不要在 raw program 处理完毕之前释放 builder
    koopa_delete_raw_program_builder(builder);
}

// 访问 raw program
void Visit(const koopa_raw_program_t &program, std::ostream &outfile)
{
    // 执行一些其他的必要操作
    outfile << "  .text" << endl;

    // 访问所有全局变量
    Visit(program.values, outfile);

    // 访问所有函数
    Visit(program.funcs, outfile);
}

// 访问 raw slice ---- 通用的处理函数
void Visit(const koopa_raw_slice_t &slice, std::ostream &outfile)
{
    for (size_t i = 0; i < slice.len; ++i)
    {
        auto ptr = slice.buffer[i];
        // 根据 slice 的 kind 决定将 ptr 视作何种元素
        switch (slice.kind)
        {
        case KOOPA_RSIK_FUNCTION:
            // 访问函数func
            Visit_func(reinterpret_cast<koopa_raw_function_t>(ptr), outfile);
            break;
        case KOOPA_RSIK_BASIC_BLOCK:
            // 访问基本块block
            Visit_bblcok(reinterpret_cast<koopa_raw_basic_block_t>(ptr), outfile);
            break;
        case KOOPA_RSIK_VALUE:
            // 访问指令value
            Visit_val(reinterpret_cast<koopa_raw_value_t>(ptr), outfile, regmap);
            break;
        case KOOPA_RSIK_TYPE:
            // 访问type
            // Visit(reinterpret_cast<koopa_raw_type_t>(ptr));
            break;
        default:
            // 我们暂时不会遇到其他内容, 于是不对其做任何处理
            assert(false);
        }
    }
}

// 访问func
void Visit_func(const koopa_raw_function_t &func, std::ostream &outfile)
{

    // 执行一些其他的必要操作
    string func_name = func->name;

    outfile << "  .globl " << func_name.substr(1) << endl;
    outfile << func_name.substr(1) << ":" << endl;
    koopa_raw_type_t type = func->ty;

    //访问参数表
    Visit(func->params, outfile);
    // 访问所有基本块
    Visit(func->bbs, outfile);
}

// 访问basic block
void Visit_bblcok(const koopa_raw_basic_block_t &bblock, std::ostream &outfile)
{
    // 执行一些其他的必要操作
    string func_name = bblock->name;
    //访问参数表
    Visit(bblock->params, outfile);
    //访问基本块所使用的值。
    Visit(bblock->used_by, outfile);

    // 访问所有指令
    Visit(bblock->insts, outfile);
    return;
}

// 访问value
void Visit_val(const koopa_raw_value_t &value, std::ostream &outfile, std::map<koopa_raw_value_t, int> &map_reg)
{
    // 根据指令类型判断后续需要如何访问
    const auto &kind = value->kind;
    switch (kind.tag)
    {
    case KOOPA_RVT_RETURN:
        // 访问 return 指令
        Visit_ret(kind.data.ret, outfile, map_reg);
        break;
    case KOOPA_RVT_INTEGER:
        // 访问 integer 指令
        Visit_int(kind.data.integer, outfile);
        break;
    case KOOPA_RVT_BINARY:
        // 访问binary指令
        cout << "访问二元运算指令";
        Visit_binary(value, outfile, register_num, map_reg);
        break;
    default:
        // 其他类型暂时遇不到
        assert(false);
    }
    return;
}

// 访问对应类型指令的函数定义略
// 视需求自行实现
// ...

//访问ret指令
void Visit_ret(const koopa_raw_return_t &ret, std::ostream &outfile, std::map<koopa_raw_value_t, int> &map_reg)
{
    koopa_raw_value_t retval = ret.value;
    const auto &kind = retval->kind;
    if (kind.tag == KOOPA_RVT_INTEGER)
    {                            // value指向int 数值
        outfile << "  li\ta0, "; //访问指令返回后面跟的为int数值, 直接放入a0中
        Visit_val(ret.value, outfile, map_reg);
        outfile << "\n";
    }
    else
    { // value指向了上一条指令的结果寄存器
        outfile << "  mv\ta0, t" + to_string(map_reg[ret.value]) + "\n"; //将上一条指令的结果寄存器写入其中
        // 写入
        cout << "return's last instrucions register=" << map_reg[ret.value] << endl;
    }
    outfile << "  ret" << endl;
    return;
}

//访问int指令
void Visit_int(const koopa_raw_integer_t &integer, std::ostream &outfile)
{
    cout << "访问int" << endl;
    int32_t intnum = integer.value;
    outfile << intnum; //将需要的数值写入文件中
    return;
}
