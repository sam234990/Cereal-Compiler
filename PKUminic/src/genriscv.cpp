#include <string>
#include <cassert>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include <map>
#include "../include/genricsv.h"
#include "../include/symbol.h"
#include "koopa.h"

using namespace std;

int register_num = 0;
map<koopa_raw_value_t, int> regmap;

// 将字符串型 koopa ir转成内存型koopa ir并进行后端代码生成
void koopa_ir_from_str(string irstr, ostream &outfile, symbol::kirinfo &kirinfo)
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
    // koopa_dump_to_stdout(program);

    // 释放 Koopa IR 程序占用的内存
    koopa_delete_program(program);

    // 处理 raw program
    // 遍历raw

    kirinfo.register_num = 0;
    Visit(raw, outfile, kirinfo);
    // 处理完成, 释放 raw program builder 占用的内存
    // 注意, raw program 中所有的指针指向的内存均为 raw program builder 的内存
    // 所以不要在 raw program 处理完毕之前释放 builder
    koopa_delete_raw_program_builder(builder);
}

// 访问 raw program
void Visit(const koopa_raw_program_t &program, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 执行一些其他的必要操作

    // 访问所有全局变量
    Visit(program.values, outfile, kirinfo);
    // 访问所有函数
    Visit(program.funcs, outfile, kirinfo);
}

// 访问 raw slice ---- 通用的处理函数
void Visit(const koopa_raw_slice_t &slice, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    for (size_t i = 0; i < slice.len; ++i)
    {
        auto ptr = slice.buffer[i];
        // 根据 slice 的 kind 决定将 ptr 视作何种元素，同一slice中存放着所有item为同一类型
        switch (slice.kind)
        {
        case KOOPA_RSIK_FUNCTION:
            // 访问函数func
            Visit_func(reinterpret_cast<koopa_raw_function_t>(ptr), outfile, kirinfo);
            break;
        case KOOPA_RSIK_BASIC_BLOCK:
            // 访问基本块block
            Visit_bblcok(reinterpret_cast<koopa_raw_basic_block_t>(ptr), outfile, kirinfo);
            break;
        case KOOPA_RSIK_VALUE:
            // 访问指令value
            Visit_val(reinterpret_cast<koopa_raw_value_t>(ptr), outfile, kirinfo);
            break;
        case KOOPA_RSIK_TYPE:
            // 访问type
            // Visit(reinterpret_cast<koopa_raw_type_t>(ptr));
            break;
        default:
            // 我们暂时不会遇到其他内容, 于是不对其做任何处理
            assert(false);
        }
        kirinfo.register_num = 0; //每次访问一条指令之后寄存器数值清空
    }
}

// 访问func
void Visit_func(const koopa_raw_function_t &func, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 0. 将kirinfo恢复为初值 保存初始寄存器分配号 stack大小 是否已经访问call指令 stack_a的大小
    kirinfo.reset_func_kir();

    if (func->bbs.len == 0)
    { //如果是函数声明则跳过
        return;
    }

    // 执行一些其他的必要操作
    outfile << "  .text" << endl;
    string func_name = func->name;
    // 将 @ 符号去除
    outfile << "  .globl " << func_name.substr(1) << endl;
    outfile << func_name.substr(1) << ":" << endl;
    // koopa_raw_type_t type = func->ty;

    //访问参数表
    func_params_reg(func->params, outfile, kirinfo);

    // prologue : 计算所有基本块的所有指令所需要的堆栈空间并分配。
    func_prologue(func->bbs, outfile, kirinfo);

    // 访问所有基本块
    Visit(func->bbs, outfile, kirinfo);
    outfile << "\n";
}

// 访问func的形参表
void func_params_reg(const koopa_raw_slice_t &params, std::ostream &outfile, symbol::kirinfo &kirinfo)
{

    for (size_t i = 0; i < params.len; ++i)
    { //访问每一个形参
        auto ptr = params.buffer[i];
        auto value = reinterpret_cast<koopa_raw_value_t>(ptr);
        if (value->kind.tag != KOOPA_RVT_FUNC_ARG_REF)
        {
            std::cerr << "程序错误：函数形参类型未符合预期" << std::endl;
        }
        kirinfo.regmap[value] = i; //为每个形参分配寄存器 当大于等于8时为栈帧
    }
}

