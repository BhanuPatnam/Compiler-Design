#include "lexer.h"
#include "y.tab.h"

// External Bison variables
extern YYSTYPE yylval;
char* yytext = NULL;
FILE* yyin = NULL;
static Lexer global_lexer;
static int lexer_initialized = 0;

void lexer_init_with_file(FILE* file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* source = (char*)malloc(size + 1);
    fread(source, 1, size, file);
    source[size] = '\0';

    global_lexer.source = source;
    global_lexer.pos = 0;
    global_lexer.line = 1;
    lexer_initialized = 1;
}

void lexer_reset(void) {
    global_lexer.pos = 0;
    global_lexer.line = 1;
    lexer_initialized = 1;
}

static char peek() {
    return global_lexer.source[global_lexer.pos];
}

static char advance() {
    if (global_lexer.source[global_lexer.pos] == '\0') return '\0';
    return global_lexer.source[global_lexer.pos++];
}

static void skip_whitespace() {
    while (isspace(peek())) {
        if (peek() == '\n') {
            global_lexer.line++;
            current_line++;
        }
        advance();
    }
}

static int lookup_keyword(const char* value) {
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

int yylex() {
    if (!lexer_initialized) {
        if (yyin) {
            lexer_init_with_file(yyin);
        } else {
            return 0; // EOF
        }
    }

    skip_whitespace();

    char current = peek();
    if (current == '\0') return 0; // EOF

    size_t start_pos = global_lexer.pos;

    if (isalpha(current) || current == '_') {
        while (isalnum(peek()) || peek() == '_') {
            advance();
        }
        size_t len = global_lexer.pos - start_pos;
        if (yytext) free(yytext);
        yytext = (char*)malloc(len + 1);
        strncpy(yytext, &global_lexer.source[start_pos], len);
        yytext[len] = '\0';

        int type = lookup_keyword(yytext);
        if (type == TOK_IDENTIFIER) {
            yylval.id = strdup(yytext);
        }
        return type;
    }

    if (isdigit(current)) {
        int is_float = 0;
        while (isdigit(peek())) {
            advance();
        }
        if (peek() == '.') {
            is_float = 1;
            advance();
            while (isdigit(peek())) {
                advance();
            }
        }
        size_t len = global_lexer.pos - start_pos;
        if (yytext) free(yytext);
        yytext = (char*)malloc(len + 1);
        strncpy(yytext, &global_lexer.source[start_pos], len);
        yytext[len] = '\0';
        yylval.id = strdup(yytext);
        return is_float ? TOK_FLOAT_LITERAL : TOK_INT_LITERAL;
    }

    if (current == '\'') {
        advance(); // skip '
        if (peek() != '\0' && peek() != '\'') {
            advance(); // the char
        }
        if (peek() == '\'') {
            advance(); // skip '
        }
        size_t len = global_lexer.pos - start_pos;
        if (yytext) free(yytext);
        yytext = (char*)malloc(len + 1);
        strncpy(yytext, &global_lexer.source[start_pos], len);
        yytext[len] = '\0';
        yylval.id = strdup(yytext);
        return TOK_CHAR_LITERAL;
    }

    // Handle symbols
    advance();
    size_t len = global_lexer.pos - start_pos;
    if (yytext) free(yytext);
    yytext = (char*)malloc(len + 1);
    strncpy(yytext, &global_lexer.source[start_pos], len);
    yytext[len] = '\0';

    switch (current) {
        case '+': return TOK_PLUS;
        case '-': {
            if (peek() == '>') {
                advance();
                free(yytext);
                yytext = strdup("->");
                return TOK_ASSIGN;
            }
            return TOK_MINUS;
        }
        case '*': return TOK_STAR;
        case '/': return TOK_SLASH;
        case '(': return TOK_LPAREN;
        case ')': return TOK_RPAREN;
        case ',': return TOK_COMMA;
        case '=': return TOK_EQ;
        case '<': {
            if (peek() == '-') {
                advance();
                free(yytext);
                yytext = strdup("<-");
                return TOK_ASSIGN;
            }
            return TOK_LT;
        }
        case '>': return TOK_GT;
    }

    // Handle UTF-8 (←, ≠, ≤, ≥)
    if ((unsigned char)current == 0xE2) {
        char next1 = advance();
        char next2 = advance();
        free(yytext);
        yytext = (char*)malloc(4);
        yytext[0] = current;
        yytext[1] = next1;
        yytext[2] = next2;
        yytext[3] = '\0';
        if (strcmp(yytext, "←") == 0) return TOK_ASSIGN;
        if (strcmp(yytext, "≠") == 0) return TOK_NEQ;
        if (strcmp(yytext, "≤") == 0) return TOK_LE;
        if (strcmp(yytext, "≥") == 0) return TOK_GE;
        return TOK_UNKNOWN;
    }

    return TOK_UNKNOWN;
}
