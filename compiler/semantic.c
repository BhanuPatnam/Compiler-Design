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
        printf("  [Semantic Trace] Defining symbol: %s\n", name);
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
        printf("  [Semantic Trace] Defining function: %s\n", name);
        function_names[function_count++] = strdup(name);
    }
}

// Check nodes recursively
static int check_node(ASTNode* node) {
    if (!node) return 1;
    int success = 1;

    switch (node->type) {
        case NODE_INT_EXPR:
        case NODE_FLOAT_EXPR:
        case NODE_CHAR_EXPR: break;
        case NODE_VARIABLE_EXPR:
            printf("  [Semantic Trace] Checking variable usage: %s\n", node->data.variable.name);
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
            printf("  [Semantic Trace] Checking function call: %s\n", node->data.func_call.name);
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
            if (node->data.assignment.target->type == NODE_VARIABLE_EXPR) {
                printf("  [Semantic Trace] Checking assignment to variable: %s\n", node->data.assignment.target->data.variable.name);
                define_symbol(node->data.assignment.target->data.variable.name);
            } else {
                printf("  [Semantic Trace] Checking assignment to complex target\n");
                success &= check_node(node->data.assignment.target);
            }
            break;
        case NODE_DEREF_EXPR:
            printf("  [Semantic Trace] Checking dereference of: %s\n", node->data.deref.name);
            if (!is_symbol_defined(node->data.deref.name)) {
                fprintf(stderr, "Semantic Error: Undefined pointer '%s' used in dereference.\n", node->data.deref.name);
                return 0;
            }
            break;
        case NODE_ADDR_OF_EXPR:
            printf("  [Semantic Trace] Checking address-of operator on: %s\n", node->data.addr_of.name);
            if (!is_symbol_defined(node->data.addr_of.name)) {
                fprintf(stderr, "Semantic Error: Undefined variable '%s' used in address-of operator.\n", node->data.addr_of.name);
                return 0;
            }
            break;
        case NODE_ARRAY_ACCESS_EXPR:
            printf("  [Semantic Trace] Checking array access: %s\n", node->data.array_access.name);
            if (!is_symbol_defined(node->data.array_access.name)) {
                fprintf(stderr, "Semantic Error: Undefined array '%s' used in access.\n", node->data.array_access.name);
                return 0;
            }
            for (int i = 0; i < node->data.array_access.index_count; i++) {
                success &= check_node(node->data.array_access.indices[i]);
            }
            break;
        case NODE_ARRAY_DECL_STMT:
            printf("  [Semantic Trace] Checking array declaration: %s\n", node->data.array_decl.name);
            define_symbol(node->data.array_decl.name);
            break;
        case NODE_POINTER_DECL_STMT:
            printf("  [Semantic Trace] Checking pointer declaration: %s\n", node->data.pointer_decl.name);
            define_symbol(node->data.pointer_decl.name);
            break;
        case NODE_DECL_STMT:
            printf("  [Semantic Trace] Checking variable declaration: %s (type: %s)\n", node->data.decl.name, node->data.decl.type);
            define_symbol(node->data.decl.name);
            break;
        case NODE_STRUCT_DEF_STMT:
            printf("  [Semantic Trace] Checking struct definition: %s\n", node->data.struct_def.struct_name);
            define_symbol(node->data.struct_def.struct_name);
            // In a real compiler, we'd also check struct members for validity
            for (int i = 0; i < node->data.struct_def.member_count; i++) {
                success &= check_node(node->data.struct_def.members[i]);
            }
            break;
        case NODE_STRUCT_MEMBER_ACCESS_EXPR:
            printf("  [Semantic Trace] Checking struct member access: .%s\n", node->data.struct_access.member_name);
            success &= check_node(node->data.struct_access.target);
            break;
        case NODE_IF_STMT:
            printf("  [Semantic Trace] Checking IF statement\n");
            success &= check_node(node->data.if_stmt.condition);
            for (int i = 0; i < node->data.if_stmt.then_count; i++) success &= check_node(node->data.if_stmt.then_branch[i]);
            for (int i = 0; i < node->data.if_stmt.else_count; i++) success &= check_node(node->data.if_stmt.else_branch[i]);
            break;
        case NODE_FOR_STMT:
            printf("  [Semantic Trace] Checking FOR loop with variable: %s\n", node->data.for_stmt.var);
            // For loop variable is defined in the loop header
            define_symbol(node->data.for_stmt.var);
            success &= check_node(node->data.for_stmt.start);
            success &= check_node(node->data.for_stmt.end);
            for (int i = 0; i < node->data.for_stmt.body_count; i++) success &= check_node(node->data.for_stmt.body[i]);
            break;
        case NODE_WHILE_STMT:
            printf("  [Semantic Trace] Checking WHILE loop\n");
            success &= check_node(node->data.while_stmt.condition);
            for (int i = 0; i < node->data.while_stmt.body_count; i++) success &= check_node(node->data.while_stmt.body[i]);
            break;
        case NODE_FUNCTION_STMT: {
            printf("  [Semantic Trace] Checking function definition: %s\n", node->data.function.name);
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
            printf("  [Semantic Trace] Exiting function scope: %s\n", node->data.function.name);
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
