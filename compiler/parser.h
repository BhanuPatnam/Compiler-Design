#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum {
    NODE_NUMBER_EXPR,
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
    NODE_PROGRAM
} NodeType;

struct ASTNode;

typedef struct {
    int value;
} NumberExpr;

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
    char* name;
    struct ASTNode* value;
} AssignmentStmt;

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
        NumberExpr number;
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
        Program program;
    } data;
} ASTNode;

typedef struct {
    Token* tokens;
    int count;
    int pos;
} Parser;

Parser parser_init(Token* tokens, int count);
ASTNode* parser_parse(Parser* parser);
void ast_free(ASTNode* node);

#endif
