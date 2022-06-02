#include "../include/ast.h"
#include "../include/symbol.h"
#include <iostream>
#include "sysy.tab.hpp"

using namespace std;

// 表达式相关中间代码生成
// ==================== PrimaryExpAST  Class ====================
string PrimaryExpAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    if (exp != NULL)
    { //该PrimaryExp为exp
        string temp = exp->Dump(inputstr, astinfo);
        return temp;
    }
    else if (leftval != NULL)
    { //该PrimaryExp为lval
        return leftval->Dump(inputstr, astinfo);
    }
    else
    { //该PrimaryExp为Number 这里不用写入
        if (astinfo.funcRpara_size.empty())
        { //当 实参表达式 中出现了数值则表示为数值型
            astinfo.funcRpara_size.push_back(0);
        }
        return to_string(number);
    }
}

// ==================== UnaryExpAST  Class ====================
string UnaryExpAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{

    if (this->rhs != NULL)
    {                                                     //该UnaryExp为第二种
        string symbol_rhs = rhs->Dump(inputstr, astinfo); //获取右操作数symbol
        string temp;                                      //本行symbol
        string expstr;                                    //本行的命令
        switch (unaryop)
        {
        case ADD: // ADD无操作，使用上一symbol
            return symbol_rhs;
            break;
        case SUB:
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = sub 0, " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case NOT:
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = eq " + symbol_rhs + ", 0\n";
            inputstr.append(expstr);
            break;
        default:
            break;
        }
        return temp;
    }
    else if (this->primaryexp != NULL)
    { //该UnaryExp为PrimaryExp
        return primaryexp->Dump(inputstr, astinfo);
    }
    else
    { //该 UnaryExp为 FuncCall
        return this->funccall->Dump(inputstr, astinfo);
    }
}

// ==================== FuncCallAST  Class ====================
string FuncCallAST::Dump(string &inputstr, symbol::astinfo &astinfo)
{

    // 1. 在函数表中查找该函数名判断是否存在
    auto &symbol_table = symbol::gFuncTable;
    const auto &var_iter = symbol_table.find(this->ident_name);
    if (var_iter == symbol_table.end())
    { //如果未找到，则报错
        symbol::SemanticError(line_num, "function :" + this->ident_name + " not defined!");
    }

    // 2. 获取每一个实参数
    vector<string> rparamsymbol;
    if (this->funcrparamslist != NULL)
    { //如果有实参表则获取
        this->funcrparamslist->Dump(inputstr, astinfo, rparamsymbol, this->ident_name);
        // 2.1 判断实参表和函数形参表个数是否相同
        if (this->funcrparamslist->rparamslist.size() != var_iter->second.shape_list_.size())
        {
            symbol::SemanticError(line_num, "function call :" + this->ident_name + "\'s parameters is incorrect");
        }
    }

    // 3. 使用call调用
    string temp;
    string instr;

    if (var_iter->second.functype == "int")
    { //如果函数类型是int型则有返回值
        temp = "%" + to_string(astinfo.symbolnum++);
        instr = "  " + temp + " = call ";
    }
    else
    { //没有返回值
        temp = "";
        instr = "  call ";
    }
    instr = instr + "@" + this->ident_name + "(";
    for (int i = 0; i < rparamsymbol.size(); i++)
    {
        instr = instr + rparamsymbol[i];
        if (i != (rparamsymbol.size() - 1))
        {
            instr = instr + ", ";
        }
    }
    instr = instr + ")\n";
    inputstr.append(instr);

    return temp;
}

// ==================== FuncRParamsListAST  Class ====================
void FuncRParamsListAST::Dump(string &inputstr, symbol::astinfo &astinfo, vector<string> &rparamsymbol, string ident_name)
{
    const auto &functable = symbol::gFuncTable[ident_name];
    // 1. 获取计算每一个实参
    for (int i = 0; i < this->rparamslist.size(); i++)
    {
        auto item_size = functable.shape_list_[i];
        astinfo.funcRpara_size.clear(); //当前实参维数信息清空
        string temp = this->rparamslist[i]->Dump(inputstr, astinfo);
        rparamsymbol.push_back(temp);

        if (item_size.size() != astinfo.funcRpara_size.size())
        {
            cout << item_size.size() << " " << astinfo.funcRpara_size.size() << endl;
            symbol::SemanticError(line_num, "function parameters is incorrect" + temp);
        }
        else
        {
            for (int j = 0; j < item_size.size(); j++)
            {
                if (item_size[j] != astinfo.funcRpara_size[j])
                {
                    cout << item_size[j] << " " << astinfo.funcRpara_size[j] << endl;
                    symbol::SemanticError(line_num, to_string(i) + "function parameters is incorrect in size" + temp);
                }
            }
        }
    }
    astinfo.funcRpara_size.clear(); //当前实参维数信息清空
    astinfo.funcRpara_size.push_back(0);//当前函数调用作为实参传递
    return;
}

