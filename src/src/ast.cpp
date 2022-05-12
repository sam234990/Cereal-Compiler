#include "../include/ast.h"
#include <iostream>
#include "sysy.tab.hpp"

using namespace std;

int symbolnum = 0;


// ==================== CompUnitAST  Class ====================
void CompUnitAST::Dump(string &inputstr) const {
    // inputstr = "Koopa IR: \n";
    func_def->Dump(inputstr);
    return;
}

// ==================== FuncDefAST  Class ====================
void FuncDefAST::Dump(string &inputstr) const {
    inputstr = inputstr + "fun @" + ident + "(): ";
    func_type->Dump(inputstr);
    inputstr.append("{\n");
    block->Dump(inputstr);
    inputstr.append("}");
    return;
}

// ==================== FuncTypeAST  Class ====================
void FuncTypeAST::Dump(string &inputstr) const {
    if(functype == "int") inputstr.append("i32 ");
    else inputstr.append(" ");
    return;
}

// ==================== BlockAST  Class ====================
void BlockAST::Dump(string &inputstr) const {
    inputstr.append("%entry: \n");
    stmt->Dump(inputstr); 
    inputstr.append("\n");
    return;
}

// ==================== StmtAST  Class ====================
void StmtAST::Dump(string &inputstr) const {
    string temp = Exp->Dump(inputstr);
    inputstr.append("  ret " + temp);
    return;
}


// ==================== PrimaryExpAST  Class ====================
std::string PrimaryExpAST::Dump(string &inputstr) const {
    if (exp == NULL)
    {//该PrimaryExp为Number 这里不用写入
        // inputstr.append(to_string(number));
        return to_string(number);
    }
    else{//该PrimaryExp为exp
        string temp = exp->Dump(inputstr);
        return temp;
    }
}


// ==================== UnaryExpAST  Class ====================
std::string UnaryExpAST::Dump(string &inputstr) const {
    if (primaryexp == NULL)
    {//该UnaryExp为第二种
        string symbol_rhs = rhs->Dump(inputstr);//获取右操作数symbol
        string temp;//本行symbol
        string expstr;//本行的命令
        switch (unaryop)
        {
        case ADD://ADD无操作，使用上一symbol
            return symbol_rhs;
            break;
        case SUB:
            temp = "%" + to_string(symbolnum++ );
            expstr = "  " + temp + " = sub 0, " + symbol_rhs + "\n";
            inputstr = inputstr.append(expstr);
            break;
        case NOT:
            temp = "%" + to_string(symbolnum++ );
            expstr = "  " + temp + " = eq " + symbol_rhs + ", 0\n";
            inputstr = inputstr.append(expstr);
            break;
        default:
            break;
        }
        
        return temp;
    }
    else{//该UnaryExp为PrimaryExp
        string temp = primaryexp->Dump(inputstr);
        return temp;
    }
}