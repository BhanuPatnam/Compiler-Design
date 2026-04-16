#include "optimization.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int optimizations_made = 0;
static int optimization_errors = 0;

static ASTNode* constant_folding(ASTNode* node) {
    if (!node) return NULL;

    if (node->type == NODE_BINARY_EXPR) {
        node->data.binary.left = constant_folding(node->data.binary.left);
        node->data.binary.right = constant_folding(node->data.binary.right);

        if (node->data.binary.left && node->data.binary.right && 
            node->data.binary.left->type == NODE_INT_EXPR && node->data.binary.right->type == NODE_INT_EXPR) {
            int left = node->data.binary.left->data.int_expr.value;
            int right = node->data.binary.right->data.int_expr.value;
            int res = 0;
            int foldable = 1;

            if (strcmp(node->data.binary.op, "+") == 0) res = left + right;
            else if (strcmp(node->data.binary.op, "-") == 0) res = left - right;
            else if (strcmp(node->data.binary.op, "*") == 0) res = left * right;
            else if (strcmp(node->data.binary.op, "/") == 0) {
                if (right != 0) res = left / right;
                else {
                    printf("  [Optimization Error] Division by zero detected: %d / %d\n", left, right);
                    optimization_errors++;
                    foldable = 0;
                }
            } else foldable = 0;

            if (foldable) {
                optimizations_made++;
                printf("  Optimizing: %d %s %d -> %d\n", left, node->data.binary.op, right, res);
                ASTNode* new_node = create_node(NODE_INT_EXPR);
                new_node->data.int_expr.value = res;
                // In a real compiler we'd free the old nodes
                return new_node;
            }
        }
    } else if (node->type == NODE_PROGRAM) {
        for (int i = 0; i < node->data.program.stmt_count; i++) {
            node->data.program.statements[i] = constant_folding(node->data.program.statements[i]);
        }
    } else if (node->type == NODE_ASSIGNMENT_STMT) {
        node->data.assignment.value = constant_folding(node->data.assignment.value);
    } else if (node->type == NODE_IF_STMT) {
        node->data.if_stmt.condition = constant_folding(node->data.if_stmt.condition);
        for (int i = 0; i < node->data.if_stmt.then_count; i++) node->data.if_stmt.then_branch[i] = constant_folding(node->data.if_stmt.then_branch[i]);
        for (int i = 0; i < node->data.if_stmt.else_count; i++) node->data.if_stmt.else_branch[i] = constant_folding(node->data.if_stmt.else_branch[i]);
    } else if (node->type == NODE_WHILE_STMT) {
        node->data.while_stmt.condition = constant_folding(node->data.while_stmt.condition);
        for (int i = 0; i < node->data.while_stmt.body_count; i++) node->data.while_stmt.body[i] = constant_folding(node->data.while_stmt.body[i]);
    } else if (node->type == NODE_FOR_STMT) {
        node->data.for_stmt.start = constant_folding(node->data.for_stmt.start);
        node->data.for_stmt.end = constant_folding(node->data.for_stmt.end);
        for (int i = 0; i < node->data.for_stmt.body_count; i++) node->data.for_stmt.body[i] = constant_folding(node->data.for_stmt.body[i]);
    } else if (node->type == NODE_FUNCTION_STMT) {
        for (int i = 0; i < node->data.function.body_count; i++) node->data.function.body[i] = constant_folding(node->data.function.body[i]);
    } else if (node->type == NODE_PRINT_STMT) {
        node->data.print.value = constant_folding(node->data.print.value);
    } else if (node->type == NODE_RETURN_STMT) {
        node->data.return_stmt.value = constant_folding(node->data.return_stmt.value);
    }

    return node;
}

ASTNode* optimize_ast(ASTNode* node, int* success) {
    optimizations_made = 0;
    optimization_errors = 0;
    ASTNode* optimized = constant_folding(node);
    if (optimizations_made == 0) {
        printf("  No constant folding optimizations possible.\n");
    } else {
        printf("  Total optimizations made: %d\n", optimizations_made);
    }
    
    if (optimization_errors > 0) {
        *success = 0;
    } else {
        *success = 1;
    }
    return optimized;
}
