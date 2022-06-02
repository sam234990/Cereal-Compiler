/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.5.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 6 "/root/compiler/PKUminic/src/sysy.y"

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


#line 86 "/root/compiler/PKUminic/build/sysy.tab.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
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

#line 135 "/root/compiler/PKUminic/build/sysy.tab.cpp"

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
    LT = 264,
    LE = 265,
    GT = 266,
    GE = 267,
    EQ = 268,
    NE = 269,
    AND = 270,
    OR = 271,
    INT = 272,
    VOID = 273,
    RETURN = 274,
    CONST = 275,
    IF = 276,
    ELSE = 277,
    WHILE = 278,
    BREAK = 279,
    CONTINUE = 280,
    IDENT = 281,
    INT_CONST = 282
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 27 "/root/compiler/PKUminic/src/sysy.y"

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

#line 199 "/root/compiler/PKUminic/build/sysy.tab.cpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (std::unique_ptr<BaseAST> &ast);

#endif /* !YY_YY_ROOT_COMPILER_PKUMINIC_BUILD_SYSY_TAB_HPP_INCLUDED  */



#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))

/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   212

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  37
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  105
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  175

#define YYUNDEFTOK  2
#define YYMAXUTOK   282


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      35,    36,     2,     2,    29,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    28,
       2,    30,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    33,     2,    34,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    90,    90,    95,    99,   103,   107,   114,   118,   124,
     131,   139,   142,   143,   147,   152,   157,   161,   167,   170,
     179,   183,   187,   191,   198,   203,   210,   216,   219,   224,
     227,   232,   235,   241,   249,   257,   266,   278,   283,   289,
     292,   299,   304,   307,   314,   317,   323,   328,   335,   338,
     342,   343,   344,   345,   346,   349,   352,   355,   358,   363,
     368,   375,   380,   383,   388,   393,   398,   401,   406,   409,
     412,   418,   423,   427,   431,   436,   439,   444,   448,   454,
     457,   462,   465,   470,   474,   479,   482,   487,   490,   495,
     498,   503,   508,   509,   512,   513,   514,   515,   518,   519,
     520,   523,   524,   527,   528,   529
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ADD", "SUB", "NOT", "MULT", "DIV",
  "MOD", "LT", "LE", "GT", "GE", "EQ", "NE", "AND", "OR", "INT", "VOID",
  "RETURN", "CONST", "IF", "ELSE", "WHILE", "BREAK", "CONTINUE", "IDENT",
  "INT_CONST", "';'", "','", "'='", "'['", "']'", "'{'", "'}'", "'('",
  "')'", "$accept", "Root", "CompUnit", "Decl", "ConstDecl", "BType",
  "ConstDef", "ConstDefOne", "ConstDefArray", "IdentArray", "InitValArray",
  "InitValArrayList", "InitVal", "VarDecl", "Def", "DefOne", "DefArray",
  "FuncDef", "FuncFParamsList", "FuncFParam", "FuncFParamOne",
  "FuncFParamArray", "Block", "BlockItemList", "BlockItem", "Stmt",
  "ReturnStmt", "AssignStmt", "IfStmt", "WhileStmt", "Exp", "LVal",
  "PrimaryExp", "Ident", "UnaryExp", "FuncCall", "FuncRParamsList",
  "MultExp", "AddExp", "RelExp", "EqExp", "LAndExp", "LOrExp", "Number",
  "EqualOp", "RelOp", "UnaryOp", "AddOp", "MultOp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,    59,    44,
      61,    91,    93,   123,   125,    40,    41
};
# endif

