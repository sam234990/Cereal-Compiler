%code requires { 
    #include <memory>
    #include <string>
    #include "../include/ast.h"
}
%{
#include "../include/ast.h"
#include <iostream>
#include <memory>
#include <vector>
#include <string>

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<BaseAST> &ast, const char *s);
extern int yyget_lineno();

using namespace std;

%}
// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<BaseAST> &ast }

// yylval 的定义, 定义成了一个联合体 (union)
%union {
    int key_op;
    std::string *str_val;
    int int_val;
    BaseAST *ast_val;
    ExpAST *ast_exp;
    DeclareAST *ast_decl;
    DefineAST *ast_define; 
    StmtAST *ast_stmt;
    BlockAST *ast_block;
    LeftValAST *ast_lval;
    IdentifierAST *ast_ident;
}

%token <key_op> ADD SUB NOT 
%token <key_op> MULT DIV MOD
%token <key_op> LT LE GT GE
%token <key_op> EQ NE 
%token <key_op> AND 
%token <key_op> OR
%token <key_op> INT VOID RETURN CONST


%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_decl> Decl ConstDecl VarDecl
%type <ast_define> ConstDef ConstDefOne Def DefOne
%type <ast_val> FuncDef FuncType 
%type <ast_stmt> Stmt  BlockItem ReturnStmt AssignStmt
%type <ast_block>Block BlockItemList
%type <ast_ident> Ident
%type <ast_lval> LVal

%type <ast_exp> Exp UnaryExp PrimaryExp AddExp MultExp RelExp EqExp LAndExp LOrExp
%type <ast_exp> InitVal
%type <int_val> Number 
%type <key_op> UnaryOp AddOp MultOp EqualOp RelOp
%type <key_op> BType

%%
CompUnit
    : FuncDef{
        auto comp_unit = make_unique<CompUnitAST>();
        comp_unit->func_def = unique_ptr<BaseAST>($1);
        comp_unit->line_num = yyget_lineno();
        ast = move(comp_unit);
    };

Decl
    : ConstDecl';'{//将分号移至上方, 使 constdecl变成左递归。
        $$ = ($1);
    }
    |
    VarDecl';'{
        $$ = ($1);
    }
    ;

ConstDecl
    : CONST BType ConstDef {//为左值新建一个声明类
        auto const_decl = new DeclareAST();
        const_decl->btype = ($2);
        const_decl->define_list_.push_back(($3));//并将定义语句放入其中
        $$ = const_decl;
    }
    |
    ConstDecl ',' ConstDef{
        $$ = ($1);//左递归
        $$->define_list_.push_back($3);//将定义语句的放入其中
    };



BType
    : INT ;

ConstDef
    : ConstDefOne{ $$ = ($1); }
    /* |ConstDefArray */
    ;

ConstDefOne
    : IDENT '=' InitVal{
        $$ = new DefOneInitAST(*unique_ptr<string>($1), unique_ptr<ExpAST>($3), true, yyget_lineno());
    };

InitVal
    : AddExp{ $$ = ($1); };//用 InitVal 和 ConstInitArray 代替 ConstExp

VarDecl
    : BType Def {
        auto var_decl = new DeclareAST();
        var_decl->btype = ($1);
        var_decl->define_list_.push_back(($2));//并将定义语句放入其中
        $$ = var_decl;
    }
    |
    VarDecl ',' Def {
        $$ = ($1);//左递归
        $$->define_list_.push_back($3);//将定义语句的放入其中
    };

Def
    : DefOne{
        $$ = ($1);
    };

DefOne
    : IDENT '=' InitVal{
        $$ = new DefOneInitAST(*unique_ptr<string>($1), unique_ptr<ExpAST>($3), false, yyget_lineno());
    }
    |
    IDENT{
        $$ = new DefOneAST(*unique_ptr<string>($1), yyget_lineno());
    };