// ==================== BinaryExpAST  Class ====================
string BinaryExpAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    if (firstformexp == NULL)
    {                                                     //该MultExpAST为第二种
        string symbol_lhs = lhs->Dump(inputstr, astinfo); //获取左操作数symbol
        string symbol_rhs = rhs->Dump(inputstr, astinfo); //获取右操作数symbol
        string temp;                                      //本行symbol
        string expstr;                                    //本行的命令
        switch (operators)
        {
        case ADD: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = add " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case SUB:
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = sub " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case MULT: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = mul " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case DIV:
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = div " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case MOD:
            temp = "%" + to_string(astinfo.symbolnum++);
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
        return firstformexp->Dump(inputstr, astinfo);
        ;
    }
}

// ==================== CondExpAST  Class ====================
string CondExpAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    if (firstformexp == NULL)
    { //该 CondExpAST 为第二种

        if (this->condoper == AND || this->condoper == OR)
        { //进行短路求值
            return this->Short_Circuit_Eval(inputstr, astinfo);
        }

        string symbol_lhs = lhs->Dump(inputstr, astinfo); //获取左操作数symbol
        string symbol_rhs = rhs->Dump(inputstr, astinfo); //获取右操作数symbol
        string temp, temp_1, temp_2;                      //本行symbol
        string expstr;                                    //本行的命令
        switch (condoper)
        {
        case AND:                                          // 进行短路求值时不进行如下操作
            temp_1 = "%" + to_string(astinfo.symbolnum++); //判断左是否不等于0
            expstr = "  " + temp_1 + " = ne 0, " + symbol_lhs + '\n';
            temp_2 = "%" + to_string(astinfo.symbolnum++); //判断右是否不等于0
            expstr = expstr + "  " + temp_2 + " = ne 0, " + symbol_rhs + '\n';
            temp = "%" + to_string(astinfo.symbolnum++); // 与运算获得结果
            expstr = expstr + "  " + temp + " = and " + temp_1 + ", " + temp_2 + "\n";
            inputstr.append(expstr);
            cout << "error\n";
            break;
        case OR: // 进行短路求值时不进行如下操作
            temp_1 = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp_1 + " = or " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = ne 0, " + temp_1 + "\n"; // 与0比较获得结果
            inputstr.append(expstr);
            cout << "error\n";
            break;
        case NE: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = ne " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case EQ: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = eq " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case GT: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = gt " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case LT: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = lt " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case GE: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = ge " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        case LE: //
            temp = "%" + to_string(astinfo.symbolnum++);
            expstr = "  " + temp + " = le " + symbol_lhs + ", " + symbol_rhs + "\n";
            inputstr.append(expstr);
            break;
        default:
            break;
        }
        return temp;
    }
    else
    { //该CondExpAST为firstformexp
        return firstformexp->Dump(inputstr, astinfo);
        ;
    }
}

