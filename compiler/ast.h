#ifndef AST_H
#define AST_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

// Forward declarations of AST nodes for the Visitor pattern
class NumberExprNode;
class VariableExprNode;
class BinaryExprNode;
class FunctionCallExprNode;
class AssignmentStmtNode;
class IfStmtNode;
class ForStmtNode;
class WhileStmtNode;
class FunctionStmtNode;
class PrintStmtNode;
class ReturnStmtNode;
class ProgramNode;

/**
 * @brief Visitor interface for the AST.
 * This pattern allows us to separate operations (like codegen, analysis) 
 * from the data structure itself, making it more maintainable.
 */
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;
    virtual void visit(const NumberExprNode& node) = 0;
    virtual void visit(const VariableExprNode& node) = 0;
    virtual void visit(const BinaryExprNode& node) = 0;
    virtual void visit(const FunctionCallExprNode& node) = 0;
    virtual void visit(const AssignmentStmtNode& node) = 0;
    virtual void visit(const IfStmtNode& node) = 0;
    virtual void visit(const ForStmtNode& node) = 0;
    virtual void visit(const WhileStmtNode& node) = 0;
    virtual void visit(const FunctionStmtNode& node) = 0;
    virtual void visit(const PrintStmtNode& node) = 0;
    virtual void visit(const ReturnStmtNode& node) = 0;
    virtual void visit(const ProgramNode& node) = 0;
};

/**
 * @brief Base class for all nodes in the Abstract Syntax Tree.
 */
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor& visitor) const = 0;
};

class ExprNode : public ASTNode {};
class StmtNode : public ASTNode {};

class NumberExprNode : public ExprNode {
public:
    int value;
    NumberExprNode(int value) : value(value) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class VariableExprNode : public ExprNode {
public:
    string name;
    VariableExprNode(const string& name) : name(name) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class BinaryExprNode : public ExprNode {
public:
    string op;
    unique_ptr<ExprNode> left, right;
    BinaryExprNode(const string& op, unique_ptr<ExprNode> left, unique_ptr<ExprNode> right)
        : op(op), left(move(left)), right(move(right)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class FunctionCallExprNode : public ExprNode {
public:
    string name;
    vector<unique_ptr<ExprNode>> args;
    FunctionCallExprNode(const string& name, vector<unique_ptr<ExprNode>> args)
        : name(name), args(move(args)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class AssignmentStmtNode : public StmtNode {
public:
    string name;
    unique_ptr<ExprNode> value;
    AssignmentStmtNode(const string& name, unique_ptr<ExprNode> value)
        : name(name), value(move(value)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class IfStmtNode : public StmtNode {
public:
    unique_ptr<ExprNode> condition;
    vector<unique_ptr<StmtNode>> thenBranch, elseBranch;
    IfStmtNode(unique_ptr<ExprNode> condition, vector<unique_ptr<StmtNode>> thenBranch, vector<unique_ptr<StmtNode>> elseBranch)
        : condition(move(condition)), thenBranch(move(thenBranch)), elseBranch(move(elseBranch)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class ForStmtNode : public StmtNode {
public:
    string var;
    unique_ptr<ExprNode> start, end;
    vector<unique_ptr<StmtNode>> body;
    ForStmtNode(const string& var, unique_ptr<ExprNode> start, unique_ptr<ExprNode> end, vector<unique_ptr<StmtNode>> body)
        : var(var), start(move(start)), end(move(end)), body(move(body)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class WhileStmtNode : public StmtNode {
public:
    unique_ptr<ExprNode> condition;
    vector<unique_ptr<StmtNode>> body;
    WhileStmtNode(unique_ptr<ExprNode> condition, vector<unique_ptr<StmtNode>> body)
        : condition(move(condition)), body(move(body)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class FunctionStmtNode : public StmtNode {
public:
    string name;
    vector<string> params;
    vector<unique_ptr<StmtNode>> body;
    FunctionStmtNode(const string& name, vector<string> params, vector<unique_ptr<StmtNode>> body)
        : name(name), params(move(params)), body(move(body)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class PrintStmtNode : public StmtNode {
public:
    unique_ptr<ExprNode> value;
    PrintStmtNode(unique_ptr<ExprNode> value) : value(move(value)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class ReturnStmtNode : public StmtNode {
public:
    unique_ptr<ExprNode> value;
    ReturnStmtNode(unique_ptr<ExprNode> value) : value(move(value)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

class ProgramNode : public ASTNode {
public:
    vector<unique_ptr<StmtNode>> statements;
    ProgramNode(vector<unique_ptr<StmtNode>> statements) : statements(move(statements)) {}
    void accept(ASTVisitor& visitor) const override { visitor.visit(*this); }
};

#endif
