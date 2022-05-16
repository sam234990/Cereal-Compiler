#pragma once
#ifndef AST_H
#define AST_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../include/symbol.h"

// 表达式的基类
class ExpAST;

// 所有定义语句的基类
class DefineAST;

// 所有语句的基类
class StmtAST;
class IdentifierAST;

// 所有 AST 的基类
class BaseAST
{
public:
    int line_num;
    virtual ~BaseAST() = default;
    virtual void Dump(std::string &inputstr) const = 0;
};


// ==================== 表达式相关基类 ====================
class ExpAST
{
public:
    std::string symbolexp;
    virtual ~ExpAST() = default;
    virtual std::string Dump(std::string &inputstr) const = 0;
    virtual int cal() const = 0;
};

// lVal
class LeftValAST : public ExpAST
{
public:
    std::unique_ptr<IdentifierAST> ident;
    std::string Dump(std::string &inputstr) const override;
    int cal() const override; //求计算值
};

// IDENT类
class IdentifierAST : public LeftValAST
{
public:
    int line_num;
    std::string ident_name;
    std::string Dump(std::string &inputstr) const override;
    int cal() const override; //求计算值
};

// PrimaryExp类
class PrimaryExpAST : public ExpAST
{
public:
    int number;
    std::unique_ptr<ExpAST> exp;
    std::unique_ptr<LeftValAST> leftval;
    std::string Dump(std::string &inputstr) const override;
    int cal() const override; //求计算值
};

// 单目运算类 AST
class UnaryExpAST : public ExpAST
{
public:
    std::unique_ptr<ExpAST> primaryexp; // 第一种形式primaryexp

    int unaryop;
    std::unique_ptr<ExpAST> rhs; // 右值
    std::string Dump(std::string &inputstr) const override;
    int cal() const override; //求计算值
};

// 二元运算类 AST
class BinaryExpAST : public ExpAST
{
public:
    BinaryExpAST(std::unique_ptr<ExpAST> l, int oper, std::unique_ptr<ExpAST> r);
    BinaryExpAST(std::unique_ptr<ExpAST> first);

    std::unique_ptr<ExpAST> firstformexp; // 第一种形式的exp
    int operators;
    std::unique_ptr<ExpAST> lhs;
    std::unique_ptr<ExpAST> rhs;
    std::string Dump(std::string &inputstr) const override;
    int cal() const override; //求计算值
};

// 条件运算类 AST
class CondExpAST : public ExpAST
{
public:
    CondExpAST(std::unique_ptr<ExpAST> first);
    CondExpAST(std::unique_ptr<ExpAST> l, int oper, std::unique_ptr<ExpAST> r);

    std::unique_ptr<ExpAST> firstformexp; // 第一种形式的exp
    int condoper;
    std::unique_ptr<ExpAST> lhs;
    std::unique_ptr<ExpAST> rhs;
    std::string Dump(std::string &inputstr) const override;
    int cal() const override;
};



// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST
{
public:
    // 用智能指针管理对象
    std::unique_ptr<BaseAST> func_def;
    void Dump(std::string &inputstr) const override; //利用dump遍历抽象语法树，生成string形式的KoopaIR
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST
{
public:
    std::unique_ptr<BaseAST> func_type;
    std::string ident;
    std::unique_ptr<StmtAST> block;
    void Dump(std::string &inputstr) const override;
};

// FuncTypeAST 也是 BaseAST
class FuncTypeAST : public BaseAST
{
public:
    std::string functype;
    void Dump(std::string &inputstr) const override;
};

// ====================  语句 相关基类 ====================
class StmtAST : public BaseAST
{
public:
    virtual ~StmtAST() = default;
    virtual void Dump(std::string &inputstr) const = 0;
};

// BlockAST 是block的基本单元,里面装着各种stmtast,包括stmt和decl语句
class BlockAST : public StmtAST
{
public:
    std::vector<StmtAST *> blockitemlist;
    void Dump(std::string &inputstr) const override;
};

// 返回语句
class ReturnStmtAST : public StmtAST
{
public:
    int number;
    std::unique_ptr<ExpAST> Exp;
    void Dump(std::string &inputstr) const override;
};

class AssignStmtAST : public StmtAST
{
public:
    int line_num;
    std::unique_ptr<LeftValAST> leftval;
    std::unique_ptr<ExpAST> Exp;
    AssignStmtAST(std::unique_ptr<LeftValAST> leftval, std::unique_ptr<ExpAST> Exp, int line_num);

    void Dump(std::string &inputstr) const override;
};

// 声明语句 语句的一种, define_list_存放各种define语句
class DeclareAST : public StmtAST
{
public:
    int btype;
    std::vector<DefineAST *> define_list_; //存放各种define语句
    void Dump(std::string &inputstr) const override;
};

// ==================== define语句相关基类 ====================
class DefineAST
{
public:
    virtual ~DefineAST() = default;
    virtual void Dump(std::string &inputstr) const = 0;
};

// 变量声明 AST
class DefOneAST : public DefineAST
{
public:
    int line_num;
    std::string varname;

    DefOneAST(std::string varname, int line_num);
    void Dump(std::string &inputstr) const override;
};

// 变量声明并赋值AST
class DefOneInitAST : public DefineAST
{
public:
    int line_num;
    bool is_const;
    std::string varnameinit;
    std::unique_ptr<ExpAST> expvalue;

    DefOneInitAST(std::string name, std::unique_ptr<ExpAST> exp, bool is_const, int line_num);
    void Dump(std::string &inputstr) const override;
};


#endif