string CondExpAST::Short_Circuit_Eval(string &inputstr, symbol::astinfo &astinfo) const
{
    // string symbol_rhs = rhs->Dump(inputstr, astinfo); //获取右操作数symbol
    string temp, temp_1, temp_2; //本行symbol
    string symbol_1, symbol_2;   //计算右操作数 与 结果部分 名称
    //创造变量 @void 作为结果,由于c语言中不允许使用标识符作为名称，这里借用
    if (astinfo.and_num == 0)
    { //如果第一次使用创造变量需alloc
        inputstr.append("  @void = alloc i32\n");
        astinfo.and_num++;
    }
    //为避免被短路求值不执行这里增加执行。
    if (astinfo.or_num == 0)
    { //如果第一次使用创造变量需alloc
        inputstr.append("  @int = alloc i32\n");
        astinfo.or_num++;
    }

    if (this->condoper == AND)
    {
        astinfo.and_num++;
        symbol_1 = "%and_right_" + to_string(astinfo.and_num);
        symbol_2 = "%and_end_" + to_string(astinfo.and_num);

        // 1. 预先设置结果为假
        string symbol_lhs = this->lhs->Dump(inputstr, astinfo); //获取左操作数symbol
        temp = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + temp + " = eq 0, " + symbol_lhs + "\n"); //判断左是否等于0
        inputstr.append("  store 0, @void\n");

        // 2. 如果左值为 0 ，直接跳转至结果不计算右值，结果为假
        inputstr.append("  br " + temp + ", " + symbol_2 + ", " + symbol_1 + "\n\n");

        // 3. 开始计算右值
        inputstr.append(symbol_1 + ":\n");
        string symbol_rhs = this->rhs->Dump(inputstr, astinfo); //获取右操作数symbol
        temp_1 = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + temp_1 + " = ne 0, " + symbol_rhs + "\n"); //判断右是否等于0
        inputstr.append("  store " + temp_1 + ", @void\n");
        inputstr.append("  jump  " + symbol_2 + "\n\n");

        // 4. 将结果取出并返回
        inputstr.append(symbol_2 + ":\n"); //结尾处开始
        temp = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + temp + "= load @void\n");
    }
    else if (this->condoper == OR)
    {
        astinfo.or_num++;
        symbol_1 = "%or_right_" + to_string(astinfo.or_num);
        symbol_2 = "%or_end_" + to_string(astinfo.or_num);

        // 1. 预先设置结果为真
        inputstr.append("  store 1, @int\n");
        string symbol_lhs = this->lhs->Dump(inputstr, astinfo); //获取左操作数symbol
        // temp = "%" + to_string(astinfo.symbolnum++);
        // inputstr.append("  " + temp + " = eq 0, " + symbol_lhs + "\n");//判断左是否等于0

        // 2. 如果左值为0，跳转至计算右值，否则跳转至结尾-结果为真
        inputstr.append("  br " + symbol_lhs + ", " + symbol_2 + ", " + symbol_1 + "\n\n");

        // 3. 开始计算右值
        inputstr.append(symbol_1 + ":\n");
        string symbol_rhs = this->rhs->Dump(inputstr, astinfo); //获取右操作数symbol
        temp_1 = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + temp_1 + " = ne 0, " + symbol_rhs + "\n"); //判断右是否不等于0
        inputstr.append("  store " + temp_1 + ", @int\n");
        inputstr.append("  jump  " + symbol_2 + "\n\n");

        // 4. 将结果取出并返回
        inputstr.append(symbol_2 + ":\n"); //结尾处开始
        temp = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + temp + "= load @int\n");
    }
    else
    {
        std::cerr << "程序错误 " << this->line_num << std::endl;
    }

    return temp;
}

// ==================== LeftValAST  Class ====================
string LeftValAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    if (this->ident != NULL)
    { //当前左值变量为 单一变量
        return ident->Dump(inputstr, astinfo);
    }
    else
    { //当前左值变量为数组
        // 1. 计算出其index指向的内存地址
        astinfo.is_funcRparam = false; //初始设置为不是数组实参
        string arrayindex = this->identarray->Dump(inputstr, astinfo);
        //  递归从 当前作用域 -> 根作用域 的符号表中判断是否有该变量
        symbol::SymbolItem item;
        symbol::finditem(astinfo.ast_scope_id, this->identarray->ident_name, line_num, item);

        // 2.1 如果是 数组实参 则不需要load
        if (astinfo.is_funcRparam)
        {
            return arrayindex;
        }
        else
        {
            // 2.2 其余情况使用load语句将其load进来
            string temp = "%" + to_string(astinfo.symbolnum++);
            inputstr.append("  " + temp + " = load " + arrayindex + "\n");
            return temp;
        }
    }
}

