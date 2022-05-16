#pragma once
#ifndef __SYMBOL__H_
#define __SYMBOL__H_

#include <iostream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace symbol
{
    // 符号表中的一项
class SymbolItem
{
public:
    bool is_const;//是否是const 类型
    bool is_array;
    vector<int> result;//如果是constdefone应该有结果。

    SymbolItem(){};
    SymbolItem(bool is_c, bool is_a);
    // SymbolItem(bool is_c, bool is_a, int size);
    ~SymbolItem() = default;
};


// 作用域
class Scope
{
public:
    unordered_map<string, SymbolItem> symboltable;
    int scope_id;
    int parent_scope_id; // 父作用域id

    Scope()
    {
        scope_id = 1;
        parent_scope_id = 0;
    };
    ~Scope() = default;
};



extern Scope* Scope_1 ;

void SemanticError(int line_no, const std::string &&error_msg);


} // namespace symbol



#endif