#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * @brief Token types supported by our compiler.
 * We include standard operators, control flow keywords, and literals.
 */
enum class TokenType {
    IDENTIFIER,
    NUMBER,
    ASSIGN,      // ←
    PLUS,        // +
    MINUS,       // -
    STAR,        // *
    SLASH,       // /
    LPAREN,      // (
    RPAREN,      // )
    COMMA,       // ,
    IF,          // if
    THEN,        // then
    ELSE,        // else
    END,         // end
    FOR,         // for
    TO,          // to
    WHILE,       // while
    DO,          // do
    FUNCTION,    // function
    PRINT,       // print
    RETURN,      // return
    EQ,          // =
    NEQ,         // ≠
    LT,          // <
    GT,          // >
    LE,          // ≤
    GE,          // ≥
    END_OF_FILE,
    UNKNOWN
};

struct Token {
    TokenType type;
    string value;
    int line;
};

class Lexer {
public:
    Lexer(const string& source);
    vector<Token> tokenize();

private:
    string source;
    size_t pos;
    int line;

    char peek();
    char advance();
    void skipWhitespace();
    Token readIdentifier();
    Token readNumber();
    Token readSymbol();
};

#endif
