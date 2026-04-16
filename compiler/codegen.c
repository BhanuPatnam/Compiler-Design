#include "codegen.h"

// Simple Symbol Table to track declared variables
static char* declared_vars[1000];
static int declared_count = 0;

static void clear_symbols() {
    for (int i = 0; i < declared_count; i++) free(declared_vars[i]);
    declared_count = 0;
}

static int is_declared(const char* name) {
    for (int i = 0; i < declared_count; i++) {
        if (strcmp(declared_vars[i], name) == 0) return 1;
    }
    return 0;
}

static void add_symbol(const char* name) {
    if (!is_declared(name)) {
        declared_vars[declared_count++] = strdup(name);
    }
}

static void print_indent(int level, FILE* out) {
    for (int i = 0; i < level; i++) fprintf(out, "    ");
}

static const char* get_c_type(ASTNode* node) {
    if (!node) return "int";
    switch (node->type) {
        case NODE_INT_EXPR: return "int";
        case NODE_FLOAT_EXPR: return "float";
        case NODE_CHAR_EXPR: return "char";
        default: return "auto"; // C++ auto or just default to int
    }
}

static void generate_node(ASTNode* node, int indent_level, FILE* out) {
    if (!node) return;
    switch (node->type) {
        case NODE_INT_EXPR:
            fprintf(out, "%d", node->data.int_expr.value);
            break;
        case NODE_FLOAT_EXPR:
            fprintf(out, "%g", node->data.float_expr.value);
            break;
        case NODE_CHAR_EXPR:
            fprintf(out, "'%c'", node->data.char_expr.value);
            break;
        case NODE_VARIABLE_EXPR:
            fprintf(out, "%s", node->data.variable.name);
            break;
        case NODE_BINARY_EXPR: {
            fprintf(out, "(");
            generate_node(node->data.binary.left, 0, out);
            char* op = node->data.binary.op;
            if (strcmp(op, "=") == 0) op = "==";
            else if (strcmp(op, "≠") == 0) op = "!=";
            else if (strcmp(op, "≤") == 0) op = "<=";
            else if (strcmp(op, "≥") == 0) op = ">=";
            fprintf(out, " %s ", op);
            generate_node(node->data.binary.right, 0, out);
            fprintf(out, ")");
            break;
        }
        case NODE_FUNCTION_CALL_EXPR: {
            fprintf(out, "%s(", node->data.func_call.name);
            for (int i = 0; i < node->data.func_call.arg_count; i++) {
                generate_node(node->data.func_call.args[i], 0, out);
                if (i < node->data.func_call.arg_count - 1) fprintf(out, ", ");
            }
            fprintf(out, ")");
            break;
        }
        case NODE_ASSIGNMENT_STMT:
            print_indent(indent_level, out);
            if (node->data.assignment.target->type == NODE_VARIABLE_EXPR) {
                char* name = node->data.assignment.target->data.variable.name;
                if (!is_declared(name)) {
                    const char* type = get_c_type(node->data.assignment.value);
                    if (strcmp(type, "auto") == 0) type = "int";
                    fprintf(out, "%s %s = ", type, name);
                    add_symbol(name);
                } else {
                    fprintf(out, "%s = ", name);
                }
            } else {
                generate_node(node->data.assignment.target, 0, out);
                fprintf(out, " = ");
            }
            generate_node(node->data.assignment.value, 0, out);
            fprintf(out, ";\n");
            break;
        case NODE_DEREF_EXPR:
            fprintf(out, "(");
            for (int i = 0; i < node->data.deref.level; i++) fprintf(out, "*");
            fprintf(out, "%s)", node->data.deref.name);
            break;
        case NODE_ADDR_OF_EXPR:
            fprintf(out, "(&%s)", node->data.addr_of.name);
            break;
        case NODE_ARRAY_ACCESS_EXPR:
            fprintf(out, "%s", node->data.array_access.name);
            for (int i = 0; i < node->data.array_access.index_count; i++) {
                fprintf(out, "[");
                generate_node(node->data.array_access.indices[i], 0, out);
                fprintf(out, "]");
            }
            break;
        case NODE_ARRAY_DECL_STMT:
            print_indent(indent_level, out);
            fprintf(out, "%s %s", node->data.array_decl.type, node->data.array_decl.name);
            for (int i = 0; i < node->data.array_decl.dim_count; i++) {
                fprintf(out, "[%d]", node->data.array_decl.sizes[i]);
            }
            fprintf(out, ";\n");
            add_symbol(node->data.array_decl.name);
            break;
        case NODE_POINTER_DECL_STMT:
            print_indent(indent_level, out);
            fprintf(out, "%s", node->data.pointer_decl.type);
            for (int i = 0; i < node->data.pointer_decl.level; i++) fprintf(out, "*");
            fprintf(out, " %s;\n", node->data.pointer_decl.name);
            add_symbol(node->data.pointer_decl.name);
            break;
        case NODE_DECL_STMT:
            print_indent(indent_level, out);
            fprintf(out, "%s %s;\n", node->data.decl.type, node->data.decl.name);
            add_symbol(node->data.decl.name);
            break;
        case NODE_IF_STMT:
            print_indent(indent_level, out);
            fprintf(out, "if ");
            generate_node(node->data.if_stmt.condition, 0, out);
            fprintf(out, " {\n");
            for (int i = 0; i < node->data.if_stmt.then_count; i++) {
                generate_node(node->data.if_stmt.then_branch[i], indent_level + 1, out);
            }
            print_indent(indent_level, out);
            fprintf(out, "}");
            if (node->data.if_stmt.else_count > 0) {
                fprintf(out, " else {\n");
                for (int i = 0; i < node->data.if_stmt.else_count; i++) {
                    generate_node(node->data.if_stmt.else_branch[i], indent_level + 1, out);
                }
                print_indent(indent_level, out);
                fprintf(out, "}\n");
            } else {
                fprintf(out, "\n");
            }
            break;
        case NODE_FOR_STMT:
            print_indent(indent_level, out);
            // Convert 'for i <- 1 to n' into C 'for (int i = 1; i <= n; i++)'
            fprintf(out, "for (int %s = ", node->data.for_stmt.var);
            generate_node(node->data.for_stmt.start, 0, out);
            fprintf(out, "; %s <= ", node->data.for_stmt.var);
            generate_node(node->data.for_stmt.end, 0, out);
            fprintf(out, "; %s++) {\n", node->data.for_stmt.var);
            for (int i = 0; i < node->data.for_stmt.body_count; i++) {
                generate_node(node->data.for_stmt.body[i], indent_level + 1, out);
            }
            print_indent(indent_level, out);
            fprintf(out, "}\n");
            break;
        case NODE_WHILE_STMT:
            print_indent(indent_level, out);
            fprintf(out, "while (");
            generate_node(node->data.while_stmt.condition, 0, out);
            fprintf(out, ") {\n");
            for (int i = 0; i < node->data.while_stmt.body_count; i++) {
                generate_node(node->data.while_stmt.body[i], indent_level + 1, out);
            }
            print_indent(indent_level, out);
            fprintf(out, "}\n");
            break;
        case NODE_FUNCTION_STMT: {
            clear_symbols();
            for (int i = 0; i < node->data.function.param_count; i++) {
                add_symbol(node->data.function.params[i]);
            }
            print_indent(indent_level, out);
            fprintf(out, "int %s(", node->data.function.name);
            for (int i = 0; i < node->data.function.param_count; i++) {
                fprintf(out, "int %s", node->data.function.params[i]);
                if (i < node->data.function.param_count - 1) fprintf(out, ", ");
            }
            fprintf(out, ") {\n");
            for (int i = 0; i < node->data.function.body_count; i++) {
                generate_node(node->data.function.body[i], indent_level + 1, out);
            }
            print_indent(indent_level, out);
            fprintf(out, "}\n\n");
            break;
        }
        case NODE_PRINT_STMT:
            print_indent(indent_level, out);
            const char* type = get_c_type(node->data.print.value);
            const char* fmt = "%d";
            if (strcmp(type, "float") == 0) fmt = "%f";
            else if (strcmp(type, "char") == 0) fmt = "%c";
            fprintf(out, "printf(\"%s\\n\", ", fmt);
            generate_node(node->data.print.value, 0, out);
            fprintf(out, ");\n");
            break;
        case NODE_RETURN_STMT:
            print_indent(indent_level, out);
            fprintf(out, "return ");
            generate_node(node->data.return_stmt.value, 0, out);
            fprintf(out, ";\n");
            break;
        case NODE_PROGRAM:
            // This case handles the body of the program. 
            // In main.c we will separate functions from the main body.
            for (int i = 0; i < node->data.program.stmt_count; i++) {
                generate_node(node->data.program.statements[i], indent_level, out);
            }
            break;
    }
}

