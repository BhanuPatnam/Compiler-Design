#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOK_IDENTIFIER,
    TOK_NUMBER,
    TOK_ASSIGN,      // <- or ←
    TOK_PLUS,        // +
    TOK_MINUS,       // -
    TOK_STAR,        // *
    TOK_SLASH,       // /
    TOK_LPAREN,      // (
    TOK_RPAREN,      // )
    TOK_COMMA,       // ,
    TOK_IF,          // if
    TOK_THEN,        // then
    TOK_ELSE,        // else
    TOK_END,         // end
    TOK_FOR,         // for
    TOK_TO,          // to
    TOK_WHILE,       // while
    TOK_DO,          // do
    TOK_FUNCTION,    // function
    TOK_PRINT,       // print
    TOK_RETURN,      // return
    TOK_EQ,          // =
    TOK_NEQ,         // ≠
    TOK_LT,          // <
    TOK_GT,          // >
    TOK_LE,          // ≤
    TOK_GE,          // ≥
    TOK_EOF,
    TOK_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char* value;
    int line;
} Token;

typedef struct {
    const char* source;
    size_t pos;
    int line;
} Lexer;

Lexer lexer_init(const char* source);
Token* lexer_tokenize(Lexer* lexer, int* token_count);
void token_free(Token* token);
void tokens_free(Token* tokens, int count);

#endif
