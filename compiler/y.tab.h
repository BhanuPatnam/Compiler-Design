/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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
     TOK_STRUCT = 293,
     TOK_DOT = 294,
     TOK_UNKNOWN = 295
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
#define TOK_STRUCT 293
#define TOK_DOT 294
#define TOK_UNKNOWN 295




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
/* Line 1529 of yacc.c.  */
#line 158 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