// ==================== IdentifierAST  Class ====================
string IdentifierAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{
    // 1. 递归从 当前作用域 -> 根作用域 的符号表中判断是否有该变量
    symbol::SymbolItem item;
    symbol::finditem(astinfo.ast_scope_id, ident_name, line_num, item);
    string item_name, temp; // item在koopair的名称  本行symbol
    item_name = item.koopa_ir_name;

    // TODO 在数组实参的时候需要修改 FINISH array 时的指针类型应该与函数定义一直

    // 2.1 当前 ident 实际为数组，
    if (item.is_array)
    {                                // ident 表示数组实参， 传递数组参数相当于传递其第一个元素的地址
        string base_ptr, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针

        base_ptr = item.koopa_ir_name;
        if (item.is_funcFparam)
        {
            arrayindex = "%" + to_string(astinfo.symbolnum++);                           // load
            inputstr.append("  " + arrayindex + " = load " + item.koopa_ir_name + "\n"); //初始时为在base中的名称
        }
        else
        {
            arrayindex = "%" + to_string(astinfo.symbolnum++);
            inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", 0\n"); // 剩余几维补0
        }
        astinfo.funcRpara_size.clear();
        cout << item_name + "astinfo.funcRpara_size.push_back(-1)" << endl;
        astinfo.funcRpara_size.push_back(-1); //表示第一维指针
        if (item.dimension_len > 1)
        { //如果维数更多，将第1维开始的加入其中，如果是函数形参则应该全部传入
            int i;
            if (item.is_funcFparam)
                i = 0;
            else
                i = 1;

            for (; i < item.dimension_len; i++)
            {
                astinfo.funcRpara_size.push_back(item.dimension_size[i]);
            }
            cout << "more" << endl;
        }

        astinfo.is_funcRparam = true;
        return arrayindex;
    }
    else if (item.is_const)
    { // 2.2 const常量
        if (astinfo.funcRpara_size.empty())
        { //当实参表中出现了变量则表示为数值型
            astinfo.funcRpara_size.push_back(0);
        }
        return to_string(item.result.back());
    }
    else
    { // 2.3 非const类型  load出来并返回
        temp = "%" + to_string(astinfo.symbolnum++);
        string expstr; //本行的命令
        expstr = "  " + temp + " = load " + item_name + "\n";
        inputstr.append(expstr);
        if (astinfo.funcRpara_size.empty())
        { //当实参表中出现了变量则表示为数值型
            astinfo.funcRpara_size.push_back(0);
        }
        return temp;
    }
}