#define YYPACT_NINF (-132)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      84,  -132,    13,     8,    49,    84,  -132,    92,    33,    97,
    -132,    17,    36,  -132,  -132,  -132,  -132,    36,    26,    16,
    -132,  -132,  -132,  -132,    44,    -2,   117,  -132,  -132,  -132,
     148,  -132,   163,   163,    -1,    22,   163,   156,  -132,    42,
      57,   -18,  -132,  -132,    69,   163,    22,  -132,  -132,  -132,
      -4,  -132,   163,    75,  -132,  -132,  -132,  -132,  -132,  -132,
    -132,   103,    89,    62,   167,   107,   113,  -132,   163,   101,
      42,   -10,    64,  -132,   125,    61,  -132,   112,     8,    42,
     163,  -132,  -132,    18,   123,  -132,  -132,  -132,   163,  -132,
    -132,   163,  -132,  -132,  -132,  -132,   163,  -132,  -132,   163,
     163,   163,  -132,  -132,  -132,    42,  -132,  -132,    29,  -132,
    -132,   147,   114,   134,   143,   160,  -132,  -132,  -132,    44,
    -132,   111,  -132,  -132,  -132,  -132,  -132,  -132,   164,   154,
     159,  -132,  -132,   161,  -132,  -132,     7,  -132,  -132,   103,
      89,    62,   167,   107,  -132,   150,  -132,  -132,   166,   163,
     163,  -132,  -132,  -132,  -132,  -132,   163,  -132,  -132,   163,
    -132,  -132,  -132,  -132,   165,   168,   169,  -132,   137,   137,
    -132,   173,  -132,   137,  -132
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    11,     0,     0,     0,     2,     3,     0,     0,     0,
       5,     0,     0,     1,     4,     6,     7,     0,    30,    32,
      25,    27,    28,     8,     0,     0,     0,     9,    12,    13,
       0,    10,     0,     0,     0,     0,     0,    30,    26,     0,
       0,     0,    37,    39,    40,     0,     0,    98,    99,   100,
      71,    91,     0,    67,    29,    24,    69,    72,    66,    79,
      74,    81,    83,    85,    87,    89,    65,    70,     0,     0,
       0,     0,     0,    31,     0,     0,    35,    41,     0,     0,
       0,    14,    15,     0,     0,   103,   104,   105,     0,   101,
     102,     0,    96,    97,    94,    95,     0,    92,    93,     0,
       0,     0,    73,    16,    33,     0,    18,    22,     0,    20,
      17,     0,     0,     0,     0,     0,    58,    45,    48,     0,
      54,     0,    46,    49,    50,    51,    52,    53,     0,    69,
       0,    38,    36,     0,    76,    77,     0,    68,    80,    82,
      84,    86,    88,    90,    34,     0,    19,    60,     0,     0,
       0,    56,    57,    44,    47,    55,     0,    42,    43,     0,
      75,    23,    21,    59,     0,     0,     0,    78,     0,     0,
      61,    62,    64,     0,    63
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -132,  -132,  -132,    28,  -132,     2,   179,  -132,  -132,     0,
     -40,  -132,   -42,  -132,   175,  -132,  -132,   195,   171,   124,
    -132,  -132,   -29,  -132,    82,  -131,  -132,  -132,  -132,  -132,
     -32,   -61,  -132,  -132,   -59,  -132,  -132,   115,   116,   108,
     109,   110,  -132,  -132,  -132,  -132,  -132,  -132,  -132
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     4,     5,   118,     7,    40,    27,    28,    29,    53,
      73,   108,    54,     9,    20,    21,    22,    10,    41,    42,
      43,    44,   120,   121,   122,   123,   124,   125,   126,   127,
     128,    56,    57,    58,    59,    60,   136,    61,    62,    63,
      64,    65,    66,    67,    99,    96,    68,    91,    88
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      55,    69,     8,    81,    74,    12,    82,     8,    19,   102,
      76,    78,    30,    55,   129,     1,     1,    30,    79,    78,
      84,    47,    48,    49,    19,     1,   105,    33,     6,   138,
     109,    83,   107,    14,    39,    70,   159,   171,   172,    11,
      55,   104,   174,   160,    50,    51,    35,    36,   133,    13,
     132,   135,    25,    52,   134,    72,    32,    33,   145,    18,
     129,    34,    26,   146,    47,    48,    49,    47,    48,    49,
      37,    92,    93,    94,    95,    75,   144,   119,     1,   148,
     111,     3,   112,    77,   113,   114,   115,    50,    51,   116,
      50,    51,    89,    90,    75,   117,    52,    72,   106,    52,
      80,     1,     2,   162,     3,   161,    36,   129,   129,    85,
      86,    87,   129,    55,    47,    48,    49,   164,   165,    19,
      16,    17,   100,   119,   166,    23,    24,   167,     1,   101,
     111,     3,   112,   103,   113,   114,   115,    50,    51,   116,
      47,    48,    49,   130,    75,   153,    52,    45,    33,   149,
      47,    48,    49,    47,    48,    49,   111,   110,   112,   137,
     113,   114,   115,    50,    51,   116,    47,    48,    49,   150,
      75,   151,    52,    50,    51,   147,    50,    51,    46,    36,
      97,    98,    52,    72,   156,    52,    32,    33,   152,    50,
      51,   157,   155,   158,   163,   173,    31,   170,    52,    38,
      15,   168,   131,   154,   169,    71,   139,   141,     0,   142,
       0,   143,   140
};

