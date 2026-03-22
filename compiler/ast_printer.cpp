#include "ast_printer.h"

namespace {

struct IndentGuard {
    int& level;
    explicit IndentGuard(int& l) : level(l) { ++level; }
    ~IndentGuard() { --level; }
};

}  // namespace

string ASTPrinter::print(const ProgramNode& program) {
    output.str("");
    output.clear();
    indentLevel = 0;
    program.accept(*this);
    return output.str();
}

void ASTPrinter::indent() {
    for (int i = 0; i < indentLevel; ++i) {
        output << "  ";
    }
}

void ASTPrinter::newline() {
    output << '\n';
    indent();
}

void ASTPrinter::parenthesize(const string& name, const ASTNode& node) {
    output << "(" << name;
    {
        IndentGuard g(indentLevel);
        output << "\n";
        indent();
        node.accept(*this);
    }
    newline();
    output << ")";
}

void ASTPrinter::parenthesize(const string& name, const vector<unique_ptr<StmtNode>>& nodes) {
    output << "(" << name;
    {
        IndentGuard g(indentLevel);
        for (const auto& stmt : nodes) {
            output << "\n";
            indent();
            stmt->accept(*this);
        }
    }
    newline();
    output << ")";
}

void ASTPrinter::visit(const ProgramNode& node) {
    parenthesize("program", node.statements);
}

void ASTPrinter::visit(const NumberExprNode& node) {
    output << node.value;
}

void ASTPrinter::visit(const VariableExprNode& node) {
    output << node.name;
}

void ASTPrinter::visit(const BinaryExprNode& node) {
    output << "(" << node.op << " ";
    node.left->accept(*this);
    output << " ";
    node.right->accept(*this);
    output << ")";
}

void ASTPrinter::visit(const FunctionCallExprNode& node) {
    output << "(call " << node.name;
    for (const auto& arg : node.args) {
        output << " ";
        arg->accept(*this);
    }
    output << ")";
}

void ASTPrinter::visit(const AssignmentStmtNode& node) {
    output << "(assign " << node.name << " ";
    node.value->accept(*this);
    output << ")";
}

void ASTPrinter::visit(const IfStmtNode& node) {
    output << "(if ";
    node.condition->accept(*this);
    output << "\n";
    {
        IndentGuard g(indentLevel);
        indent();
        parenthesize("then", node.thenBranch);
        if (!node.elseBranch.empty()) {
            newline();
            parenthesize("else", node.elseBranch);
        }
    }
    newline();
    output << ")";
}

void ASTPrinter::visit(const ForStmtNode& node) {
    output << "(for " << node.var << " from ";
    node.start->accept(*this);
    output << " to ";
    node.end->accept(*this);
    output << "\n";
    {
        IndentGuard g(indentLevel);
        indent();
        parenthesize("do", node.body);
    }
    newline();
    output << ")";
}

void ASTPrinter::visit(const WhileStmtNode& node) {
    output << "(while ";
    node.condition->accept(*this);
    output << "\n";
    {
        IndentGuard g(indentLevel);
        indent();
        parenthesize("do", node.body);
    }
    newline();
    output << ")";
}

void ASTPrinter::visit(const FunctionStmtNode& node) {
    output << "(defun " << node.name << " (";
    bool first = true;
    for (const auto& param : node.params) {
        if (!first) {
            output << " ";
        }
        first = false;
        output << param;
    }
    output << ")\n";
    {
        IndentGuard g(indentLevel);
        indent();
        parenthesize("body", node.body);
    }
    newline();
    output << ")";
}

void ASTPrinter::visit(const PrintStmtNode& node) {
    output << "(print ";
    node.value->accept(*this);
    output << ")";
}

void ASTPrinter::visit(const ReturnStmtNode& node) {
    output << "(return ";
    node.value->accept(*this);
    output << ")";
}
