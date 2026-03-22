#include "codegen.h"

namespace {

string normalizeOp(const string& op) {
    if (op == "=") return "==";
    if (op == "≠") return "!=";
    if (op == "≤") return "<=";
    if (op == "≥") return ">=";
    return op;
}

struct IndentGuard {
    int& level;
    explicit IndentGuard(int& l) : level(l) { ++level; }
    ~IndentGuard() { --level; }
};

}  // namespace

void CodeGenerator::indent() {
    for (int i = 0; i < indentLevel; ++i) {
        output << "    ";
    }
}

void CodeGenerator::emitStmtList(const vector<unique_ptr<StmtNode>>& stmts) {
    if (stmts.empty()) {
        indent();
        output << "pass\n";
    } else {
        for (const auto& stmt : stmts) {
            stmt->accept(*this);
            output << "\n";
        }
    }
}

string CodeGenerator::generate(const ProgramNode& program) {
    output.str("");
    output.clear();
    indentLevel = 0;
    program.accept(*this);
    return output.str();
}

void CodeGenerator::visit(const ProgramNode& node) {
    for (const auto& stmt : node.statements) {
        stmt->accept(*this);
        output << "\n";
    }
}

void CodeGenerator::visit(const NumberExprNode& node) {
    output << node.value;
}

void CodeGenerator::visit(const VariableExprNode& node) {
    output << node.name;
}

void CodeGenerator::visit(const BinaryExprNode& node) {
    output << "(";
    node.left->accept(*this);
    output << " " << normalizeOp(node.op) << " ";
    node.right->accept(*this);
    output << ")";
}

void CodeGenerator::visit(const FunctionCallExprNode& node) {
    output << node.name << "(";
    bool first = true;
    for (const auto& arg : node.args) {
        if (!first) {
            output << ", ";
        }
        first = false;
        arg->accept(*this);
    }
    output << ")";
}

void CodeGenerator::visit(const AssignmentStmtNode& node) {
    indent();
    output << node.name << " = ";
    node.value->accept(*this);
}

void CodeGenerator::visit(const IfStmtNode& node) {
    indent();
    output << "if ";
    node.condition->accept(*this);
    output << ":\n";

    {
        IndentGuard g(indentLevel);
        emitStmtList(node.thenBranch);
    }

    if (!node.elseBranch.empty()) {
        indent();
        output << "else:\n";
        IndentGuard g(indentLevel);
        emitStmtList(node.elseBranch);
    }
}

void CodeGenerator::visit(const ForStmtNode& node) {
    indent();
    output << "for " << node.var << " in range(";
    node.start->accept(*this);
    output << ", ";
    node.end->accept(*this);
    output << " + 1):\n";

    IndentGuard g(indentLevel);
    emitStmtList(node.body);
}

void CodeGenerator::visit(const WhileStmtNode& node) {
    indent();
    output << "while ";
    node.condition->accept(*this);
    output << ":\n";

    IndentGuard g(indentLevel);
    emitStmtList(node.body);
}

void CodeGenerator::visit(const FunctionStmtNode& node) {
    indent();
    output << "def " << node.name << "(";
    bool first = true;
    for (const auto& param : node.params) {
        if (!first) {
            output << ", ";
        }
        first = false;
        output << param;
    }
    output << "):\n";

    IndentGuard g(indentLevel);
    emitStmtList(node.body);
}

void CodeGenerator::visit(const PrintStmtNode& node) {
    indent();
    output << "print(";
    node.value->accept(*this);
    output << ")";
}

void CodeGenerator::visit(const ReturnStmtNode& node) {
    indent();
    output << "return ";
    node.value->accept(*this);
}
