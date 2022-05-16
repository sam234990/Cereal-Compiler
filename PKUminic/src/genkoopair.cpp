#include "../include/ast.h"
#include "../include/symbol.h"
#include <iostream>
#include "sysy.tab.hpp"

using namespace std;

int symbolnum = 0;

// ==================== CompUnitAST  Class ====================
void CompUnitAST::Dump(string &inputstr) const
{
    // inputstr = "Koopa IR: \n";
    func_def->Dump(inputstr);
    return;
}

// ==================== FuncDefAST  Class ====================
void FuncDefAST::Dump(string &inputstr) const
{
    inputstr = inputstr + "fun @" + ident + "(): ";
    func_type->Dump(inputstr);
    inputstr.append("{\n");
    block->Dump(inputstr);
    inputstr.append("}");
    return;
}

// ==================== FuncTypeAST  Class ====================
void FuncTypeAST::Dump(string &inputstr) const
{
    if (functype == "int")
        inputstr.append("i32 ");
    else
        inputstr.append(" ");
    return;
}

// ==================== BlockAST  Class ====================
void BlockAST::Dump(string &inputstr) const
{
    if (blockitemlist.size() == 0)
    { //如果里面为空直接返回
        return;
    }

    inputstr.append("%entry: \n");
    // cout << "-------blockitemlist.size():" << blockitemlist.size() << endl;
    // stmt->Dump(inputstr);
    for (int i = 0; i < blockitemlist.size(); i++)
    { //遍历每一个stmt
        blockitemlist[i]->Dump(inputstr);
    }

    inputstr.append("\n");
    return;
}

// ==================== ReturnStmtAST  Class ====================
void ReturnStmtAST::Dump(string &inputstr) const
{
    string temp = Exp->Dump(inputstr);

    inputstr.append("  ret " + temp);
    return;
}

void AssignStmtAST::Dump(string &inputstr) const
{
    //获取表达式右边symbol
    string symbol_rhs = this->Exp->Dump(inputstr);
    // this->leftval->Dump(inputstr);
    string symbollval = this->leftval;

    inputstr.append("  store " + symbol_rhs + ", @" + this->varnameinit + '\n');
    

    return;
}

void DeclareAST::Dump(string &inputstr) const
{
    if (define_list_.size() == 0)
    { //如果里面为空直接返回
        return;
    }
    cout << define_list_.size() << endl;
    for (int i = 0; i < define_list_.size(); i++)
    { //遍历每一个define语句
        define_list_[i]->Dump(inputstr);
    }
    return;
}

// ==================== DefOneAST  Class ====================
void DefOneAST::Dump(string &inputstr) const
{ //仅声明变量,用alloc语句申请内存
    inputstr.append("  @" + this->varname + " = alloc i32\n");
    auto &item = symbol::Scope_1->symboltable[this->varname];
    item.koopa_ir_name = '@' + this->varname;
    return;
}

// ==================== DefOneInitAST  Class ====================
void DefOneInitAST::Dump(string &inputstr) const
{
    if (this->is_const)
    { //遍历到 const 时,应该计算右值的结果,并将结果写入到对应的符号表中
        int result = expvalue->cal();
        auto &item = symbol::Scope_1->symboltable[this->varnameinit];
        item.result.push_back(result);
        // SymbolItem
        cout << "name" << endl;
        return;
    }
    else
    { //非const时,
        //先用alloc语句申请内存
        inputstr.append("  @" + this->varnameinit + " = alloc i32\n");
        auto &item = symbol::Scope_1->symboltable[this->varnameinit];
        item.koopa_ir_name = '@' + this->varnameinit;
        //后用store存储起来。
        string symbol_rhs = expvalue->Dump(inputstr);
        inputstr.append("  store " + symbol_rhs + ", @" + this->varnameinit + '\n');
        return;
    }
}

// ==================== LeftValAST  Class ====================
string LeftValAST::Dump(string &inputstr) const
{
    return ident->Dump(inputstr);
}

int LeftValAST::cal() const
{
    return ident->cal();
}

// ==================== IdentifierAST  Class ====================
string IdentifierAST::Dump(string &inputstr) const
{
    //判断符号表中是否有该变量
    auto &symbol_table = symbol::Scope_1->symboltable;
    const auto &var_iter = symbol_table.find(ident_name);
    if (var_iter == symbol_table.end())
    { //如果不存在报错
        symbol::SemanticError(line_num, "variable is not defined!");
    }
    auto &item = symbol::Scope_1->symboltable[ident_name];
    if (item.is_const)
    { //如果是const常量
        return to_string(item.result.back());
    }
    else
    {                  // 非const类型, load出来并返回
        string temp;   //本行symbol
        string expstr; //本行的命令
        string symbol_rhs;
        symbol_rhs = item.koopa_ir_name;
        temp = "%" + to_string(symbolnum++);
        expstr = "  " + temp + " = load " + symbol_rhs + "\n";
        inputstr.append(expstr);
        return temp;
    }
}

int IdentifierAST::cal() const
{
    int number;
    auto &item = symbol::Scope_1->symboltable[ident_name];
    number = item.result.back();
    cout << number << endl;
    return number;
}