// 函数的prologue 计算分配的栈空间大小等等工作
void func_prologue(const koopa_raw_slice_t &bbsslice, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    int stack_size = 0;
    int stack_s = 0; //局部变量分配的栈空间
    int stack_r = 0; //为  ra 分配的栈空间
    int stack_a = 0; //传参预留的栈空间
    // 1. 计算局部变量的栈空间S
    for (size_t i = 0; i < bbsslice.len; ++i)
    { //访问每一个基本块
        auto ptr = bbsslice.buffer[i];
        auto bblock = reinterpret_cast<koopa_raw_basic_block_t>(ptr);
        auto instr_slice = bblock->insts;
        for (size_t j = 0; j < instr_slice.len; j++)
        { //访问基本块中的指令
            const auto &value = reinterpret_cast<koopa_raw_value_t>(instr_slice.buffer[j]);
            if (value->ty->tag != KOOPA_RTT_UNIT)
            { //如果是指令的类型不是 unit
                // 1. 分配其在stack中的具体位置, 并记录, 数组为其基址地址， 数组形参为指针，也是4字节一个地址
                kirinfo.stack_map[value] = stack_s;
                if (value->kind.tag == KOOPA_RVT_ALLOC)
                { // 2. 如果是当前指令为 alloc 类型

                    // TODO 数组形参alloc可能需要修改
                    assert(value->ty->tag == KOOPA_RTT_POINTER);
                    auto kind = value->ty->data.pointer.base; //获取当前 指针的base
                    int arraysize = 1;                        //初始化数组长度
                    while (kind->tag == KOOPA_RTT_ARRAY)
                    { //初始化数值时此部分跳过
                        //如果当前 kind 指向的为数组
                        int cursize = kind->data.array.len; //获取当前维度的长度
                        arraysize *= cursize;
                        kind = kind->data.array.base; //获取当前数组的base
                    }
                    stack_s += 4 * arraysize;
                    // cout << value->name << " arraysize: "<< arraysize <<endl;
                }
                else
                { // 2. 非alloc类型在栈中分配 4字节
                    // 为每个指令分配 4 字节
                    stack_s += 4;
                }
            }

            if (value->kind.tag == KOOPA_RVT_CALL)
            { //访问到call指令
                // 2. 计算 ra 的栈空间 R
                stack_r = 4;

                // 3. 计算传参预留的栈空间 A
                if ((int(value->kind.data.call.args.len) - 8) > stack_a)
                { // max (len - 8, 0)
                    // cout << int(value->kind.data.call.args.len) - 8 <<endl;
                    stack_a = int(value->kind.data.call.args.len) - 8;
                }
            }
        }
    }
    stack_a *= 4;
    stack_size = stack_s + stack_r + stack_a;

    // 2. 计算
    if ((stack_size % 16) != 0)
    { //按16字节补齐
        stack_size = (stack_size / 16 + 1) * 16;
    }
    kirinfo.stack_value_base = stack_a;
    kirinfo.stack_size = stack_size;

    if (stack_size != 0)
    {
        if (stack_size > 2047)
        {
            outfile << "  li\tt0, " + to_string(-stack_size) + "\n";
            outfile << "  add\tsp, sp, t0\n";
        }
        else
        {
            outfile << "  addi\tsp, sp, " << to_string(-stack_size) << endl;
        }
    }

    if (stack_r != 0)
    { //保存 ra 寄存器，如果函数体中含有 call 指令
        if (stack_size > 2047)
        {
            outfile << "  li\tt0, " + to_string(stack_size - 4) + "\n";
            outfile << "  add\tt0, sp, t0\n";
            outfile << "  sw\tra, 0(t0)\n";
        }
        else
        {
            outfile << "  sw\tra, " + to_string(stack_size - 4) + "(sp)\n";
        }
        kirinfo.has_call = true; //后面函数中包含了call语句
    }

    cout << "************stack_size = " << stack_size << endl;
    return;
}

// 访问basic block
void Visit_bblcok(const koopa_raw_basic_block_t &bblock, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 执行一些其他的必要操作
    string bblock_name = bblock->name;
    cout << "bblock_name-" << bblock_name << endl;

    if (bblock_name != "%entry")
    { //如果basicblock不是 %entry 则将名称写入
        // 将 @ 符号去
        outfile << bblock_name.substr(1) << ":" << endl;
    }

    //访问参数表
    Visit(bblock->params, outfile, kirinfo);
    //访问基本块所使用的值。
    // Visit(bblock->used_by, outfile, kirinfo);

    // 访问所有指令
    Visit(bblock->insts, outfile, kirinfo);
    return;
}

