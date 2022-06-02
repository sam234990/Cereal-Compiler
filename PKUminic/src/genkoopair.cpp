#include "../include/ast.h"
#include "../include/symbol.h"
#include <iostream>
#include "sysy.tab.hpp"

using namespace std;

// ==================== CompUnitAST  Class ====================
void CompUnitAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // inputstr = "Koopa IR: \n";
    // astinfo.symbolnum = 0;
    // astinfo.if_num = 0;

    // 1.  程序初始化
    this->init_koopair(inputstr, astinfo);
    // 2. 建立全局作用域
    int scope_id = symbol::Scope_1.size(); //获取当前scope的vector长度，作为当前作用域的id
    if (scope_id != 0)
    {
        std::cerr << "程序错误" << std::endl;
    }
    else
    {
        auto bscope = new symbol::Scope(scope_id, -1); //全局作用域的父作用域为 -1
        astinfo.ast_scope_id = scope_id;               //修改当前ast scope为新建的id
        symbol::Scope_1.push_back(*bscope);
    }

    //遍历每一个func 和 decl
    cout << "compunitlist.size()---" << this->compunitlist.size() << endl;
    for (int i = 0; i < this->compunitlist.size(); i++)
    {
        astinfo.ast_scope_id = 0; //处于全局作用域中
        this->compunitlist[i]->Dump(inputstr, astinfo);
        inputstr.append("\n");
    }

    // func_def->Dump(inputstr, astinfo);

    return;
}

void CompUnitAST::init_koopair(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 将函数定义加入到koopair的最前端
    inputstr.append("decl @getint(): i32\n");
    inputstr.append("decl @getch(): i32\n");
    inputstr.append("decl @starttime()\n");
    inputstr.append("decl @stoptime()\n");

    inputstr.append("decl @putint(i32)\n");
    inputstr.append("decl @putch(i32)\n");
    
    inputstr.append("decl @getarray(*i32): i32\n");
    inputstr.append("decl @putarray(i32, *i32)\n\n");

    // 2. 把系统库中的函数添加到函数表中
    symbol::gFuncTable.insert({"getint", {"int"}});
    symbol::gFuncTable.insert({"getch", {"int"}});
    symbol::gFuncTable.insert({"starttime", {"void"}});
    symbol::gFuncTable.insert({"stoptime", {"void"}});

    auto temp_func_0 = new symbol::FuncTableItem("void");
    vector<int> temp;
    temp.push_back(0);
    temp_func_0->shape_list_.push_back(temp);
    symbol::gFuncTable.insert({"putint", *temp_func_0});
    symbol::gFuncTable.insert({"putch", *temp_func_0});

    auto temp_func_1 = new symbol::FuncTableItem("int");
    vector<int> temp_1;
    temp_1.push_back(-1); //表第一维指针的
    temp_func_1->shape_list_.push_back(temp_1);
    symbol::gFuncTable.insert({"getarray", *temp_func_1});

    auto temp_func_2 = new symbol::FuncTableItem("void");
    vector<int> temp_2, temp_3;
    temp_2.push_back(0);  //表数值
    temp_3.push_back(-1); //表第一维指针的
    temp_func_2->shape_list_.push_back(temp_2);
    temp_func_2->shape_list_.push_back(temp_3);
    symbol::gFuncTable.insert({"putarray", *temp_func_2});

    symbol::gFuncTable.insert({"putf", {"void"}});
}

// ==================== FuncDefAST  Class ====================
void FuncDefAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    astinfo.ast_scope_id = 0;
    astinfo.has_return = false; //刚进入当前函数没有返回
    astinfo.symbolnum = 0;      //每个函数体中的数字可以重建

    //  1.对函数名的重名判断
    // 注意SysY语言中函数名可以与变量名重名
    auto &symbol_table = symbol::gFuncTable;
    const auto &var_iter = symbol_table.find(this->ident);
    if (var_iter == symbol_table.end())
    { // 如果为空,则添加到函数表中

        auto temp_func = new symbol::FuncTableItem(this->func_type);
        astinfo.func_type = this->func_type;
        symbol::gFuncTable.insert({this->ident, *temp_func}); //添加到函数表中
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "function :" + this->ident + "define repetition");
    }

    inputstr = inputstr + "fun @" + this->ident + "(";
    astinfo.func_name = this->ident;
    //有形参表,没有则直接跳过
    if (this->funcfparamlist != NULL)
    {
        this->funcfparamlist->Dump(inputstr, astinfo);
    }

    inputstr.append(")");

    if (this->func_type == "int")
    {
        inputstr.append(": i32 ");
    }
    else
    {
        inputstr.append(" ");
    }
    inputstr.append("{\n");

    // 2. 进入函数部分
    inputstr.append("%entry: \n"); //表示函数的进入部分
    // 2.1 有形参表, 对形参表每个变量alloc，没有则直接跳过
    if (this->funcfparamlist != NULL)
    {
        this->funcfparamlist->FuncFParamLISTAlloc(inputstr, astinfo);
    }
    // 3. 遍历函数下每一个block
    block->Dump(inputstr, astinfo);

    if (!astinfo.has_return)
    { //如果当前函数中没写返回语句，需补充
        if (this->func_type == "void")
        {
            inputstr.append("  ret\n");
        }
        else
        {
            inputstr.append("  ret 0\n");
        }
    }
    astinfo.has_return = false;
    inputstr.append("}\n");

    // 4. 当前函数结束，由于函数没有嵌套，因此当前作用域为全局变量，作用域为0
    astinfo.ast_scope_id = 0;
    return;
}

