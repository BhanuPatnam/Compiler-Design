#include "parser.h"
#include <iostream>
#include <stdexcept>

using namespace std;

// Stage 2: Syntax Analysis (Parsing) & AST Construction
// The Parser takes the stream of tokens and builds a tree structure (the Abstract Syntax Tree).
// This tree represents the logical structure of the program based on our grammar rules.

Parser::Parser(const vector<Token>& tokens) : tokens(tokens), pos(0) {}

Token Parser::peek() {
    return tokens[pos];
}

Token Parser::advance() {
    if (pos < tokens.size()) pos++;
    return tokens[pos - 1];
}

bool Parser::check(TokenType type) {
    return peek().type == type;
}

bool Parser::match(TokenType type) {
    if (check(type)) {
        advance();
        return true;
    }
    return false;
}

Token Parser::consume(TokenType type, const string& message) {
    if (check(type)) return advance();
    throw runtime_error("Parser error at line " + to_string(peek().line) + ": " + message + ". Got '" + peek().value + "'");
}

unique_ptr<ProgramNode> Parser::parse() {
    vector<unique_ptr<StmtNode>> statements;
    while (!check(TokenType::END_OF_FILE)) {
        try {
            statements.push_back(parseStatement());
        } catch (const runtime_error& e) {
            cerr << e.what() << endl;
            // Simple error recovery: skip until next statement start or end
            while (!check(TokenType::END_OF_FILE) && 
                   !check(TokenType::IF) && !check(TokenType::FOR) && 
                   !check(TokenType::WHILE) && !check(TokenType::FUNCTION) && 
                   !check(TokenType::PRINT) && !check(TokenType::IDENTIFIER)) {
                advance();
            }
        }
    }
    return make_unique<ProgramNode>(move(statements));
}

unique_ptr<StmtNode> Parser::parseStatement() {
    if (match(TokenType::IF)) return parseIf();
    if (match(TokenType::FOR)) return parseFor();
    if (match(TokenType::WHILE)) return parseWhile();
    if (match(TokenType::FUNCTION)) return parseFunction();
    if (match(TokenType::PRINT)) return parsePrint();
    if (match(TokenType::RETURN)) return parseReturn();
    if (check(TokenType::IDENTIFIER)) return parseAssignment();
    
    throw runtime_error("Unknown statement starting with " + peek().value);
}

unique_ptr<StmtNode> Parser::parseAssignment() {
    Token name = consume(TokenType::IDENTIFIER, "Expected identifier");
    consume(TokenType::ASSIGN, "Expected '<-' in assignment");
    auto value = parseExpression();
    return make_unique<AssignmentStmtNode>(name.value, move(value));
}

unique_ptr<StmtNode> Parser::parseIf() {
    auto condition = parseExpression();
    consume(TokenType::THEN, "Expected 'then' after if condition");
    
    vector<unique_ptr<StmtNode>> thenBranch;
    while (!check(TokenType::ELSE) && !check(TokenType::END)) {
        thenBranch.push_back(parseStatement());
    }

    vector<unique_ptr<StmtNode>> elseBranch;
    if (match(TokenType::ELSE)) {
        while (!check(TokenType::END)) {
            elseBranch.push_back(parseStatement());
        }
    }

    consume(TokenType::END, "Expected 'end' after if statement");
    if (peek().type == TokenType::IF) advance(); // Optional "end if"

    return make_unique<IfStmtNode>(move(condition), move(thenBranch), move(elseBranch));
}

unique_ptr<StmtNode> Parser::parseFor() {
    Token var = consume(TokenType::IDENTIFIER, "Expected identifier in for loop");
    consume(TokenType::ASSIGN, "Expected '<-' in for loop");
    auto start = parseExpression();
    consume(TokenType::TO, "Expected 'to' in for loop");
    auto end = parseExpression();
    
    vector<unique_ptr<StmtNode>> body;
    while (!check(TokenType::END)) {
        body.push_back(parseStatement());
    }
    
    consume(TokenType::END, "Expected 'end' after for loop");
    if (peek().type == TokenType::FOR) advance(); // Optional "end for"

    return make_unique<ForStmtNode>(var.value, move(start), move(end), move(body));
}