// 访问value
void Visit_val(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 根据指令类型判断后续需要如何访问
    const auto &kind = value->kind;
    switch (kind.tag)
    {
    case KOOPA_RVT_RETURN:
        outfile << "\t #ret \n";
        // 访问 return 指令
        cout << "访问ret指令" << endl;
        Visit_ret(kind.data.ret, outfile, kirinfo);
        break;
    case KOOPA_RVT_INTEGER:
        // 访问 integer 指令
        // cout << "访问int指令" << endl;
        Visit_int(kind.data.integer, outfile, kirinfo);
        break;
    case KOOPA_RVT_BINARY:
        // 访问binary指令
        Visit_binary(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_ALLOC:
        // cout << "访问alloc指令" << endl;
        Visit_alloc(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_LOAD:
        outfile << "\t #load \n";
        Visit_load(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_STORE:
        outfile << "\t #store \n";
        Visit_store(kind.data.store, outfile, kirinfo);
        break;
    case KOOPA_RVT_BRANCH:
        outfile << "\t #branch \n";
        Visit_branch(kind.data.branch, outfile, kirinfo);
        break;
    case KOOPA_RVT_JUMP:
        outfile << "\t #jump \n";
        Visit_jump(kind.data.jump, outfile, kirinfo);
        break;
    case KOOPA_RVT_CALL:
        outfile << "\t #call \n";
        // cout << "call" << endl;
        Visit_call(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_GLOBAL_ALLOC:
        // outfile << "\t #global alloc \n";
        Visit_global_alloc(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_AGGREGATE:
        // cout << "\t 访问 aggregate \n";
        visit_aggregate(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_GET_ELEM_PTR:
        // cout << "\t 访问 getelemptr \n";
        outfile << "\t #getelemptr \n";
        visit_getelemptr(value, outfile, kirinfo);
        break;
    case KOOPA_RVT_GET_PTR:
        // cout << "\t 访问 getptr \n";
        outfile << "\t #getptr \n";
        visit_getptr(value, outfile, kirinfo);
        break;
    default:
        // 其他类型暂时遇不到
        assert(false);
    }
    // kirinfo.register_num = 0; //每次访问一条指令之后寄存器数值清空
    return;
}

// 访问对应类型指令的函数定义略
// 视需求自行实现
// ...

//访问ret指令
void Visit_ret(const koopa_raw_return_t &ret, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    if (ret.value != NULL)
    {
        koopa_raw_value_t retval = ret.value;
        const auto &kind = retval->kind;
        string retreg;
        string retstack;
        if (kind.tag == KOOPA_RVT_INTEGER)
        {                            // value指向int 数值
            outfile << "  li\ta0, "; //访问指令返回后面跟的为int数值, 直接放入a0中
            Visit_val(ret.value, outfile, kirinfo);
            outfile << "\n";
        }
        else
        { // value指向了上一条指令的结果寄存器
            // retreg = get_reg_(ret.value, kirinfo);
            retstack = kirinfo.find_value_in_stack(outfile, ret.value);
            outfile << "  lw\ta0, " + retstack + "\n"; //将上一条指令的结果存入stack的位置lw进a0
            // 写入
            cout << "return's last instrucions stack location =" << retstack << endl;
        }
    }
    else
    {
        cout << "return后为空" << endl;
    }

    //函数的epilogue
    // 1. 如果当前函数之前遍历过call语句，ra寄存器被修改
    if (kirinfo.has_call)
    { //恢复ra寄存器数值
        if (kirinfo.stack_size > 2047)
        {
            outfile << "  li\tt0, " + to_string(kirinfo.stack_size - 4) + "\n";
            outfile << "  add\tt0, sp, t0\n";
            outfile << "  lw\tra, 0(t0)\n";
        }
        else
        {
            outfile << "  lw\tra, " + to_string(kirinfo.stack_size - 4) + "(sp)\n";
        }
    }
    // 2. 栈帧复原
    if (kirinfo.stack_size != 0)
    {
        if (kirinfo.stack_size > 2047)
        {
            outfile << "  li\tt0, " + to_string(kirinfo.stack_size) + "\n";
            outfile << "  add\tsp, sp, t0\n";
        }
        else
        {
            outfile << "  addi\tsp, sp, " << to_string(kirinfo.stack_size) << endl;
        }
    }

    outfile << "  ret" << endl;
    return;
}

//访问int指令
void Visit_int(const koopa_raw_integer_t &integer, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // cout << "访问int" << endl;
    int32_t intnum = integer.value;
    outfile << intnum; //将需要的数值写入文件中
    return;
}

//访问 alloc 指令
void Visit_alloc(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo)
{ //在函数prologue中已经在stack中为变量分配了空间
    cout << value->name << endl;
    //此阶段没有语句
    return;
}

//访问 load 指令
void Visit_load(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo)
{ // lw 指令，先lw进寄存器中，然后再存入当前指令对应stack的位置

    // 1.先lw进寄存器中
    //  string instrreg = "t0";// 暂时应该没有寄存器冲突

    // 当前指令用t0
    kirinfo.register_num++;
    const auto &load = value->kind.data.load;
    string loadstack, global_name;
    switch (load.src->kind.tag)
    {
    case KOOPA_RVT_INTEGER:
        //如果是integer型
        outfile << "  li\tt0, ";
        Visit_val(load.src, outfile, kirinfo);
        outfile << "\n";
        break;
    case KOOPA_RVT_ALLOC:
        //如果是alloc型
        loadstack = kirinfo.find_value_in_stack(outfile, load.src); // load指令右值对应的stack位置
        outfile << "  lw\tt0, " + loadstack << endl;
        break;
    case KOOPA_RVT_GLOBAL_ALLOC:
        //如果是global alloc型
        // 1. 先获取全局变量的名称
        global_name = load.src->name;
        // 去掉@符号
        global_name = global_name.substr(1);
        // 2. 将其地址存入t0中
        outfile << "  la\tt0, " + global_name << endl;
        // 3. 利用基址加变址的方式将对应内存存进来。
        outfile << "  lw\tt0, 0(t0)\n";
        break;
    case KOOPA_RVT_GET_ELEM_PTR:
        //如果是 getelemptr 型
        // 1. 将其指针的地址中的内容取出
        loadstack = kirinfo.find_value_in_stack(outfile, load.src); // load指令右值对应的stack位置
        outfile << "  lw\tt0, " + loadstack << endl;
        //再将其load进来
        outfile << "  lw\tt0, 0(t0)\n";
        break;
    case KOOPA_RVT_GET_PTR:
        //如果是 getelemptr 型
        // 1. 将其指针的地址中的内容取出
        loadstack = kirinfo.find_value_in_stack(outfile, load.src); // load指令右值对应的stack位置
        outfile << "  lw\tt0, " + loadstack << endl;
        //再将其load进来
        outfile << "  lw\tt0, 0(t0)\n";
        break;
    default:
        cout << "load.src->kind.tag = " << load.src->kind.tag << endl;
        std::cerr << "程序错误：load 指令的目标类型不符合预期" << std::endl;

        break;
    }

    // 2.再存入当前 load 指令对应stack的位置
    string instr_stack = kirinfo.find_value_in_stack(outfile, value);
    outfile << "  sw\tt0, " + instr_stack << endl;
    return;
}

//访问 store 指令
void Visit_store(const koopa_raw_store_t &store, std::ostream &outfile, symbol::kirinfo &kirinfo)
{ // store 指令先load进来, 再存入对应为该指令对应的位置。

    // 1.先load进来
    // 当前指令用t0
    string store_value_reg = "t" + to_string(kirinfo.register_num++);
    // 1.1如果是函数形参则不需要load进来
    if (store.value->kind.tag == KOOPA_RVT_FUNC_ARG_REF)
    {
        // cout << store.value->name << "被函数所使用" << endl;
        string dest_stack = kirinfo.find_value_in_stack(outfile, store.dest);
        if (kirinfo.regmap[store.value] >= 8)
        { //当形参没有在寄存器中
            //当前参数存放在上一个函数的栈帧区域
            string loadstack = to_string((kirinfo.regmap[store.value] - 8) * 4 + kirinfo.stack_size) + "(sp)";
            outfile << "  lw\t" + store_value_reg + ", " + loadstack << endl;

            outfile << "  sw\t" + store_value_reg + ", " + dest_stack << endl;
            return;
        }
        string temp = "a" + to_string(kirinfo.regmap[store.value]);
        outfile << "  sw\t" + temp + ", " + dest_stack << endl;
        return;
    }
    else if (store.value->kind.tag == KOOPA_RVT_INTEGER)
    { //如果是数值型
        outfile << "  li\t" + store_value_reg + ", ";
        Visit_val(store.value, outfile, kirinfo);
        outfile << "\n";
    }
    else
    {
        //如果不是数值型
        string loadstack = kirinfo.find_value_in_stack(outfile, store.value); // load指令右值对应的stack位置
        outfile << "  lw\t" + store_value_reg + ", " + loadstack << endl;
    }

    // 2.再存入dest对应stack的位置
    string global_name, dest_stack;
    string dest_reg = "t" + to_string(kirinfo.register_num++);
    switch (store.dest->kind.tag)
    {
    case KOOPA_RVT_GLOBAL_ALLOC:
        //如果存入的位置是全局变量
        // 1. 再申请一个变量计算存入的内存位置
        // 先获取全局变量名称
        global_name = store.dest->name;
        // 去掉@符号
        global_name = global_name.substr(1);
        outfile << "  la\t" + dest_reg + ", " + global_name << endl;
        // 2. sw语句存入
        outfile << "  sw\t" + store_value_reg + ", 0(" + dest_reg + ")\n";
        break;
    case KOOPA_RVT_ALLOC:
        //如果存入的位置是局部变量
        dest_stack = kirinfo.find_value_in_stack(outfile, store.dest);
        outfile << "  sw\t" + store_value_reg + ", " + dest_stack << endl;
        break;
    case KOOPA_RVT_GET_ELEM_PTR:
        //如果是 getelemptr 型
        //将指针对应stack中存放的内容取出 ： 指针指向的地址
        ++kirinfo.register_num;
        dest_stack = kirinfo.find_value_in_stack(outfile, store.dest);
        outfile << "  lw\t" + dest_reg + ", " + dest_stack << endl;
        //将内容从存入 地址对应的内存位置
        outfile << "  sw\t" + store_value_reg + ", 0(" + dest_reg + ")" << endl;
        break;
    case KOOPA_RVT_GET_PTR:
        //如果是getptr型
        ++kirinfo.register_num;
        dest_stack = kirinfo.find_value_in_stack(outfile, store.dest);
        outfile << "  lw\t" + dest_reg + ", " + dest_stack << endl;
        //将内容从存入 地址对应的内存位置
        outfile << "  sw\t" + store_value_reg + ", 0(" + dest_reg + ")" << endl;
        break;
    default:
        cout << "store.dest->kind.tag: " << store.dest->kind.tag << endl;

        std::cerr << "程序错误：store dest 类型不符合预期" << std::endl;
        break;
    }

    return;
}

//访问 jump 指令
void Visit_jump(const koopa_raw_jump_t &jump, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    string jump_target = jump.target->name;
    cout << "jump_target:   " << jump_target << endl;

    // 将 @ 符号去除
    outfile << "  j\t" + jump_target.substr(1) + "\n\n";
    return;
}

//访问 branch 指令
void Visit_branch(const koopa_raw_branch_t &branch, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 1.将cond的结果 load 进来
    // 当前指令用t0
    kirinfo.register_num++;
    if (branch.cond->kind.tag == KOOPA_RVT_INTEGER)
    { //如果是数值型
        outfile << "  li\tt0, ";
        Visit_val(branch.cond, outfile, kirinfo);
        outfile << "\n";
    }
    else
    {
        //如果不是数值型
        string loadstack = kirinfo.find_value_in_stack(outfile, branch.cond); // load指令右值对应的stack位置
        outfile << "  lw\tt0, " + loadstack << endl;
    }

    string true_name = branch.true_bb->name;
    string false_name = branch.false_bb->name;

    // 将 @ 符号去除
    outfile << "  bnez\tt0, " + true_name.substr(1) + "\n";
    outfile << "  j\t" + false_name.substr(1) + "\n\n";

    return;
}

//访问 call 指令
void Visit_call(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 0. 遍历到call语句
    kirinfo.has_call = true;

    const auto &call = value->kind.data.call;
    // 1. 如果有实参，则将各个实参存入a0-7寄存器 和 sp的栈帧中
    cout << call.args.len << endl;

    for (int i = 0; i < call.args.len; i++)
    {
        koopa_raw_value_t arg_value = reinterpret_cast<koopa_raw_value_t>(call.args.buffer[i]);
        kirinfo.register_num = 0;
        if (i < 8)
        { // 用lw 到 a0 - a7
            if (arg_value->kind.tag == KOOPA_RVT_INTEGER)
            {                                                 //如果是数值型
                int num = arg_value->kind.data.integer.value; //获取数值
                outfile << "  li\ta" + to_string(i) + ", " + to_string(num) + "\n";
            }
            else
            {
                string arg_stack = kirinfo.find_value_in_stack(outfile, arg_value); //把栈帧中的内存存入
                outfile << "  lw\ta" + to_string(i) + ", " + arg_stack + "\n";
            }
        }
        else
        {
            string arg_reg = "t" + to_string(kirinfo.register_num++);
            if (arg_value->kind.tag == KOOPA_RVT_INTEGER)
            {                                                 //如果是数值型
                int num = arg_value->kind.data.integer.value; //获取数值
                outfile << "  li\t" + arg_reg + ", " + to_string(num) + "\n";
                outfile << "  sw\t" + arg_reg + ", " + to_string((i - 8) * 4) + "(sp)\n";
            }
            else
            {
                string arg_stack = kirinfo.find_value_in_stack(outfile, arg_value); //把栈帧中的内存存入
                outfile << "  lw\t" + arg_reg + ", " + arg_stack + "\n";
                outfile << "  sw\t" + arg_reg + ", " + to_string((i - 8) * 4) + "(sp)\n";
            }
        }
    }
    cout << call.callee->name << endl;
    string funcname = call.callee->name;
    // 将 @ 符号去除
    funcname = funcname.substr(1);
    // 2. call函数调用
    outfile << "  call\t" + funcname + "\n";

    // 3.如果当前call具有返回值，需要将当前call得到的结果保存在栈帧中
    if (value->ty->tag != KOOPA_RTT_UNIT)
    {
        string call_stack = kirinfo.find_value_in_stack(outfile, value);
        outfile << "  sw\ta0, " + call_stack + "\n";
    }
}

//访问 global alloc 指令
void Visit_global_alloc(const koopa_raw_value_t &value, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    // 1. 数据段
    outfile << "  .data\n  .globl ";
    // 2. 获取全局变量定义的名称
    string global_name = value->name;
    // 将 @ 符号去除
    global_name = global_name.substr(1);
    outfile << global_name << "\n"
            << global_name << ":\n";
    const auto &init = value->kind.data.global_alloc.init;
    // cout << init->kind.tag << endl;
    if (init->kind.tag == KOOPA_RVT_ZERO_INIT)
    { //如果初始值为zeroinit
        if (value->ty->tag == KOOPA_RTT_POINTER)
        {                      //如果用zeroinit初始化全0数组
            int arraysize = 1; //初始化数组长度
            auto kind = value->ty->data.pointer.base;
            while (kind->tag == KOOPA_RTT_ARRAY)
            { //初始化数值时此部分跳过
                //如果当前 kind 指向的为数组
                int cursize = kind->data.array.len; //获取当前维度的长度
                arraysize *= cursize;
                kind = kind->data.array.base; //获取当前数组的base
            }
            arraysize *= 4;
            // int sizeofarray = 4 * (value->ty->data.pointer.base->data.array.len);
            outfile << "  .zero " + to_string(arraysize) + "\n";
        }
        else
        {
            cout << ".zero" << value->ty->tag << endl;
            outfile << "  .zero 4\n\n";
        }
    }
    else if (init->kind.tag == KOOPA_RVT_INTEGER)
    { //是数值初始
        int num = init->kind.data.integer.value;
        outfile << "  .word " + to_string(num) + "\n";
    }
    else if (init->kind.tag == KOOPA_RVT_AGGREGATE)
    { // 用 aggregate初始化数组
        // 暂时处理一维数组情况
        // 访问aggregate
        Visit_val(init, outfile, kirinfo);
    }
    else
    {
        std::cerr << "程序错误：全局变量初始化不符合预期" << std::endl;
    }
    outfile << endl;
    return;
}

// 访问 aggregate 数组初始化语句
void visit_aggregate(const koopa_raw_value_t &aggregate, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    auto elems = aggregate->kind.data.aggregate.elems;
    for (size_t i = 0; i < elems.len; ++i)
    {
        auto ptr = elems.buffer[i];
        // 根据 elems 的 kind 决定将 ptr 视作何种元素，同一 elems 中存放着所有 elem 为同一类型
        if (elems.kind == KOOPA_RSIK_VALUE)
        { //如果当前的 elem 为 value 类型
            auto elem = reinterpret_cast<koopa_raw_value_t>(ptr);
            if (elem->kind.tag == KOOPA_RVT_AGGREGATE)
            { //如果是 aggregate 型则循环嵌套访问
                visit_aggregate(elem, outfile, kirinfo);
            }
            else if (elem->kind.tag == KOOPA_RVT_INTEGER)
            { //如果是 integer 型 则用 .word + int
                outfile << "  .word ";
                Visit_int(elem->kind.data.integer, outfile, kirinfo); //访问int型
                outfile << endl;
            }
            else if (elem->kind.tag == KOOPA_RVT_ZERO_INIT)
            { //如果是zeroinit类型
                auto kind = elem->ty->data.pointer.base;
                int arraysize = 1; //初始化数组长度
                while (kind->tag == KOOPA_RTT_ARRAY)
                { //初始化数值时此部分跳过
                    //如果当前 kind 指向的为数组
                    int cursize = kind->data.array.len; //获取当前维度的长度
                    arraysize *= cursize;
                    kind = kind->data.array.base; //获取当前数组的base
                }
                arraysize *= 4;
                outfile << "  .zero " + to_string(arraysize) << endl;
            }
            else
            {
                std::cerr << "程序错误： aggregate 的elem 类型不符合预期" << std::endl;
            }
        }
        else
        {
            cout << "elem.kind = " << elems.kind << endl;
            std::cerr << "程序错误： aggregate 的elem 类型不符合预期" << std::endl;
        }
    }
}

//访问 getelemptr 指令
void visit_getelemptr(const koopa_raw_value_t &getelemptr, std::ostream &outfile, symbol::kirinfo &kirinfo)
{
    auto src = getelemptr->kind.data.get_elem_ptr.src;
    auto index = getelemptr->kind.data.get_elem_ptr.index;

    auto kind = getelemptr->ty->data.pointer.base;
    int arraysize = 1; //初始化数组长度
    while (kind->tag == KOOPA_RTT_ARRAY)
    { //初始化数值时此部分跳过
        //如果当前 kind 指向的为数组
        int cursize = kind->data.array.len; //获取当前维度的长度
        arraysize *= cursize;
        kind = kind->data.array.base; //获取当前数组的base
    }
    // cout << arraysize << endl;

    string src_reg = "t" + to_string(kirinfo.register_num++); //本行的src地址所用的寄存器
    // 1. 计算 src 的地址
    if (src->kind.tag == KOOPA_RVT_GLOBAL_ALLOC)
    { //如果当前src为全局变量分配
        // 先获取全局变量名称
        string global_name = src->name;
        // 去掉@符号
        global_name = global_name.substr(1);
        outfile << "  la\t" + src_reg + ", " + global_name << endl;
    }
    else if (src->kind.tag == KOOPA_RVT_ALLOC) //如果当前指向的是局部变量
    {
        int srcstack = kirinfo.find_value_in_stack_int(src); // src 对应的stack位置
        if (srcstack < 2047)
        { //如果srcstack 的size 小于 12 位 最大为+2047
            outfile << "  addi\t" + src_reg + ", sp, " + to_string(srcstack) << endl;
        }
        else
        {
            outfile << "  li\t" + src_reg + ", " + to_string(srcstack) << endl;
            outfile << "  add\t" + src_reg + ", sp, " + src_reg << endl;
        }
    }
    else if (src->kind.tag == KOOPA_RVT_GET_ELEM_PTR)
    { //如果当前指向的是指针 getelemptr 将里面的内容load进来
        string srcstack = kirinfo.find_value_in_stack(outfile, src);
        outfile << "  lw\t" + src_reg + ", " + srcstack << endl;
    }
    else if (src->kind.tag == KOOPA_RVT_GET_PTR)
    { //如果当前指向的是指针 getptr 将里面的内容load进来
        string srcstack = kirinfo.find_value_in_stack(outfile, src);
        outfile << "  lw\t" + src_reg + ", " + srcstack << endl;
        // cout << "src->kind.tag:" << src->kind.tag << endl;
    }
    else
    {
        cout << "src->kind.tag = " << src->kind.tag << endl;
        std::cerr << "程序错误： getelemptr 的 src 类型不符合预期" << std::endl;
    }

    // 2. 获得 index 的大小
    string indexreg = "t" + to_string(kirinfo.register_num++);
    if (index->kind.tag == KOOPA_RVT_INTEGER)
    { // index 要么为数值型
        if (index->kind.data.integer.value == 0 && arraysize == 1)
        { // index 为 0 且为最低一维时 不用计算index，直接return
            string geteleptr_stack = kirinfo.find_value_in_stack(outfile, getelemptr);
            outfile << "  sw\t" + src_reg + ", " + geteleptr_stack + "\n";
            return;
        }

        outfile << "  li\t" + indexreg + ", ";
        Visit_int(index->kind.data.integer, outfile, kirinfo); //访问int型
    }
    else
    { // 要么为 %x koopa ir变量
        string index_stack = kirinfo.find_value_in_stack(outfile, index);
        outfile << "  lw\t" + indexreg + ", " + index_stack;
        // cout << "index.kind = " << index->kind.tag << endl;
        // std::cerr << "程序错误： getelemptr 的 index 类型不符合预期" << std::endl;
    }

    string size_reg = "t" + to_string(kirinfo.register_num++);
    arraysize *= 4;
    outfile << "\n  li\t" + size_reg + ", " + to_string(arraysize) + "\n"; //当前指针的大小
    outfile << "  mul\t" + indexreg + ", " + indexreg + ", " + size_reg + "\n";

    // 3. 计算 getelemptr 的结果
    outfile << "  add\t" + src_reg + ", " + src_reg + ", " + indexreg + "\n";

    string geteleptr_stack = kirinfo.find_value_in_stack(outfile, getelemptr);
    outfile << "  sw\t" + src_reg + ", " + geteleptr_stack + "\n";
    return;
}

void visit_getptr(const koopa_raw_value_t &getptr, std::ostream &outfile, symbol::kirinfo &kirinfo)
{

    // TODO
    auto src = getptr->kind.data.get_ptr.src;
    auto index = getptr->kind.data.get_ptr.index;
    // cout << "src->ty->data.pointer.base->tag:" << src->ty->data.pointer.base->tag << endl;

    auto kind = src->ty->data.pointer.base; // src为指针，获取指针的基地址，以此来求 src指向内容的size
    int arraysize = 1;                      //初始化数组长度
    while (kind->tag == KOOPA_RTT_ARRAY)
    { //初始化数值时此部分跳过
        //如果当前 kind 指向的为数组
        int cursize = kind->data.array.len; //获取当前维度的长度
        arraysize *= cursize;
        kind = kind->data.array.base; //获取当前数组的base
    }
    // cout << "arraysize:" << arraysize << endl;

    string src_reg = "t" + to_string(kirinfo.register_num++); //本行的 src 地址所用的寄存器

    // 1. 计算 src 的地址 TODO 这一部分的 src 应该只是从 load 获得的
    if (src->kind.tag == KOOPA_RVT_LOAD)
    {
        string srcstack = kirinfo.find_value_in_stack(outfile, src);
        outfile << "  lw\t" + src_reg + ", " + srcstack << endl;
    }
    else
    {
        cout << "src->kind.tag = " << src->kind.tag << endl;
        std::cerr << "程序错误： getptr 的 src 类型不符合预期" << std::endl;
    }

    // 2. 获得 index 的大小
    string indexreg = "t" + to_string(kirinfo.register_num++);
    if (index->kind.tag == KOOPA_RVT_INTEGER)
    { // index 要么为数值型
        if (index->kind.data.integer.value == 0 && arraysize == 1)
        { // index 为 0 且为最低一维时 不用计算index，直接return
            string getptr_stack = kirinfo.find_value_in_stack(outfile, getptr);
            outfile << "  sw\t" + src_reg + ", " + getptr_stack + "\n";
            return;
        }

        outfile << "  li\t" + indexreg + ", ";
        Visit_int(index->kind.data.integer, outfile, kirinfo); //访问int型
    }
    else
    { // 要么为 %x koopa ir变量
        string index_stack = kirinfo.find_value_in_stack(outfile, index);
        outfile << "  lw\t" + indexreg + ", " + index_stack;
        // cout << "index.kind = " << index->kind.tag << endl;
        // std::cerr << "程序错误： getelemptr 的 index 类型不符合预期" << std::endl;
    }

    string size_reg = "t" + to_string(kirinfo.register_num++);
    arraysize *= 4;
    outfile << "\n  li\t" + size_reg + ", " + to_string(arraysize) + "\n"; //当前指针的大小
    outfile << "  mul\t" + indexreg + ", " + indexreg + ", " + size_reg + "\n";

    // 3. 计算 getelemptr 的结果
    outfile << "  add\t" + src_reg + ", " + src_reg + ", " + indexreg + "\n";

    string getptr_stack = kirinfo.find_value_in_stack(outfile, getptr);
    outfile << "  sw\t" + src_reg + ", " + getptr_stack + "\n";
    return;
}