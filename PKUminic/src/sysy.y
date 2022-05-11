%code requires { 
    #include <memory>
    #include <string>
    #include "../include/ast.h"
}
%{
#include "../include/ast.h"
#include <iostream>
#include <memory>
#include <string>

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);

using namespace std;

%}

%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 定义成了一个联合体 (union)
%union {
    int key_op;
    std::string *str_val;
    int int_val;
    BaseAST *ast_val;
}

%token <key_op> ADD SUB NOT
%token INT VOID RETURN 
%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt
%type <int_val> Number Exp UnaryExp PrimaryExp UnaryOp 
/* %type <int_val> Exp AddExp MulExp */

%%
CompUnit
    : FuncDef{
        auto comp_unit = make_unique<CompUnitAST>();
        comp_unit->func_def = unique_ptr<BaseAST>($1);
        ast = move(comp_unit);
    }
    ;

FuncDef
    : FuncType IDENT '(' ')' Block{
        auto ast = new FuncDefAST();
        ast->func_type = unique_ptr<BaseAST>($1);
        ast->ident = *unique_ptr<string>($2);
        ast->block = unique_ptr<BaseAST>($5);
        $$ = ast;
    };

FuncType
    : INT{
        auto ast = new FuncTypeAST();
        ast->functype = "int";
        $$ = ast;
    }
    |
    VOID{
        auto ast = new FuncTypeAST();
        ast->functype = "void";
        $$ = ast;
    };

Block
    : '{' Stmt '}'{
        auto ast = new BlockAST();
        ast->stmt = unique_ptr<BaseAST>($2);
        $$ = ast;
    };

Stmt
    : RETURN Exp ';'{
        auto ast = new StmtAST();
        ast->number =($2);
        $$ = ast;
    };
 
Exp
    : UnaryExp{

    };

PrimaryExp
    : '(' Exp ')'
    | Number;


UnaryExp
    : PrimaryExp | UnaryOp UnaryExp;



Number
    : INT_CONST {
        $$ = ($1);
    };

UnaryOp
    : ADD
    | SUB
    | NOT;
%%


void yyerror(unique_ptr<BaseAST> &ast, const char *s){
    cerr << "error: " << s << endl;
}