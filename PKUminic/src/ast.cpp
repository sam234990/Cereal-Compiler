#include "../include/ast.h"
#include "../include/symbol.h"
#include <iostream>
#include <memory>
#include "sysy.tab.hpp"

using namespace std;

AssignStmtAST::AssignStmtAST(std::unique_ptr<LeftValAST> leftval, std::unique_ptr<ExpAST> Exp, int line_num)
{
    this->Exp = move(Exp);
    this->line_num = line_num;
    this->leftval = move(leftval);
}

// ==================== DefOneAST  Class ====================
DefOneAST::DefOneAST(std::string varname, int line_num)
{
    this->varname = varname;
    this->line_num = line_num;
    // 实现同一作用域下变量重名判断
    auto &symbol_table = symbol::Scope_1->symboltable;
    const auto &var_iter = symbol_table.find(varname);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(false, false);
        symbol_table.insert({varname, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable" + varname + "define repetition");
    }
}

// ==================== DefOneInitAST  Class ====================
DefOneInitAST::DefOneInitAST(string name, unique_ptr<ExpAST> exp, bool is_const, int line_num)
{
    this->is_const = is_const;
    this->line_num = line_num;
    this->expvalue = move(exp);
    this->varnameinit = name;
    // 实现同一作用域下变量重名判断
    auto &symbol_table = symbol::Scope_1->symboltable;
    const auto &var_iter = symbol_table.find(varnameinit);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        if (is_const)
        { //如果是const类型,则将is_const设置为true
            auto temp = new symbol::SymbolItem(true, false);
            symbol_table.insert({varnameinit, *temp}); // 对于constdefone赋值直接添加到符号表中.
        }
        else
        {
            auto temp = new symbol::SymbolItem(false, false);
            symbol_table.insert({varnameinit, *temp}); // 对于constdefone赋值直接添加到符号表中.
        }
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable" + varnameinit + "define repetition");
    }
}

// ==================== BinaryExpAST  Class ====================
BinaryExpAST::BinaryExpAST(unique_ptr<ExpAST> first)
{
    firstformexp = move(first);
    rhs = NULL; //表示该MultExp为UnaryExp
    lhs = NULL; //表示该MultExp为UnaryExp
}

BinaryExpAST::BinaryExpAST(unique_ptr<ExpAST> l, int oper, unique_ptr<ExpAST> r)
{
    firstformexp = NULL; //表示该MultExp为第二种
    lhs = move(l);
    operators = oper;
    rhs = move(r);
}

// ==================== CondExpAST  Class ====================
CondExpAST::CondExpAST(unique_ptr<ExpAST> first)
{
    firstformexp = move(first);
    rhs = NULL; //表示该MultExp为UnaryExp
    lhs = NULL; //表示该MultExp为UnaryExp
}

CondExpAST::CondExpAST(unique_ptr<ExpAST> l, int oper, unique_ptr<ExpAST> r)
{
    firstformexp = NULL; //表示该MultExp为第二种
    lhs = move(l);
    condoper = oper;
    rhs = move(r);
}
