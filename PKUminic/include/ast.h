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

// 所有 AST 的基类
class BaseAST
{
public:
    int line_num;

    BaseAST(int line_num);
    virtual ~BaseAST() = default;
    virtual void Dump(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
};

// ==================== 表达式相关基类 ====================
class ExpAST
{
public:
    std::string symbolexp;
    int line_num;

    ExpAST(int line_num);
    virtual ~ExpAST() = default;
    virtual std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
    virtual int cal(symbol::astinfo &astinfo) const = 0;
};

class IdentifierAST;
class IdentArrayAST;

// lVal
class LeftValAST : public ExpAST
{
public:
    std::unique_ptr<IdentifierAST> ident;
    std::unique_ptr<IdentArrayAST> identarray;

    LeftValAST(std::unique_ptr<IdentifierAST> ident, std::unique_ptr<IdentArrayAST> identarray, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    int cal(symbol::astinfo &astinfo) const override; //求计算值
};

// IDENT类
class IdentifierAST : public ExpAST
{
public:
    int line_num;
    std::string ident_name;

    IdentifierAST(std::string ident_name, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    int cal(symbol::astinfo &astinfo) const override; //求变量值
};

// IdentArray 类
class IdentArrayAST : public ExpAST
{
public:
    std::string ident_name;
    std::vector<ExpAST *> shape_list;
    
    IdentArrayAST(std::string ident_name, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override; //用于查找数组中 index 的变量，计算出其地址但不 load
    void arrayalloc(std::string &inputstr, symbol::astinfo &astinfo);                 //用于声明使用
    int cal(symbol::astinfo &astinfo) const override;                                 //求 数组中 index 的变量值
};

// PrimaryExp类
class PrimaryExpAST : public ExpAST
{
public:
    int number;
    std::unique_ptr<ExpAST> exp;
    std::unique_ptr<LeftValAST> leftval;

    PrimaryExpAST(int number, unique_ptr<ExpAST> exp, unique_ptr<LeftValAST> leftval, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    int cal(symbol::astinfo &astinfo) const override; //求计算值
};

class FuncCallAST;
class FuncRParamsListAST;

// 单目运算类 AST
class UnaryExpAST : public ExpAST
{
public:
    std::unique_ptr<ExpAST> primaryexp; // 第一种形式primaryexp
    int unaryop;
    std::unique_ptr<ExpAST> rhs;           // 右值
    std::unique_ptr<FuncCallAST> funccall; // 函数调用

    UnaryExpAST(unique_ptr<ExpAST> primaryexp, int unaryop, unique_ptr<ExpAST> rhs,
                std::unique_ptr<FuncCallAST> funccall, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    int cal(symbol::astinfo &astinfo) const override; //求计算值
};

//函数调用
class FuncCallAST
{
public:
    int line_num;
    std::string ident_name;
    std::unique_ptr<FuncRParamsListAST> funcrparamslist;

    FuncCallAST(std::string ident_name, std::unique_ptr<FuncRParamsListAST> funcrparamslist, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo);
};

//函数实参列表
class FuncRParamsListAST
{
public:
    int line_num;
    std::vector<ExpAST *> rparamslist;

    FuncRParamsListAST(int line_num) : line_num(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo, std::vector<string> &rparamsymbol, std::string ident_name);
};

// 二元运算类 AST
class BinaryExpAST : public ExpAST
{
public:
    std::unique_ptr<ExpAST> firstformexp; // 第一种形式的exp
    int operators;
    std::unique_ptr<ExpAST> lhs;
    std::unique_ptr<ExpAST> rhs;

    BinaryExpAST(std::unique_ptr<ExpAST> l, int oper, std::unique_ptr<ExpAST> r, std::unique_ptr<ExpAST> first, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    int cal(symbol::astinfo &astinfo) const override; //求计算值
};

// 条件运算类 AST
class CondExpAST : public ExpAST
{
public:
    std::unique_ptr<ExpAST> firstformexp; // 第一种形式的exp
    int condoper;
    std::unique_ptr<ExpAST> lhs;
    std::unique_ptr<ExpAST> rhs;

