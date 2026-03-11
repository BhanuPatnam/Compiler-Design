#include "codegen.h"
#include <iostream>

using namespace std;

// Stage 3: Code Generation
// This stage traverses the AST and produces the final "AlgoLang" code.
// Using the Visitor pattern ensures that each node knows how to 'accept' a visitor,
// and the visitor knows how to 'visit' each specific node type.

void CodeGenerator::indent() {
    for (int i = 0; i < indentLevel; ++i) {
        output << "    ";
    }
}

string CodeGenerator::generate(const ProgramNode& program) {
    output.str("");
    output.clear();
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
    
    string op = node.op;
    if (op == "=") op = "==";
    else if (op == "≠") op = "!=";
    else if (op == "≤") op = "<=";
    else if (op == "≥") op = ">=";
    
    output << " " << op << " ";
    node.right->accept(*this);
    output << ")";
}

void CodeGenerator::visit(const FunctionCallExprNode& node) {
    output << node.name << "(";
    for (size_t i = 0; i < node.args.size(); ++i) {
        node.args[i]->accept(*this);
        if (i < node.args.size() - 1) output << ", ";
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
    
    indentLevel++;
    if (node.thenBranch.empty()) {
        indent(); output << "pass\n";
    } else {
        for (const auto& s : node.thenBranch) {
            s->accept(*this);
            output << "\n";
        }
    }
    indentLevel--;

    if (!node.elseBranch.empty()) {
        indent();
        output << "else:\n";
        indentLevel++;
        for (const auto& s : node.elseBranch) {
            s->accept(*this);
            output << "\n";
        }
        indentLevel--;
    }
}

void CodeGenerator::visit(const ForStmtNode& node) {
    indent();
    output << "for " << node.var << " in range(";
    node.start->accept(*this);
    output << ", ";
    node.end->accept(*this);
    output << " + 1):\n";
    
    indentLevel++;
    if (node.body.empty()) {
        indent(); output << "pass\n";
    } else {
        for (const auto& s : node.body) {
            s->accept(*this);
            output << "\n";
        }
    }
    indentLevel--;
}

void CodeGenerator::visit(const WhileStmtNode& node) {
    indent();
    output << "while ";
    node.condition->accept(*this);
    output << ":\n";
    
    indentLevel++;
    if (node.body.empty()) {
        indent(); output << "pass\n";
    } else {
        for (const auto& s : node.body) {
            s->accept(*this);
            output << "\n";
        }
    }
    indentLevel--;
}

void CodeGenerator::visit(const FunctionStmtNode& node) {
    indent();
    output << "def " << node.name << "(";
    for (size_t i = 0; i < node.params.size(); ++i) {
        output << node.params[i];
        if (i < node.params.size() - 1) output << ", ";
    }
    output << "):\n";
    
    indentLevel++;
    if (node.body.empty()) {
        indent(); output << "pass\n";
    } else {
        for (const auto& s : node.body) {
            s->accept(*this);
            output << "\n";
        }
    }
    indentLevel--;
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
