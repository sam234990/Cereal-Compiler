#pragma once
#ifndef __SYMBOL__H_
#define __SYMBOL__H_

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include "koopa.h"
#include <map>

using namespace std;

namespace symbol
{
    // 符号表中的一项
    class SymbolItem
    {
    public:
        bool is_const;              //是否是const 类型
        bool is_array;              // 是否是数组 类型
        int dimension_len;          // 表示数组有几个维数 数值类型应为 0
        vector<int> dimension_size; // 表示数组每一维度的深度，从底到外
        // std::vector<int> dim_total_num; // a[2][3][4] -> 24,12,4,1
        vector<int> result;         //如果是constdefone应该有结果。数值类型有1 个值。数组展开后存成1维结果
        string koopa_ir_name;       // 该符号在koopa ir中的名称，一般为 @name1，最后数字表示第几个重复定义

        SymbolItem(){};
        SymbolItem(bool is_const, bool is_array);
        // SymbolItem(bool is_c, bool is_a, int size);
        ~SymbolItem() = default;
    };

    // 作用域
    class Scope
    {
    public:
        unordered_map<string, SymbolItem> symboltable;
        // func形参表的父作用域恒为全局变量，将其 parent_scope_id 设为0
        //若没有形参表，第一个block的父作用域也是全局变量作用域，因此 parent_scope_id 也为0
        //全局作用域为第一个作用域，其父作用域为-1
        int scope_id;        //当前作用域id
        int parent_scope_id; // 父作用域id

        Scope(){};
        Scope(int id, int parid) : scope_id(id), parent_scope_id(parid){};
        ~Scope() = default;
    };

    class FuncTableItem
    {
    public:
        string functype; // VOID INT
        std::vector<std::vector<int>> shape_list_;
        // TODO: 如何处理库函数的size
        FuncTableItem(string functype) : functype(functype) {}
        FuncTableItem() {}
        void Print(std::ostream &outfile = std::clog);
    };
    using FuncTable = std::unordered_map<std::string, FuncTableItem>;

    using Scopes = std::vector<Scope>;
    extern Scopes Scope_1;
    extern FuncTable gFuncTable;

    void finditem(int cur_scope_id, string ident_name, int line_num, SymbolItem &item);
    void SemanticError(int line_no, const std::string &&error_msg);
    void koopa_ir_name(string ident_name, SymbolItem &item);

    class astinfo
    { //使用该指针作为全局变量 在遍历ast时， 向上、向下传递数据
    public:
        int symbolnum; //变量num
        bool is_main;
        int ast_scope_id;  // 当前ast对应的scopeid
        int if_num;        // 程序中if的数量
        int and_num;       // 程序中and的数量
        int or_num;        // 程序中or的数量
        bool has_return;   // 用于表示当前作用域中是否含有return语句
        int while_num;     // 程序中while的数量
        int cur_while_num; // 当前ast对应的while号，用于处理while套while现象，多个break、continue
        bool is_iteration; // 用于表示当前ast是否处于循环体中，用于break与continue语句的语义判断
        int break_num;     // 程序中break的数量
        int continue_num;  // 程序中continue的数量
        string func_type;  //当前所处函数的返回值类型
        int array_offset;
        int brace_num;                  // 当前位置(array_offset_)有几个大括号
        std::vector<int> dim_total_num; // a[2][3][4] -> 24,12,4,1

        astinfo();
        ~astinfo() = default;
        void reset_array_info(); // 重置数组有关的全局变量信息
    };

    class kirinfo
    { //使用该指针作为全局变量 在遍历KoopaIR时， 向上、向下传递数据
    public:
        int register_num;     //寄存器分配号
        int stack_size;       //函数栈帧大小
        int stack_value_base; //若函数的形参超过8个，则stack中栈帧底部部分地址存放其多余形参，
        bool has_call;        //用于判断 当前指令 前的部分是否有过call指令，用于epilogue中从战阵中恢复ra寄存器值。

        map<koopa_raw_value_t, int> regmap;    //当前指令对应的寄存器号
        map<koopa_raw_value_t, int> stack_map; //当前指令对应的栈帧偏移量，未加stack_value_base

        kirinfo();
        ~kirinfo() = default;

        void reset_func_kir();
        std::string find_value_in_stack(std::ostream &outfile, koopa_raw_value_t value);
        int find_value_in_stack_int(koopa_raw_value_t value);
    };

} // namespace symbol

#endif