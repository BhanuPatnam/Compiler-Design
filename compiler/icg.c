#include "icg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int temp_count = 0;
static int label_count = 0;

static char* new_temp() {
    char* temp = (char*)malloc(10);
    sprintf(temp, "t%d", temp_count++);
    return temp;
}

static char* new_label() {
    char* label = (char*)malloc(10);
    sprintf(label, "L%d", label_count++);
    return label;
}

static char* process_icg(ASTNode* node) {
    if (!node) return NULL;

    switch (node->type) {
        case NODE_INT_EXPR: {
            char* res = (char*)malloc(20);
            sprintf(res, "%d", node->data.int_expr.value);
            return res;
        }
        case NODE_FLOAT_EXPR: {
            char* res = (char*)malloc(20);
            sprintf(res, "%g", node->data.float_expr.value);
            return res;
        }
        case NODE_CHAR_EXPR: {
            char* res = (char*)malloc(10);
            sprintf(res, "'%c'", node->data.char_expr.value);
            return res;
        }
        case NODE_VARIABLE_EXPR:
            return strdup(node->data.variable.name);
        case NODE_BINARY_EXPR: {
            char* left = process_icg(node->data.binary.left);
            char* right = process_icg(node->data.binary.right);
            char* temp = new_temp();
            printf("  %s = %s %s %s\n", temp, left, node->data.binary.op, right);
            free(left); free(right);
            return temp;
        }
        case NODE_FUNCTION_CALL_EXPR: {
            char** args = (char**)malloc(sizeof(char*) * node->data.func_call.arg_count);
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                args[i] = process_icg(node->data.func_call.args[i]);
            }
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                printf("  param %s\n", args[i]);
                free(args[i]);
            }
            char* temp = new_temp();
            printf("  %s = call %s, %d\n", temp, node->data.func_call.name, node->data.func_call.arg_count);
            free(args);
            return temp;
        }
        case NODE_ASSIGNMENT_STMT: {
            char* val = process_icg(node->data.assignment.value);
            if (node->data.assignment.target->type == NODE_VARIABLE_EXPR) {
                printf("  %s = %s\n", node->data.assignment.target->data.variable.name, val);
            } else if (node->data.assignment.target->type == NODE_ARRAY_ACCESS_EXPR) {
                char* idx = process_icg(node->data.assignment.target->data.array_access.indices[0]); // simplify to 1D for TAC view
                printf("  %s[%s] = %s\n", node->data.assignment.target->data.array_access.name, idx, val);
                free(idx);
            }
            free(val);
            return NULL;
        }
        case NODE_IF_STMT: {
            char* cond = process_icg(node->data.if_stmt.condition);
            char* label_else = new_label();
            char* label_end = new_label();
            printf("  if not %s goto %s\n", cond, label_else);
            for (int i = 0; i < node->data.if_stmt.then_count; i++) process_icg(node->data.if_stmt.then_branch[i]);
            printf("  goto %s\n", label_end);
            printf("%s:\n", label_else);
            for (int i = 0; i < node->data.if_stmt.else_count; i++) process_icg(node->data.if_stmt.else_branch[i]);
            printf("%s:\n", label_end);
            free(cond); free(label_else); free(label_end);
            return NULL;
        }
        case NODE_FOR_STMT: {
            char* start = process_icg(node->data.for_stmt.start);
            char* end = process_icg(node->data.for_stmt.end);
            char* label_start = new_label();
            char* label_end = new_label();
            printf("  %s = %s\n", node->data.for_stmt.var, start);
            printf("%s:\n", label_start);
            char* temp = new_temp();
            printf("  %s = %s <= %s\n", temp, node->data.for_stmt.var, end);
            printf("  if not %s goto %s\n", temp, label_end);
            for (int i = 0; i < node->data.for_stmt.body_count; i++) process_icg(node->data.for_stmt.body[i]);
            printf("  %s = %s + 1\n", node->data.for_stmt.var, node->data.for_stmt.var);
            printf("  goto %s\n", label_start);
            printf("%s:\n", label_end);
            free(start); free(end); free(label_start); free(label_end); free(temp);
            return NULL;
        }
        case NODE_WHILE_STMT: {
            char* label_start = new_label();
            char* label_end = new_label();
            printf("%s:\n", label_start);
            char* cond = process_icg(node->data.while_stmt.condition);
            printf("  if not %s goto %s\n", cond, label_end);
            for (int i = 0; i < node->data.while_stmt.body_count; i++) process_icg(node->data.while_stmt.body[i]);
            printf("  goto %s\n", label_start);
            printf("%s:\n", label_end);
            free(cond); free(label_start); free(label_end);
            return NULL;
        }
        case NODE_FUNCTION_STMT: {
            printf("func begin %s\n", node->data.function.name);
            for (int i = 0; i < node->data.function.body_count; i++) process_icg(node->data.function.body[i]);
            printf("func end\n\n");
            return NULL;
        }
        case NODE_PRINT_STMT: {
            char* val = process_icg(node->data.print.value);
            printf("  print %s\n", val);
            free(val);
            return NULL;
        }
        case NODE_RETURN_STMT: {
            char* val = process_icg(node->data.return_stmt.value);
            printf("  return %s\n", val);
            free(val);
            return NULL;
        }
        case NODE_PROGRAM:
            for (int i = 0; i < node->data.program.stmt_count; i++) process_icg(node->data.program.statements[i]);
            return NULL;
        case NODE_DEREF_EXPR: {
            char* temp = new_temp();
            printf("  %s = *%s\n", temp, node->data.deref.name);
            return temp;
        }
        case NODE_ADDR_OF_EXPR: {
            char* temp = new_temp();
            printf("  %s = &%s\n", temp, node->data.addr_of.name);
            return temp;
        }
        case NODE_ARRAY_ACCESS_EXPR: {
            char* idx = process_icg(node->data.array_access.indices[0]);
            char* temp = new_temp();
            printf("  %s = %s[%s]\n", temp, node->data.array_access.name, idx);
            free(idx);
            return temp;
        }
        case NODE_ARRAY_DECL_STMT:
            printf("  alloc %s, %d\n", node->data.array_decl.name, node->data.array_decl.sizes[0]);
            return NULL;
        case NODE_POINTER_DECL_STMT:
            printf("  alloc_ptr %s\n", node->data.pointer_decl.name);
            return NULL;
        case NODE_DECL_STMT:
            printf("  alloc %s\n", node->data.decl.name);
            return NULL;
        default: return NULL;
    }
}

void generate_icg(ASTNode* node) {
    temp_count = 0;
    label_count = 0;
    process_icg(node);
}
