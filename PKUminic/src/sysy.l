%option noyywrap
%option nounput
%option noinput
%x COMMENT

%{
#include <cstdlib>
#include <string>

#include "sysy.tab.hpp"

using namespace std;
%}
/* 空白符和注释 */
WhiteSpace    [ \t\n\r]*
LineComment   "//".*$

/* 标识符 */
Identifier    [a-zA-Z_][a-zA-Z0-9_]*

/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+
/* 第一个为多行注释 */
%%
"/*"		{BEGIN(COMMENT);/* 多行注释忽略, 不做任何操作 */}
<COMMENT>"*/"  {BEGIN(INITIAL);}
<COMMENT>([^*]|\n)+|.
<COMMENT><<EOF>>   {printf("Unterminated comment\n"); return 0;}

{WhiteSpace}    {/* 空白符忽略, 不做任何操作 */}
{LineComment}   {/* 单行注释忽略, 不做任何操作 */}


"+"             { yylval.key_op = ADD; return ADD;}
"-"             { yylval.key_op = SUB; return SUB;}
"!"             { yylval.key_op = NOT; return NOT;}
"*"             { yylval.key_op = MULT; return MULT;}
"/"             { yylval.key_op = DIV; return DIV;}
"%"             { yylval.key_op = MOD; return MOD;}
"<"			    { yylval.key_op = LT; return LT; }
">"			    { yylval.key_op = GT; return GT; }
"<="		    { yylval.key_op = LE; return LE; }
">="	    	{ yylval.key_op = GE; return GE; }
"=="		    { yylval.key_op = EQ; return EQ; }
"!="		    { yylval.key_op = NE; return NE; }
"!"		    	{ yylval.key_op = NOT; return NOT; }
"&&"	    	{ yylval.key_op = AND; return AND; }
"||"	    	{ yylval.key_op = OR; return OR; }


"int"           { return INT; }
"void"          { return VOID; }
"return"        { return RETURN; }

{Identifier}    { yylval.str_val = new string(yytext); return IDENT; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

.               { return yytext[0]; }
%%