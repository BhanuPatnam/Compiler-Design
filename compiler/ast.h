#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    NODE_INT_EXPR,
    NODE_FLOAT_EXPR,
    NODE_CHAR_EXPR,
    NODE_VARIABLE_EXPR,
    NODE_BINARY_EXPR,
    NODE_FUNCTION_CALL_EXPR,
    NODE_ASSIGNMENT_STMT,
    NODE_IF_STMT,
    NODE_FOR_STMT,
    NODE_WHILE_STMT,
    NODE_FUNCTION_STMT,
    NODE_PRINT_STMT,
    NODE_RETURN_STMT,
    NODE_DEREF_EXPR,
    NODE_ADDR_OF_EXPR,
    NODE_ARRAY_ACCESS_EXPR,
    NODE_ARRAY_DECL_STMT,
    NODE_POINTER_DECL_STMT,
    NODE_DECL_STMT,
    NODE_PROGRAM
} NodeType;

struct ASTNode;

typedef struct {
    int value;
} IntExpr;

typedef struct {
    float value;
} FloatExpr;

typedef struct {
    char value;
} CharExpr;

typedef struct {
    char* name;
} VariableExpr;

typedef struct {
    char* op;
    struct ASTNode* left;
    struct ASTNode* right;
} BinaryExpr;

typedef struct {
    char* name;
    struct ASTNode** args;
    int arg_count;
} FunctionCallExpr;

typedef struct {
    struct ASTNode* target;
    struct ASTNode* value;
} AssignmentStmt;

typedef struct {
    char* name;
    struct ASTNode* expr;
    int level;
} DerefExpr;

typedef struct {
    char* name;
} AddrOfExpr;

typedef struct {
    char* name;
    struct ASTNode** indices;
    int index_count;
} ArrayAccessExpr;

typedef struct {
    char* name;
    int* sizes;
    int dim_count;
    char* type;
} ArrayDeclStmt;

typedef struct {
    char* name;
    char* type;
    int level;
} PointerDeclStmt;

typedef struct {
    char* name;
    char* type;
} DeclStmt;

typedef struct {
    struct ASTNode* condition;
    struct ASTNode** then_branch;
    int then_count;
    struct ASTNode** else_branch;
    int else_count;
} IfStmt;

typedef struct {
    char* var;
    struct ASTNode* start;
    struct ASTNode* end;
    struct ASTNode** body;
    int body_count;
} ForStmt;

typedef struct {
    struct ASTNode* condition;
    struct ASTNode** body;
    int body_count;
} WhileStmt;

typedef struct {
    char* name;
    char** params;
    int param_count;
    struct ASTNode** body;
    int body_count;
} FunctionStmt;

typedef struct {
    struct ASTNode* value;
} PrintStmt;

typedef struct {
    struct ASTNode* value;
} ReturnStmt;

typedef struct {
    struct ASTNode** statements;
    int stmt_count;
} Program;

// ASTNode represents a node in the Parse Tree
typedef struct ASTNode {
    NodeType type;
    union {
        IntExpr int_expr;
        FloatExpr float_expr;
        CharExpr char_expr;
        VariableExpr variable;
        BinaryExpr binary;
        FunctionCallExpr func_call;
        AssignmentStmt assignment;
        IfStmt if_stmt;
        ForStmt for_stmt;
        WhileStmt while_stmt;
        FunctionStmt function;
        PrintStmt print;
        ReturnStmt return_stmt;
        DerefExpr deref;
        AddrOfExpr addr_of;
        ArrayAccessExpr array_access;
        ArrayDeclStmt array_decl;
        PointerDeclStmt pointer_decl;
        DeclStmt decl;
        Program program;
    } data;
} ASTNode;

// AST Utilities
ASTNode* create_node(NodeType type);
void ast_free(ASTNode* node);

#endif
