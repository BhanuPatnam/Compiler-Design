#include "lexer.h"
#include <iostream>
#include <cctype>
#include <map>

using namespace std;

// Stage 1: Lexical Analysis
// The Lexer breaks down the raw source code into a series of meaningful "Tokens".
// Each token has a type (e.g., IDENTIFIER, IF, PLUS) and a value (e.g., "sum", "+").

Lexer::Lexer(const string& source) : source(source), pos(0), line(1) {}

char Lexer::peek() {
    if (pos >= source.length()) return '\0';
    return source[pos];
}

char Lexer::advance() {
    if (pos >= source.length()) return '\0';
    return source[pos++];
}

void Lexer::skipWhitespace() {
    while (pos < source.length() && isspace(peek())) {
        if (peek() == '\n') line++;
        advance();
    }
}

Token Lexer::readIdentifier() {
    size_t start = pos;
    while (pos < source.length() && (isalnum(peek()) || peek() == '_')) {
        advance();
    }
    string value = source.substr(start, pos - start);
    
    static const map<string, TokenType> keywords = {
        {"if", TokenType::IF},
        {"then", TokenType::THEN},
        {"else", TokenType::ELSE},
        {"end", TokenType::END},
        {"for", TokenType::FOR},
        {"to", TokenType::TO},
        {"while", TokenType::WHILE},
        {"do", TokenType::DO},
        {"function", TokenType::FUNCTION},
        {"print", TokenType::PRINT},
        {"return", TokenType::RETURN}
    };

    if (keywords.count(value)) {
        return {keywords.at(value), value, line};
    }
    return {TokenType::IDENTIFIER, value, line};
}

Token Lexer::readNumber() {
    size_t start = pos;
    while (pos < source.length() && isdigit(peek())) {
        advance();
    }
    return {TokenType::NUMBER, source.substr(start, pos - start), line};
}

Token Lexer::readSymbol() {
    char current = advance();
    switch (current) {
        case '+': return {TokenType::PLUS, "+", line};
        case '-': return {TokenType::MINUS, "-", line};
        case '*': return {TokenType::STAR, "*", line};
        case '/': return {TokenType::SLASH, "/", line};
        case '(': return {TokenType::LPAREN, "(", line};
        case ')': return {TokenType::RPAREN, ")", line};
        case ',': return {TokenType::COMMA, ",", line};
        case '=': return {TokenType::EQ, "=", line};
        case '<': return {TokenType::LT, "<", line};
        case '>': return {TokenType::GT, ">", line};
    }

    // Handle Unicode symbols (UTF-8)
    // ← : E2 86 90
    // ≠ : E2 89 A0
    // ≤ : E2 89 A4
    // ≥ : E2 89 A5
    if ((unsigned char)current == 0xE2) {
        char next1 = advance();
        char next2 = advance();
        string symbol = {current, next1, next2};
        if (symbol == "←") return {TokenType::ASSIGN, symbol, line};
        if (symbol == "≠") return {TokenType::NEQ, symbol, line};
        if (symbol == "≤") return {TokenType::LE, symbol, line};
        if (symbol == "≥") return {TokenType::GE, symbol, line};
        return {TokenType::UNKNOWN, symbol, line};
    }

    return {TokenType::UNKNOWN, string(1, current), line};
}

vector<Token> Lexer::tokenize() {
    vector<Token> tokens;
    while (pos < source.length()) {
        skipWhitespace();
        if (pos >= source.length()) break;

        char current = peek();
        if (isalpha(current) || current == '_') {
            tokens.push_back(readIdentifier());
        } else if (isdigit(current)) {
            tokens.push_back(readNumber());
        } else {
            Token t = readSymbol();
            if (t.type != TokenType::UNKNOWN) {
                tokens.push_back(t);
            } else {
                cerr << "Lexer error: Unknown symbol '" << t.value << "' at line " << line << endl;
            }
        }
    }
    tokens.push_back({TokenType::END_OF_FILE, "", line});
    return tokens;
}