static const yytype_int16 yycheck[] =
{
      32,    33,     0,    45,    36,     3,    46,     5,     8,    68,
      39,    29,    12,    45,    75,    17,    17,    17,    36,    29,
      52,     3,     4,     5,    24,    17,    36,    31,     0,    88,
      72,    35,    72,     5,    36,    36,    29,   168,   169,    26,
      72,    70,   173,    36,    26,    27,    30,    31,    80,     0,
      79,    83,    35,    35,    36,    33,    30,    31,    29,    26,
     121,    35,    26,    34,     3,     4,     5,     3,     4,     5,
      26,     9,    10,    11,    12,    33,   105,    75,    17,   111,
      19,    20,    21,    26,    23,    24,    25,    26,    27,    28,
      26,    27,     3,     4,    33,    34,    35,    33,    34,    35,
      31,    17,    18,   145,    20,   145,    31,   168,   169,     6,
       7,     8,   173,   145,     3,     4,     5,   149,   150,   119,
      28,    29,    15,   121,   156,    28,    29,   159,    17,    16,
      19,    20,    21,    32,    23,    24,    25,    26,    27,    28,
       3,     4,     5,    31,    33,    34,    35,    30,    31,    35,
       3,     4,     5,     3,     4,     5,    19,    32,    21,    36,
      23,    24,    25,    26,    27,    28,     3,     4,     5,    35,
      33,    28,    35,    26,    27,    28,    26,    27,    30,    31,
      13,    14,    35,    33,    30,    35,    30,    31,    28,    26,
      27,    32,    28,    32,    28,    22,    17,    28,    35,    24,
       5,    36,    78,   121,    36,    34,    91,    99,    -1,   100,
      -1,   101,    96
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    17,    18,    20,    38,    39,    40,    41,    42,    50,
      54,    26,    42,     0,    40,    54,    28,    29,    26,    46,
      51,    52,    53,    28,    29,    35,    26,    43,    44,    45,
      46,    43,    30,    31,    35,    30,    31,    26,    51,    36,
      42,    55,    56,    57,    58,    30,    30,     3,     4,     5,
      26,    27,    35,    46,    49,    67,    68,    69,    70,    71,
      72,    74,    75,    76,    77,    78,    79,    80,    83,    67,
      36,    55,    33,    47,    67,    33,    59,    26,    29,    36,
      31,    49,    47,    35,    67,     6,     7,     8,    85,     3,
       4,    84,     9,    10,    11,    12,    82,    13,    14,    81,
      15,    16,    71,    32,    59,    36,    34,    47,    48,    49,
      32,    19,    21,    23,    24,    25,    28,    34,    40,    42,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
      31,    56,    59,    67,    36,    67,    73,    36,    71,    74,
      75,    76,    77,    78,    59,    29,    34,    28,    67,    35,
      35,    28,    28,    34,    61,    28,    30,    32,    32,    29,
      36,    47,    49,    28,    67,    67,    67,    67,    36,    36,
      28,    62,    62,    22,    62
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    37,    38,    39,    39,    39,    39,    40,    40,    41,
      41,    42,    43,    43,    44,    45,    46,    46,    47,    47,
      48,    48,    48,    48,    49,    50,    50,    51,    51,    52,
      52,    53,    53,    54,    54,    54,    54,    55,    55,    56,
      56,    57,    58,    58,    59,    59,    60,    60,    61,    61,
      62,    62,    62,    62,    62,    62,    62,    62,    62,    63,
      63,    64,    65,    65,    66,    67,    68,    68,    69,    69,
      69,    70,    71,    71,    71,    72,    72,    73,    73,    74,
      74,    75,    75,    76,    76,    77,    77,    78,    78,    79,
      79,    80,    81,    81,    82,    82,    82,    82,    83,    83,
      83,    84,    84,    85,    85,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     2,     2,     3,
       3,     1,     1,     1,     3,     3,     4,     4,     2,     3,
       1,     3,     1,     3,     1,     2,     3,     1,     1,     3,
       1,     3,     1,     5,     6,     5,     6,     1,     3,     1,
       1,     2,     4,     4,     3,     2,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     2,     2,     2,     1,     3,
       2,     4,     5,     7,     5,     1,     1,     1,     3,     1,
       1,     1,     1,     2,     1,     4,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (ast, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, ast); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (ast);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, ast);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, int yyrule, std::unique_ptr<BaseAST> &ast)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[+yyssp[yyi + 1 - yynrhs]],
                       &yyvsp[(yyi + 1) - (yynrhs)]
                                              , ast);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ast); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
