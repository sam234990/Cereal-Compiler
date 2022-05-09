#include "../include/ast.h"
#include <iostream>

// ==================== CompUnitAST  Class ====================
void CompUnitAST::Dump() const{
    std::cout << "CompUnitAST { ";
    func_def->Dump();
    std::cout << " }";
}

// ==================== FuncDefAST  Class ====================
void FuncDefAST::Dump() const {
    std::cout << "FuncDefAST { ";
    func_type->Dump();
    std::cout << ", " << ident << ", ";
    block->Dump();
    std::cout << " }";
}

// ==================== FuncTypeAST  Class ====================
void FuncTypeAST::Dump() const {
    std::cout << "FuncTypeAST { ";
    std::cout << "int ";
    std::cout << " }";
}

// ==================== BlockAST  Class ====================
void BlockAST::Dump() const {
    std::cout << "BlockAST { ";
    stmt->Dump();
    std::cout << " }";
}

// ==================== StmtAST  Class ====================
void StmtAST::Dump() const {
    std::cout << "StmtAST { ";
    std::cout <<" "<< number <<" ";
    std::cout << " }";
}