unique_ptr<StmtNode> Parser::parseWhile() {
    auto condition = parseExpression();
    consume(TokenType::DO, "Expected 'do' after while condition");
    
    vector<unique_ptr<StmtNode>> body;
    while (!check(TokenType::END)) {
        body.push_back(parseStatement());
    }
    
    consume(TokenType::END, "Expected 'end' after while loop");
    if (peek().type == TokenType::WHILE) advance(); // Optional "end while"

    return make_unique<WhileStmtNode>(move(condition), move(body));
}

unique_ptr<StmtNode> Parser::parseFunction() {
    Token name = consume(TokenType::IDENTIFIER, "Expected function name");
    consume(TokenType::LPAREN, "Expected '(' after function name");
    
    vector<string> params;
    if (!check(TokenType::RPAREN)) {
        do {
            params.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name").value);
        } while (match(TokenType::COMMA));
    }
    consume(TokenType::RPAREN, "Expected ')' after parameters");
    
    vector<unique_ptr<StmtNode>> body;
    while (!check(TokenType::END)) {
        body.push_back(parseStatement());
    }
    
    consume(TokenType::END, "Expected 'end' after function");
    if (peek().type == TokenType::FUNCTION) advance(); // Optional "end function"

    return make_unique<FunctionStmtNode>(name.value, params, move(body));
}

unique_ptr<StmtNode> Parser::parsePrint() {
    consume(TokenType::LPAREN, "Expected '(' after print");
    auto value = parseExpression();
    consume(TokenType::RPAREN, "Expected ')' after print expression");
    return make_unique<PrintStmtNode>(move(value));
}

unique_ptr<StmtNode> Parser::parseReturn() {
    auto value = parseExpression();
    return make_unique<ReturnStmtNode>(move(value));
}

unique_ptr<ExprNode> Parser::parseExpression() {
    auto left = parseTerm();
    while (check(TokenType::PLUS) || check(TokenType::MINUS) || 
           check(TokenType::EQ) || check(TokenType::NEQ) || 
           check(TokenType::LT) || check(TokenType::GT) || 
           check(TokenType::LE) || check(TokenType::GE)) {
        Token op = advance();
        auto right = parseTerm();
        left = make_unique<BinaryExprNode>(op.value, move(left), move(right));
    }
    return left;
}

unique_ptr<ExprNode> Parser::parseTerm() {
    auto left = parseFactor();
    while (check(TokenType::STAR) || check(TokenType::SLASH)) {
        Token op = advance();
        auto right = parseFactor();
        left = make_unique<BinaryExprNode>(op.value, move(left), move(right));
    }
    return left;
}

unique_ptr<ExprNode> Parser::parseFactor() {
    if (match(TokenType::NUMBER)) {
        return make_unique<NumberExprNode>(stoi(tokens[pos-1].value));
    }
    if (match(TokenType::LPAREN)) {
        auto expr = parseExpression();
        consume(TokenType::RPAREN, "Expected ')' after expression");
        return expr;
    }
    if (check(TokenType::IDENTIFIER)) {
        Token name = advance();
        if (match(TokenType::LPAREN)) {
            vector<unique_ptr<ExprNode>> args;
            if (!check(TokenType::RPAREN)) {
                do {
                    args.push_back(parseExpression());
                } while (match(TokenType::COMMA));
            }
            consume(TokenType::RPAREN, "Expected ')' after arguments");
            return make_unique<FunctionCallExprNode>(name.value, move(args));
        }
        return make_unique<VariableExprNode>(name.value);
    }
    
    throw runtime_error("Expected expression, got '" + peek().value + "'");
}
