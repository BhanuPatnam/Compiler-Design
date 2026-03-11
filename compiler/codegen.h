#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

/**
 * @brief The CodeGenerator implements the ASTVisitor interface to produce AlgoLang code.
 * It maintains state such as indentation level and an output string stream.
 */
class CodeGenerator : public ASTVisitor {
public:
    string generate(const ProgramNode& program);

    // Expression visitors
    void visit(const NumberExprNode& node) override;
    void visit(const VariableExprNode& node) override;
    void visit(const BinaryExprNode& node) override;
    void visit(const FunctionCallExprNode& node) override;

    // Statement visitors
    void visit(const AssignmentStmtNode& node) override;
    void visit(const IfStmtNode& node) override;
    void visit(const ForStmtNode& node) override;
    void visit(const WhileStmtNode& node) override;
    void visit(const FunctionStmtNode& node) override;
    void visit(const PrintStmtNode& node) override;
    void visit(const ReturnStmtNode& node) override;
    
    // Program visitor
    void visit(const ProgramNode& node) override;

private:
    stringstream output;
    int indentLevel = 0;

    void indent();
};

#endif