    CondExpAST(std::unique_ptr<ExpAST> l, int oper, std::unique_ptr<ExpAST> r, std::unique_ptr<ExpAST> first, int line_num);
    std::string Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    std::string Short_Circuit_Eval(std::string &inputstr, symbol::astinfo &astinfo) const;
    int cal(symbol::astinfo &astinfo) const override;
};

//==================== 程序框架相关类 ====================
// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST
{
public:
    // 用智能指针管理对象
    // std::unique_ptr<BaseAST> func_def;
    std::vector<BaseAST *> compunitlist;

    CompUnitAST(int line_num) : BaseAST(line_num){};
    void init_koopair(string &inputstr, symbol::astinfo &astinfo) const;
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override; //利用dump遍历抽象语法树，生成string形式的KoopaIR
};

class FuncFParamsListAST;
class FuncFParamAST;
class FuncTypeAST;

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST
{
public:
    std::string func_type;
    std::string ident;
    std::unique_ptr<FuncFParamsListAST> funcfparamlist;
    std::unique_ptr<StmtAST> block;

    FuncDefAST(int line_num) : BaseAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

//函数参数列表类， fparamlist 包含了所有的参数
class FuncFParamsListAST : public BaseAST
{
public:
    std::vector<FuncFParamAST *> funcfparamlist;

    FuncFParamsListAST(int line_num) : BaseAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    void FuncFParamLISTAlloc(std::string &inputstr, symbol::astinfo &astinfo);
};

//函数参数表项的父类
class FuncFParamAST
{
public:
    int line_num;

    FuncFParamAST(int line_num) : line_num(line_num){};
    virtual ~FuncFParamAST() = default;
    virtual void Dump(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
    virtual void FuncFParamAlloc(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
};

// 变量 参数表项
class FuncFParamOneAST : public FuncFParamAST
{
public:
    std::string ident_name;

    FuncFParamOneAST(std::string ident_name, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    void FuncFParamAlloc(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 数组变量 参数参数表项
class FuncFParamArrayAST : public FuncFParamAST
{
public:
    std::string ident_name;
    std::vector<ExpAST *> shape_list; //数组除第一维外的长度

    FuncFParamArrayAST(std::string ident_name, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
    void FuncFParamAlloc(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// ====================  语句 相关基类 ====================
class StmtAST : public BaseAST
{
public:
    StmtAST(int line_num);
    virtual ~StmtAST() = default;
    virtual void Dump(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
};

// BlockAST 是block的基本单元,里面装着各种stmtast,包括stmt和decl语句
class BlockItemAST : public StmtAST
{
public:
    std::vector<StmtAST *> blockitemlist;
    BlockItemAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 返回语句
class ReturnStmtAST : public StmtAST
{
public:
    int number;
    std::unique_ptr<ExpAST> Exp;

    ReturnStmtAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

//赋值语句
class AssignStmtAST : public StmtAST
{
public:
    std::unique_ptr<LeftValAST> leftval;
    std::unique_ptr<ExpAST> Exp;

    AssignStmtAST(std::unique_ptr<LeftValAST> leftval, std::unique_ptr<ExpAST> Exp, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

class IfElseStmtAST : public StmtAST
{
public:
    std::unique_ptr<ExpAST> Exp;
    std::unique_ptr<StmtAST> stmt1;
    std::unique_ptr<StmtAST> stmt2;

    IfElseStmtAST(std::unique_ptr<ExpAST> Exp, std::unique_ptr<StmtAST> stmt1,
                  std::unique_ptr<StmtAST> stmt2, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

class WhileStmtAST : public StmtAST
{
public:
    std::unique_ptr<ExpAST> Exp;
    std::unique_ptr<StmtAST> stmt;

    WhileStmtAST(std::unique_ptr<ExpAST> Exp, std::unique_ptr<StmtAST> stmt, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// stmt语句中表达式语句，没有赋值
class ExpstmtAST : public StmtAST
{
public:
    std::unique_ptr<ExpAST> Exp;

    ExpstmtAST(std::unique_ptr<ExpAST> Exp, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// stmt语句中的break语句
class BreakstmtAST : public StmtAST
{
public:
    BreakstmtAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// stmt语句中的continue语句
class ContinuestmtAST : public StmtAST
{
public:
    ContinuestmtAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// stmt语句中的空语句
class VoidstmtAST : public StmtAST
{
public:
    VoidstmtAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 声明语句 语句的一种, define_list_存放各种define语句
class DeclareAST : public StmtAST
{
public:
    int btype;
    std::vector<DefineAST *> define_list_; //存放各种define语句

    DeclareAST(int line_num) : StmtAST(line_num){};
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// ==================== define语句相关基类 ====================
class DefineAST
{
public:
    int line_num;

    DefineAST(int line_num) : line_num(line_num){};
    virtual ~DefineAST() = default;
    virtual void Dump(std::string &inputstr, symbol::astinfo &astinfo) const = 0;
};

// 变量声明 AST
class DefOneAST : public DefineAST
{
public:
    std::string varname;

    DefOneAST(std::string varname, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 变量声明并赋值 AST
class DefOneInitAST : public DefineAST
{
public:
    bool is_const;
    std::string varnameinit;
    std::unique_ptr<ExpAST> expvalue;

    DefOneInitAST(std::string name, std::unique_ptr<ExpAST> exp, bool is_const, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

class InitValArrayAST;

// 数组变量声明 AST
class DefArray : public DefineAST
{
public:
    std::unique_ptr<IdentArrayAST> identarray;

    DefArray(std::unique_ptr<IdentArrayAST> identarray, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 数组变量声明并赋值 AST
class DefArrayInitAST : public DefineAST
{
public:
    bool is_const;

    std::unique_ptr<IdentArrayAST> identarray;
    std::unique_ptr<InitValArrayAST> initvalarray;

    DefArrayInitAST(std::unique_ptr<IdentArrayAST> identarray, std::unique_ptr<InitValArrayAST> initvalarray, bool is_const, int line_num);
    void Dump(std::string &inputstr, symbol::astinfo &astinfo) const override;
};

// 数组赋值部分 AST
// InitValArrayList 为一个 vector，其基本元素为 InitValArray。
// InitValArray 可以为 InitVal、InitValArray 完成数组嵌套赋值
class InitValArrayAST
{
public:
    int line_num;
    bool is_exp; //表示当前initvalarray是否是表达式
    std::unique_ptr<ExpAST> initval;
    std::vector<InitValArrayAST *> initvalarraylist; // 如果为表达式则此vector的size为0

    InitValArrayAST(bool is_exp, std::unique_ptr<ExpAST> initval, int line_num);
    void aggreate(std::string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item, int deep);          //使用aggregate初始化数组
    void cal(symbol::astinfo &astinfo, symbol::SymbolItem &item);                                                //递归遍历自身，将结果写入item.result中
    void init_store(std::string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item);                  //使用getelemptr 和 store 初始化数组
    void assign_arrayval(string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item, string storesrc); // 针对每个数值使用getelemptr 和 store初始化赋值
};

#endif