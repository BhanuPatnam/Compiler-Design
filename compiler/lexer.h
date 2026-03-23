#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "y.tab.h"

// Lexer structure
typedef struct {
    const char* source;
    size_t pos;
    int line;
} Lexer;

// Token structure (used for manual lexing if needed, but Bison uses yylval)
typedef struct {
    int type;
    char* value;
    int line;
} Token;

// Bison/Flex interface
extern int yylex(void);
extern char* yytext;
extern int current_line;
extern FILE* yyin;

// Manual Lexer functions
void lexer_init_with_file(FILE* file);
void lexer_reset(void);
int get_next_token(void);

#endif
