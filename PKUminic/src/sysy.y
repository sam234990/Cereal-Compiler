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
    ExpAst *ast_exp;
}

%token <key_op> ADD SUB NOT
%token INT VOID RETURN 
%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_val> FuncDef FuncType Block Stmt 
%type <ast_exp> Exp UnaryExp PrimaryExp 
%type <int_val> Number UnaryOp

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
        ast->Exp = unique_ptr<ExpAst>($2);
        $$ = ast;
    };
 
Exp
    : UnaryExp{ 
        $$ = ($1); 
    };

PrimaryExp
    : '(' Exp ')'{
        auto ast = new PrimaryExpAST();
        ast->exp = unique_ptr<ExpAst>($2);
        $$ = ast;
    }
    | Number{ 
        auto ast = new PrimaryExpAST();
        ast->number = ($1);//表示该PrimaryExp为Number
        ast->exp = NULL;
        $$ = ast;
    };


UnaryExp
    : PrimaryExp{ 
        auto ast = new UnaryExpAST();
        ast->primaryexp = unique_ptr<ExpAst>($1);
        ast->rhs = NULL;//表示该UnaryExp为PrimaryExp
        $$ = ast;
    }
    | UnaryOp UnaryExp{
        auto ast = new UnaryExpAST();
        ast->unaryop = ($1);
        ast->primaryexp = NULL;//表示该UnaryExp为第二种
        ast->rhs = unique_ptr<ExpAst>($2);
        $$ = ast;
    };


Number
    : INT_CONST {
        $$ = ($1);
    };

UnaryOp
    : ADD{ $$ = ($1); }
    | SUB{ $$ = ($1); }
    | NOT{ $$ = ($1); };
%%


void yyerror(unique_ptr<BaseAST> &ast, const char *s){
    cerr << "error: " << s << endl;
}