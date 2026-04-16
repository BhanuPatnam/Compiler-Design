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

// Helper for arrays
int* add_size(int* list, int* count, int size) {
    list = (int*)realloc(list, sizeof(int) * (*count + 1));
    list[(*count)++] = size;
    return list;
}

ASTNode** add_index(ASTNode** list, int* count, ASTNode* index) {
    list = (ASTNode**)realloc(list, sizeof(ASTNode*) * (*count + 1));
    list[(*count)++] = index;
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
    struct {
        int* list;
        int count;
    } size_list;
    struct {
        struct ASTNode** list;
        int count;
    } index_list;
    int level;
}

%token <id> TOK_IDENTIFIER
%token <id> TOK_INT_LITERAL
%token <id> TOK_FLOAT_LITERAL
%token <id> TOK_CHAR_LITERAL
%token TOK_IF TOK_THEN TOK_ELSE TOK_END TOK_FOR TOK_TO TOK_WHILE TOK_DO TOK_FUNCTION TOK_PRINT TOK_RETURN
%token TOK_ASSIGN TOK_PLUS TOK_MINUS TOK_STAR TOK_SLASH TOK_LPAREN TOK_RPAREN TOK_COMMA
%token TOK_EQ TOK_NEQ TOK_LT TOK_GT TOK_LE TOK_GE
%token TOK_ADDR_OF TOK_LBRACKET TOK_RBRACKET
%token TOK_INT_TYPE TOK_FLOAT_TYPE TOK_CHAR_TYPE
%token TOK_STRUCT TOK_DOT
%token TOK_UNKNOWN

%type <node> program statement expression term factor primary lvalue declaration struct_def
%type <stmt_list> statement_list struct_member_list
%type <param_list> param_list
%type <arg_list> arg_list
%type <level> stars
%type <size_list> brackets
%type <index_list> indices

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
        printf("  [Parsing Rule] If-Then-Else statement matched.\n");
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = $2;
        node->data.if_stmt.then_branch = $4.list;
        node->data.if_stmt.then_count = $4.count;
        node->data.if_stmt.else_branch = $6.list;
        node->data.if_stmt.else_count = $6.count;
        $$ = node;
    }
    | TOK_IF expression TOK_THEN statement_list TOK_END TOK_IF {
        printf("  [Parsing Rule] If-Then statement matched.\n");
        ASTNode* node = create_node(NODE_IF_STMT);
        node->data.if_stmt.condition = $2;
        node->data.if_stmt.then_branch = $4.list;
        node->data.if_stmt.then_count = $4.count;
        node->data.if_stmt.else_branch = NULL;
        node->data.if_stmt.else_count = 0;
        $$ = node;
    }
    | TOK_FOR TOK_IDENTIFIER TOK_ASSIGN expression TOK_TO expression statement_list TOK_END TOK_FOR {
        printf("  [Parsing Rule] For loop matched for variable: %s\n", $2);
        ASTNode* node = create_node(NODE_FOR_STMT);
        node->data.for_stmt.var = strdup($2);
        node->data.for_stmt.start = $4;
        node->data.for_stmt.end = $6;
        node->data.for_stmt.body = $7.list;
        node->data.for_stmt.body_count = $7.count;
        $$ = node;
    }
    | TOK_WHILE expression TOK_DO statement_list TOK_END TOK_WHILE {
        printf("  [Parsing Rule] While loop matched.\n");
        ASTNode* node = create_node(NODE_WHILE_STMT);
        node->data.while_stmt.condition = $2;
        node->data.while_stmt.body = $4.list;
        node->data.while_stmt.body_count = $4.count;
        $$ = node;
    }
    | TOK_FUNCTION TOK_IDENTIFIER TOK_LPAREN param_list TOK_RPAREN statement_list TOK_END TOK_FUNCTION {
        printf("  [Parsing Rule] Function definition matched: %s\n", $2);
        ASTNode* node = create_node(NODE_FUNCTION_STMT);
        node->data.function.name = strdup($2);
        node->data.function.params = $4.list;
        node->data.function.param_count = $4.count;
        node->data.function.body = $6.list;
        node->data.function.body_count = $6.count;
        $$ = node;
    }
    | TOK_PRINT TOK_LPAREN expression TOK_RPAREN {
        printf("  [Parsing Rule] Print statement matched.\n");
        ASTNode* node = create_node(NODE_PRINT_STMT);
        node->data.print.value = $3;
        $$ = node;
    }
    | TOK_RETURN expression {
        printf("  [Parsing Rule] Return statement matched.\n");
        ASTNode* node = create_node(NODE_RETURN_STMT);
        node->data.return_stmt.value = $2;
        $$ = node;
    }
    | lvalue TOK_ASSIGN expression {
        printf("  [Parsing Rule] Assignment statement matched.\n");
        ASTNode* node = create_node(NODE_ASSIGNMENT_STMT);
        node->data.assignment.target = $1;
        node->data.assignment.value = $3;
        $$ = node;
    }
    | declaration { 
        printf("  [Parsing Rule] Variable/Array declaration matched.\n");
        $$ = $1; 
    }
    | struct_def {
        printf("  [Parsing Rule] Struct definition matched.\n");
        $$ = $1;
    }
    ;

