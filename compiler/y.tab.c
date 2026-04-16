/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_IDENTIFIER = 258,
     TOK_INT_LITERAL = 259,
     TOK_FLOAT_LITERAL = 260,
     TOK_CHAR_LITERAL = 261,
     TOK_IF = 262,
     TOK_THEN = 263,
     TOK_ELSE = 264,
     TOK_END = 265,
     TOK_FOR = 266,
     TOK_TO = 267,
     TOK_WHILE = 268,
     TOK_DO = 269,
     TOK_FUNCTION = 270,
     TOK_PRINT = 271,
     TOK_RETURN = 272,
     TOK_ASSIGN = 273,
     TOK_PLUS = 274,
     TOK_MINUS = 275,
     TOK_STAR = 276,
     TOK_SLASH = 277,
     TOK_LPAREN = 278,
     TOK_RPAREN = 279,
     TOK_COMMA = 280,
     TOK_EQ = 281,
     TOK_NEQ = 282,
     TOK_LT = 283,
     TOK_GT = 284,
     TOK_LE = 285,
     TOK_GE = 286,
     TOK_ADDR_OF = 287,
     TOK_LBRACKET = 288,
     TOK_RBRACKET = 289,
     TOK_INT_TYPE = 290,
     TOK_FLOAT_TYPE = 291,
     TOK_CHAR_TYPE = 292,
     TOK_UNKNOWN = 293
   };
#endif
/* Tokens.  */
#define TOK_IDENTIFIER 258
#define TOK_INT_LITERAL 259
#define TOK_FLOAT_LITERAL 260
#define TOK_CHAR_LITERAL 261
#define TOK_IF 262
#define TOK_THEN 263
#define TOK_ELSE 264
#define TOK_END 265
#define TOK_FOR 266
#define TOK_TO 267
#define TOK_WHILE 268
#define TOK_DO 269
#define TOK_FUNCTION 270
#define TOK_PRINT 271
#define TOK_RETURN 272
#define TOK_ASSIGN 273
#define TOK_PLUS 274
#define TOK_MINUS 275
#define TOK_STAR 276
#define TOK_SLASH 277
#define TOK_LPAREN 278
#define TOK_RPAREN 279
#define TOK_COMMA 280
#define TOK_EQ 281
#define TOK_NEQ 282
#define TOK_LT 283
#define TOK_GT 284
#define TOK_LE 285
#define TOK_GE 286
#define TOK_ADDR_OF 287
#define TOK_LBRACKET 288
#define TOK_RBRACKET 289
#define TOK_INT_TYPE 290
#define TOK_FLOAT_TYPE 291
#define TOK_CHAR_TYPE 292
#define TOK_UNKNOWN 293




/* Copy the first part of user declarations.  */
#line 1 "parser.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int current_line;
void yyerror(const char *s);

ASTNode* final_root = NULL;

// Helper to add a statement to a list
ASTNode** add_stmt(ASTNode** list, int* count, ASTNode* stmt) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = stmt;
    return list;
}

// Helper to add a parameter to a list
char** add_param(char** list, int* count, char* param) {
    list = (char**)realloc(list, sizeof(char*) * (*count + 1));
    list[(*count)++] = strdup(param);
    return list;
}

// Helper to add an argument to a list
ASTNode** add_arg(ASTNode** list, int* count, ASTNode* arg) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = arg;
    return list;
}

// Helper for arrays
int* add_size(int* list, int* count, int size) {
    list = (int*)realloc(list, sizeof(int) * (*count + 1));
    list[(*count)++] = size;
    return list;
}

