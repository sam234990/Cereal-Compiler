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
    CompUnitAST *ast_comp;
    FuncTypeAST *ast_functype;
    FuncFParamsListAST *ast_funcFlist;
    FuncFParamAST *ast_funcF;
    FuncFParamOneAST *ast_funcFone;
    FuncFParamArrayAST *ast_funcFarray;
    ExpAST *ast_exp;
    FuncRParamsListAST *ast_funcRlist;
    FuncCallAST *ast_funcR;
    DeclareAST *ast_decl;
    DefineAST *ast_define; 
    StmtAST *ast_stmt;
    BlockItemAST *ast_block;
    LeftValAST *ast_lval;
    IdentifierAST *ast_ident;
    IdentArrayAST *ast_identarray;
    InitValArrayAST *initvalarray;
    std::vector<InitValArrayAST *> *initcalarraylist;
}

%token <key_op> ADD SUB NOT 
%token <key_op> MULT DIV MOD
%token <key_op> LT LE GT GE
%token <key_op> EQ NE 
%token <key_op> AND 
%token <key_op> OR

%token <key_op> INT VOID RETURN CONST IF ELSE WHILE BREAK CONTINUE

%token <str_val> IDENT
%token <int_val> INT_CONST

%type <ast_decl> Decl ConstDecl VarDecl
%type <ast_define> ConstDef ConstDefOne ConstDefArray Def DefOne DefArray
%type <ast_val> FuncDef 
%type <ast_val> Root 
%type <ast_comp> CompUnit
%type <ast_funcFlist> FuncFParamsList
%type <ast_funcF> FuncFParam 
%type <ast_funcFone> FuncFParamOne 
%type <ast_funcFarray> FuncFParamArray
%type <ast_stmt> Stmt BlockItem ReturnStmt AssignStmt IfStmt WhileStmt
%type <ast_block> Block BlockItemList
%type <ast_ident> Ident
%type <ast_identarray> IdentArray
%type <ast_lval> LVal
%type <initvalarray> InitValArray
%type <initcalarraylist> InitValArrayList

%type <ast_exp> Exp UnaryExp PrimaryExp AddExp MultExp RelExp EqExp LAndExp LOrExp
%type <ast_exp> InitVal
%type <ast_funcRlist> FuncRParamsList
%type <ast_funcR> FuncCall
%type <int_val> Number 
%type <key_op> UnaryOp AddOp MultOp EqualOp RelOp
%type <key_op> BType

%%
Root: CompUnit {
    ast = unique_ptr<CompUnitAST>($1);
}