struct_def:
    TOK_STRUCT TOK_IDENTIFIER struct_member_list TOK_END TOK_STRUCT {
        ASTNode* node = create_node(NODE_STRUCT_DEF_STMT);
        node->data.struct_def.struct_name = strdup($2);
        node->data.struct_def.members = $3.list;
        node->data.struct_def.member_count = $3.count;
        $$ = node;
    }
    ;

struct_member_list:
    /* empty */ {
        $$.list = NULL;
        $$.count = 0;
    }
    | struct_member_list declaration {
        $$.list = add_stmt($1.list, &$1.count, $2);
        $$.count = $1.count;
    }
    ;

lvalue:
    TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_VARIABLE_EXPR);
        node->data.variable.name = strdup($1);
        $$ = node;
    }
    | stars TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_DEREF_EXPR);
        node->data.deref.name = strdup($2);
        node->data.deref.expr = NULL;
        node->data.deref.level = $1;
        $$ = node;
    }
    | TOK_IDENTIFIER indices {
        ASTNode* node = create_node(NODE_ARRAY_ACCESS_EXPR);
        node->data.array_access.name = strdup($1);
        node->data.array_access.indices = $2.list;
        node->data.array_access.index_count = $2.count;
        $$ = node;
    }
    | lvalue TOK_DOT TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_STRUCT_MEMBER_ACCESS_EXPR);
        node->data.struct_access.target = $1;
        node->data.struct_access.member_name = strdup($3);
        $$ = node;
    }
    ;

stars:
    TOK_STAR { $$ = 1; }
    | stars TOK_STAR { $$ = $1 + 1; }
    ;

indices:
    TOK_LBRACKET expression TOK_RBRACKET {
        $$.list = add_index(NULL, &$$.count, $2);
    }
    | indices TOK_LBRACKET expression TOK_RBRACKET {
        $$.list = add_index($1.list, &$1.count, $3);
        $$.count = $1.count;
    }
    ;

