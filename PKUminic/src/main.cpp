#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "koopa.h"
#include "../include/util.h"
#include "../include/ast.h"
#include "../include/genricsv.h"
#include "../include/symbol.h"
#include "sysy.tab.hpp"
using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);
extern int yylex_destroy();
extern void yyset_lineno(int _line_number);


int main(int argc, char const *argv[])
{
    //对编译输入的处理：输入文件、输出文件
    assert(argc == 5);
    auto mode = argv[1];
    auto input = argv[2];
    auto type = argv[3];

    auto output = argv[4];

    //打开输入文件
    yyin = fopen(input, "r");
    assert(yyin);

    // flex & bison 处理并生成抽象语法树
    yyset_lineno(1);
    unique_ptr<BaseAST> ast;
    auto ast_ret = yyparse(ast);
    yylex_destroy();
    assert(!ast_ret);
    

    symbol::astinfo astinfo;
    //遍历抽象语法树返回字符串形式的Koopa IR
    string irstr;
    ast->Dump(irstr, astinfo);
    // cout << irstr << endl;

    if (string(mode) == "-koopa")
    { //输出为koopa模式
        // cout << irstr << endl;
        //写入对应文件
        write_file(output, irstr);
        return 0;
    }

    symbol::kirinfo kirinfo;

    if (string(mode) == "-riscv")
    { //输出为riscv模式
        string write_file_name = output;
        ofstream os;                           //创建一个文件输出流对象
        os.open(write_file_name, ios::out);    //将对象与文件关联
        koopa_ir_from_str(irstr, os, kirinfo); //字符串形式的Koopa IR转内存形式并遍历生成
        os.close();
        return 0;
    }

    return 0;
}