#  else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                yy_state_t *yyssp, int yytoken)
{
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Actual size of YYARG. */
  int yycount = 0;
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[+*yyssp];
      YYPTRDIFF_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
      yysize = yysize0;
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYPTRDIFF_T yysize1
                    = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    /* Don't count the "%s"s in the final size, but reserve room for
       the terminator.  */
    YYPTRDIFF_T yysize1 = yysize + (yystrlen (yyformat) - 2 * yycount) + 1;
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, std::unique_ptr<BaseAST> &ast)
{
  YYUSE (yyvaluep);
  YYUSE (ast);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (std::unique_ptr<BaseAST> &ast)
{
    yy_state_fast_t yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss;
    yy_state_t *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYPTRDIFF_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2:
#line 90 "/root/compiler/PKUminic/src/sysy.y"
               {
    ast = unique_ptr<CompUnitAST>((yyvsp[0].ast_comp));
}
#line 1513 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 3:
#line 95 "/root/compiler/PKUminic/src/sysy.y"
           {
        (yyval.ast_comp) = new CompUnitAST(yyget_lineno());
        (yyval.ast_comp)->compunitlist.push_back(static_cast<BaseAST*>((yyvsp[0].ast_decl)));
    }
#line 1522 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 4:
#line 99 "/root/compiler/PKUminic/src/sysy.y"
                    {
        (yyval.ast_comp) = ((yyvsp[-1].ast_comp));//左递归
        (yyval.ast_comp)->compunitlist.push_back(static_cast<BaseAST*>((yyvsp[0].ast_decl))); //将全局声明放入
    }
#line 1531 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 5:
#line 103 "/root/compiler/PKUminic/src/sysy.y"
             { 
        (yyval.ast_comp) = new CompUnitAST(yyget_lineno());
        (yyval.ast_comp)->compunitlist.push_back(static_cast<BaseAST*>((yyvsp[0].ast_val)));
    }
#line 1540 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 6:
#line 107 "/root/compiler/PKUminic/src/sysy.y"
                      {
        (yyval.ast_comp) = ((yyvsp[-1].ast_comp));//左递归
        (yyval.ast_comp)->compunitlist.push_back(static_cast<BaseAST*>((yyvsp[0].ast_val))); //将函数块放入
    }
#line 1549 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 7:
#line 114 "/root/compiler/PKUminic/src/sysy.y"
                  {//将分号移至上方, 使 constdecl变成左递归。
        (yyval.ast_decl) = ((yyvsp[-1].ast_decl));
    }
#line 1557 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 8:
#line 118 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.ast_decl) = ((yyvsp[-1].ast_decl));
    }
#line 1565 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 9:
#line 124 "/root/compiler/PKUminic/src/sysy.y"
                           {//为左值新建一个声明类
        auto const_decl = new DeclareAST(yyget_lineno());
        const_decl->btype = ((yyvsp[-1].key_op));
        const_decl->define_list_.push_back(((yyvsp[0].ast_define)));//并将定义语句放入其中
        (yyval.ast_decl) = const_decl;
    }
#line 1576 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 10:
#line 131 "/root/compiler/PKUminic/src/sysy.y"
                          {
        (yyval.ast_decl) = ((yyvsp[-2].ast_decl));//左递归
        (yyval.ast_decl)->define_list_.push_back((yyvsp[0].ast_define));//将定义语句的放入其中
    }
#line 1585 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 12:
#line 142 "/root/compiler/PKUminic/src/sysy.y"
                 { (yyval.ast_define) = ((yyvsp[0].ast_define)); }
#line 1591 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 13:
#line 143 "/root/compiler/PKUminic/src/sysy.y"
                   { (yyval.ast_define) = ((yyvsp[0].ast_define)); }
#line 1597 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 14:
#line 147 "/root/compiler/PKUminic/src/sysy.y"
                       {
        (yyval.ast_define) = new DefOneInitAST(*unique_ptr<string>((yyvsp[-2].str_val)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), true, yyget_lineno());
    }
#line 1605 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 15:
#line 152 "/root/compiler/PKUminic/src/sysy.y"
                                 {
        (yyval.ast_define) = new DefArrayInitAST(unique_ptr<IdentArrayAST>((yyvsp[-2].ast_identarray)), unique_ptr<InitValArrayAST>((yyvsp[0].initvalarray)), true, yyget_lineno());
    }
#line 1613 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 16:
#line 157 "/root/compiler/PKUminic/src/sysy.y"
                       {
        (yyval.ast_identarray) = new IdentArrayAST(*unique_ptr<string>((yyvsp[-3].str_val)), yyget_lineno());
        (yyval.ast_identarray)->shape_list.push_back((yyvsp[-1].ast_exp));
    }
#line 1622 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 17:
#line 161 "/root/compiler/PKUminic/src/sysy.y"
                            {//左递归 将数组维数加入至数组中
        (yyval.ast_identarray) = ((yyvsp[-3].ast_identarray));
        (yyval.ast_identarray)->shape_list.push_back((yyvsp[-1].ast_exp));
    }
#line 1631 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 18:
#line 167 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.initvalarray) = new InitValArrayAST(false, NULL, yyget_lineno());
    }
#line 1639 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 19:
#line 170 "/root/compiler/PKUminic/src/sysy.y"
                               {
        (yyval.initvalarray) = new InitValArrayAST(false, NULL, yyget_lineno());
        (yyval.initvalarray)->initvalarraylist.swap(*(yyvsp[-1].initcalarraylist));
        delete (yyvsp[-1].initcalarraylist);
    }
#line 1649 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 20:
#line 179 "/root/compiler/PKUminic/src/sysy.y"
             {
        (yyval.initcalarraylist) = new vector<InitValArrayAST *>;
        (yyval.initcalarraylist)->push_back(new InitValArrayAST(true, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno()));
    }
