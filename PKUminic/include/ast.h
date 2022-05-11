#pragma once
#ifndef AST_H
#define AST_H


#include <iostream>
#include <memory>
#include <string>

// 所有 AST 的基类
class BaseAST {
    public:
        virtual ~BaseAST() = default;
        virtual std::string Dump() const = 0;
};

// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
        std::unique_ptr<BaseAST> func_def;
        std::string Dump() const override;//利用dump遍历抽象语法树，生成string形式的KoopaIR
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;
        std::string Dump() const override;
};

// FuncTypeAST 也是 BaseAST
class FuncTypeAST : public BaseAST{
    public:
        std::string functype;
        std::string Dump() const override;
};

// BlockAST 也是 BaseAST
class BlockAST : public BaseAST{
    public:
        std::unique_ptr<BaseAST> stmt;
        std::string Dump() const override;
};

// StmtAST 也是 BaseAST
class StmtAST : public BaseAST{
    public:
        int number;
        std::string Dump() const override;
};

#endif