declaration:
    TOK_INT_TYPE TOK_IDENTIFIER {
        printf("  [Parsing Rule] Simple integer declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup($2);
        node->data.decl.type = strdup("int");
        $$ = node;
    }
    | TOK_FLOAT_TYPE TOK_IDENTIFIER {
        printf("  [Parsing Rule] Simple float declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup($2);
        node->data.decl.type = strdup("float");
        $$ = node;
    }
    | TOK_CHAR_TYPE TOK_IDENTIFIER {
        printf("  [Parsing Rule] Simple char declaration matched.\n");
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup($2);
        node->data.decl.type = strdup("char");
        $$ = node;
    }
    | TOK_IDENTIFIER TOK_IDENTIFIER {
        printf("  [Parsing Rule] Struct instance declaration matched: %s %s\n", $1, $2);
        ASTNode* node = create_node(NODE_DECL_STMT);
        node->data.decl.name = strdup($2);
        char* type = (char*)malloc(strlen($1) + 8);
        sprintf(type, "struct %s", $1);
        node->data.decl.type = type;
        $$ = node;
    }
    | TOK_INT_TYPE TOK_IDENTIFIER brackets {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup($2);
        node->data.array_decl.sizes = $3.list;
        node->data.array_decl.dim_count = $3.count;
        node->data.array_decl.type = strdup("int");
        $$ = node;
    }
    | TOK_FLOAT_TYPE TOK_IDENTIFIER brackets {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup($2);
        node->data.array_decl.sizes = $3.list;
        node->data.array_decl.dim_count = $3.count;
        node->data.array_decl.type = strdup("float");
        $$ = node;
    }
    | TOK_CHAR_TYPE TOK_IDENTIFIER brackets {
        ASTNode* node = create_node(NODE_ARRAY_DECL_STMT);
        node->data.array_decl.name = strdup($2);
        node->data.array_decl.sizes = $3.list;
        node->data.array_decl.dim_count = $3.count;
        node->data.array_decl.type = strdup("char");
        $$ = node;
    }
    | TOK_INT_TYPE stars TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup($3);
        node->data.pointer_decl.type = strdup("int");
        node->data.pointer_decl.level = $2;
        $$ = node;
    }
    | TOK_FLOAT_TYPE stars TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup($3);
        node->data.pointer_decl.type = strdup("float");
        node->data.pointer_decl.level = $2;
        $$ = node;
    }
    | TOK_CHAR_TYPE stars TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_POINTER_DECL_STMT);
        node->data.pointer_decl.name = strdup($3);
        node->data.pointer_decl.type = strdup("char");
        node->data.pointer_decl.level = $2;
        $$ = node;
    }
    ;

brackets:
    TOK_LBRACKET TOK_INT_LITERAL TOK_RBRACKET {
        $$.list = add_size(NULL, &$$.count, atoi($2));
    }
    | brackets TOK_LBRACKET TOK_INT_LITERAL TOK_RBRACKET {
        $$.list = add_size($1.list, &$1.count, atoi($3));
        $$.count = $1.count;
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
        printf("  [Parsing Rule] Binary Expression (+) matched.\n");
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("+");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_MINUS term {
        printf("  [Parsing Rule] Binary Expression (-) matched.\n");
        ASTNode* node = create_node(NODE_BINARY_EXPR);
        node->data.binary.op = strdup("-");
        node->data.binary.left = $1;
        node->data.binary.right = $3;
        $$ = node;
    }
    | expression TOK_EQ term {
        printf("  [Parsing Rule] Comparison Expression (=) matched.\n");
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
    | TOK_ADDR_OF TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_ADDR_OF_EXPR);
        node->data.addr_of.name = strdup($2);
        $$ = node;
    }
    | stars TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_DEREF_EXPR);
        node->data.deref.name = strdup($2);
        node->data.deref.expr = NULL;
        node->data.deref.level = $1;
        $$ = node;
    }
    | TOK_IDENTIFIER indices {
        ASTNode* node = create_node(NODE_ARRAY_ACCESS_EXPR);
        node->data.array_access.name = strdup($1);
        node->data.array_access.indices = $2.list;
        node->data.array_access.index_count = $2.count;
        $$ = node;
    }
    | factor TOK_DOT TOK_IDENTIFIER {
        ASTNode* node = create_node(NODE_STRUCT_MEMBER_ACCESS_EXPR);
        node->data.struct_access.target = $1;
        node->data.struct_access.member_name = strdup($3);
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
