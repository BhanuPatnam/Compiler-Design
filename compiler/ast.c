#include "ast.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

void ast_free(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_INT_EXPR:
        case NODE_FLOAT_EXPR:
        case NODE_CHAR_EXPR: break;
        case NODE_VARIABLE_EXPR: free(node->data.variable.name); break;
        case NODE_BINARY_EXPR:
            free(node->data.binary.op);
            ast_free(node->data.binary.left);
            ast_free(node->data.binary.right);
            break;
        case NODE_FUNCTION_CALL_EXPR:
            free(node->data.func_call.name);
            for (int i = 0; i < node->data.func_call.arg_count; i++) ast_free(node->data.func_call.args[i]);
            free(node->data.func_call.args);
            break;
        case NODE_ASSIGNMENT_STMT:
            ast_free(node->data.assignment.target);
            ast_free(node->data.assignment.value);
            break;
        case NODE_DEREF_EXPR:
            free(node->data.deref.name);
            ast_free(node->data.deref.expr);
            break;
        case NODE_ADDR_OF_EXPR:
            free(node->data.addr_of.name);
            break;
        case NODE_ARRAY_ACCESS_EXPR:
            free(node->data.array_access.name);
            for (int i = 0; i < node->data.array_access.index_count; i++) ast_free(node->data.array_access.indices[i]);
            free(node->data.array_access.indices);
            break;
        case NODE_ARRAY_DECL_STMT:
            free(node->data.array_decl.name);
            free(node->data.array_decl.sizes);
            free(node->data.array_decl.type);
            break;
        case NODE_POINTER_DECL_STMT:
            free(node->data.pointer_decl.name);
            free(node->data.pointer_decl.type);
            break;
        case NODE_DECL_STMT:
            free(node->data.decl.name);
            free(node->data.decl.type);
            break;
        case NODE_STRUCT_DEF_STMT:
            free(node->data.struct_def.struct_name);
            for (int i = 0; i < node->data.struct_def.member_count; i++) ast_free(node->data.struct_def.members[i]);
            free(node->data.struct_def.members);
            break;
        case NODE_STRUCT_MEMBER_ACCESS_EXPR:
            ast_free(node->data.struct_access.target);
            free(node->data.struct_access.member_name);
            break;
        case NODE_IF_STMT:
            ast_free(node->data.if_stmt.condition);
            for (int i = 0; i < node->data.if_stmt.then_count; i++) ast_free(node->data.if_stmt.then_branch[i]);
            free(node->data.if_stmt.then_branch);
            for (int i = 0; i < node->data.if_stmt.else_count; i++) ast_free(node->data.if_stmt.else_branch[i]);
            free(node->data.if_stmt.else_branch);
            break;
        case NODE_FOR_STMT:
            free(node->data.for_stmt.var);
            ast_free(node->data.for_stmt.start);
            ast_free(node->data.for_stmt.end);
            for (int i = 0; i < node->data.for_stmt.body_count; i++) ast_free(node->data.for_stmt.body[i]);
            free(node->data.for_stmt.body);
            break;
        case NODE_WHILE_STMT:
            ast_free(node->data.while_stmt.condition);
            for (int i = 0; i < node->data.while_stmt.body_count; i++) ast_free(node->data.while_stmt.body[i]);
            free(node->data.while_stmt.body);
            break;
        case NODE_FUNCTION_STMT:
            free(node->data.function.name);
            for (int i = 0; i < node->data.function.param_count; i++) free(node->data.function.params[i]);
            free(node->data.function.params);
            for (int i = 0; i < node->data.function.body_count; i++) ast_free(node->data.function.body[i]);
            free(node->data.function.body);
            break;
        case NODE_PRINT_STMT: ast_free(node->data.print.value); break;
        case NODE_RETURN_STMT: ast_free(node->data.return_stmt.value); break;
        case NODE_PROGRAM:
            for (int i = 0; i < node->data.program.stmt_count; i++) ast_free(node->data.program.statements[i]);
            free(node->data.program.statements);
            break;
    }
    free(node);
}
