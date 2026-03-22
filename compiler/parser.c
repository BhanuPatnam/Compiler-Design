#include "parser.h"
#include <stdarg.h>

Parser parser_init(Token* tokens, int count) {
    Parser p;
    p.tokens = tokens;
    p.count = count;
    p.pos = 0;
    return p;
}

static Token peek(Parser* p) {
    return p->tokens[p->pos];
}

static Token advance(Parser* p) {
    if (p->pos < p->count) return p->tokens[p->pos++];
    return p->tokens[p->count - 1];
}

static int match(Parser* p, TokenType type) {
    if (peek(p).type == type) {
        advance(p);
        return 1;
    }
    return 0;
}

static Token consume(Parser* p, TokenType type, const char* message) {
    if (peek(p).type == type) return advance(p);
    fprintf(stderr, "Parser error at line %d: %s. Got '%s'\n", peek(p).line, message, peek(p).value);
    exit(1);
}

static ASTNode* create_node(NodeType type) {
    ASTNode* node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = type;
    return node;
}

// Forward declarations
static ASTNode* parse_expression(Parser* p);
static ASTNode* parse_statement(Parser* p);

static ASTNode* parse_primary(Parser* p) {
    if (match(p, TOK_NUMBER)) {
        ASTNode* node = create_node(NODE_NUMBER_EXPR);
        node->data.number.value = atoi(p->tokens[p->pos - 1].value);
        return node;
    }
    if (match(p, TOK_LPAREN)) {
        ASTNode* expr = parse_expression(p);
        consume(p, TOK_RPAREN, "Expected ')' after expression");
        return expr;
    }
    if (peek(p).type == TOK_IDENTIFIER) {
        Token name = advance(p);
        if (match(p, TOK_LPAREN)) {
            ASTNode* node = create_node(NODE_FUNCTION_CALL_EXPR);
            node->data.func_call.name = strdup(name.value);
            node->data.func_call.args = NULL;
            node->data.func_call.arg_count = 0;
            if (peek(p).type != TOK_RPAREN) {
                int capacity = 10;
                node->data.func_call.args = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
                do {
                    if (node->data.func_call.arg_count >= capacity) {
                        capacity *= 2;
                        node->data.func_call.args = (ASTNode**)realloc(node->data.func_call.args, sizeof(ASTNode*) * capacity);
                    }
                    node->data.func_call.args[node->data.func_call.arg_count++] = parse_expression(p);
                } while (match(p, TOK_COMMA));
            }
            consume(p, TOK_RPAREN, "Expected ')' after arguments");
            return node;
        }
        ASTNode* node = create_node(NODE_VARIABLE_EXPR);
        node->data.variable.name = strdup(name.value);
        return node;
    }
    fprintf(stderr, "Expected expression, got '%s'\n", peek(p).value);
    exit(1);
}

static ASTNode* parse_factor(Parser* p) {
    ASTNode* left = parse_primary(p);
    while (peek(p).type == TOK_STAR || peek(p).type == TOK_SLASH) {
        Token op = advance(p);
        ASTNode* right = parse_primary(p);
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup(op.value);
        node->data.binary.left = left;
        node->data.binary.right = right;
        left = node;
    }
    return left;
}

static ASTNode* parse_expression(Parser* p) {
    ASTNode* left = parse_factor(p);
    while (peek(p).type == TOK_PLUS || peek(p).type == TOK_MINUS ||
           peek(p).type == TOK_EQ || peek(p).type == TOK_NEQ ||
           peek(p).type == TOK_LT || peek(p).type == TOK_GT ||
           peek(p).type == TOK_LE || peek(p).type == TOK_GE) {
        Token op = advance(p);
        ASTNode* right = parse_factor(p);
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup(op.value);
        node->data.binary.left = left;
        node->data.binary.right = right;
        left = node;
    }
    return left;
}

static ASTNode* parse_if(Parser* p) {
    ASTNode* node = create_node(NODE_IF_STMT);
    node->data.if_stmt.condition = parse_expression(p);
    consume(p, TOK_THEN, "Expected 'then' after if condition");
    
    int capacity = 10;
    node->data.if_stmt.then_branch = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    node->data.if_stmt.then_count = 0;
    while (peek(p).type != TOK_ELSE && peek(p).type != TOK_END) {
        if (node->data.if_stmt.then_count >= capacity) {
            capacity *= 2;
            node->data.if_stmt.then_branch = (ASTNode**)realloc(node->data.if_stmt.then_branch, sizeof(ASTNode*) * capacity);
        }
        node->data.if_stmt.then_branch[node->data.if_stmt.then_count++] = parse_statement(p);
    }

    node->data.if_stmt.else_branch = NULL;
    node->data.if_stmt.else_count = 0;
    if (match(p, TOK_ELSE)) {
        capacity = 10;
        node->data.if_stmt.else_branch = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
        while (peek(p).type != TOK_END) {
            if (node->data.if_stmt.else_count >= capacity) {
                capacity *= 2;
                node->data.if_stmt.else_branch = (ASTNode**)realloc(node->data.if_stmt.else_branch, sizeof(ASTNode*) * capacity);
            }
            node->data.if_stmt.else_branch[node->data.if_stmt.else_count++] = parse_statement(p);
        }
    }
    consume(p, TOK_END, "Expected 'end' after if statement");
    if (peek(p).type == TOK_IF) advance(p);
    return node;
}

