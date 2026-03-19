#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "ast.h"
#include <string>
#include <sstream>

/**
 * @brief The ASTPrinter uses the Visitor pattern to create a human-readable
 * S-expression style representation of the Abstract Syntax Tree.
 */
class ASTPrinter : public ASTVisitor {
public:
    string print(const ProgramNode& program);

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
    void parenthesize(const string& name, const ASTNode& node);
    void parenthesize(const string& name, const vector<unique_ptr<StmtNode>>& nodes);
};

#endif
