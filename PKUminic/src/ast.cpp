#include "../include/ast.h"
#include <iostream>

using namespace std;


// ==================== CompUnitAST  Class ====================
string CompUnitAST::Dump() const{
    string retstr ;
    // retstr = "Koopa IR: \n";
    retstr = retstr + func_def->Dump();
    return retstr;
}

// ==================== FuncDefAST  Class ====================
string FuncDefAST::Dump() const {
    string retstr ;
    retstr = "fun @" + ident + "(): ";
    retstr = retstr + func_type->Dump();
    retstr = retstr + "{\n";
    retstr = retstr + block->Dump();
    retstr = retstr + "}";
    return retstr;
}

// ==================== FuncTypeAST  Class ====================
string FuncTypeAST::Dump() const {
    string retstr ;
    if(functype == "int") retstr = "i32 ";
    else retstr = " ";
    return retstr;
}

// ==================== BlockAST  Class ====================
string BlockAST::Dump() const {
    string retstr ;
    retstr = "%entry: \n" + stmt->Dump() + "\n";
    return retstr;
}

// ==================== StmtAST  Class ====================
string StmtAST::Dump() const {
    string retstr ;
    retstr = "  ret ";
    retstr.append(to_string(number));
    return retstr;
}