// ==================== FuncFParamsListAST  Class ====================
void FuncFParamsListAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    int scope_id = symbol::Scope_1.size(); //获取当前scope的vector长度，作为当前作用域的id
    // 1. 函数的形参表作用域的父作用域恒为全局变量作用域
    //  因此形参表的父作用域id恒为0
    auto bscope = new symbol::Scope(scope_id, 0);
    astinfo.ast_scope_id = scope_id; //修改当前ast scope为新建的id
    symbol::Scope_1.push_back(*bscope);

    for (int i = 0; i < this->funcfparamlist.size(); i++)
    { //遍历每一个形参
        this->funcfparamlist[i]->Dump(inputstr, astinfo);
        if (i != (this->funcfparamlist.size() - 1))
        {
            inputstr.append(", ");
        }
    }

    return;
}

void FuncFParamsListAST::FuncFParamLISTAlloc(string &inputstr, symbol::astinfo &astinfo)
{
    for (int i = 0; i < this->funcfparamlist.size(); i++)
    { //遍历每一个形参，alloc
        this->funcfparamlist[i]->FuncFParamAlloc(inputstr, astinfo);
    }

    return;
}

// ==================== FuncFParamOneAST  Class ====================
void FuncFParamOneAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 语义分析
    //加入名称并判断
    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    const auto &var_iter = symbol_table.find(this->ident_name);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(false, false);
        symbol_table.insert({this->ident_name, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "Function paramater : " + this->ident_name + "define repetition");
    }

    vector<int> paramsize;
    paramsize.push_back(0);
    auto &functableitem = symbol::gFuncTable[astinfo.func_name]; // 形参所在函数的函数表项
    functableitem.shape_list_.push_back(paramsize);

    //形参的在koopa ir中的变量名进行alloc时保存的变量名
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->ident_name];
    symbol::koopa_ir_name(this->ident_name, item);
    item.is_funcFparam = true; //是函数形参

    // 在koopa ir中的函数声明中使用 %和名称来命名
    // 以此来区分alloc分配的kir变量名和形参中的变量。
    inputstr.append('%' + this->ident_name + " : i32");
    return;
}

void FuncFParamOneAST::FuncFParamAlloc(string &inputstr, symbol::astinfo &astinfo) const
{
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->ident_name];
    string temp = item.koopa_ir_name;
    inputstr.append("  " + temp + " = alloc i32\n");
    //在koopa ir中的函数声明中使用 %和名称来命名
    inputstr.append("  store %" + this->ident_name + ", " + temp + '\n');
    return;
}

// ==================== FuncFParamArrayAST  Class ====================
void FuncFParamArrayAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{
    // cout << "in FuncFParamArrayAST Dump() " << endl;
    // 1. 语义分析
    //加入名称并判断
    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    const auto &var_iter = symbol_table.find(this->ident_name);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(false, false);
        symbol_table.insert({this->ident_name, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "Function paramater : " + this->ident_name + "define repetition");
    }

    //形参的在koopa ir中的变量名进行alloc时保存的变量名
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->ident_name];
    item.dimension_len = this->shape_list.size();
    item.is_funcFparam = true; //是函数形参
    item.is_array = true;
    symbol::koopa_ir_name(this->ident_name, item);

    string arrayfparamtype = "*";
    string temp = "i32";

    vector<int> paramsize;
    paramsize.push_back(-1); //表第一维指针的

    if (this->shape_list.empty())
    { //如果当前 Exp列表为空
        item.dimension_len = 0;
    }
    else
    {
        //循环获得数组各维的维度 除第一维以外
        item.dimension_len = this->shape_list.size();
        for (int i = 0; i < this->shape_list.size(); i++)
        {
            arrayfparamtype.append("[");
            int num = this->shape_list[i]->cal(astinfo);
            item.dimension_size.push_back(num); //将数组位数加入符号表中
            paramsize.push_back(num);
            temp.append(", " + to_string(num) + "]");
        }
    }

    arrayfparamtype.append(temp);
    // 在 ir 中的 函数声明中 使用  %名称
    // 以此来区分alloc分配的koopair变量名和形参中的变量。
    inputstr.append('%' + this->ident_name + " : " + arrayfparamtype);

    auto &functableitem = symbol::gFuncTable[astinfo.func_name]; // 形参所在函数的函数表项
    functableitem.shape_list_.push_back(paramsize);

    return;
}

