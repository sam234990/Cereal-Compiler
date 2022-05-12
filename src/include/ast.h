#pragma once
#ifndef AST_H
#define AST_H


#include <iostream>
#include <memory>
#include <string>

// 表达式的基类
class ExpAst;

// 所有 AST 的基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;
        virtual void Dump(std::string &inputstr) const = 0;

};

// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
        std::unique_ptr<BaseAST> func_def;
        void Dump(std::string &inputstr) const override;//利用dump遍历抽象语法树，生成string形式的KoopaIR
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;
        void Dump(std::string &inputstr) const override;
};

// FuncTypeAST 也是 BaseAST
class FuncTypeAST : public BaseAST{
    public:
        std::string functype;
        void Dump(std::string &inputstr) const override;
};

// BlockAST 也是 BaseAST
class BlockAST : public BaseAST{
    public:
        std::unique_ptr<BaseAST> stmt;
        void Dump(std::string &inputstr) const override;
};

// StmtAST 也是 BaseAST
class StmtAST : public BaseAST{
    public:
        int number;
        std::unique_ptr<ExpAst> Exp;
        void Dump(std::string &inputstr) const override;
};

class ExpAst{
    public:
        std::string symbolexp;
        virtual ~ExpAst() = default;
        virtual std::string Dump(std::string &inputstr) const = 0;
};

class PrimaryExpAST : public ExpAst{
    public:
        int number;
        std::unique_ptr<ExpAst> exp;
        std::string Dump(std::string &inputstr) const override;
};

class UnaryExpAST : public ExpAst{
    public:
        int unaryop; 
        std::unique_ptr<ExpAst> primaryexp;
        std::unique_ptr<ExpAst> rhs;
        std::string Dump(std::string &inputstr) const override;
};

#endif