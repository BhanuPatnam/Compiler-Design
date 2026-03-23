%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex();
extern int current_line;
void yyerror(const char *s);

ASTNode* final_root = NULL;

// Helper to add a statement to a list
ASTNode** add_stmt(ASTNode** list, int* count, ASTNode* stmt) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = stmt;
    return list;
}

// Helper to add a parameter to a list
char** add_param(char** list, int* count, char* param) {
    list = (char**)realloc(list, sizeof(char*) * (*count + 1));
    list[(*count)++] = strdup(param);
    return list;
}

// Helper to add an argument to a list
ASTNode** add_arg(ASTNode** list, int* count, ASTNode* arg) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = arg;
    return list;
}
%}

%union {
    int num;
    float fnum;
    char ch;
    char* id;
    struct ASTNode* node;
    struct {
        struct ASTNode** list;
        int count;
    } stmt_list;
    struct {
        char** list;
        int count;
    } param_list;
    struct {
        struct ASTNode** list;
        int count;
    } arg_list;
}

%token <id> TOK_IDENTIFIER
%token <id> TOK_INT_LITERAL
%token <id> TOK_FLOAT_LITERAL
%token <id> TOK_CHAR_LITERAL
%token TOK_IF TOK_THEN TOK_ELSE TOK_END TOK_FOR TOK_TO TOK_WHILE TOK_DO TOK_FUNCTION TOK_PRINT TOK_RETURN
%token TOK_ASSIGN TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_LPAREN TOK_RPAREN TOK_COMMA
%token TOK_EQ TOK_NEQ TOK_LT TOK_GT TOK_LE TOK_GE
%token TOK_UNKNOWN

%type <node> program statement expression term factor primary
%type <stmt_list> statement_list
%type <param_list> param_list
%type <arg_list> arg_list

%left TOK_PLUS TOK_MINUS
%left TOK_STAR TOK_SLASH
%nonassoc TOK_EQ TOK_NEQ TOK_LT TOK_GT TOK_LE TOK_GE

%%

program:
    statement_list {
        ASTNode* node = create_node(NODE_PROGRAM);
        node->data.program.statements = $1.list;
        node->data.program.stmt_count = $1.count;
        final_root = node;
    }
    ;

statement_list:
    /* empty */ {
        $$.list = NULL;
        $$.count = 0;
    }
    | statement_list statement {
        $$.list = add_stmt($1.list, &$1.count, $2);
        $$.count = $1.count;
    }
    ;

statement:
    TOK_IF expression TOK_THEN statement_list TOK_ELSE statement_list TOK_END TOK_IF {
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = $2;
        node->data.if_stmt.then_branch = $4.list;
        node->data.if_stmt.then_count = $4.count;
        node->data.if_stmt.else_branch = $6.list;
        node->data.if_stmt.else_count = $6.count;
        $$ = node;
    }
    | TOK_IF expression TOK_THEN statement_list TOK_END TOK_IF {
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = $2;
        node->data.if_stmt.then_branch = $4.list;
        node->data.if_stmt.then_count = $4.count;
        node->data.if_stmt.else_branch = NULL;
        node->data.if_stmt.else_count = 0;
        $$ = node;
    }
    | TOK_FOR TOK_IDENTIFIER TOK_ASSIGN expression TOK_TO expression statement_list TOK_END TOK_FOR {
        ASTNode* node = create_node(NODE_FOR_STMT);
        node->data.for_stmt.var = strdup($2);
        node->data.for_stmt.start = $4;
        node->data.for_stmt.end = $6;
        node->data.for_stmt.body = $7.list;
        node->data.for_stmt.body_count = $7.count;
        $$ = node;
    }
    | TOK_WHILE expression TOK_DO statement_list TOK_END TOK_WHILE {
        ASTNode* node = create_node(NODE_WHILE_STMT);
        node->data.while_stmt.condition = $2;
        node->data.while_stmt.body = $4.list;
        node->data.while_stmt.body_count = $4.count;
        $$ = node;
    }
    | TOK_FUNCTION TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN statement_list TOK_END TOK_FUNCTION {
        ASTNode* node = create_node(NODE_FUNCTION_STMT);
        node->data.function.name = strdup($2);
        node->data.function.params = $4.list;
        node->data.function.param_count = $4.count;
        node->data.function.body = $6.list;
        node->data.function.body_count = $6.count;
        $$ = node;
    }
    | TOK_PRINT TOK_LPAREN expression TOK_RPAREN {
        ASTNode* node = create_node(NODE_PRINT_STMT);
        node->data.print.value = $3;
        $$ = node;
    }
    | TOK_RETURN expression {
        ASTNode* node = create_node(NODE_RETURN_STMT);
        node->data.return_stmt.value = $2;
        $$ = node;
    }
    | TOK_IDENTIFIER TOK_ASSIGN expression {
        ASTNode* node = create_node(NODE_ASSIGNMENT_STMT);
        node->data.assignment.name = strdup($1);
        node->data.assignment.value = $3;
        $$ = node;
    }
    ;

param_list:
    /* empty */ {
        $$.list = NULL;
        $$.count = 0;
    }
    | TOK_IDENTIFIER {
        $$.list = add_param(NULL, &$$.count, $1);
    }
    | param_list TOK_COMMA TOK_IDENTIFIER {
        $$.list = add_param($1.list, &$1.count, $3);
        $$.count = $1.count;
    }
    ;

expression:
    term { $$ = $1; }
    | expression TOK_PLUS term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("+");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_MINUS term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("-");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_EQ term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("=");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_NEQ term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≠");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_LT term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("<");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_GT term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup(">");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_LE term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≤");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_GE term {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("≥");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    ;

term:
    factor { $$ = $1; }
    | term TOK_STAR factor {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("*");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | term TOK_SLASH factor {
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("/");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    ;

factor:
    primary { $$ = $1; }
    | TOK_IDENTIFIER TOK_LPAREN arg_list TOK_RPAREN {
        ASTNode* node = create_node(NODE_FUNCTION_CALL_EXPR);
        node->data.func_call.name = strdup($1);
        node->data.func_call.args = $3.list;
        node->data.func_call.arg_count = $3.count;
        $$ = node;
    }
    ;

arg_list:
    /* empty */ {
        $$.list = NULL;
        $$.count = 0;
    }
    | expression {
        $$.list = add_arg(NULL, &$$.count, $1);
    }
    | arg_list TOK_COMMA expression {
        $$.list = add_arg($1.list, &$1.count, $3);
        $$.count = $1.count;
    }
    ;

primary:
    TOK_INT_LITERAL {
        ASTNode* node = create_node(NODE_INT_EXPR);
        node->data.int_expr.value = atoi($1);
        $$ = node;
    }
    | TOK_FLOAT_LITERAL {
        ASTNode* node = create_node(NODE_FLOAT_EXPR);
        node->data.float_expr.value = atof($1);
        $$ = node;
    }
    | TOK_CHAR_LITERAL {
        ASTNode* node = create_node(NODE_CHAR_EXPR);
        // $1 is 'c', we want the character at index 1
        node->data.char_expr.value = $1[1];
        $$ = node;
    }
    | TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_VARIABLE_EXPR);
        node->data.variable.name = strdup($1);
        $$ = node;
    }
    | TOK_LPAREN expression TOK_RPAREN {
        $$ = $2;
    }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Parser error at line %d: %s\n", current_line, s);
}