void FuncFParamArrayAST::FuncFParamAlloc(std::string &inputstr, symbol::astinfo &astinfo) const
{
    // TODO FINISH
    //为当前 数组形参 在当前函数中分配空间
    // cout << "in FuncFParamArrayAST FuncFParamAlloc() " << endl;
    string arrayfparamtype = "*";
    string temp = "i32";
    if (!this->shape_list.empty())
    {
        for (int i = 0; i < this->shape_list.size(); i++)
        {
            arrayfparamtype.append("[");
            int num = this->shape_list[i]->cal(astinfo);
            temp.append(", " + to_string(num) + "]");
        }
    }
    arrayfparamtype.append(temp);

    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->ident_name];
    string item_name = item.koopa_ir_name;
    inputstr.append("  " + item_name + " = alloc " + arrayfparamtype + "\n");
    //在koopa ir中的函数声明中使用 %和名称来命名
    inputstr.append("  store %" + this->ident_name + ", " + item_name + '\n');
}

// ==================== BlockAST  Class ====================
void BlockItemAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 新建作用域，并将其用于后面的ast遍历
    int scope_id = symbol::Scope_1.size(); //获取当前scope的vector长度，作为当前作用域的id
    int parent_id = astinfo.ast_scope_id;  //当前ast作为现有的block父scopeid

    // // 1. 没有func形参表，当前block为函数后第一个block
    // if (scope_id == 0 || astinfo.ast_scope_id == 0)
    // {
    //     parent_id = 0; // 父id作用域为-1表示到根，其父为全局作用域
    // }
    // else
    // {
    //     // 2. 有func形参表，当前block为func形参表后第一个block
    //     parent_id = astinfo.ast_scope_id;
    // }

    auto bscope = new symbol::Scope(scope_id, parent_id);

    astinfo.ast_scope_id = scope_id; //修改当前ast scope为新建的id
    symbol::Scope_1.push_back(*bscope);
    // cout << "当前作用域id" << astinfo.ast_scope_id << endl;
    // 2. 遍历当前作用域下所有的指令
    if (blockitemlist.size() == 0)
    { //如果里面为空直接返回
        return;
    }
    // cout << "-------blockitemlist.size():" << blockitemlist.size() << endl;
    for (int i = 0; i < blockitemlist.size(); i++)
    { //遍历每一个stmt
        if (astinfo.has_return)
        { //当前block中前面的语句中已经有返回语句了
            // cout << "遍历到return 后面不翻译" << endl;
            break; //后面的部分直接忽略
        }
        blockitemlist[i]->Dump(inputstr, astinfo);
    }

    // 3. 修改ast作用域 原来的作用域
    astinfo.ast_scope_id = parent_id;
    return;
}

// ==================== ReturnStmtAST  Class ====================
void ReturnStmtAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{

    if (this->Exp != NULL)
    {
        if (astinfo.func_type != "int")
        { //当前函数有返回值报错
            symbol::SemanticError(line_num, "'void' function returning a value");
        }

        string temp = Exp->Dump(inputstr, astinfo);
        inputstr.append("  ret " + temp + "\n");
    }
    else
    {
        if (astinfo.func_type == "int")
        { //当前函数有返回值报错
            symbol::SemanticError(line_num, "'void' function returning a value");
        }
        inputstr.append("  ret\n");
    }
    astinfo.has_return = true; //遍历到return语句
    return;
}

// ==================== AssignStmtAST  Class ====================
void AssignStmtAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    //获取表达式右边symbol
    string symbol_rhs = this->Exp->Dump(inputstr, astinfo);

    //  从当前作用域->根作用域查找
    string ident_name;
    symbol::SymbolItem item;
    string symbol_l;
    if (this->leftval->ident != NULL)
    {
        ident_name = this->leftval->ident->ident_name;
        symbol::finditem(astinfo.ast_scope_id, ident_name, line_num, item);
        symbol_l = item.koopa_ir_name;
    }
    else
    { //计算出index指向的地址
        symbol_l = this->leftval->identarray->Dump(inputstr, astinfo);
    }

    inputstr.append("  store " + symbol_rhs + ", " + symbol_l + '\n');

    return;
}

