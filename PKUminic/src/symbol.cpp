#include "../include/symbol.h"
#include <memory>
#include <iostream>

namespace symbol
{

Scope *Scope_1 = new Scope(); //定义全局作用域

// 符号表项
SymbolItem::SymbolItem(bool is_c, bool is_a)
{
    is_const = is_c;
    is_array = is_a;
    result.reserve(5);
}

void SemanticError(int line_no, const std::string &&error_msg)
{
    std::cerr << "语义错误 at line " << line_no << " : " << error_msg << std::endl;
    exit(255);
}

// SymbolItem::SymbolItem(bool is_c, bool is_a, int size)
// {
//     is_const = is_c;
//     is_array = is_a;
//     result.reserve(size);
// }

} // namespace symbol
