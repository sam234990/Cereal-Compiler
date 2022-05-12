#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <fstream>
#include "koopa.h"
#include "../include/util.h"
#include "../include/ast.h"
#include "../include/koopair.h"

using namespace std;

extern FILE *yyin;
extern int yyparse(unique_ptr<BaseAST> &ast);

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
    
    //flex&bison吹了并生成抽象语法树
    unique_ptr<BaseAST> ast;
    auto ast_ret = yyparse(ast);
    assert(!ast_ret);

    //遍历抽象语法树返回字符串形式的Koopa IR
    string irstr;
    ast->Dump(irstr);
    // cout << irstr << endl;

    if (string(mode) == "-koopa")
    {//输出为koopa模式
        cout << irstr << endl;
        //写入对应文件
        write_file(output, irstr);
        return 0;
    }

    if (string(mode) == "-riscv")
    {//输出为riscv模式
        string write_file_name = output;
        ofstream os;     //创建一个文件输出流对象
        os.open(write_file_name, ios::out);//将对象与文件关联
        
        // string objstr;
        // objstr = 
        koopa_ir_from_str(irstr, os);//字符串形式的Koopa IR转内存形式并遍历生成
        os.close();
        return 0;
    }
    
    return 0;
}
