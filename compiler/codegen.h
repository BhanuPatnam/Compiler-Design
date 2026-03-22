#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

// Walks the AST and emits Python-style output (visitor).
class CodeGenerator : public ASTVisitor {
public:
    string generate(const ProgramNode& program);

    void visit(const NumberExprNode& node) override;
    void visit(const VariableExprNode& node) override;
    void visit(const BinaryExprNode& node) override;
    void visit(const FunctionCallExprNode& node) override;

    void visit(const AssignmentStmtNode& node) override;
    void visit(const IfStmtNode& node) override;
    void visit(const ForStmtNode& node) override;
    void visit(const WhileStmtNode& node) override;
    void visit(const FunctionStmtNode& node) override;
    void visit(const PrintStmtNode& node) override;
    void visit(const ReturnStmtNode& node) override;
    void visit(const ProgramNode& node) override;

private:
    stringstream output;
    int indentLevel = 0;

    void indent();
    void emitStmtList(const vector<unique_ptr<StmtNode>>& stmts);
};

#endif