// ==================== IfElseStmtAST  Class ====================
void IfElseStmtAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 计算当前是第几个if else，多个ifelse时可以为symbol命名
    int if_num = astinfo.if_num++;

    // 2. 获取条件表达式的symbol
    // inputstr.append("// if 的 条件判断部分\n");

    string symbol_cond_result = this->Exp->Dump(inputstr, astinfo);

    string symbol_then, symbol_else, symbol_end;
    string if_br;
    symbol_then = "%then_" + to_string(if_num);
    symbol_end = "%end_" + to_string(if_num);

    // 3. 判断是否有else语句，
    if (this->stmt2 == NULL)
    { // 没有else语句
        if_br = "  br " + symbol_cond_result + ", " + symbol_then + ", " + symbol_end + "\n\n";
    }
    else
    { // 有else语句
        symbol_else = "%else_" + to_string(if_num);
        if_br = "  br " + symbol_cond_result + ", " + symbol_then + ", " + symbol_else + "\n\n";
    }

    inputstr.append(if_br);
    // inputstr.append("// if 的 if分支\n");
    inputstr.append(symbol_then + ": \n");
    bool parent_return = astinfo.has_return;
    astinfo.has_return = false; //设置当前分支下面没有返回语句
    // if分支部分
    this->stmt1->Dump(inputstr, astinfo);
    if (!astinfo.has_return)
    { //如果分支部分中没有出现了ret指令则用跳转指令。
      //跳转至结束部分
        inputstr.append("  jump " + symbol_end + "\n\n");
    }
    astinfo.has_return = parent_return; //恢复原来状态

    if (this->stmt2 != NULL)
    { // 有else语句
        // inputstr.append("//if 语句的 else 分支\n");
        inputstr.append(symbol_else + ": \n");
        bool parent_return = astinfo.has_return;
        astinfo.has_return = false; //设置当前分支下面没有返回语句
        // if 的else部分
        this->stmt2->Dump(inputstr, astinfo);
        if (!astinfo.has_return)
        { //如果分支部分中没有出现了ret指令则用跳转指令。
          //跳转至结束部分
            inputstr.append("  jump " + symbol_end + "\n\n");
        }
        astinfo.has_return = parent_return; //恢复原来状态
    }
    // 4. 结束部分
    // inputstr.append("// if 语句之后的内容, if/else 分支的交汇处\n");
    inputstr.append(symbol_end + ": \n");
    return;
}

// ==================== WhileStmtAST  Class ====================
void WhileStmtAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 计算当前是第几个while，多个while时可以为symbol命名
    int while_num = astinfo.while_num++;

    string symbol_cond, symbol_body, symbol_end;
    symbol_cond = "%while_cond_" + to_string(while_num); // while语句的条件部分
    symbol_body = "%while_body_" + to_string(while_num); // while语句的循环体部分
    symbol_end = "%while_end_" + to_string(while_num);   // while语句的结束部分
    // 2. 先跳转至 cond条件判断部分
    inputstr.append("  jump " + symbol_cond + "\n\n");

    // 3. cond条件判断部分 条件判断后分支跳转至 body循环体（真） end结束（假）
    inputstr.append(symbol_cond + ": \n");
    string symbol_cond_result = this->Exp->Dump(inputstr, astinfo);
    inputstr.append("  br " + symbol_cond_result + ", " + symbol_body + ", " + symbol_end + "\n\n");

    // 4. body循环体部分 循环体结束后跳转至条件判断部分
    inputstr.append(symbol_body + ": \n");
    // 4.1. while循环体内 全局变量初始设置
    bool parent_return = astinfo.has_return;
    astinfo.has_return = false; // 初始设置为 没有return语句
    bool parent_iter = astinfo.is_iteration;
    astinfo.is_iteration = true; // 表示接下来的stmt部分处于循环体中

    int parent_while_num = astinfo.cur_while_num;
    astinfo.cur_while_num = while_num; //表示当前的while号

    // 4.2. while循环体内语句访问
    this->stmt->Dump(inputstr, astinfo);
    if (!astinfo.has_return) // if 中的 return已经被忽略置为false
    {                        // 如果循环体中部分不有return语句, 该部分需要jump至cond条件判断部分
        inputstr.append("  jump " + symbol_cond + "\n\n");
    }

    // 4.3. 全局变量恢复
    astinfo.has_return = parent_return;       //恢复原来的状态
    astinfo.is_iteration = parent_iter;       //恢复原来的状态
    astinfo.cur_while_num = parent_while_num; //返回原来while号

    // 5. end结束部分
    inputstr.append(symbol_end + ": \n");
    return;
}

// ==================== ExpstmtAST  Class ====================
void ExpstmtAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{ //直接调用该 Exp 的dump指令
    this->Exp->Dump(inputstr, astinfo);
}

// ==================== BreakstmtAST  Class ====================
void BreakstmtAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 语义判断
    if (!astinfo.is_iteration)
    { //如果当前语句不处于循环体中
        /* code */
        symbol::SemanticError(line_num, "The current statement \"break\" is not in the loop! ");
    }
    int break_num = astinfo.break_num++;
    string break_end = "%break_end_" + to_string(break_num);             // break后面的部分，若break后面还有语句，正常翻译
    string while_end = "%while_end_" + to_string(astinfo.cur_while_num); // while语句的结束部分
    inputstr.append("  jump " + while_end + "\n\n");
    inputstr.append(break_end + ":\n");

    return;
}