#line 1658 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 21:
#line 183 "/root/compiler/PKUminic/src/sysy.y"
                                  {
        (yyval.initcalarraylist) = ((yyvsp[-2].initcalarraylist));
        (yyval.initcalarraylist)->push_back(new InitValArrayAST(true, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno()));
    }
#line 1667 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 22:
#line 187 "/root/compiler/PKUminic/src/sysy.y"
                  {
        (yyval.initcalarraylist) = new vector<InitValArrayAST *>;
        (yyval.initcalarraylist)->push_back((yyvsp[0].initvalarray));
    }
#line 1676 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 23:
#line 191 "/root/compiler/PKUminic/src/sysy.y"
                                       {
        (yyval.initcalarraylist) = ((yyvsp[-2].initcalarraylist));
        (yyval.initcalarraylist)->push_back((yyvsp[0].initvalarray));
    }
#line 1685 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 24:
#line 198 "/root/compiler/PKUminic/src/sysy.y"
         { //这里用了addexp,应该接exp。需修改TODO
        (yyval.ast_exp) = ((yyvsp[0].ast_exp)); 
    }
#line 1693 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 25:
#line 203 "/root/compiler/PKUminic/src/sysy.y"
                {
        auto var_decl = new DeclareAST(yyget_lineno());
        var_decl->btype = ((yyvsp[-1].key_op));
        var_decl->define_list_.push_back(((yyvsp[0].ast_define)));//并将定义语句放入其中
        (yyval.ast_decl) = var_decl;
    }
#line 1704 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 26:
#line 210 "/root/compiler/PKUminic/src/sysy.y"
                    {
        (yyval.ast_decl) = ((yyvsp[-2].ast_decl));//左递归
        (yyval.ast_decl)->define_list_.push_back((yyvsp[0].ast_define));//将定义语句的放入其中
    }
#line 1713 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 27:
#line 216 "/root/compiler/PKUminic/src/sysy.y"
            {
        (yyval.ast_define) = ((yyvsp[0].ast_define));
    }
#line 1721 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 28:
#line 219 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.ast_define) = ((yyvsp[0].ast_define));
    }
#line 1729 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 29:
#line 224 "/root/compiler/PKUminic/src/sysy.y"
                       {
        (yyval.ast_define) = new DefOneInitAST(*unique_ptr<string>((yyvsp[-2].str_val)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), false, yyget_lineno());
    }
#line 1737 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 30:
#line 227 "/root/compiler/PKUminic/src/sysy.y"
           {
        (yyval.ast_define) = new DefOneAST(*unique_ptr<string>((yyvsp[0].str_val)), yyget_lineno());
    }
#line 1745 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 31:
#line 232 "/root/compiler/PKUminic/src/sysy.y"
                                 {
        (yyval.ast_define) = new DefArrayInitAST(unique_ptr<IdentArrayAST>((yyvsp[-2].ast_identarray)), unique_ptr<InitValArrayAST>((yyvsp[0].initvalarray)), false, yyget_lineno());
    }
#line 1753 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 32:
#line 235 "/root/compiler/PKUminic/src/sysy.y"
                {
        (yyval.ast_define) = new DefArray(unique_ptr<IdentArrayAST>((yyvsp[0].ast_identarray)), yyget_lineno());
    }
#line 1761 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 33:
#line 241 "/root/compiler/PKUminic/src/sysy.y"
                               {
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "int";
        ast->ident = *unique_ptr<string>((yyvsp[-3].str_val));
        ast->funcfparamlist = NULL;
        ast->block = unique_ptr<StmtAST>((yyvsp[0].ast_block));
        (yyval.ast_val) = ast;
    }
#line 1774 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 34:
#line 249 "/root/compiler/PKUminic/src/sysy.y"
                                               {
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "int";
        ast->ident = *unique_ptr<string>((yyvsp[-4].str_val));
        ast->funcfparamlist = unique_ptr<FuncFParamsListAST>((yyvsp[-2].ast_funcFlist));
        ast->block = unique_ptr<StmtAST>((yyvsp[0].ast_block));
        (yyval.ast_val) = ast;
    }
#line 1787 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 35:
#line 257 "/root/compiler/PKUminic/src/sysy.y"
                              {
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "void";
        // ast->func_type = unique_ptr<FuncTypeAST>($1);
        ast->ident = *unique_ptr<string>((yyvsp[-3].str_val));
        ast->funcfparamlist = NULL;
        ast->block = unique_ptr<StmtAST>((yyvsp[0].ast_block));
        (yyval.ast_val) = ast;
    }