ASTNode** add_index(ASTNode** list, int* count, ASTNode* index) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = index;
    return list;
}


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 48 "parser.y"
{
    int num;
    float fnum;
    char ch;
    char* id;
    struct ASTNode* node;
    struct {
        struct ASTNode** list;
        int count;
    } stmt_list;
    struct {
        char** list;
        int count;
    } param_list;
    struct {
        struct ASTNode** list;
        int count;
    } arg_list;
    struct {
        int* list;
        int count;
    } size_list;
    struct {
        struct ASTNode** list;
        int count;
    } index_list;
    int level;
}
/* Line 193 of yacc.c.  */
#line 248 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 261 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   297

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  15
/* YYNRULES -- Number of rules.  */
#define YYNRULES  59
/* YYNRULES -- Number of states.  */
#define YYNSTATES  125

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     6,     9,    18,    25,    35,    42,
      51,    56,    59,    63,    65,    67,    70,    73,    75,    78,
      82,    87,    90,    93,    96,   100,   104,   108,   112,   116,
     120,   124,   129,   130,   132,   136,   138,   142,   146,   150,
     154,   158,   162,   166,   170,   172,   176,   180,   182,   187,
     190,   193,   196,   197,   199,   203,   205,   207,   209,   211
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      40,     0,    -1,    41,    -1,    -1,    41,    42,    -1,     7,
      49,     8,    41,     9,    41,    10,     7,    -1,     7,    49,
       8,    41,    10,     7,    -1,    11,     3,    18,    49,    12,
      49,    41,    10,    11,    -1,    13,    49,    14,    41,    10,
      13,    -1,    15,     3,    23,    48,    24,    41,    10,    15,
      -1,    16,    23,    49,    24,    -1,    17,    49,    -1,    43,
      18,    49,    -1,    46,    -1,     3,    -1,    44,     3,    -1,
       3,    45,    -1,    21,    -1,    44,    21,    -1,    33,    49,
      34,    -1,    45,    33,    49,    34,    -1,    35,     3,    -1,
      36,     3,    -1,    37,     3,    -1,    35,     3,    47,    -1,
      36,     3,    47,    -1,    37,     3,    47,    -1,    35,    44,
       3,    -1,    36,    44,     3,    -1,    37,    44,     3,    -1,
      33,     4,    34,    -1,    47,    33,     4,    34,    -1,    -1,
       3,    -1,    48,    25,     3,    -1,    50,    -1,    49,    19,
      50,    -1,    49,    20,    50,    -1,    49,    26,    50,    -1,
      49,    27,    50,    -1,    49,    28,    50,    -1,    49,    29,
      50,    -1,    49,    30,    50,    -1,    49,    31,    50,    -1,
      51,    -1,    50,    21,    51,    -1,    50,    22,    51,    -1,
      53,    -1,     3,    23,    52,    24,    -1,    32,     3,    -1,
      44,     3,    -1,     3,    45,    -1,    -1,    49,    -1,    52,
      25,    49,    -1,     4,    -1,     5,    -1,     6,    -1,     3,
      -1,    23,    49,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   103,   103,   112,   116,   123,   133,   143,   153,   161,
     171,   177,   183,   190,   197,   202,   209,   219,   220,   224,
     227,   234,   241,   248,   255,   263,   271,   279,   286,   293,
     303,   306,   313,   317,   320,   327,   328,   336,   344,   352,
     359,   366,   373,   380,   390,   391,   398,   408,   409,   416,
     421,   428,   438,   442,   445,   452,   457,   462,   468,   473
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TOK_IDENTIFIER", "TOK_INT_LITERAL",
  "TOK_FLOAT_LITERAL", "TOK_CHAR_LITERAL", "TOK_IF", "TOK_THEN",
  "TOK_ELSE", "TOK_END", "TOK_FOR", "TOK_TO", "TOK_WHILE", "TOK_DO",
  "TOK_FUNCTION", "TOK_PRINT", "TOK_RETURN", "TOK_ASSIGN", "TOK_PLUS",
  "TOK_MINUS", "TOK_STAR", "TOK_SLASH", "TOK_LPAREN", "TOK_RPAREN",
  "TOK_COMMA", "TOK_EQ", "TOK_NEQ", "TOK_LT", "TOK_GT", "TOK_LE", "TOK_GE",
  "TOK_ADDR_OF", "TOK_LBRACKET", "TOK_RBRACKET", "TOK_INT_TYPE",
  "TOK_FLOAT_TYPE", "TOK_CHAR_TYPE", "TOK_UNKNOWN", "$accept", "program",
  "statement_list", "statement", "lvalue", "stars", "indices",
  "declaration", "brackets", "param_list", "expression", "term", "factor",
  "arg_list", "primary", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    39,    40,    41,    41,    42,    42,    42,    42,    42,
      42,    42,    42,    42,    43,    43,    43,    44,    44,    45,
      45,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      47,    47,    48,    48,    48,    49,    49,    49,    49,    49,
      49,    49,    49,    49,    50,    50,    50,    51,    51,    51,
      51,    51,    52,    52,    52,    53,    53,    53,    53,    53
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     2,     8,     6,     9,     6,     8,
       4,     2,     3,     1,     1,     2,     2,     1,     2,     3,
       4,     2,     2,     2,     3,     3,     3,     3,     3,     3,
       3,     4,     0,     1,     3,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     1,     3,     3,     1,     4,     2,
       2,     2,     0,     1,     3,     1,     1,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     1,    14,     0,     0,     0,     0,     0,
       0,    17,     0,     0,     0,     4,     0,     0,    13,     0,
      16,    58,    55,    56,    57,     0,     0,     0,     0,    35,
      44,    47,     0,     0,     0,     0,    11,    21,     0,    22,
       0,    23,     0,     0,    15,    18,     0,     0,    52,    51,
       0,    49,    50,     3,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     3,    32,     0,     0,    24,
      27,    25,    28,    26,    29,    12,    19,     0,    53,     0,
      59,     0,    36,    37,    38,    39,    40,    41,    42,    43,
      45,    46,     0,     0,    33,     0,    10,     0,     0,    20,
      48,     0,     3,     0,     0,     0,     3,     0,    30,     0,
      54,     0,     6,     3,     8,     0,    34,    31,     0,     0,
       0,     5,     0,     9,     7
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     1,     2,    15,    16,    27,    20,    18,    69,    95,
      28,    29,    30,    79,    31
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -52
static const yytype_int16 yypact[] =
{
     -52,    31,   164,   -52,   -18,    44,    42,    44,    56,    50,
      44,   -52,     0,     2,     4,   -52,    57,    14,   -52,    44,
      64,   -17,   -52,   -52,   -52,    44,    74,    15,   176,   -13,
     -52,   -52,    60,   214,    78,    44,   266,    70,    19,    70,
      36,    70,    40,    44,   -52,   -52,   163,    44,    44,    64,
     240,   -52,   -52,   -52,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    44,    44,   -52,   108,   253,   110,    79,
     -52,    79,   -52,    79,   -52,   266,   -52,   227,   266,    47,
     -52,    17,   -13,   -13,    93,    93,    93,    93,    93,    93,
     -52,   -52,   196,    53,   -52,    69,   -52,    82,   114,   -52,
     -52,    44,   -52,   112,    44,   107,   -52,   118,   -52,    88,
     266,    89,   -52,   266,   -52,   120,   -52,   -52,   121,   137,
     117,   -52,   123,   -52,   -52
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -52,   -52,   -51,   -52,   -52,    -2,   122,   -52,     5,   -52,
      -6,    26,    45,   -52,   -52
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      17,    33,    81,    37,    36,    39,    48,    41,    62,    63,
      38,    40,    42,    46,    93,    19,    19,    44,    52,    50,
       4,    11,    70,    11,     5,    11,   102,   103,     6,    67,
       7,     3,     8,     9,    10,    45,    45,    75,    11,    72,
      45,    77,    78,    74,    71,    32,    73,    21,    22,    23,
      24,   111,    12,    13,    14,   115,     4,    45,    92,    34,
       5,    45,   119,   105,     6,    11,     7,    25,     8,     9,
      10,   100,   101,    35,    11,    43,    26,    51,    64,    17,
      82,    83,    84,    85,    86,    87,    88,    89,    12,    13,
      14,    17,     4,   106,   107,   110,     5,    47,   113,   118,
       6,    66,     7,    68,     8,     9,    10,    90,    91,    17,
      11,    94,    98,    17,    97,    63,   108,    17,   109,   112,
     114,   116,   117,     4,    12,    13,    14,     5,   121,     0,
     120,     6,   123,     7,   124,     8,     9,    10,     0,     0,
       4,    11,     0,    49,     5,     0,     0,   122,     6,     0,
       7,     0,     8,     9,    10,    12,    13,    14,    11,     0,
       0,     0,     0,     0,     0,     0,     0,     4,     0,     0,
       0,     5,    12,    13,    14,     6,     0,     7,     0,     8,
       9,    10,    54,    55,    53,    11,     0,     0,     0,    56,
      57,    58,    59,    60,    61,    54,    55,    76,     0,    12,
      13,    14,    56,    57,    58,    59,    60,    61,   104,     0,
       0,     0,     0,     0,     0,    54,    55,     0,     0,     0,
       0,     0,    56,    57,    58,    59,    60,    61,    65,     0,
       0,     0,     0,    54,    55,     0,     0,     0,     0,     0,
      56,    57,    58,    59,    60,    61,    54,    55,     0,     0,
       0,     0,     0,    56,    57,    58,    59,    60,    61,    54,
      55,    99,     0,     0,    80,     0,    56,    57,    58,    59,
      60,    61,    54,    55,     0,     0,     0,    96,     0,    56,
      57,    58,    59,    60,    61,    54,    55,     0,     0,     0,
       0,     0,    56,    57,    58,    59,    60,    61
};