// ==================== IdentArrayAST  Class ====================
string IdentArrayAST::Dump(string &inputstr, symbol::astinfo &astinfo) const
{

    //查找index对应的数值  计算出其地址但不 load 此处index为exp非constexp，因此需要先将exp计算出来
    // 1. 递归从 当前作用域 -> 根作用域 的符号表中判断是否有该变量

    symbol::SymbolItem item;
    symbol::finditem(astinfo.ast_scope_id, ident_name, line_num, item);
    // cout << "in IdentArrayAST dump " << item.koopa_ir_name << endl;
    // TODO 在 数组实参、 数组参数表 时可能有bug
    // 传递数组参数相当于传递其第一个元素的地址

    // 2.1 this->shape_list.size() = item.dimension_len
    if (this->shape_list.size() == item.dimension_len)
    {
        if (item.is_funcFparam) // 2.1.1 item是函数形参  此时是形参作为函数实参调用使用
        {
            string base_ptr, curindex, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针
            // load
            base_ptr = "%" + to_string(astinfo.symbolnum++);
            inputstr.append("  " + base_ptr + " = load " + item.koopa_ir_name + "\n"); //初始时为在base中的名称
            // getptr
            arrayindex = "%" + to_string(astinfo.symbolnum++);
            curindex = this->shape_list[0]->Dump(inputstr, astinfo);
            inputstr.append("  " + arrayindex + " = getptr " + base_ptr + ", " + curindex + "\n");
            base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用

            for (int i = 1; i < item.dimension_len; i++)
            {
                curindex = this->shape_list[i]->Dump(inputstr, astinfo);
                arrayindex = "%" + to_string(astinfo.symbolnum++);

                inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", " + curindex + "\n");
                base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
            }
            arrayindex = "%" + to_string(astinfo.symbolnum++);
            inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", 0\n"); // 最后一维补0
            astinfo.is_funcRparam = true;
            astinfo.funcRpara_size.clear();
            astinfo.funcRpara_size.push_back(-1); //表示为指针，且只有一维
            return arrayindex;
        }
        else
        {                                          // 2.1.2 item不是函数形参 当前表示为数组某个点的值
            string base_ptr, curindex, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针
            base_ptr = item.koopa_ir_name;         //初始时为在base中的名称
            for (int i = 0; i < item.dimension_len; i++)
            {
                curindex = this->shape_list[i]->Dump(inputstr, astinfo);
                arrayindex = "%" + to_string(astinfo.symbolnum++);

                inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", " + curindex + "\n");
                base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
            }

            astinfo.is_funcRparam = false;
            return arrayindex;
        }
    }
    else if (this->shape_list.size() > item.dimension_len)
    { // 2.2 this->shape_list.size() > item.dimension_len
        //则 item 表示为函数数组形参，在koopair中为指针 实际上 this->shape_list.size() = item.dimension_len + 1
        //使用 getptr 指令获取 item 的指针
        string base_ptr, curindex, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针
        // 2.2.1 load
        base_ptr = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + base_ptr + " = load " + item.koopa_ir_name + "\n"); //初始时为在base中的名称
        // 2.2.2 getptr
        arrayindex = "%" + to_string(astinfo.symbolnum++);
        curindex = this->shape_list[0]->Dump(inputstr, astinfo);
        inputstr.append("  " + arrayindex + " = getptr " + base_ptr + ", " + curindex + "\n");
        base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用

        for (int i = 1; i < item.dimension_len + 1; i++)
        {
            curindex = this->shape_list[i]->Dump(inputstr, astinfo);
            arrayindex = "%" + to_string(astinfo.symbolnum++);

            inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", " + curindex + "\n");
            base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
        }

        astinfo.is_funcRparam = false;
        return arrayindex;
    }
    else
    { // 2.3 this->shape_list.size() < item.dimension_len
        // 省略最后一个维度 eg: int a[4][3],  a[1] 是包含三个元素的一维数组，a[1]可以作为参数传递给类型为 int[] 的函数形参
        // 传递数组参数相当于传递其第一个元素的地址 即 增加一维的0即可 ；上述 eg 传a[1][0] 即可 a 时传 a[0]
        string base_ptr, curindex, arrayindex; //基指针， 当前的index数值， 使用 getelemptr 获得或的 指针
        int i;
        if (item.is_funcFparam) // item是函数形参  此时是形参作为函数实参调用使用
        {
            // load
            base_ptr = "%" + to_string(astinfo.symbolnum++);
            inputstr.append("  " + base_ptr + " = load " + item.koopa_ir_name + "\n"); //初始时为在base中的名称
            // getptr
            arrayindex = "%" + to_string(astinfo.symbolnum++);
            curindex = this->shape_list[0]->Dump(inputstr, astinfo);
            inputstr.append("  " + arrayindex + " = getptr " + base_ptr + ", " + curindex + "\n");
            base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
            i = 1;
        }
        else
        {
            base_ptr = item.koopa_ir_name; //初始时为在base中的名称
            i = 0;
        }

        for (; i < this->shape_list.size(); i++)
        {
            curindex = this->shape_list[i]->Dump(inputstr, astinfo);
            arrayindex = "%" + to_string(astinfo.symbolnum++);

            inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", " + curindex + "\n");
            base_ptr = arrayindex; //更新 基地址指针为 当前指针，多维度时嵌套使用
        }
        // cout << "this->shape_list.size()" << this->shape_list.size() << "item.dimension_len:" << item.dimension_len << endl;

        arrayindex = "%" + to_string(astinfo.symbolnum++);
        inputstr.append("  " + arrayindex + " = getelemptr " + base_ptr + ", 0\n"); // 此维补0

        astinfo.is_funcRparam = true;
        astinfo.funcRpara_size.clear();
        astinfo.funcRpara_size.push_back(-1);
        int sizei;
        if (item.is_funcFparam)
        {
            sizei = this->shape_list.size();
        }
        else
        {
            sizei = this->shape_list.size() + 1;
        }

        for (; sizei < item.dimension_len; sizei++)
        { //如果维数更多，将第1维开始的加入其中
            astinfo.funcRpara_size.push_back(item.dimension_size[sizei]);
        }

        return arrayindex;
    }
}