#line 1801 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 36:
#line 266 "/root/compiler/PKUminic/src/sysy.y"
                                              {
        auto ast = new FuncDefAST(yyget_lineno());
        ast->func_type = "void";
        // ast->func_type = unique_ptr<FuncTypeAST>($1);
        ast->ident = *unique_ptr<string>((yyvsp[-4].str_val));
        ast->funcfparamlist = unique_ptr<FuncFParamsListAST>((yyvsp[-2].ast_funcFlist));
        ast->block = unique_ptr<StmtAST>((yyvsp[0].ast_block));
        (yyval.ast_val) = ast;
    }
#line 1815 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 37:
#line 278 "/root/compiler/PKUminic/src/sysy.y"
                {
        (yyval.ast_funcFlist) = new FuncFParamsListAST(yyget_lineno());
        (yyval.ast_funcFlist)->funcfparamlist.push_back((yyvsp[0].ast_funcF));

    }
#line 1825 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 38:
#line 283 "/root/compiler/PKUminic/src/sysy.y"
                                    {
        (yyval.ast_funcFlist) = ((yyvsp[-2].ast_funcFlist));
        (yyval.ast_funcFlist)->funcfparamlist.push_back((yyvsp[0].ast_funcF));//左递归
    }
#line 1834 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 39:
#line 289 "/root/compiler/PKUminic/src/sysy.y"
                   {
        (yyval.ast_funcF) = static_cast<FuncFParamAST*>((yyvsp[0].ast_funcFone));
    }
#line 1842 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 40:
#line 292 "/root/compiler/PKUminic/src/sysy.y"
                     {
        (yyval.ast_funcF) = static_cast<FuncFParamAST*>((yyvsp[0].ast_funcFarray));
    }
#line 1850 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 41:
#line 299 "/root/compiler/PKUminic/src/sysy.y"
                 {
        (yyval.ast_funcFone) = new FuncFParamOneAST(*unique_ptr<string>((yyvsp[0].str_val)), yyget_lineno());
    }
#line 1858 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 42:
#line 304 "/root/compiler/PKUminic/src/sysy.y"
                          {
        (yyval.ast_funcFarray) = new FuncFParamArrayAST(*unique_ptr<string>((yyvsp[-2].str_val)), yyget_lineno());
    }
#line 1866 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 43:
#line 307 "/root/compiler/PKUminic/src/sysy.y"
                                 {
        (yyval.ast_funcFarray) = ((yyvsp[-3].ast_funcFarray));
        (yyval.ast_funcFarray)->shape_list.push_back((yyvsp[-1].ast_exp));
    }
#line 1875 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 44:
#line 314 "/root/compiler/PKUminic/src/sysy.y"
                           {
        (yyval.ast_block) = ((yyvsp[-1].ast_block));
    }
#line 1883 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 45:
#line 317 "/root/compiler/PKUminic/src/sysy.y"
             {
        auto ast = new BlockItemAST(yyget_lineno());//blockitemlist中为空
        (yyval.ast_block) = ast;
    }
#line 1892 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 46:
#line 323 "/root/compiler/PKUminic/src/sysy.y"
               {
        auto ast = new BlockItemAST(yyget_lineno());
        ast->blockitemlist.push_back((yyvsp[0].ast_stmt));
        (yyval.ast_block) = ast;
    }
#line 1902 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 47:
#line 328 "/root/compiler/PKUminic/src/sysy.y"
                             {
        (yyval.ast_block) = ((yyvsp[-1].ast_block));//左递归
        (yyval.ast_block)->blockitemlist.push_back((yyvsp[0].ast_stmt)); //将语句块放入
    }
#line 1911 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 48:
#line 335 "/root/compiler/PKUminic/src/sysy.y"
          {
        (yyval.ast_stmt) = static_cast<StmtAST*>((yyvsp[0].ast_decl));
    }
#line 1919 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 49:
#line 338 "/root/compiler/PKUminic/src/sysy.y"
          { (yyval.ast_stmt) = ((yyvsp[0].ast_stmt)); }
#line 1925 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 54:
#line 346 "/root/compiler/PKUminic/src/sysy.y"
           {
        (yyval.ast_stmt) = static_cast<StmtAST*>((yyvsp[0].ast_block));
    }
#line 1933 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 55:
#line 349 "/root/compiler/PKUminic/src/sysy.y"
             {
        (yyval.ast_stmt) = static_cast<StmtAST*>(new ExpstmtAST(unique_ptr<ExpAST>((yyvsp[-1].ast_exp)), yyget_lineno()));
    }
#line 1941 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 56:
#line 352 "/root/compiler/PKUminic/src/sysy.y"
               {
        (yyval.ast_stmt) = static_cast<StmtAST*>(new BreakstmtAST(yyget_lineno()));
    }
#line 1949 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 57:
#line 355 "/root/compiler/PKUminic/src/sysy.y"
                  {
        (yyval.ast_stmt) = static_cast<StmtAST*>(new ContinuestmtAST(yyget_lineno()));
    }
