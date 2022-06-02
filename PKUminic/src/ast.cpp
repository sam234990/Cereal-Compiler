#include "../include/ast.h"
#include "../include/symbol.h"
#include <iostream>
#include <memory>
#include "sysy.tab.hpp"

using namespace std;

// ==================== BaseAST  Class ====================
BaseAST::BaseAST(int line_num)
{
    this->line_num = line_num;
}

// ==================== ExpAST  Class ====================
ExpAST::ExpAST(int line_num)
{
    this->line_num = line_num;
}

// ==================== LeftValAST  Class ====================
LeftValAST::LeftValAST(unique_ptr<IdentifierAST> ident, unique_ptr<IdentArrayAST> identarray, int line_num) : ExpAST(line_num)
{
    if (ident == NULL)
    {
        this->ident = NULL;
    }
    else
    {
        this->ident = move(ident);
    }

    if (identarray == NULL)
    {
        this->identarray = NULL;
    }
    else
    {
        this->identarray = move(identarray);
    }
}

// ==================== IdentifierAST  Class ====================
IdentifierAST::IdentifierAST(string ident_name, int line_num) : ExpAST(line_num)
{
    this->ident_name = ident_name;
}

// ==================== IdentArrayAST  Class ====================
IdentArrayAST::IdentArrayAST(string ident_name, int line_num) : ExpAST(line_num)
{
    this->ident_name = ident_name;
}

// ==================== PrimaryExpAST  Class ====================
PrimaryExpAST::PrimaryExpAST(int number, unique_ptr<ExpAST> exp,
                             unique_ptr<LeftValAST> leftval, int line_num) : ExpAST(line_num)
{
    this->number = number;
    if (exp == NULL)
    {
        this->exp = NULL;
    }
    else
    {
        this->exp = move(exp);
    }

    if (leftval == NULL)
    {
        this->leftval = NULL;
    }
    else
    {
        this->leftval = move(leftval);
    }
}

// ==================== UnaryExpAST  Class ====================
UnaryExpAST::UnaryExpAST(unique_ptr<ExpAST> primaryexp, int unaryop, unique_ptr<ExpAST> rhs,
                         unique_ptr<FuncCallAST> funccall, int line_num) : ExpAST(line_num)
{
    this->unaryop = unaryop;

    if (primaryexp == NULL)
    {
        this->primaryexp = NULL;
    }
    else
    {
        this->primaryexp = move(primaryexp);
    }

    if (rhs == NULL)
    {
        this->rhs = NULL;
    }
    else
    {
        this->rhs = move(rhs);
    }

    if (funccall == NULL)
    {
        this->funccall = NULL;
    }
    else
    {
        this->funccall = move(funccall);
    }
}

// ==================== FuncCallAST  Class ====================
FuncCallAST::FuncCallAST(std::string ident_name, std::unique_ptr<FuncRParamsListAST> funcrparamslist, int line_num)
{
    this->ident_name = ident_name;
    this->line_num = line_num;
    if (funcrparamslist == NULL)
    {
        this->funcrparamslist = NULL;
    }
    else
    {
        this->funcrparamslist = move(funcrparamslist);
    }
}

// ==================== BinaryExpAST  Class ====================
BinaryExpAST::BinaryExpAST(unique_ptr<ExpAST> l, int oper, unique_ptr<ExpAST> r,
                           unique_ptr<ExpAST> first, int line_num) : ExpAST(line_num)
{
    this->operators = oper;
    if (first == NULL)
    {
        this->firstformexp = NULL; //表示该MultExp为第二种
    }
    else
    {
        this->firstformexp = move(first);
    }

    if (l == NULL)
    {
        this->lhs = NULL;
    }
    else
    {
        this->lhs = move(l);
    }

    if (r == NULL)
    {
        this->lhs = NULL; //表示该MultExp为UnaryExp
    }
    else
    {
        this->rhs = move(r);
    }
}