CompUnit
    : Decl {
        $$ = new CompUnitAST(yyget_lineno());
        $$->compunitlist.push_back(static_cast<BaseAST*>($1));
    }
    | CompUnit Decl {
        $$ = ($1);//左递归
        $$->compunitlist.push_back(static_cast<BaseAST*>($2)); //将全局声明放入
    }
    | FuncDef{ 
        $$ = new CompUnitAST(yyget_lineno());
        $$->compunitlist.push_back(static_cast<BaseAST*>($1));
    }
    | CompUnit FuncDef{
        $$ = ($1);//左递归
        $$->compunitlist.push_back(static_cast<BaseAST*>($2)); //将函数块放入
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
        auto const_decl = new DeclareAST(yyget_lineno());
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
    | ConstDefArray{ $$ = ($1); };
    

ConstDefOne
    : IDENT '=' InitVal{
        $$ = new DefOneInitAST(*unique_ptr<string>($1), unique_ptr<ExpAST>($3), true, yyget_lineno());
    };

ConstDefArray
    : IdentArray '=' InitValArray{
        $$ = new DefArrayInitAST(unique_ptr<IdentArrayAST>($1), unique_ptr<InitValArrayAST>($3), true, yyget_lineno());
    };

IdentArray
    : IDENT '[' Exp ']'{
        $$ = new IdentArrayAST(*unique_ptr<string>($1), yyget_lineno());
        $$->shape_list.push_back($3);
    }
    | IdentArray '[' Exp ']'{//左递归 将数组维数加入至数组中
        $$ = ($1);
        $$->shape_list.push_back($3);
    };

InitValArray
    : '{' '}' {
        $$ = new InitValArrayAST(false, NULL, yyget_lineno());
    }
    | '{' InitValArrayList '}' {
        $$ = new InitValArrayAST(false, NULL, yyget_lineno());
        $$->initvalarraylist.swap(*$2);
        delete $2;
    };

//InitValArrayList 为一个 vector，其基本元素为 InitValArray。
//InitValArray 可以为 InitVal、InitValArray
InitValArrayList
    : InitVal{
        $$ = new vector<InitValArrayAST *>;
        $$->push_back(new InitValArrayAST(true, unique_ptr<ExpAST>($1), yyget_lineno()));
    }
    | InitValArrayList ',' InitVal{
        $$ = ($1);
        $$->push_back(new InitValArrayAST(true, unique_ptr<ExpAST>($3), yyget_lineno()));
    }
    | InitValArray{
        $$ = new vector<InitValArrayAST *>;
        $$->push_back($1);
    }
    | InitValArrayList ',' InitValArray{
        $$ = ($1);
        $$->push_back($3);
    };


InitVal
    : Exp{ //这里用了addexp,应该接exp。需修改TODO
        $$ = ($1); 
    };//用 InitVal 和 ConstInitArray 代替 ConstExp

VarDecl
    : BType Def {
        auto var_decl = new DeclareAST(yyget_lineno());
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
    }
    | DefArray{
        $$ = ($1);
    };

DefOne
    : IDENT '=' InitVal{
        $$ = new DefOneInitAST(*unique_ptr<string>($1), unique_ptr<ExpAST>($3), false, yyget_lineno());
    }
    | IDENT{
        $$ = new DefOneAST(*unique_ptr<string>($1), yyget_lineno());
    };

DefArray
    : IdentArray '=' InitValArray{
        $$ = new DefArrayInitAST(unique_ptr<IdentArrayAST>($1), unique_ptr<InitValArrayAST>($3), false, yyget_lineno());
    }
    | IdentArray{
        $$ = new DefArray(unique_ptr<IdentArrayAST>($1), yyget_lineno());
    }


FuncDef
    : BType IDENT '(' ')' Block{
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "int";
        ast->ident = *unique_ptr<string>($2);
        ast->funcfparamlist = NULL;
        ast->block = unique_ptr<StmtAST>($5);
        $$ = ast;
    }
    | BType IDENT '(' FuncFParamsList ')' Block{
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "int";
        ast->ident = *unique_ptr<string>($2);
        ast->funcfparamlist = unique_ptr<FuncFParamsListAST>($4);
        ast->block = unique_ptr<StmtAST>($6);
        $$ = ast;
    }
    | VOID IDENT '(' ')' Block{
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "void";
        // ast->func_type = unique_ptr<FuncTypeAST>($1);
        ast->ident = *unique_ptr<string>($2);
        ast->funcfparamlist = NULL;
        ast->block = unique_ptr<StmtAST>($5);
        $$ = ast;
    }
    | VOID IDENT '(' FuncFParamsList ')' Block{
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "void";
        // ast->func_type = unique_ptr<FuncTypeAST>($1);
        ast->ident = *unique_ptr<string>($2);
        ast->funcfparamlist = unique_ptr<FuncFParamsListAST>($4);
        ast->block = unique_ptr<StmtAST>($6);
        $$ = ast;
    }
    ;

FuncFParamsList
    : FuncFParam{
        $$ = new FuncFParamsListAST(yyget_lineno());
        $$->funcfparamlist.push_back($1);

    }
    | FuncFParamsList ',' FuncFParam{
        $$ = ($1);
        $$->funcfparamlist.push_back($3);//左递归
    };

FuncFParam
    : FuncFParamOne{
        $$ = static_cast<FuncFParamAST*>($1);
    }
    | FuncFParamArray{
        $$ = static_cast<FuncFParamAST*>($1);
    }
    ;
    

FuncFParamOne
    : BType IDENT{
        $$ = new FuncFParamOneAST(*unique_ptr<string>($2), yyget_lineno());
    };

FuncFParamArray
    : BType IDENT '[' ']' {
        $$ = new FuncFParamArrayAST(*unique_ptr<string>($2), yyget_lineno());
    }
    | FuncFParamArray '[' Exp ']'{
        $$ = ($1);
        $$->shape_list.push_back($3);
    };


Block
    : '{' BlockItemList '}'{
        $$ = ($2);
    }
    | '{' '}'{
        auto ast = new BlockItemAST(yyget_lineno());//blockitemlist中为空
        $$ = ast;
    };

BlockItemList
    : BlockItem{
        auto ast = new BlockItemAST(yyget_lineno());
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
    : ReturnStmt
    | AssignStmt
    | IfStmt
    | WhileStmt
    | Block{
        $$ = static_cast<StmtAST*>($1);
    }
    | Exp ';'{
        $$ = static_cast<StmtAST*>(new ExpstmtAST(unique_ptr<ExpAST>($1), yyget_lineno()));
    }
    | BREAK ';'{
        $$ = static_cast<StmtAST*>(new BreakstmtAST(yyget_lineno()));
    }
    | CONTINUE ';'{
        $$ = static_cast<StmtAST*>(new ContinuestmtAST(yyget_lineno()));
    }
    | ';'{
        $$ = static_cast<StmtAST*>(new VoidstmtAST(yyget_lineno()));
    };

ReturnStmt
    : RETURN Exp ';'{
        auto ast = new ReturnStmtAST(yyget_lineno());
        ast->Exp = unique_ptr<ExpAST>($2);
        $$ = ast;
    }
    | RETURN ';'{
        auto ast = new ReturnStmtAST(yyget_lineno());
        ast->Exp = NULL;//表示该部分无返回值
        $$ = ast;
    };

AssignStmt
    : LVal '=' Exp';'{
        $$ = new AssignStmtAST(unique_ptr<LeftValAST>($1), unique_ptr<ExpAST>($3), yyget_lineno());
    };

IfStmt
    : IF '(' Exp ')' Stmt{
        $$ = new IfElseStmtAST(unique_ptr<ExpAST>($3), unique_ptr<StmtAST>($5), NULL, yyget_lineno());
    }
    | IF '(' Exp ')' Stmt ELSE Stmt{
        $$ = new IfElseStmtAST(unique_ptr<ExpAST>($3), unique_ptr<StmtAST>($5), unique_ptr<StmtAST>($7), yyget_lineno());
    };

WhileStmt
    : WHILE '(' Exp ')' Stmt{
        $$ = new WhileStmtAST(unique_ptr<ExpAST>($3), unique_ptr<StmtAST>($5), yyget_lineno());
    };

Exp
    : LOrExp{ 
        $$ = ($1); 
    };

LVal
    : Ident{
        $$ = new LeftValAST(unique_ptr<IdentifierAST>($1), NULL, yyget_lineno());
    }
    | IdentArray{
        $$ = new LeftValAST(NULL, unique_ptr<IdentArrayAST>($1), yyget_lineno());
    };

PrimaryExp
    : '(' Exp ')'{
        $$ = new PrimaryExpAST(0, unique_ptr<ExpAST>($2), NULL, yyget_lineno());
    }
    | LVal{
        $$ = new PrimaryExpAST(0, NULL, unique_ptr<LeftValAST>($1), yyget_lineno());
    }
    | Number{ 
        $$ = new PrimaryExpAST(($1), NULL, NULL, yyget_lineno());
    };


Ident
    : IDENT{
        $$ = new IdentifierAST(*unique_ptr<string>($1), yyget_lineno());
    };

UnaryExp
    : PrimaryExp{ 
        $$ = new UnaryExpAST(unique_ptr<ExpAST>($1), 0, NULL, NULL, yyget_lineno());
        
    }
    | UnaryOp UnaryExp{
        $$ = new UnaryExpAST(NULL, ($1), unique_ptr<ExpAST>($2), NULL, yyget_lineno());
        
    }
    | FuncCall{
        $$ = new UnaryExpAST(NULL, 0, NULL, unique_ptr<FuncCallAST>($1), yyget_lineno());
    };

FuncCall
    : IDENT '(' FuncRParamsList ')'{
        $$ = new FuncCallAST(*unique_ptr<string>($1), unique_ptr<FuncRParamsListAST>($3), yyget_lineno());
    }
    | IDENT '(' ')'{
        $$ = new FuncCallAST(*unique_ptr<string>($1), NULL, yyget_lineno());
    } ;

FuncRParamsList
    : Exp {
        $$ = new FuncRParamsListAST(yyget_lineno());
        $$->rparamslist.push_back($1);
    }
    | FuncRParamsList ',' Exp {
        $$ = ($1);
        $$->rparamslist.push_back($3);
    };

MultExp
    : UnaryExp{
        $$ = new BinaryExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());
    }
    | MultExp MultOp UnaryExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
    };

AddExp
    : MultExp{
        $$ = new BinaryExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());
    } 
    | AddExp AddOp MultExp{
        $$ = new BinaryExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
     };

RelExp
    :  AddExp{
        $$ = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());

    }
    | RelExp RelOp AddExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
     };

EqExp
    : RelExp{
        $$ = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());
    }
    | EqExp EqualOp RelExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
    };

LAndExp
    : EqExp {
        $$ = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());
    }
    | LAndExp AND EqExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
    };
    
LOrExp
    : LAndExp {
        $$ = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>($1), yyget_lineno());
    }
    | LOrExp OR LAndExp{
        $$ = new CondExpAST(unique_ptr<ExpAST>($1), ($2), unique_ptr<ExpAST>($3), NULL, yyget_lineno());
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