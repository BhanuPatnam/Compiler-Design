#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Symbol Table
static char* symbols[1000];
static int symbol_count = 0;

static char* function_names[100];
static int function_count = 0;

static void clear_symbols() {
    for (int i = 0; i < symbol_count; i++) free(symbols[i]);
    symbol_count = 0;
}

static int is_symbol_defined(const char* name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbols[i], name) == 0) return 1;
    }
    return 0;
}

static void define_symbol(const char* name) {
    if (!is_symbol_defined(name)) {
        symbols[symbol_count++] = strdup(name);
    }
}

static int is_function_defined(const char* name) {
    for (int i = 0; i < function_count; i++) {
        if (strcmp(function_names[i], name) == 0) return 1;
    }
    return 0;
}

static void define_function(const char* name) {
    if (!is_function_defined(name)) {
        function_names[function_count++] = strdup(name);
    }
}

// Check nodes recursively
static int check_node(ASTNode* node) {
    if (!node) return 1;
    int success = 1;

    switch (node->type) {
        case NODE_NUMBER_EXPR: break;
        case NODE_VARIABLE_EXPR:
            if (!is_symbol_defined(node->data.variable.name)) {
                fprintf(stderr, "Semantic Error: Undefined variable '%s' used in expression.\n", node->data.variable.name);
                return 0;
            }
            break;
        case NODE_BINARY_EXPR:
            success &= check_node(node->data.binary.left);
            success &= check_node(node->data.binary.right);
            break;
        case NODE_FUNCTION_CALL_EXPR:
            if (!is_function_defined(node->data.func_call.name)) {
                fprintf(stderr, "Semantic Warning: Function '%s' is called but not defined.\n", node->data.func_call.name);
                // We'll treat this as a warning for now, but you can return 0 for error.
            }
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                success &= check_node(node->data.func_call.args[i]);
            }
            break;
        case NODE_ASSIGNMENT_STMT:
            // In our simple model, the first assignment IS the declaration
            // So we check the value being assigned first
            success &= check_node(node->data.assignment.value);
            define_symbol(node->data.assignment.name);
            break;
        case NODE_IF_STMT:
            success &= check_node(node->data.if_stmt.condition);
            for (int i = 0; i < node->data.if_stmt.then_count; i++) success &= check_node(node->data.if_stmt.then_branch[i]);
            for (int i = 0; i < node->data.if_stmt.else_count; i++) success &= check_node(node->data.if_stmt.else_branch[i]);
            break;
        case NODE_FOR_STMT:
            // For loop variable is defined in the loop header
            define_symbol(node->data.for_stmt.var);
            success &= check_node(node->data.for_stmt.start);
            success &= check_node(node->data.for_stmt.end);
            for (int i = 0; i < node->data.for_stmt.body_count; i++) success &= check_node(node->data.for_stmt.body[i]);
            break;
        case NODE_WHILE_STMT:
            success &= check_node(node->data.while_stmt.condition);
            for (int i = 0; i < node->data.while_stmt.body_count; i++) success &= check_node(node->data.while_stmt.body[i]);
            break;
        case NODE_FUNCTION_STMT: {
            // New scope for function body
            // (In a more advanced compiler, we'd use a stack of symbol tables)
            char* current_symbols[1000];
            int current_count = symbol_count;
            for (int i = 0; i < symbol_count; i++) current_symbols[i] = strdup(symbols[i]);
            
            clear_symbols();
            for (int i = 0; i < node->data.function.param_count; i++) {
                define_symbol(node->data.function.params[i]);
            }
            
            for (int i = 0; i < node->data.function.body_count; i++) {
                success &= check_node(node->data.function.body[i]);
            }
            
            // Restore outer symbols
            clear_symbols();
            for (int i = 0; i < current_count; i++) {
                symbols[symbol_count++] = current_symbols[i];
            }
            break;
        }
        case NODE_PRINT_STMT: success &= check_node(node->data.print.value); break;
        case NODE_RETURN_STMT: success &= check_node(node->data.return_stmt.value); break;
        case NODE_PROGRAM:
            // First, collect all function names
            for (int i = 0; i < node->data.program.stmt_count; i++) {
                if (node->data.program.statements[i]->type == NODE_FUNCTION_STMT) {
                    define_function(node->data.program.statements[i]->data.function.name);
                }
            }
            // Then, check all statements
            for (int i = 0; i < node->data.program.stmt_count; i++) {
                success &= check_node(node->data.program.statements[i]);
            }
            break;
    }
    return success;
}

int semantic_analyze(ASTNode* root) {
    clear_symbols();
    function_count = 0;
    return check_node(root);
}