// 表达式相关中间代码生成
// ==================== PrimaryExpAST  Class ====================
string PrimaryExpAST::Dump(string &inputstr) const
{
    if (exp != NULL)
    { //该PrimaryExp为exp
        string temp = exp->Dump(inputstr);
        return temp;
    }
    else if (leftval != NULL)
    { //该PrimaryExp为lval
        return leftval->Dump(inputstr);
    }
    else
    { //该PrimaryExp为Number 这里不用写入
        // inputstr.append(to_string(number));
        return to_string(number);
    }
}

int PrimaryExpAST::cal() const
{
    if (exp != NULL)
    { //该PrimaryExp为exp
        return exp->cal();
    }
    else if (leftval != NULL)
    { //该PrimaryExp为lval
        return leftval->cal();
    }
    else
    { //该PrimaryExp为Number
        return number;
    }
}

// ==================== UnaryExpAST  Class ====================
string UnaryExpAST::Dump(string &inputstr) const
{
    if (primaryexp == NULL)
    {                                            //该UnaryExp为第二种
        string symbol_rhs = rhs->Dump(inputstr); //获取右操作数symbol
        string temp;                             //本行symbol
        string expstr;                           //本行的命令
        switch (unaryop)
        {
        case ADD: // ADD无操作，使用上一symbol
            return symbol_rhs;
            break;
        case SUB:
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = sub 0, " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case NOT:
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = eq " + symbol_rhs + ", 0\n";
            inputstr.append(expstr);
            break;
        default:
            break;
        }
        return temp;
    }
    else
    { //该UnaryExp为PrimaryExp
        return primaryexp->Dump(inputstr);
        ;
    }
}

int UnaryExpAST::cal() const
{
    if (primaryexp == NULL)
    {                             //该UnaryExp为第二种
        int num_rhs = rhs->cal(); //获取右操作数数值
        switch (unaryop)
        {
        case ADD: // ADD无操作，使用上一数值
            return num_rhs;
        case SUB:
            return -num_rhs;
        case NOT:
            cout << "error: operation \"!\" is impremissible in const " << endl;
            return 0;
        default:
            break;
        }
        return 0;
    }
    else
    { //该UnaryExp为PrimaryExp
        return primaryexp->cal();
    }
}

// ==================== BinaryExpAST  Class ====================
string BinaryExpAST::Dump(string &inputstr) const
{
    if (firstformexp == NULL)
    {                                            //该MultExpAST为第二种
        string symbol_lhs = lhs->Dump(inputstr); //获取左操作数symbol
        string symbol_rhs = rhs->Dump(inputstr); //获取右操作数symbol
        string temp;                             //本行symbol
        string expstr;                           //本行的命令
        switch (operators)
        {
        case ADD: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = add " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case SUB:
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = sub " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case MULT: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = mul " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case DIV:
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = div " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case MOD:
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = mod " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        default:
            break;
        }
        return temp;
    }
    else
    { //该BinaryExpAST为firstformexp
        return firstformexp->Dump(inputstr);
        ;
    }
}

int BinaryExpAST::cal() const
{
    if (firstformexp == NULL)
    {                             //该MultExpAST为第二种
        int num_lhs = lhs->cal(); //获取左操作数数值
        int num_rhs = rhs->cal(); //获取右操作数数值
        switch (operators)
        {
        case ADD: //
            return num_lhs + num_rhs;
        case SUB:
            return num_lhs - num_rhs;
        case MULT: //
            return num_lhs * num_rhs;
        case DIV:
            return num_lhs / num_rhs;
        case MOD:
            return num_lhs % num_rhs;
        default:
            cout << "error: operation \"!\" is impremissible in const " << endl;
            return 0;
        }
    }
    else
    { //该BinaryExpAST为firstformexp
        return firstformexp->cal();
    }
}

// ==================== CondExpAST  Class ====================
string CondExpAST::Dump(string &inputstr) const
{
    if (firstformexp == NULL)
    {                                            //该MultExpAST为第二种
        string symbol_lhs = lhs->Dump(inputstr); //获取左操作数symbol
        string symbol_rhs = rhs->Dump(inputstr); //获取右操作数symbol
        string temp, temp_1, temp_2;             //本行symbol
        string expstr;                           //本行的命令
        switch (condoper)
        {
        case AND:                                  //
            temp_1 = "%" + to_string(symbolnum++); //判断左是否不等于0
            expstr = "  " + temp_1 + " = ne 0, " + symbol_lhs + '\n';
            temp_2 = "%" + to_string(symbolnum++); //判断右是否不等于0
            expstr = expstr + "  " + temp_2 + " = ne 0, " + symbol_rhs + '\n';
            temp = "%" + to_string(symbolnum++); // 与运算获得结果
            expstr = expstr + "  " + temp + " = and " + temp_1 + ", " + temp_2 + "\n";
            inputstr.append(expstr);
            break;
        case OR: //
            temp_1 = "%" + to_string(symbolnum++);
            expstr = "  " + temp_1 + " = or " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = ne 0, " + temp_1 + "\n"; // 与0比较获得结果
            inputstr.append(expstr);
            break;
        case NE: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = ne " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case EQ: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = eq " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case GT: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = gt " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case LT: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = lt " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case GE: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = ge " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case LE: //
            temp = "%" + to_string(symbolnum++);
            expstr = "  " + temp + " = le " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        default:
            break;
        }
        return temp;
    }
    else
    { //该BinaryExpAST为firstformexp
        return firstformexp->Dump(inputstr);
        ;
    }
}

int CondExpAST::cal() const
{
    cout << "cal conditon expression should not be used in this state" << endl;
    return 0;
}