FuncDef
    : FuncType IDENT '(' ')' Block{
        auto ast = new FuncDefAST();
        ast->func_type = unique_ptr<BaseAST>($1);
        ast->ident = *unique_ptr<string>($2);
        ast->block = unique_ptr<StmtAST>($5);
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
    : '{' BlockItemList '}'{
        $$ = ($2);
    }
    | '{' '}'{
        auto ast = new BlockAST();//blockitemlist中为空
        $$ = ast;
    };

BlockItemList
    : BlockItem{
        auto ast = new BlockAST();
        ast->blockitemlist.push_back($1);
        $$ = ast;
    }
    | BlockItemList BlockItem{
        $$ = ($1);//左递归
        $$->blockitemlist.push_back($2); //将语句块放入
    };


BlockItem
    : Decl{
        $$ = static_cast<StmtAST*>($1);
    }
    | Stmt{ $$ = ($1); };


Stmt
    : ReturnStmt{ $$ = ($1); }
    | AssignStmt;

ReturnStmt
    : RETURN Exp ';'{
        auto ast = new ReturnStmtAST();
        ast->Exp = unique_ptr<ExpAST>($2);
        $$ = ast;
    };

AssignStmt
    : LVal '=' Exp';'{
        $$ = new AssignStmtAST(unique_ptr<LeftValAST>($1), unique_ptr<ExpAST>($3), yyget_lineno());
    };

Exp
    : LOrExp{ 
        $$ = ($1); 
    };

LVal
    : Ident{
        auto ast = new LeftValAST();
        ast->ident = unique_ptr<IdentifierAST>($1);
        $$ = ast;
    };

PrimaryExp
    : '(' AddExp ')'{
        auto ast = new PrimaryExpAST();
        ast->leftval = NULL; //表示该PrimaryExp为exp
        ast->exp = unique_ptr<ExpAST>($2);
        $$ = ast;
    }
    | LVal{
        auto ast = new PrimaryExpAST();
        ast->leftval = unique_ptr<LeftValAST>($1);
        ast->exp = NULL;
        $$ = ast;
    }
    | Number{ 
        auto ast = new PrimaryExpAST();
        ast->number = ($1);
        ast->leftval = NULL;
        ast->exp = NULL;//表示该PrimaryExp为Number
        $$ = ast;
    };


Ident
    : IDENT{
        auto ast = new IdentifierAST();
        ast->line_num = yyget_lineno();
        ast->ident_name = *unique_ptr<string>($1);
        $$ = ast;
    };

UnaryExp
    : PrimaryExp{ 
        auto ast = new UnaryExpAST();
        ast->primaryexp = unique_ptr<ExpAST>($1);
        ast->rhs = NULL;//表示该UnaryExp为PrimaryExp
        $$ = ast;
    }
    | UnaryOp UnaryExp{
        auto ast = new UnaryExpAST();
        ast->unaryop = ($1);
        ast->primaryexp = NULL;//表示该UnaryExp为第二种
        ast->rhs = unique_ptr<ExpAST>($2);
        $$ = ast;
    };

MultExp
    : UnaryExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1));
    }
    | MultExp MultOp UnaryExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
    };

AddExp
    : MultExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1));
    } 
    | AddExp AddOp MultExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
     };

RelExp
    :  AddExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1));

    }
    | RelExp RelOp AddExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
     };

EqExp
    : RelExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1));
    }
    | EqExp EqualOp RelExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
    };

LAndExp
    : EqExp {
        $$ = new CondExpAST(unique_ptr<ExpAST>($1));
    }
    | LAndExp AND EqExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
    };
    
LOrExp
    : LAndExp {
        $$ = new CondExpAST(unique_ptr<ExpAST>($1));
    }
    | LOrExp OR LAndExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3));
    }; 

Number
    : INT_CONST {
        $$ = ($1);
    };

EqualOp
    : EQ{ $$ = ($1); }
    | NE{ $$ = ($1); };

RelOp
    : GT{ $$ = ($1); }
    | GE{ $$ = ($1); }
    | LT{ $$ = ($1); }
    | LE{ $$ = ($1); };

UnaryOp
    : ADD{ $$ = ($1); }
    | SUB{ $$ = ($1); }
    | NOT{ $$ = ($1); };

AddOp
    : ADD{ $$ = ($1); }
    | SUB{ $$ = ($1); };

MultOp
    : MULT{ $$ = ($1); }
    | DIV{ $$ = ($1); }
    | MOD{ $$ = ($1); };

%%


void yyerror(unique_ptr<BaseAST> &ast, const char *s){
    cerr << "error: (line:"<< yyget_lineno() <<") :" << s << endl;
}