void codegen_generate(ASTNode* node, FILE* out) {
    if (node->type == NODE_PROGRAM) {
        // First pass: Generate all functions
        for (int i = 0; i < node->data.program.stmt_count; i++) {
            if (node->data.program.statements[i]->type == NODE_FUNCTION_STMT) {
                generate_node(node->data.program.statements[i], 0, out);
            }
        }

        // Second pass: Generate main function for top-level statements
        clear_symbols();
        fprintf(out, "int main() {\n");
        for (int i = 0; i < node->data.program.stmt_count; i++) {
            if (node->data.program.statements[i]->type != NODE_FUNCTION_STMT) {
                generate_node(node->data.program.statements[i], 1, out);
            }
        }
        fprintf(out, "    return 0;\n");
        fprintf(out, "}\n");
    } else {
        generate_node(node, 0, out);
    }
}

static void print_ast_indent(int level) {
    for (int i = 0; i < level; i++) printf("  ");
}

void ast_print(ASTNode* node, int indent) {
    if (!node) return;
    print_ast_indent(indent);
    switch (node->type) {
        case NODE_INT_EXPR: printf("(int %d)\n", node->data.int_expr.value); break;
        case NODE_FLOAT_EXPR: printf("(float %g)\n", node->data.float_expr.value); break;
        case NODE_CHAR_EXPR: printf("(char '%c')\n", node->data.char_expr.value); break;
        case NODE_VARIABLE_EXPR: printf("(var %s)\n", node->data.variable.name); break;
        case NODE_BINARY_EXPR:
            printf("(binary %s\n", node->data.binary.op);
            ast_print(node->data.binary.left, indent + 1);
            ast_print(node->data.binary.right, indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_FUNCTION_CALL_EXPR:
            printf("(call %s\n", node->data.func_call.name);
            for (int i = 0; i < node->data.func_call.arg_count; i++) ast_print(node->data.func_call.args[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_ASSIGNMENT_STMT:
            printf("(assign\n");
            ast_print(node->data.assignment.target, indent + 1);
            ast_print(node->data.assignment.value, indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_DEREF_EXPR:
            printf("(deref level=%d %s)\n", node->data.deref.level, node->data.deref.name);
            break;
        case NODE_ADDR_OF_EXPR:
            printf("(addr_of %s)\n", node->data.addr_of.name);
            break;
        case NODE_ARRAY_ACCESS_EXPR:
            printf("(array_access %s\n", node->data.array_access.name);
            for (int i = 0; i < node->data.array_access.index_count; i++) ast_print(node->data.array_access.indices[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_ARRAY_DECL_STMT:
            printf("(array_decl %s dims=%d %s)\n", node->data.array_decl.name, node->data.array_decl.dim_count, node->data.array_decl.type);
            break;
        case NODE_POINTER_DECL_STMT:
            printf("(pointer_decl %s level=%d %s)\n", node->data.pointer_decl.name, node->data.pointer_decl.level, node->data.pointer_decl.type);
            break;
        case NODE_DECL_STMT:
            printf("(decl %s %s)\n", node->data.decl.name, node->data.decl.type);
            break;
        case NODE_PROGRAM:
            printf("(program\n");
            for (int i = 0; i < node->data.program.stmt_count; i++) ast_print(node->data.program.statements[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_IF_STMT:
            printf("(if\n");
            ast_print(node->data.if_stmt.condition, indent + 1);
            print_ast_indent(indent); printf("then\n");
            for (int i = 0; i < node->data.if_stmt.then_count; i++) ast_print(node->data.if_stmt.then_branch[i], indent + 1);
            if (node->data.if_stmt.else_count > 0) {
                print_ast_indent(indent); printf("else\n");
                for (int i = 0; i < node->data.if_stmt.else_count; i++) ast_print(node->data.if_stmt.else_branch[i], indent + 1);
            }
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_FOR_STMT:
            printf("(for %s\n", node->data.for_stmt.var);
            ast_print(node->data.for_stmt.start, indent + 1);
            ast_print(node->data.for_stmt.end, indent + 1);
            print_ast_indent(indent); printf("body\n");
            for (int i = 0; i < node->data.for_stmt.body_count; i++) ast_print(node->data.for_stmt.body[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_WHILE_STMT:
            printf("(while\n");
            ast_print(node->data.while_stmt.condition, indent + 1);
            print_ast_indent(indent); printf("body\n");
            for (int i = 0; i < node->data.while_stmt.body_count; i++) ast_print(node->data.while_stmt.body[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_FUNCTION_STMT:
            printf("(def %s (", node->data.function.name);
            for (int i = 0; i < node->data.function.param_count; i++) {
                printf("%s%s", node->data.function.params[i], i < node->data.function.param_count - 1 ? " " : "");
            }
            printf(")\n");
            for (int i = 0; i < node->data.function.body_count; i++) ast_print(node->data.function.body[i], indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_PRINT_STMT:
            printf("(print\n");
            ast_print(node->data.print.value, indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
        case NODE_RETURN_STMT:
            printf("(return\n");
            ast_print(node->data.return_stmt.value, indent + 1);
            print_ast_indent(indent); printf(")\n");
            break;
    }
}