static const yytype_int8 yycheck[] =
{
       2,     7,    53,     3,    10,     3,    23,     3,    21,    22,
      12,    13,    14,    19,    65,    33,    33,     3,     3,    25,
       3,    21,     3,    21,     7,    21,     9,    10,    11,    35,
      13,     0,    15,    16,    17,    21,    21,    43,    21,     3,
      21,    47,    48,     3,    39,     3,    41,     3,     4,     5,
       6,   102,    35,    36,    37,   106,     3,    21,    64,     3,
       7,    21,   113,    10,    11,    21,    13,    23,    15,    16,
      17,    24,    25,    23,    21,    18,    32,     3,    18,    81,
      54,    55,    56,    57,    58,    59,    60,    61,    35,    36,
      37,    93,     3,    24,    25,   101,     7,    33,   104,    10,
      11,    23,    13,    33,    15,    16,    17,    62,    63,   111,
      21,     3,    33,   115,     4,    22,    34,   119,     4,     7,
      13,     3,    34,     3,    35,    36,    37,     7,     7,    -1,
      10,    11,    15,    13,    11,    15,    16,    17,    -1,    -1,
       3,    21,    -1,    21,     7,    -1,    -1,    10,    11,    -1,
      13,    -1,    15,    16,    17,    35,    36,    37,    21,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,     3,    -1,    -1,
      -1,     7,    35,    36,    37,    11,    -1,    13,    -1,    15,
      16,    17,    19,    20,     8,    21,    -1,    -1,    -1,    26,
      27,    28,    29,    30,    31,    19,    20,    34,    -1,    35,
      36,    37,    26,    27,    28,    29,    30,    31,    12,    -1,
      -1,    -1,    -1,    -1,    -1,    19,    20,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    31,    14,    -1,
      -1,    -1,    -1,    19,    20,    -1,    -1,    -1,    -1,    -1,
      26,    27,    28,    29,    30,    31,    19,    20,    -1,    -1,
      -1,    -1,    -1,    26,    27,    28,    29,    30,    31,    19,
      20,    34,    -1,    -1,    24,    -1,    26,    27,    28,    29,
      30,    31,    19,    20,    -1,    -1,    -1,    24,    -1,    26,
      27,    28,    29,    30,    31,    19,    20,    -1,    -1,    -1,
      -1,    -1,    26,    27,    28,    29,    30,    31
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    40,    41,     0,     3,     7,    11,    13,    15,    16,
      17,    21,    35,    36,    37,    42,    43,    44,    46,    33,
      45,     3,     4,     5,     6,    23,    32,    44,    49,    50,
      51,    53,     3,    49,     3,    23,    49,     3,    44,     3,
      44,     3,    44,    18,     3,    21,    49,    33,    23,    45,
      49,     3,     3,     8,    19,    20,    26,    27,    28,    29,
      30,    31,    21,    22,    18,    14,    23,    49,    33,    47,
       3,    47,     3,    47,     3,    49,    34,    49,    49,    52,
      24,    41,    50,    50,    50,    50,    50,    50,    50,    50,
      51,    51,    49,    41,     3,    48,    24,     4,    33,    34,
      24,    25,     9,    10,    12,    10,    24,    25,    34,     4,
      49,    41,     7,    49,    13,    41,     3,    34,    10,    41,
      10,     7,    10,    15,    11
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

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
#line 103 "parser.y"
    {
        ASTNode* node = create_node(NODE_PROGRAM);
        node->data.program.statements = (yyvsp[(1) - (1)].stmt_list).list;
        node->data.program.stmt_count = (yyvsp[(1) - (1)].stmt_list).count;
        final_root = node;
    ;}
    break;

  case 3:
#line 112 "parser.y"
    {
        (yyval.stmt_list).list = NULL;
        (yyval.stmt_list).count = 0;
    ;}
    break;

  case 4:
#line 116 "parser.y"
    {
        (yyval.stmt_list).list = add_stmt((yyvsp[(1) - (2)].stmt_list).list, &(yyvsp[(1) - (2)].stmt_list).count, (yyvsp[(2) - (2)].node));
        (yyval.stmt_list).count = (yyvsp[(1) - (2)].stmt_list).count;
    ;}
    break;

  case 5:
#line 123 "parser.y"
    {
        printf("  [Parsing Rule] If-Then-Else statement matched.\n");
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = (yyvsp[(2) - (8)].node);
        node->data.if_stmt.then_branch = (yyvsp[(4) - (8)].stmt_list).list;
        node->data.if_stmt.then_count = (yyvsp[(4) - (8)].stmt_list).count;
        node->data.if_stmt.else_branch = (yyvsp[(6) - (8)].stmt_list).list;
        node->data.if_stmt.else_count = (yyvsp[(6) - (8)].stmt_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 6:
#line 133 "parser.y"
    {
        printf("  [Parsing Rule] If-Then statement matched.\n");
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = (yyvsp[(2) - (6)].node);
        node->data.if_stmt.then_branch = (yyvsp[(4) - (6)].stmt_list).list;
        node->data.if_stmt.then_count = (yyvsp[(4) - (6)].stmt_list).count;
        node->data.if_stmt.else_branch = NULL;
        node->data.if_stmt.else_count = 0;
        (yyval.node) = node;
    ;}
    break;

  case 7:
#line 143 "parser.y"
    {
        printf("  [Parsing Rule] For loop matched for variable: %s\n", (yyvsp[(2) - (9)].id));
        ASTNode* node = create_node(NODE_FOR_STMT);
        node->data.for_stmt.var = strdup((yyvsp[(2) - (9)].id));
        node->data.for_stmt.start = (yyvsp[(4) - (9)].node);
        node->data.for_stmt.end = (yyvsp[(6) - (9)].node);
        node->data.for_stmt.body = (yyvsp[(7) - (9)].stmt_list).list;
        node->data.for_stmt.body_count = (yyvsp[(7) - (9)].stmt_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 8:
#line 153 "parser.y"
    {
        printf("  [Parsing Rule] While loop matched.\n");
        ASTNode* node = create_node(NODE_WHILE_STMT);
        node->data.while_stmt.condition = (yyvsp[(2) - (6)].node);
        node->data.while_stmt.body = (yyvsp[(4) - (6)].stmt_list).list;
        node->data.while_stmt.body_count = (yyvsp[(4) - (6)].stmt_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 9:
#line 161 "parser.y"
    {
        printf("  [Parsing Rule] Function definition matched: %s\n", (yyvsp[(2) - (8)].id));
        ASTNode* node = create_node(NODE_FUNCTION_STMT);
        node->data.function.name = strdup((yyvsp[(2) - (8)].id));
        node->data.function.params = (yyvsp[(4) - (8)].param_list).list;
        node->data.function.param_count = (yyvsp[(4) - (8)].param_list).count;
        node->data.function.body = (yyvsp[(6) - (8)].stmt_list).list;
        node->data.function.body_count = (yyvsp[(6) - (8)].stmt_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 10:
#line 171 "parser.y"
    {
        printf("  [Parsing Rule] Print statement matched.\n");
        ASTNode* node = create_node(NODE_PRINT_STMT);
        node->data.print.value = (yyvsp[(3) - (4)].node);
        (yyval.node) = node;
    ;}
    break;

  case 11:
#line 177 "parser.y"
    {
        printf("  [Parsing Rule] Return statement matched.\n");
        ASTNode* node = create_node(NODE_RETURN_STMT);
        node->data.return_stmt.value = (yyvsp[(2) - (2)].node);
        (yyval.node) = node;
    ;}
    break;

  case 12:
#line 183 "parser.y"
    {
        printf("  [Parsing Rule] Assignment statement matched.\n");
        ASTNode* node = create_node(NODE_ASSIGNMENT_STMT);
        node->data.assignment.target = (yyvsp[(1) - (3)].node);
        node->data.assignment.value = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 13:
#line 190 "parser.y"
    { 
        printf("  [Parsing Rule] Variable/Array declaration matched.\n");
        (yyval.node) = (yyvsp[(1) - (1)].node); 
    ;}
    break;

  case 14:
#line 197 "parser.y"
    {
        ASTNode* node = create_node(NODE_VARIABLE_EXPR);
        node->data.variable.name = strdup((yyvsp[(1) - (1)].id));
        (yyval.node) = node;
    ;}
    break;

  case 15:
#line 202 "parser.y"
    {
        ASTNode* node = create_node(NODE_DEREF_EXPR);
        node->data.deref.name = strdup((yyvsp[(2) - (2)].id));
        node->data.deref.expr = NULL;
        node->data.deref.level = (yyvsp[(1) - (2)].level);
        (yyval.node) = node;
    ;}
    break;

  case 16:
#line 209 "parser.y"
    {
        ASTNode* node = create_node(NODE_ARRAY_ACCESS_EXPR);
        node->data.array_access.name = strdup((yyvsp[(1) - (2)].id));
        node->data.array_access.indices = (yyvsp[(2) - (2)].index_list).list;
        node->data.array_access.index_count = (yyvsp[(2) - (2)].index_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 17:
#line 219 "parser.y"
    { (yyval.level) = 1; ;}
    break;

  case 18:
#line 220 "parser.y"
    { (yyval.level) = (yyvsp[(1) - (2)].level) + 1; ;}
    break;

  case 19:
#line 224 "parser.y"
    {
        (yyval.index_list).list = add_index(NULL, &(yyval.index_list).count, (yyvsp[(2) - (3)].node));
    ;}
    break;

  case 20:
#line 227 "parser.y"
    {
        (yyval.index_list).list = add_index((yyvsp[(1) - (4)].index_list).list, &(yyvsp[(1) - (4)].index_list).count, (yyvsp[(3) - (4)].node));
        (yyval.index_list).count = (yyvsp[(1) - (4)].index_list).count;
    ;}
    break;

  case 21:
#line 234 "parser.y"
    {
        printf("  [Parsing Rule] Simple integer declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup((yyvsp[(2) - (2)].id));
        node->data.decl.type = strdup("int");
        (yyval.node) = node;
    ;}
    break;

  case 22:
#line 241 "parser.y"
    {
        printf("  [Parsing Rule] Simple float declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup((yyvsp[(2) - (2)].id));
        node->data.decl.type = strdup("float");
        (yyval.node) = node;
    ;}
    break;

  case 23:
#line 248 "parser.y"
    {
        printf("  [Parsing Rule] Simple char declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup((yyvsp[(2) - (2)].id));
        node->data.decl.type = strdup("char");
        (yyval.node) = node;
    ;}
    break;

  case 24:
#line 255 "parser.y"
    {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup((yyvsp[(2) - (3)].id));
        node->data.array_decl.sizes = (yyvsp[(3) - (3)].size_list).list;
        node->data.array_decl.dim_count = (yyvsp[(3) - (3)].size_list).count;
        node->data.array_decl.type = strdup("int");
        (yyval.node) = node;
    ;}
    break;

  case 25:
#line 263 "parser.y"
    {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup((yyvsp[(2) - (3)].id));
        node->data.array_decl.sizes = (yyvsp[(3) - (3)].size_list).list;
        node->data.array_decl.dim_count = (yyvsp[(3) - (3)].size_list).count;
        node->data.array_decl.type = strdup("float");
        (yyval.node) = node;
    ;}
    break;

  case 26:
#line 271 "parser.y"
    {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup((yyvsp[(2) - (3)].id));
        node->data.array_decl.sizes = (yyvsp[(3) - (3)].size_list).list;
        node->data.array_decl.dim_count = (yyvsp[(3) - (3)].size_list).count;
        node->data.array_decl.type = strdup("char");
        (yyval.node) = node;
    ;}
    break;

  case 27:
#line 279 "parser.y"
    {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup((yyvsp[(3) - (3)].id));
        node->data.pointer_decl.type = strdup("int");
        node->data.pointer_decl.level = (yyvsp[(2) - (3)].level);
        (yyval.node) = node;
    ;}
    break;

  case 28:
#line 286 "parser.y"
    {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup((yyvsp[(3) - (3)].id));
        node->data.pointer_decl.type = strdup("float");
        node->data.pointer_decl.level = (yyvsp[(2) - (3)].level);
        (yyval.node) = node;
    ;}
    break;

  case 29:
#line 293 "parser.y"
    {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup((yyvsp[(3) - (3)].id));
        node->data.pointer_decl.type = strdup("char");
        node->data.pointer_decl.level = (yyvsp[(2) - (3)].level);
        (yyval.node) = node;
    ;}
    break;

  case 30:
#line 303 "parser.y"
    {
        (yyval.size_list).list = add_size(NULL, &(yyval.size_list).count, atoi((yyvsp[(2) - (3)].id)));
    ;}
    break;

  case 31:
#line 306 "parser.y"
    {
        (yyval.size_list).list = add_size((yyvsp[(1) - (4)].size_list).list, &(yyvsp[(1) - (4)].size_list).count, atoi((yyvsp[(3) - (4)].id)));
        (yyval.size_list).count = (yyvsp[(1) - (4)].size_list).count;
    ;}
    break;

  case 32:
#line 313 "parser.y"
    {
        (yyval.param_list).list = NULL;
        (yyval.param_list).count = 0;
    ;}
    break;

  case 33:
#line 317 "parser.y"
    {
        (yyval.param_list).list = add_param(NULL, &(yyval.param_list).count, (yyvsp[(1) - (1)].id));
    ;}
    break;

  case 34:
#line 320 "parser.y"
    {
        (yyval.param_list).list = add_param((yyvsp[(1) - (3)].param_list).list, &(yyvsp[(1) - (3)].param_list).count, (yyvsp[(3) - (3)].id));
        (yyval.param_list).count = (yyvsp[(1) - (3)].param_list).count;
    ;}
    break;

  case 35:
#line 327 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 36:
#line 328 "parser.y"
    {
        printf("  [Parsing Rule] Binary Expression (+) matched.\n");
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("+");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 37:
#line 336 "parser.y"
    {
        printf("  [Parsing Rule] Binary Expression (-) matched.\n");
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("-");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 38:
#line 344 "parser.y"
    {
        printf("  [Parsing Rule] Comparison Expression (=) matched.\n");
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("=");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 39:
#line 352 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≠");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 40:
#line 359 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("<");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 41:
#line 366 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup(">");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 42:
#line 373 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≤");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 43:
#line 380 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≥");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 44:
#line 390 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 45:
#line 391 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("*");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 46:
#line 398 "parser.y"
    {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("/");
        node->data.binary.left = (yyvsp[(1) - (3)].node);
        node->data.binary.right = (yyvsp[(3) - (3)].node);
        (yyval.node) = node;
    ;}
    break;

  case 47:
#line 408 "parser.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 48:
#line 409 "parser.y"
    {
        ASTNode* node = create_node(NODE_FUNCTION_CALL_EXPR);
        node->data.func_call.name = strdup((yyvsp[(1) - (4)].id));
        node->data.func_call.args = (yyvsp[(3) - (4)].arg_list).list;
        node->data.func_call.arg_count = (yyvsp[(3) - (4)].arg_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 49:
#line 416 "parser.y"
    {
        ASTNode* node = create_node(NODE_ADDR_OF_EXPR);
        node->data.addr_of.name = strdup((yyvsp[(2) - (2)].id));
        (yyval.node) = node;
    ;}
    break;

  case 50:
#line 421 "parser.y"
    {
        ASTNode* node = create_node(NODE_DEREF_EXPR);
        node->data.deref.name = strdup((yyvsp[(2) - (2)].id));
        node->data.deref.expr = NULL;
        node->data.deref.level = (yyvsp[(1) - (2)].level);
        (yyval.node) = node;
    ;}
    break;

  case 51:
#line 428 "parser.y"
    {
        ASTNode* node = create_node(NODE_ARRAY_ACCESS_EXPR);
        node->data.array_access.name = strdup((yyvsp[(1) - (2)].id));
        node->data.array_access.indices = (yyvsp[(2) - (2)].index_list).list;
        node->data.array_access.index_count = (yyvsp[(2) - (2)].index_list).count;
        (yyval.node) = node;
    ;}
    break;

  case 52:
#line 438 "parser.y"
    {
        (yyval.arg_list).list = NULL;
        (yyval.arg_list).count = 0;
    ;}
    break;

  case 53:
#line 442 "parser.y"
    {
        (yyval.arg_list).list = add_arg(NULL, &(yyval.arg_list).count, (yyvsp[(1) - (1)].node));
    ;}
    break;

  case 54:
#line 445 "parser.y"
    {
        (yyval.arg_list).list = add_arg((yyvsp[(1) - (3)].arg_list).list, &(yyvsp[(1) - (3)].arg_list).count, (yyvsp[(3) - (3)].node));
        (yyval.arg_list).count = (yyvsp[(1) - (3)].arg_list).count;
    ;}
    break;

  case 55:
#line 452 "parser.y"
    {
        ASTNode* node = create_node(NODE_INT_EXPR);
        node->data.int_expr.value = atoi((yyvsp[(1) - (1)].id));
        (yyval.node) = node;
    ;}
    break;

  case 56:
#line 457 "parser.y"
    {
        ASTNode* node = create_node(NODE_FLOAT_EXPR);
        node->data.float_expr.value = atof((yyvsp[(1) - (1)].id));
        (yyval.node) = node;
    ;}
    break;

  case 57:
#line 462 "parser.y"
    {
        ASTNode* node = create_node(NODE_CHAR_EXPR);
        // $1 is 'c', we want the character at index 1
        node->data.char_expr.value = (yyvsp[(1) - (1)].id)[1];
        (yyval.node) = node;
    ;}
    break;

  case 58:
#line 468 "parser.y"
    {
        ASTNode* node = create_node(NODE_VARIABLE_EXPR);
        node->data.variable.name = strdup((yyvsp[(1) - (1)].id));
        (yyval.node) = node;
    ;}
    break;

  case 59:
#line 473 "parser.y"
    {
        (yyval.node) = (yyvsp[(2) - (3)].node);
    ;}
    break;


/* Line 1267 of yacc.c.  */
#line 2159 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
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
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 478 "parser.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parser error at line %d: %s\n", current_line, s);
}

