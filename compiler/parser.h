#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

/**
 * @brief A Recursive Descent Parser for pseudocode.
 * It builds an Abstract Syntax Tree (AST) by following the BNF grammar.
 */
class Parser {
public:
    Parser(const vector<Token>& tokens);
    unique_ptr<ProgramNode> parse();

private:
    vector<Token> tokens;
    size_t pos;

    Token peek();
    Token advance();
    bool check(TokenType type);
    bool match(TokenType type);
    Token consume(TokenType type, const string& message);

    unique_ptr<StmtNode> parseStatement();
    unique_ptr<StmtNode> parseAssignment();
    unique_ptr<StmtNode> parseIf();
    unique_ptr<StmtNode> parseFor();
    unique_ptr<StmtNode> parseWhile();
    unique_ptr<StmtNode> parseFunction();
    unique_ptr<StmtNode> parsePrint();
    unique_ptr<StmtNode> parseReturn();

    unique_ptr<ExprNode> parseExpression();
    unique_ptr<ExprNode> parseTerm();
    unique_ptr<ExprNode> parseFactor();
    unique_ptr<ExprNode> parsePrimary();
};

#endif
