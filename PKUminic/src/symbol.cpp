#include "../include/symbol.h"
#include <memory>
#include <iostream>
#include <string>
#include <vector>

namespace symbol
{

    Scopes Scope_1;       //定义作用域
    FuncTable gFuncTable; //函数table

    // 符号表项
    SymbolItem::SymbolItem(bool is_const, bool is_array)
    {
        this->is_const = is_const;
        this->is_array = is_array;
        this->is_funcFparam = false;//默认为不是
        result.reserve(5);
    }

    // astinfo
    astinfo::astinfo()
    {
        this->and_num = 0;
        this->symbolnum = 0;
        this->or_num = 0;
        this->if_num = 0;
        this->while_num = 0;
        this->has_return = false;
        this->cur_while_num = 0;
        this->is_iteration = false;
        this->break_num = 0;
        this->continue_num = 0;
    }

    void astinfo::reset_array_info()
    {
        this->array_offset = 0;      //数组偏移量归零
        this->brace_num = 0;         //数组初始化列表括号个数归零
        this->dim_total_num.clear(); //维数信息清空
    }

    void SemanticError(int line_no, const std::string &&error_msg)
    {
        std::cerr << "语义错误 at line " << line_no << " : " << error_msg << std::endl;
        exit(255);
    }

    void finditem(int cur_scope_id, string ident_name, int line_num, SymbolItem &item)
    {
        int enquire_scope_id = cur_scope_id; //当前查找的作用域id

        bool enquire_result = false; //初始假设没找到
        while (enquire_scope_id != -1)
        { //全局作用域的id为0，根作用域的父id为-1
            // cout << "当前查找作用域id---" << enquire_scope_id << endl;
            const auto &symbol_table = symbol::Scope_1[enquire_scope_id].symboltable;
            const auto &var_iter = symbol_table.find(ident_name);
            if (var_iter != symbol_table.end())
            { // 如果找到则结束
                // 2. 取出符号表中对应的表项
                item = var_iter->second;
                // item = symbol::Scope_1[enquire_scope_id].symboltable[ident_name];
                enquire_result = true;
                break;
            }
            enquire_scope_id = symbol::Scope_1[enquire_scope_id].parent_scope_id; //设置其为父作用域id
        }
        if (!enquire_result)
        { // 如果没找到结果 报错
            symbol::SemanticError(line_num, "variable is not defined!" + ident_name);
        }
    }

    void koopa_ir_name(string ident_name, SymbolItem &item)
    {
        int koopa_ir_name_repetition = 0;
        for (size_t i = 0; i < Scope_1.size(); i++)
        {
            auto &table = Scope_1[i].symboltable;
            if (table.find(ident_name) != table.end())
            { //该名字在前面的作用域出现过
                koopa_ir_name_repetition++;
            }
        }

        item.koopa_ir_name = '@' + ident_name + to_string(koopa_ir_name_repetition);
    }

    kirinfo::kirinfo()
    {
        this->has_call = false;
        this->register_num = 0;
        this->stack_size = 0;
        this->stack_value_base = 0;
    }

    void kirinfo::reset_func_kir()
    { //刚进入函数的时候可以将ir信息恢复为初值
        this->has_call = false;
        this->register_num = 0;
        this->stack_size = 0;
        this->stack_value_base = 0;
    }

    std::string kirinfo::find_value_in_stack(std::ostream &outfile, koopa_raw_value_t value)
    {
        int temp;
        if (this->stack_value_base != 0)
        {
            temp = this->stack_map[value] + this->stack_value_base;
        }
        else
        {
            temp = this->stack_map[value];
        }
        
        if (temp > 2047)
        { //如果数值大于12位立即数
            string treg = "t" + std::to_string(this->register_num++);
            outfile << "  li\t" + treg + ", " + std::to_string(temp) + "\n";
            outfile << "  add\t" + treg + ", sp, " + treg + "\n";
            return "0(" + treg + ")";
        }
        else
        {
            return std::to_string(temp) + "(sp)";
        }
    }

    int kirinfo::find_value_in_stack_int(koopa_raw_value_t value)
    {
        if (this->stack_value_base != 0)
        { //如果有实参占用栈帧

            return this->stack_map[value] + this->stack_value_base;
        }
        else
        { // 如果没有实参占用栈帧
            return this->stack_map[value];
        }
    }

    // SymbolItem::SymbolItem(bool is_c, bool is_a, int size)
    // {
    //     is_const = is_c;
    //     is_array = is_a;
    //     result.reserve(size);
    // }

} // namespace symbol