#line 1957 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 58:
#line 358 "/root/compiler/PKUminic/src/sysy.y"
         {
        (yyval.ast_stmt) = static_cast<StmtAST*>(new VoidstmtAST(yyget_lineno()));
    }
#line 1965 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 59:
#line 363 "/root/compiler/PKUminic/src/sysy.y"
                    {
        auto ast = new ReturnStmtAST(yyget_lineno());
        ast->Exp = unique_ptr<ExpAST>((yyvsp[-1].ast_exp));
        (yyval.ast_stmt) = ast;
    }
#line 1975 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 60:
#line 368 "/root/compiler/PKUminic/src/sysy.y"
                {
        auto ast = new ReturnStmtAST(yyget_lineno());
        ast->Exp = NULL;//表示该部分无返回值
        (yyval.ast_stmt) = ast;
    }
#line 1985 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 61:
#line 375 "/root/compiler/PKUminic/src/sysy.y"
                     {
        (yyval.ast_stmt) = new AssignStmtAST(unique_ptr<LeftValAST>((yyvsp[-3].ast_lval)), unique_ptr<ExpAST>((yyvsp[-1].ast_exp)), yyget_lineno());
    }
#line 1993 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 62:
#line 380 "/root/compiler/PKUminic/src/sysy.y"
                         {
        (yyval.ast_stmt) = new IfElseStmtAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), unique_ptr<StmtAST>((yyvsp[0].ast_stmt)), NULL, yyget_lineno());
    }
#line 2001 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 63:
#line 383 "/root/compiler/PKUminic/src/sysy.y"
                                   {
        (yyval.ast_stmt) = new IfElseStmtAST(unique_ptr<ExpAST>((yyvsp[-4].ast_exp)), unique_ptr<StmtAST>((yyvsp[-2].ast_stmt)), unique_ptr<StmtAST>((yyvsp[0].ast_stmt)), yyget_lineno());
    }
#line 2009 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 64:
#line 388 "/root/compiler/PKUminic/src/sysy.y"
                            {
        (yyval.ast_stmt) = new WhileStmtAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), unique_ptr<StmtAST>((yyvsp[0].ast_stmt)), yyget_lineno());
    }
#line 2017 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 65:
#line 393 "/root/compiler/PKUminic/src/sysy.y"
            { 
        (yyval.ast_exp) = ((yyvsp[0].ast_exp)); 
    }
#line 2025 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 66:
#line 398 "/root/compiler/PKUminic/src/sysy.y"
           {
        (yyval.ast_lval) = new LeftValAST(unique_ptr<IdentifierAST>((yyvsp[0].ast_ident)), NULL, yyget_lineno());
    }
#line 2033 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 67:
#line 401 "/root/compiler/PKUminic/src/sysy.y"
                {
        (yyval.ast_lval) = new LeftValAST(NULL, unique_ptr<IdentArrayAST>((yyvsp[0].ast_identarray)), yyget_lineno());
    }
#line 2041 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 68:
#line 406 "/root/compiler/PKUminic/src/sysy.y"
                 {
        (yyval.ast_exp) = new PrimaryExpAST(0, unique_ptr<ExpAST>((yyvsp[-1].ast_exp)), NULL, yyget_lineno());
    }
#line 2049 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 69:
#line 409 "/root/compiler/PKUminic/src/sysy.y"
          {
        (yyval.ast_exp) = new PrimaryExpAST(0, NULL, unique_ptr<LeftValAST>((yyvsp[0].ast_lval)), yyget_lineno());
    }
#line 2057 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 70:
#line 412 "/root/compiler/PKUminic/src/sysy.y"
            { 
        (yyval.ast_exp) = new PrimaryExpAST(((yyvsp[0].int_val)), NULL, NULL, yyget_lineno());
    }
#line 2065 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 71:
#line 418 "/root/compiler/PKUminic/src/sysy.y"
           {
        (yyval.ast_ident) = new IdentifierAST(*unique_ptr<string>((yyvsp[0].str_val)), yyget_lineno());
    }
#line 2073 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 72:
#line 423 "/root/compiler/PKUminic/src/sysy.y"
                { 
        (yyval.ast_exp) = new UnaryExpAST(unique_ptr<ExpAST>((yyvsp[0].ast_exp)), 0, NULL, NULL, yyget_lineno());
        
    }
#line 2082 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 73:
#line 427 "/root/compiler/PKUminic/src/sysy.y"
                      {
        (yyval.ast_exp) = new UnaryExpAST(NULL, ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
        
    }
#line 2091 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 74:
#line 431 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.ast_exp) = new UnaryExpAST(NULL, 0, NULL, unique_ptr<FuncCallAST>((yyvsp[0].ast_funcR)), yyget_lineno());
    }
