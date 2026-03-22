#include "lexer.h"

Lexer lexer_init(const char* source) {
    Lexer lexer;
    lexer.source = source;
    lexer.pos = 0;
    lexer.line = 1;
    return lexer;
}

static char peek(Lexer* lexer) {
    return lexer->source[lexer->pos];
}

static char advance(Lexer* lexer) {
    if (lexer->source[lexer->pos] == '\0') return '\0';
    return lexer->source[lexer->pos++];
}

static void skip_whitespace(Lexer* lexer) {
    while (isspace(peek(lexer))) {
        if (peek(lexer) == '\n') lexer->line++;
        advance(lexer);
    }
}

static Token create_token(TokenType type, const char* value, int line) {
    Token token;
    token.type = type;
    token.value = strdup(value);
    token.line = line;
    return token;
}

static TokenType lookup_keyword(const char* value) {
    if (strcmp(value, "if") == 0) return TOK_IF;
    if (strcmp(value, "then") == 0) return TOK_THEN;
    if (strcmp(value, "else") == 0) return TOK_ELSE;
    if (strcmp(value, "end") == 0) return TOK_END;
    if (strcmp(value, "for") == 0) return TOK_FOR;
    if (strcmp(value, "to") == 0) return TOK_TO;
    if (strcmp(value, "while") == 0) return TOK_WHILE;
    if (strcmp(value, "do") == 0) return TOK_DO;
    if (strcmp(value, "function") == 0) return TOK_FUNCTION;
    if (strcmp(value, "print") == 0) return TOK_PRINT;
    if (strcmp(value, "return") == 0) return TOK_RETURN;
    return TOK_IDENTIFIER;
}

static Token read_identifier(Lexer* lexer) {
    size_t start = lexer->pos;
    while (isalnum(peek(lexer)) || peek(lexer) == '_') {
        advance(lexer);
    }
    size_t len = lexer->pos - start;
    char* value = (char*)malloc(len + 1);
    strncpy(value, &lexer->source[start], len);
    value[len] = '\0';
    TokenType type = lookup_keyword(value);
    Token t = create_token(type, value, lexer->line);
    free(value);
    return t;
}

static Token read_number(Lexer* lexer) {
    size_t start = lexer->pos;
    while (isdigit(peek(lexer))) {
        advance(lexer);
    }
    size_t len = lexer->pos - start;
    char* value = (char*)malloc(len + 1);
    strncpy(value, &lexer->source[start], len);
    value[len] = '\0';
    Token t = create_token(TOK_NUMBER, value, lexer->line);
    free(value);
    return t;
}

static Token read_symbol(Lexer* lexer) {
    char current = advance(lexer);
    switch (current) {
        case '+': return create_token(TOK_PLUS, "+", lexer->line);
        case '-': {
            if (peek(lexer) == '>') {
                advance(lexer);
                return create_token(TOK_ASSIGN, "->", lexer->line);
            }
            return create_token(TOK_MINUS, "-", lexer->line);
        }
        case '*': return create_token(TOK_STAR, "*", lexer->line);
        case '/': return create_token(TOK_SLASH, "/", lexer->line);
        case '(': return create_token(TOK_LPAREN, "(", lexer->line);
        case ')': return create_token(TOK_RPAREN, ")", lexer->line);
        case ',': return create_token(TOK_COMMA, ",", lexer->line);
        case '=': return create_token(TOK_EQ, "=", lexer->line);
        case '<': {
            if (peek(lexer) == '-') {
                advance(lexer);
                return create_token(TOK_ASSIGN, "<-", lexer->line);
            }
            return create_token(TOK_LT, "<", lexer->line);
        }
        case '>': return create_token(TOK_GT, ">", lexer->line);
    }

    // Handle UTF-8 (←, ≠, ≤, ≥)
    if ((unsigned char)current == 0xE2) {
        char next1 = advance(lexer);
        char next2 = advance(lexer);
        char symbol[4] = {current, next1, next2, '\0'};
        if (strcmp(symbol, "←") == 0) return create_token(TOK_ASSIGN, symbol, lexer->line);
        if (strcmp(symbol, "≠") == 0) return create_token(TOK_NEQ, symbol, lexer->line);
        if (strcmp(symbol, "≤") == 0) return create_token(TOK_LE, symbol, lexer->line);
        if (strcmp(symbol, "≥") == 0) return create_token(TOK_GE, symbol, lexer->line);
        return create_token(TOK_UNKNOWN, symbol, lexer->line);
    }

    char unknown[2] = {current, '\0'};
    return create_token(TOK_UNKNOWN, unknown, lexer->line);
}

Token* lexer_tokenize(Lexer* lexer, int* token_count) {
    int capacity = 100;
    Token* tokens = (Token*)malloc(sizeof(Token) * capacity);
    int count = 0;

    while (peek(lexer) != '\0') {
        skip_whitespace(lexer);
        if (peek(lexer) == '\0') break;

        if (count >= capacity - 1) {
            capacity *= 2;
            tokens = (Token*)realloc(tokens, sizeof(Token) * capacity);
        }

        char current = peek(lexer);
        if (isalpha(current) || current == '_') {
            tokens[count++] = read_identifier(lexer);
        } else if (isdigit(current)) {
            tokens[count++] = read_number(lexer);
        } else {
            tokens[count++] = read_symbol(lexer);
        }
    }

    tokens[count++] = create_token(TOK_EOF, "", lexer->line);
    *token_count = count;
    return tokens;
}

void tokens_free(Token* tokens, int count) {
    for (int i = 0; i < count; i++) {
        free(tokens[i].value);
    }
    free(tokens);
}
