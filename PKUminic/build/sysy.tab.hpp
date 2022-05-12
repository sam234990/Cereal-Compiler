/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_ROOT_COMPILER_PKUMINIC_BUILD_SYSY_TAB_HPP_INCLUDED
# define YY_YY_ROOT_COMPILER_PKUMINIC_BUILD_SYSY_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "/root/compiler/PKUminic/src/sysy.y"
 
    #include <memory>
    #include <string>
    #include "../include/ast.h"

#line 54 "/root/compiler/PKUminic/build/sysy.tab.hpp"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ADD = 258,
    SUB = 259,
    NOT = 260,
    MULT = 261,
    DIV = 262,
    MOD = 263,
    INT = 264,
    VOID = 265,
    RETURN = 266,
    LT = 267,
    LE = 268,
    GT = 269,
    GE = 270,
    EQ = 271,
    NE = 272,
    AND = 273,
    OR = 274,
    IDENT = 275,
    INT_CONST = 276
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 23 "/root/compiler/PKUminic/src/sysy.y"

    int key_op;
    std::string *str_val;
    int int_val;
    BaseAST *ast_val;
    ExpAST *ast_exp;

#line 95 "/root/compiler/PKUminic/build/sysy.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (std::unique_ptr<BaseAST> &ast);

#endif /* !YY_YY_ROOT_COMPILER_PKUMINIC_BUILD_SYSY_TAB_HPP_INCLUDED  */