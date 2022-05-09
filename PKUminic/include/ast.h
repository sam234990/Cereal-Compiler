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
        virtual void Dump() const = 0;
};

// CompUnit 是 BaseAST
class CompUnitAST : public BaseAST {
    public:
    // 用智能指针管理对象
        std::unique_ptr<BaseAST> func_def;
        void Dump() const override;
};

// FuncDef 也是 BaseAST
class FuncDefAST : public BaseAST {
    public:
        std::unique_ptr<BaseAST> func_type;
        std::string ident;
        std::unique_ptr<BaseAST> block;
        void Dump() const override;
};

// FuncTypeAST 也是 BaseAST
class FuncTypeAST : public BaseAST{
    public:
        std::string functype;
        void Dump() const override;
};

// BlockAST 也是 BaseAST
class BlockAST : public BaseAST{
    public:
        std::unique_ptr<BaseAST> stmt;
        void Dump() const override;
};

// StmtAST 也是 BaseAST
class StmtAST : public BaseAST{
    public:
        int number;
        void Dump() const override;
};

#endif