//遍历到const常量赋值语句时需要计算出其值，以下是表达式的计算操作。
int LeftValAST::cal(symbol::astinfo &astinfo) const
{
    if (this->ident != NULL)
    { //当前左值变量为 单一变量
        return ident->cal(astinfo);
    }
    else
    { //当前左值变量为数组
        return this->identarray->cal(astinfo);
    }
}

int IdentifierAST::cal(symbol::astinfo &astinfo) const
{
    int number;
    //  从当前作用域->根作用域查找
    symbol::SymbolItem item;
    symbol::finditem(astinfo.ast_scope_id, ident_name, line_num, item);
    // 判断 当前变量是否是const类型
    if (!item.is_const)
    { //当前变量不是const类型报错
        symbol::SemanticError(this->line_num, "varible " + this->ident_name + " is not const type, can not assignment to a const varible!");
    }
    if (item.is_array)
    { //当前变量为array类型报错
        symbol::SemanticError(this->line_num, "varible " + this->ident_name + " is array type, can not assignment to a const varible without giving the index!");
    }

    number = item.result.back();
    cout << number << endl;
    return number;
}

int IdentArrayAST::cal(symbol::astinfo &astinfo) const
{
    // int number;
    // 1. 从当前作用域->根作用域查找
    symbol::SymbolItem item;
    symbol::finditem(astinfo.ast_scope_id, ident_name, line_num, item);
    // 判断 当前变量是否是const类型
    if (!item.is_const)
    { //当前变量不是const类型报错
        symbol::SemanticError(this->line_num, "varible " + this->ident_name + " is not const type, can not assignment to a const varible!");
    }
    if (!item.is_array)
    { //当前变量为array类型报错
        symbol::SemanticError(this->line_num, "varible " + this->ident_name + " is not array type");
    }
    if (astinfo.ast_scope_id == 0)
    { //当前作用域为全局作用域，无论const与否均不能赋值给其他全局变量
        symbol::SemanticError(this->line_num, "global array could not assign to another global varible");
    }

    // 2. 查找index对应位置的数值
    //暂时处理一维数组情况
    //求index
    int index = this->shape_list[0]->cal(astinfo);
    return item.result[index];

    //返回数组中 指定下标的数值
}

int PrimaryExpAST::cal(symbol::astinfo &astinfo) const
{
    if (exp != NULL)
    { //该PrimaryExp为exp
        return exp->cal(astinfo);
    }
    else if (leftval != NULL)
    { //该PrimaryExp为lval
        return leftval->cal(astinfo);
    }
    else
    { //该PrimaryExp为Number
        return number;
    }
}

int UnaryExpAST::cal(symbol::astinfo &astinfo) const
{
    if (primaryexp == NULL)
    {                                    //该UnaryExp为第二种
        int num_rhs = rhs->cal(astinfo); //获取右操作数数值
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
        return primaryexp->cal(astinfo);
    }
}

int BinaryExpAST::cal(symbol::astinfo &astinfo) const
{
    if (firstformexp == NULL)
    {                                    //该MultExpAST为第二种
        int num_lhs = lhs->cal(astinfo); //获取左操作数数值
        int num_rhs = rhs->cal(astinfo); //获取右操作数数值
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
        return firstformexp->cal(astinfo);
    }
}

int CondExpAST::cal(symbol::astinfo &astinfo) const
{
    if (firstformexp == NULL)
    {                                  //该MultExpAST为第二种
        int left = lhs->cal(astinfo);  //获取左操作数数值
        int right = rhs->cal(astinfo); //获取左操作数数值
        switch (condoper)
        {
        case AND: //
            return int(left && right);
        case OR: //
            return int(left || right);
        case NE: //
            return int(left != right);
        case EQ: //
            return int(left == right);
        case GT: //
            return int(left > right);
        case LT: //
            return int(left < right);
        case GE: //
            return int(left >= right);
        case LE: //
        default:
            cout << "error" << endl;
        }
        return -1;
    }
    else
    { //该CondExpAST为firstformexp
        return firstformexp->cal(astinfo);
        ;
    }
    cout << "cal conditon expression should not be used in this state" << endl;
    return 0;
}