// ==================== ContinuestmtAST  Class ====================
void ContinuestmtAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 语义判断
    if (!astinfo.is_iteration)
    { //如果当前语句不处于循环体中
        symbol::SemanticError(line_num, "The current statement \"continue\" is not in the loop! ");
    }
    int continue_num = astinfo.continue_num++;
    string continue_end = "%continue_end_" + to_string(continue_num);      // continue 后面的部分，若 continue 后面还有语句，正常翻译
    string while_cond = "%while_cond_" + to_string(astinfo.cur_while_num); // while语句的进入 条件判断cond部分
    inputstr.append("  jump " + while_cond + "\n\n");
    inputstr.append(continue_end + ":\n");

    return;
}

// ==================== VoidstmtAST  Class ====================
void VoidstmtAST::Dump(std::string &inputstr, symbol::astinfo &astinfo) const
{ //空语句无操作
    return;
}

// ==================== DeclareAST  Class ====================
void DeclareAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    if (define_list_.size() == 0)
    { //如果里面为空直接返回
        return;
    }
    for (int i = 0; i < define_list_.size(); i++)
    { //遍历每一个define语句
        define_list_[i]->Dump(inputstr, astinfo);
    }
    return;
}

// ==================== DefOneAST  Class ====================
void DefOneAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 语义分析
    //加入名称并判断

    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    const auto &var_iter = symbol_table.find(varname);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(false, false);
        symbol_table.insert({varname, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable" + varname + "define repetition");
    }

    //设置其在koopa ir中的名称
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->varname];
    symbol::koopa_ir_name(this->varname, item);

    if (astinfo.ast_scope_id == 0)
    { //当前作用域为全局作用域
        inputstr.append("global " + item.koopa_ir_name + " = alloc i32, zeroinit\n");
    }
    else
    {
        // 仅声明变量, 用alloc语句申请内存
        inputstr.append("  " + item.koopa_ir_name + " = alloc i32\n");
    }

    return;
}

// ==================== DefOneInitAST  Class ====================
void DefOneInitAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 语义分析
    //  实现同一作用域下变量重名判断
    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    auto var_iter = symbol_table.find(this->varnameinit);

    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        if (this->is_const)
        { //如果是const类型,则将is_const设置为true
            auto temp = new symbol::SymbolItem(true, false);
            symbol_table.insert({varnameinit, *temp}); // 对于constdefone赋值直接添加到符号表中.
        }
        else
        {
            auto temp = new symbol::SymbolItem(false, false);
            symbol_table.insert({varnameinit, *temp}); // 对于constdefone赋值直接添加到符号表中.
        }
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable ---" + varnameinit + " --- define repetition");
    }

    if (this->is_const)
    { //遍历到 const 时,应该计算右值的结果,并将结果写入到对应的符号表中
        int result = expvalue->cal(astinfo);
        auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->varnameinit];
        item.result.push_back(result);

        // 表示当前变量为 单一数值型
        item.dimension_len = 0;
        // SymbolItem
        return;
    }
    else
    { //非const时

        auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->varnameinit];
        //设置koopair中的名字
        symbol::koopa_ir_name(this->varnameinit, item);

        // 1. 如果是全局变量，右侧表达式为常量表达式，可以计算
        if (astinfo.ast_scope_id == 0)
        {
            //计算右值的结果,并将结果写入到对应的符号表中
            int result = expvalue->cal(astinfo);
            item.result.push_back(result);
            // 表示当前变量为 单一数值型
            item.dimension_len = 0;
            // 使用全局声明方式并赋初值
            inputstr.append("global " + item.koopa_ir_name + " = alloc i32, " + to_string(result) + "\n");
        }
        else
        { // 2. 非全局变量情况

            //先用alloc语句申请内存
            inputstr.append("  " + item.koopa_ir_name + " = alloc i32\n");
            //后用store存储起来。
            string symbol_rhs = expvalue->Dump(inputstr, astinfo);
            inputstr.append("  store " + symbol_rhs + ", " + item.koopa_ir_name + '\n');
        }

        return;
    }
}

// ==================== DefArray  Class ====================
void DefArray::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    //此处仅声明数组，无const可能

    // 1. 语义分析
    //加入名称并判断
    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    const auto &var_iter = symbol_table.find(this->identarray->ident_name);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(false, true);
        symbol_table.insert({this->identarray->ident_name, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable" + this->identarray->ident_name + "define repetition");
    }

    //设置其在koopa ir中的名称
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->identarray->ident_name];
    symbol::koopa_ir_name(this->identarray->ident_name, item);

    if (astinfo.ast_scope_id == 0)
    { //当前作用域为全局作用域
        inputstr.append("global ");
        this->identarray->arrayalloc(inputstr, astinfo);
        // 未显示初始化的全局变量初始值为0
        inputstr.append(", zeroinit\n");
    }
    else
    {
        // 仅声明变量, 用alloc语句申请内存
        inputstr.append("  ");
        this->identarray->arrayalloc(inputstr, astinfo);
        inputstr.append("\n");
    }
    return;
}