// ==================== CondExpAST  Class ====================
CondExpAST::CondExpAST(unique_ptr<ExpAST> l, int oper, unique_ptr<ExpAST> r,
                       unique_ptr<ExpAST> first, int line_num) : ExpAST(line_num)
{
    this->condoper = oper;
    if (first == NULL)
    {
        this->firstformexp = NULL; //表示该 CondExpAST 为第二种
    }
    else
    {
        this->firstformexp = move(first);
    }

    if (l == NULL)
    {
        this->lhs = NULL;
    }
    else
    {
        this->lhs = move(l);
    }

    if (r == NULL)
    {
        this->lhs = NULL; //表示该 CondExpAST 为第一种形式的exp
    }
    else
    {
        this->rhs = move(r);
    }
}

// ==================== FuncFParamAST  Class ====================
FuncFParamOneAST::FuncFParamOneAST(string ident_name, int line_num) : FuncFParamAST(line_num)
{
    this->ident_name = ident_name;
}

// ==================== FuncFParamArrayAST  Class ====================
FuncFParamArrayAST::FuncFParamArrayAST(string ident_name, int line_num) : FuncFParamAST(line_num)
{
    this->ident_name = ident_name;
}

// ==================== StmtAST  Class ====================
StmtAST::StmtAST(int line_num) : BaseAST(line_num) {}

// ==================== AssignStmtAST  Class ====================
AssignStmtAST::AssignStmtAST(std::unique_ptr<LeftValAST> leftval,
                             std::unique_ptr<ExpAST> Exp, int line_num) : StmtAST(line_num)
{
    this->Exp = move(Exp);
    this->leftval = move(leftval);
}

// ==================== IfElseStmtAST  Class ====================
IfElseStmtAST::IfElseStmtAST(unique_ptr<ExpAST> Exp, unique_ptr<StmtAST> stmt1,
                             unique_ptr<StmtAST> stmt2, int line_num) : StmtAST(line_num)
{
    this->Exp = move(Exp);
    this->stmt1 = move(stmt1);
    if (stmt2 == NULL)
    {
        this->stmt2 = NULL;
    }
    else
    {
        this->stmt2 = move(stmt2);
    }
}

// ==================== WhileStmtAST  Class ====================
WhileStmtAST::WhileStmtAST(unique_ptr<ExpAST> Exp, unique_ptr<StmtAST> stmt,
                           int line_num) : StmtAST(line_num)
{
    this->Exp = move(Exp);
    this->stmt = move(stmt);
}

// ==================== ExpstmtAST  Class ====================
ExpstmtAST::ExpstmtAST(std::unique_ptr<ExpAST> Exp, int line_num) : StmtAST(line_num)
{
    this->Exp = move(Exp);
}

// ==================== DefOneAST  Class ====================
DefOneAST::DefOneAST(std::string varname, int line_num) : DefineAST(line_num)
{
    this->varname = varname;
}

// ==================== DefOneInitAST  Class ====================
DefOneInitAST::DefOneInitAST(string name, unique_ptr<ExpAST> exp,
                             bool is_const, int line_num) : DefineAST(line_num)
{
    this->is_const = is_const;
    this->expvalue = move(exp);
    this->varnameinit = name;
}

// ==================== DefArray  Class ====================
DefArray::DefArray(unique_ptr<IdentArrayAST> identarray, int line_num) : DefineAST(line_num)
{
    this->identarray = move(identarray);
}

// ==================== DefArrayInitAST  Class ====================
DefArrayInitAST::DefArrayInitAST(unique_ptr<IdentArrayAST> identarray, unique_ptr<InitValArrayAST> initvalarray,
                                 bool is_const, int line_num) : DefineAST(line_num)
{
    this->is_const = is_const;
    this->identarray = move(identarray);
    this->initvalarray = move(initvalarray);
}

// ==================== InitValArrayAST  Class ====================
InitValArrayAST::InitValArrayAST(bool is_exp, unique_ptr<ExpAST> initval, int line_num)
{
    this->is_exp = is_exp;
    this->line_num = line_num;
    if (initval == NULL)
    {
        this->initval = NULL;
    }
    else
    {
        this->initval = move(initval);
    }
}