static ASTNode* parse_for(Parser* p) {
    ASTNode* node = create_node(NODE_FOR_STMT);
    Token var = consume(p, TOK_IDENTIFIER, "Expected identifier in for loop");
    node->data.for_stmt.var = strdup(var.value);
    consume(p, TOK_ASSIGN, "Expected assignment in for loop");
    node->data.for_stmt.start = parse_expression(p);
    consume(p, TOK_TO, "Expected 'to' in for loop");
    node->data.for_stmt.end = parse_expression(p);
    
    int capacity = 10;
    node->data.for_stmt.body = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    node->data.for_stmt.body_count = 0;
    while (peek(p).type != TOK_END) {
        if (node->data.for_stmt.body_count >= capacity) {
            capacity *= 2;
            node->data.for_stmt.body = (ASTNode**)realloc(node->data.for_stmt.body, sizeof(ASTNode*) * capacity);
        }
        node->data.for_stmt.body[node->data.for_stmt.body_count++] = parse_statement(p);
    }
    consume(p, TOK_END, "Expected 'end' after for loop");
    if (peek(p).type == TOK_FOR) advance(p);
    return node;
}

static ASTNode* parse_while(Parser* p) {
    ASTNode* node = create_node(NODE_WHILE_STMT);
    node->data.while_stmt.condition = parse_expression(p);
    consume(p, TOK_DO, "Expected 'do' after while condition");
    
    int capacity = 10;
    node->data.while_stmt.body = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    node->data.while_stmt.body_count = 0;
    while (peek(p).type != TOK_END) {
        if (node->data.while_stmt.body_count >= capacity) {
            capacity *= 2;
            node->data.while_stmt.body = (ASTNode**)realloc(node->data.while_stmt.body, sizeof(ASTNode*) * capacity);
        }
        node->data.while_stmt.body[node->data.while_stmt.body_count++] = parse_statement(p);
    }
    consume(p, TOK_END, "Expected 'end' after while loop");
    if (peek(p).type == TOK_WHILE) advance(p);
    return node;
}

static ASTNode* parse_function(Parser* p) {
    ASTNode* node = create_node(NODE_FUNCTION_STMT);
    Token name = consume(p, TOK_IDENTIFIER, "Expected function name");
    node->data.function.name = strdup(name.value);
    consume(p, TOK_LPAREN, "Expected '(' after function name");
    
    int capacity = 10;
    node->data.function.params = (char**)malloc(sizeof(char*) * capacity);
    node->data.function.param_count = 0;
    if (peek(p).type != TOK_RPAREN) {
        do {
            if (node->data.function.param_count >= capacity) {
                capacity *= 2;
                node->data.function.params = (char**)realloc(node->data.function.params, sizeof(char*) * capacity);
            }
            Token param = consume(p, TOK_IDENTIFIER, "Expected parameter name");
            node->data.function.params[node->data.function.param_count++] = strdup(param.value);
        } while (match(p, TOK_COMMA));
    }
    consume(p, TOK_RPAREN, "Expected ')' after parameters");
    
    capacity = 10;
    node->data.function.body = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    node->data.function.body_count = 0;
    while (peek(p).type != TOK_END) {
        if (node->data.function.body_count >= capacity) {
            capacity *= 2;
            node->data.function.body = (ASTNode**)realloc(node->data.function.body, sizeof(ASTNode*) * capacity);
        }
        node->data.function.body[node->data.function.body_count++] = parse_statement(p);
    }
    consume(p, TOK_END, "Expected 'end' after function");
    if (peek(p).type == TOK_FUNCTION) advance(p);
    return node;
}

static ASTNode* parse_statement(Parser* p) {
    if (match(p, TOK_IF)) return parse_if(p);
    if (match(p, TOK_FOR)) return parse_for(p);
    if (match(p, TOK_WHILE)) return parse_while(p);
    if (match(p, TOK_FUNCTION)) return parse_function(p);
    if (match(p, TOK_PRINT)) {
        ASTNode* node = create_node(NODE_PRINT_STMT);
        consume(p, TOK_LPAREN, "Expected '(' after print");
        node->data.print.value = parse_expression(p);
        consume(p, TOK_RPAREN, "Expected ')' after print expression");
        return node;
    }
    if (match(p, TOK_RETURN)) {
        ASTNode* node = create_node(NODE_RETURN_STMT);
        node->data.return_stmt.value = parse_expression(p);
        return node;
    }
    if (peek(p).type == TOK_IDENTIFIER) {
        Token name = advance(p);
        consume(p, TOK_ASSIGN, "Expected assignment operator");
        ASTNode* node = create_node(NODE_ASSIGNMENT_STMT);
        node->data.assignment.name = strdup(name.value);
        node->data.assignment.value = parse_expression(p);
        return node;
    }
    fprintf(stderr, "Unknown statement starting with '%s'\n", peek(p).value);
    exit(1);
}

ASTNode* parser_parse(Parser* p) {
    ASTNode* node = create_node(NODE_PROGRAM);
    int capacity = 100;
    node->data.program.statements = (ASTNode**)malloc(sizeof(ASTNode*) * capacity);
    node->data.program.stmt_count = 0;
    while (peek(p).type != TOK_EOF) {
        if (node->data.program.stmt_count >= capacity) {
            capacity *= 2;
            node->data.program.statements = (ASTNode**)realloc(node->data.program.statements, sizeof(ASTNode*) * capacity);
        }
        node->data.program.statements[node->data.program.stmt_count++] = parse_statement(p);
    }
    return node;
}

void ast_free(ASTNode* node) {
    if (!node) return;
    switch (node->type) {
        case NODE_NUMBER_EXPR: break;
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
            free(node->data.assignment.name);
            ast_free(node->data.assignment.value);
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