// ==================== DefArrayInitAST  Class ====================
void DefArrayInitAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{

    // 1. 语义分析
    //加入名称并判断
    auto &symbol_table = symbol::Scope_1[astinfo.ast_scope_id].symboltable;
    const auto &var_iter = symbol_table.find(this->identarray->ident_name);
    if (var_iter == symbol_table.end())
    { //如果为空,则添加到符号表中
        auto temp = new symbol::SymbolItem(this->is_const, true);
        symbol_table.insert({this->identarray->ident_name, *temp}); // 对于constdefone赋值直接添加到符号表中.
    }
    else
    { //重名报错
        symbol::SemanticError(line_num, "variable" + this->identarray->ident_name + "define repetition");
    }

    // 2. 设置其在koopa ir中的名称
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->identarray->ident_name];
    symbol::koopa_ir_name(this->identarray->ident_name, item);

    astinfo.reset_array_info(); //清空数组相关的设置

    // 3. 分类讨论
    // 3.1 全局变量无论const与否均用aggregate赋初值，未知补0
    if (astinfo.ast_scope_id == 0)
    { // 当前作用域为全局作用域
        inputstr.append("global ");
        this->identarray->arrayalloc(inputstr, astinfo);
        inputstr.append(", ");
        // a. 递归遍历自身cal，将所有结果写入 item.result 未显示初始化的全局变量初始值为 0
        this->initvalarray->cal(astinfo, item);

        // b. 根据 item.dimension_size 递归写入 inputstr 中
        astinfo.array_offset = 0;
        this->initvalarray->aggreate(inputstr, astinfo, item, 0);
        inputstr.append("\n");
    }
    else
    {
        // 3.2 const 用 store 赋初值，未知补0
        if (this->is_const)
        { //如果是const类型
            inputstr.append("  ");
            this->identarray->arrayalloc(inputstr, astinfo);
            // 使用 init_store 写入 inpustr ，使用 cal 方法来获得每一个 constinitval 值
            inputstr.append("\n");
            this->initvalarray->init_store(inputstr, astinfo, item);
        }
        else
        { //如果不是const类型
            // 3.3 局部非const 用 store赋初值，未知为未知
            inputstr.append("  ");
            this->identarray->arrayalloc(inputstr, astinfo);

            // 使用 init_store 写入 inpustr ，使用 dump 方法来获得每一个 initval 值
            inputstr.append("\n");
            this->initvalarray->init_store(inputstr, astinfo, item);
        }
    }

    return;
}

// ==================== InitValArrayAST  Class ====================
void InitValArrayAST::aggreate(string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item, int deep)
{ //全局情况下通过递归调用 aggreate 将 rusult的结果写入 inpustr 中， 全局变量中所有表达式必为常量
    int size = item.dimension_size[deep];
    cout << deep << " " << item.dimension_len << " " << size << endl;
    if (deep == (item.dimension_len - 1))
    { //到达最深层，赋值
        //如果全是0 尝试用zerinit初始化
        bool flag = true;
        for (int i = astinfo.array_offset; i < astinfo.array_offset + size; i++)
        {
            if (item.result[i] != 0)
                flag = false;
        }
        if (flag)
        { //如果全是0
            astinfo.array_offset = astinfo.array_offset + size;
            inputstr.append("zeroinit");
            return;
        }

        inputstr.append("{");
        for (int i = 0; i < size; i++)
        {
            inputstr.append(to_string(item.result[astinfo.array_offset++])); //添加数字
            if (i != (size - 1))
            { //非最后添加 “，”
                inputstr.append(", ");
            }
        }
        inputstr.append("}");
    }
    else
    { //未到达最深层
        inputstr.append("{");
        for (int i = 0; i < size; i++)
        {
            //循环遍历自身
            this->aggreate(inputstr, astinfo, item, deep + 1);
            if (i != (size - 1))
            { //非最后添加 “，”
                inputstr.append(", ");
            }
        }
        inputstr.append("}");
    }
}