#line 2099 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 75:
#line 436 "/root/compiler/PKUminic/src/sysy.y"
                                   {
        (yyval.ast_funcR) = new FuncCallAST(*unique_ptr<string>((yyvsp[-3].str_val)), unique_ptr<FuncRParamsListAST>((yyvsp[-1].ast_funcRlist)), yyget_lineno());
    }
#line 2107 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 76:
#line 439 "/root/compiler/PKUminic/src/sysy.y"
                   {
        (yyval.ast_funcR) = new FuncCallAST(*unique_ptr<string>((yyvsp[-2].str_val)), NULL, yyget_lineno());
    }
#line 2115 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 77:
#line 444 "/root/compiler/PKUminic/src/sysy.y"
          {
        (yyval.ast_funcRlist) = new FuncRParamsListAST(yyget_lineno());
        (yyval.ast_funcRlist)->rparamslist.push_back((yyvsp[0].ast_exp));
    }
#line 2124 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 78:
#line 448 "/root/compiler/PKUminic/src/sysy.y"
                              {
        (yyval.ast_funcRlist) = ((yyvsp[-2].ast_funcRlist));
        (yyval.ast_funcRlist)->rparamslist.push_back((yyvsp[0].ast_exp));
    }
#line 2133 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 79:
#line 454 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.ast_exp) = new BinaryExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());
    }
#line 2141 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 80:
#line 457 "/root/compiler/PKUminic/src/sysy.y"
                             {
        (yyval.ast_exp) = new BinaryExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
    }
#line 2149 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 81:
#line 462 "/root/compiler/PKUminic/src/sysy.y"
             {
        (yyval.ast_exp) = new BinaryExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());
    }
#line 2157 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 82:
#line 465 "/root/compiler/PKUminic/src/sysy.y"
                          {
        (yyval.ast_exp) = new BinaryExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
     }
#line 2165 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 83:
#line 470 "/root/compiler/PKUminic/src/sysy.y"
             {
        (yyval.ast_exp) = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());

    }
#line 2174 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 84:
#line 474 "/root/compiler/PKUminic/src/sysy.y"
                         {
        (yyval.ast_exp) = new CondExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
     }
#line 2182 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 85:
#line 479 "/root/compiler/PKUminic/src/sysy.y"
            {
        (yyval.ast_exp) = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());
    }
#line 2190 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 86:
#line 482 "/root/compiler/PKUminic/src/sysy.y"
                          {
        (yyval.ast_exp) = new CondExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
    }
#line 2198 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 87:
#line 487 "/root/compiler/PKUminic/src/sysy.y"
            {
        (yyval.ast_exp) = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());
    }
#line 2206 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 88:
#line 490 "/root/compiler/PKUminic/src/sysy.y"
                       {
        (yyval.ast_exp) = new CondExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
    }
#line 2214 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 89:
#line 495 "/root/compiler/PKUminic/src/sysy.y"
              {
        (yyval.ast_exp) = new CondExpAST(NULL, 0, NULL, unique_ptr<ExpAST>((yyvsp[0].ast_exp)), yyget_lineno());
    }
#line 2222 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 90:
#line 498 "/root/compiler/PKUminic/src/sysy.y"
                       {
        (yyval.ast_exp) = new CondExpAST(unique_ptr<ExpAST>((yyvsp[-2].ast_exp)), ((yyvsp[-1].key_op)), unique_ptr<ExpAST>((yyvsp[0].ast_exp)), NULL, yyget_lineno());
    }
#line 2230 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 91:
#line 503 "/root/compiler/PKUminic/src/sysy.y"
                {
        (yyval.int_val) = ((yyvsp[0].int_val));
    }
#line 2238 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 92:
#line 508 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2244 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 93:
#line 509 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2250 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 94:
#line 512 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2256 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 95:
#line 513 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2262 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 96:
#line 514 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2268 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 97:
#line 515 "/root/compiler/PKUminic/src/sysy.y"
        { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2274 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 98:
#line 518 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2280 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 99:
#line 519 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2286 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 100:
#line 520 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2292 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 101:
#line 523 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2298 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 102:
#line 524 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2304 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 103:
#line 527 "/root/compiler/PKUminic/src/sysy.y"
          { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2310 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 104:
#line 528 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2316 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;

  case 105:
#line 529 "/root/compiler/PKUminic/src/sysy.y"
         { (yyval.key_op) = ((yyvsp[0].key_op)); }
#line 2322 "/root/compiler/PKUminic/build/sysy.tab.cpp"
    break;


#line 2326 "/root/compiler/PKUminic/build/sysy.tab.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (ast, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *, YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (ast, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, ast);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, ast);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (ast, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ast);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[+*yyssp], yyvsp, ast);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 531 "/root/compiler/PKUminic/src/sysy.y"



void yyerror(unique_ptr<BaseAST> &ast, const char *s){
    cerr << "error: (line:"<< yyget_lineno() <<") :" << s << endl;
}
