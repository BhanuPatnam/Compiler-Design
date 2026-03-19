#include "ast_printer.h"

string ASTPrinter::print(const ProgramNode& program) {
    output.str("");
    output.clear();
    program.accept(*this);
    return output.str();
}

void ASTPrinter::indent() {
    for (int i = 0; i < indentLevel; ++i) {
        output << "  ";
    }
}

void ASTPrinter::parenthesize(const string& name, const ASTNode& node) {
    output << "(" << name;
    indentLevel++;
    output << "\n";
    indent();
    node.accept(*this);
    indentLevel--;
    output << "\n";
    indent();
    output << ")";
}

void ASTPrinter::parenthesize(const string& name, const vector<unique_ptr<StmtNode>>& nodes) {
    output << "(" << name;
    indentLevel++;
    for (const auto& stmt : nodes) {
        output << "\n";
        indent();
        stmt->accept(*this);
    }
    indentLevel--;
    output << "\n";
    indent();
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
    indentLevel++;
    indent();
    parenthesize("then", node.thenBranch);
    if (!node.elseBranch.empty()) {
        output << "\n";
        indent();
        parenthesize("else", node.elseBranch);
    }
    indentLevel--;
    output << "\n";
    indent();
    output << ")";
}

void ASTPrinter::visit(const ForStmtNode& node) {
    output << "(for " << node.var << " from ";
    node.start->accept(*this);
    output << " to ";
    node.end->accept(*this);
    output << "\n";
    indentLevel++;
    indent();
    parenthesize("do", node.body);
    indentLevel--;
    output << "\n";
    indent();
    output << ")";
}

void ASTPrinter::visit(const WhileStmtNode& node) {
    output << "(while ";
    node.condition->accept(*this);
    output << "\n";
    indentLevel++;
    indent();
    parenthesize("do", node.body);
    indentLevel--;
    output << "\n";
    indent();
    output << ")";
}

void ASTPrinter::visit(const FunctionStmtNode& node) {
    output << "(defun " << node.name << " (";
    for (size_t i = 0; i < node.params.size(); ++i) {
        output << node.params[i];
        if (i < node.params.size() - 1) output << " ";
    }
    output << ")\n";
    indentLevel++;
    indent();
    parenthesize("body", node.body);
    indentLevel--;
    output << "\n";
    indent();
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