void InitValArrayAST::cal(symbol::astinfo &astinfo, symbol::SymbolItem &item)
{ //全局情况下通过递归调用 cal 将 初始值 的结果写入 rusult 中， 全局变量中所有表达式必为常量
    // 1. 通过括号和offset计算当前括号下 完整数组元素格式
    int supply_size;                                           //当前括号下完整元素个数 作为当前补充完整的结束offset
    int brace_size = astinfo.dim_total_num[astinfo.brace_num]; //通过 括号个数 brace_num 判断的元素格式
    if (astinfo.array_offset == 0)
    { //如果当前 offset 为0，使用括号判断结果
        supply_size = brace_size;
    }
    else
    { //通过offset判断
        int offset_size = 0;
        for (int s = 0; s < astinfo.dim_total_num.size(); s++)
        {
            if ((astinfo.array_offset % astinfo.dim_total_num[s]) == 0)
            { // 在 dim_total_num 中从大到小分别取余计算
                offset_size = astinfo.dim_total_num[s];
                break;
            }
        }
        supply_size = (offset_size < brace_size) ? offset_size : brace_size;
        supply_size = astinfo.array_offset + supply_size; //作为当前补充完整的结束offset
    }

    // 2. 遍历自身列表

    int i = 0;
    // 过多的数据未加括号
    for (i = 0; i < this->initvalarraylist.size(); i++)
    {
        auto tempinitval = this->initvalarraylist[i];
        // 2.1 该项为表达式
        if (tempinitval->is_exp)
        {
            astinfo.array_offset++;                       //数组偏移量+1
            int res = tempinitval->initval->cal(astinfo); //计算结果
            item.result.push_back(res);
        }
        else
        { // 2.2 该项为一个 大括号 项
            // 2.2.1. 通过offset 判断前面的表达式项是否足够填满一个数组
            bool temp_flag = false;
            for (int s = 0; s < astinfo.dim_total_num.size(); s++)
            {
                if ((astinfo.array_offset % astinfo.dim_total_num[s]) == 0)
                { // 在 dim_total_num 中从大到小分别取余计算
                    temp_flag = true;
                }
            }
            if (!temp_flag)
            { //如果没填满， 则报错
                symbol::SemanticError(this->line_num, "Current array initialization error");
            }
            astinfo.brace_num++;             // 大括号计数
            tempinitval->cal(astinfo, item); //递归调用自身
            astinfo.brace_num--;             // 大括号计数恢复
        }
    }
    // 不足部分补0
    for (; astinfo.array_offset < supply_size; astinfo.array_offset++)
    { //剩余未声明部分补0
        item.result.push_back(0);
    }
    if (astinfo.array_offset > astinfo.dim_total_num[0])
    { //如果当前offset 比 整个数组空间还大报错
        symbol::SemanticError(this->line_num, "Current array initialization error");
    }
    // cout << supply_size << endl;
    return;
}

void InitValArrayAST::init_store(string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item)
{ //全局情况下通过递归调用 init_store 获得每个初值，使用 getelemptr 和 store 初始化数组
    // 1. 通过括号和offset计算当前括号下 完整数组元素格式
    int supply_size;                                           //当前括号下完整元素个数
    int brace_size = astinfo.dim_total_num[astinfo.brace_num]; //通过 括号个数 brace_num 判断的元素格式
    if (astinfo.array_offset == 0)
    { //如果当前 offset 为 0，使用括号判断结果
        supply_size = brace_size;
    }
    else
    { //通过offset判断
        int offset_size = 0;
        for (int s = 0; s < astinfo.dim_total_num.size(); s++)
        {
            if ((astinfo.array_offset % astinfo.dim_total_num[s]) == 0)
            { // 在 dim_total_num 中从大到小分别取余计算
                offset_size = astinfo.dim_total_num[s];
                break;
            }
        }
        supply_size = (offset_size < brace_size) ? offset_size : brace_size;
        supply_size = supply_size + astinfo.array_offset; //作为当前补充完整的结束offset
    }
    // 2. 遍历自身列表
    int i = 0;
    string t, arrayindex;
    for (i = 0; i < this->initvalarraylist.size(); i++)
    {
        auto tempinitval = this->initvalarraylist[i];
        if (tempinitval->is_exp)
        { // 2.1 该项为表达式
            if (item.is_const)
            { //如果是const项
                int res = tempinitval->initval->cal(astinfo);
                item.result.push_back(res);
                t = to_string(res);
            }
            else
            { //非const项
                t = tempinitval->initval->Dump(inputstr, astinfo);
            }
            this->assign_arrayval(inputstr, astinfo, item, t); //赋值

            // arrayindex = "%" + to_string(astinfo.symbolnum++); //为当前的地址分配一个变量
            // // 使用 getelemptr 获得当前的index在数组中的地址 arrayindex
            // inputstr.append("  " + arrayindex + " = getelemptr " + item.koopa_ir_name + ", " + to_string(astinfo.array_offset - 1) + "\n");
            // inputstr.append("  store " + t + ", " + arrayindex + "\n"); // 使用 store 将 t 存入 arrayindex 中

            astinfo.array_offset++; //数组偏移量+1
        }
        else
        { // 2.2 该项为一个 大括号 项
            // 2.2.1. 通过offset 判断前面的表达式项是否足够填满一个数组
            bool temp_flag = false;
            for (int s = 0; s < astinfo.dim_total_num.size(); s++)
            {
                if ((astinfo.array_offset % astinfo.dim_total_num[s]) == 0)
                { // 在 dim_total_num 中从大到小分别取余计算
                    temp_flag = true;
                }
            }
            if (!temp_flag)
            { //如果没填满， 则报错
                symbol::SemanticError(this->line_num, "Current array initialization error");
            }
            astinfo.brace_num++;                              // 大括号计数
            tempinitval->init_store(inputstr, astinfo, item); //递归调用自身
            astinfo.brace_num--;                              // 大括号计数恢复
        }
    }

    for (; astinfo.array_offset < supply_size; astinfo.array_offset++)
    { // 为了过测试用例，无论const与否 剩余未声明部分补0
        item.result.push_back(0);
        t = to_string(0);
        this->assign_arrayval(inputstr, astinfo, item, t); //赋值
    }

    // if (item.is_const) // testcast中所有均需要补0，如测试8，使用上面注释的函数
    // {                  // const 部分补0
    //     for (; astinfo.array_offset < supply_size; astinfo.array_offset++)
    //     { //剩余未声明部分补0
    //         item.result.push_back(0);
    //         t = to_string(0);
    //         this->assign_arrayval(inputstr, astinfo, item, t); //赋值
    //     }
    // }
    // else
    // { //非const部分剩余数组加满
    //     astinfo.array_offset = supply_size;
    // }

    if (astinfo.array_offset > astinfo.dim_total_num[0])
    { //如果当前offset 比 整个数组空间还大报错
        symbol::SemanticError(this->line_num, "Current array initialization error");
    }

    return;
}

void InitValArrayAST::assign_arrayval(string &inputstr, symbol::astinfo &astinfo, symbol::SymbolItem &item, string storesrc)
{
    // 1. 根据offset计算各维度index
    vector<int> tempindex;
    int temp_offset = astinfo.array_offset;
    // eg: a[2][3][4] offset = 17 -> 111
    for (int i = 1; i < astinfo.dim_total_num.size(); i++)
    {                                                        // dim_total_num a[2][3][4] -> 24,12,4 从12开始计算
        int temp = (temp_offset / astinfo.dim_total_num[i]); // 17/12=1 5/4=1
        tempindex.push_back(temp);                           // 1 1
        temp_offset -= temp * astinfo.dim_total_num[i];      // 17-1*12=5 5-1*4=1
    }
    tempindex.push_back(temp_offset); // 1

    string base_ptr, curindex, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针
    base_ptr = item.koopa_ir_name;         //初始时为在base中的名称
    for (int i = 0; i < item.dimension_len; i++)
    {
        curindex = to_string(tempindex[i]);
        arrayindex = "%" + to_string(astinfo.symbolnum++);

        inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", " + curindex + "\n");
        base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
    }
    inputstr.append("  store " + storesrc + ", " + arrayindex + "\n"); // 使用 store 将 t 存入 arrayindex 中
}

// ==================== LeftValAST  Class ====================
void IdentArrayAST::arrayalloc(string &inputstr, symbol::astinfo &astinfo)
{ // array部分的声明操作
    // 语义重名判断已经做过
    // 1. 获取当前作用域下的符号表项
    auto &item = symbol::Scope_1[astinfo.ast_scope_id].symboltable[this->ident_name];

    // 2. alloc ident = alloc [[[i32,
    inputstr.append(item.koopa_ir_name + " = alloc ");
    for (int i = 0; i < shape_list.size(); i++)
    { //添加维数个的左方括号
        inputstr.append("[");
    }
    inputstr.append("i32, ");
    // 将数组的维数信息加入符号表中
    item.dimension_len = shape_list.size();

    int dimension_info = 1; //数组各维的总个数
    // astinfo.dim_total_num.insert(astinfo.dim_total_num.begin(), dimension_info);

    // 3. 添加维数  3], 2], 1]
    for (auto it = this->shape_list.rbegin(); it != this->shape_list.rend(); it++)
    { //反向迭代
        int temp = (*it)->cal(astinfo);
        inputstr.append(to_string(temp));

        dimension_info *= temp;
        // dim_total_num; a[2][3][4] -> 24,12,4,1 同时添加到全局和当前item中
        astinfo.dim_total_num.insert(astinfo.dim_total_num.begin(), dimension_info);
        // item.dim_total_num.insert(item.dim_total_num.begin(), dimension_info);
        if (it != (this->shape_list.rend() - 1))
        {
            inputstr.append("], ");
        }
        else
        {
            inputstr.append("]");
        }

        // 将数组每一维的深度信息加入符号表中
        item.dimension_size.insert(item.dimension_size.begin(), temp);
    }

    return;
}