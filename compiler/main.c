#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"
#include "semantic.h"
#include "y.tab.h"
#include "lexer.h"

extern int yyparse();
extern int yylex();
extern FILE* yyin;
extern char* yytext;
extern ASTNode* final_root;

int current_line = 1;

const char* get_token_name(int token) {
    switch (token) {
        case TOK_IDENTIFIER: return "IDENTIFIER";
        case TOK_INT_LITERAL: return "INT_LITERAL";
        case TOK_FLOAT_LITERAL: return "FLOAT_LITERAL";
        case TOK_CHAR_LITERAL: return "CHAR_LITERAL";
        case TOK_IF:         return "IF";
        case TOK_THEN:       return "THEN";
        case TOK_ELSE:       return "ELSE";
        case TOK_END:        return "END";
        case TOK_FOR:        return "FOR";
        case TOK_TO:         return "TO";
        case TOK_WHILE:      return "WHILE";
        case TOK_DO:         return "DO";
        case TOK_FUNCTION:   return "FUNCTION";
        case TOK_PRINT:      return "PRINT";
        case TOK_RETURN:     return "RETURN";
        case TOK_ASSIGN:     return "ASSIGN";
        case TOK_PLUS:       return "PLUS";
        case TOK_MINUS:      return "MINUS";
        case TOK_STAR:       return "STAR";
        case TOK_SLASH:      return "SLASH";
        case TOK_LPAREN:     return "LPAREN";
        case TOK_RPAREN:     return "RPAREN";
        case TOK_COMMA:      return "COMMA";
        case TOK_EQ:         return "EQ";
        case TOK_NEQ:        return "NEQ";
        case TOK_LT:         return "LT";
        case TOK_GT:         return "GT";
        case TOK_LE:         return "LE";
        case TOK_GE:         return "GE";
        case TOK_UNKNOWN:    return "UNKNOWN";
        default:             return "EOF";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.txt> <output.c>\n", argv[0]);
        return 1;
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];

    printf("--- Reading Source File: %s ---\n", input_path);
    FILE* input_file = fopen(input_path, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Could not open file %s\n", input_path);
        return 1;
    }

    // Stage 1: Lexical Analysis
    printf("\n--- [Stage 1/6] Lexical Analysis: Scanning Tokens ---\n");
    yyin = input_file;
    int token;
    while ((token = yylex()) != 0) {
        printf("Token: %-12s | Value: %s\n", get_token_name(token), yytext);
    }
    printf("-> Lexical Analysis completed.\n");

    // Stage 2: Syntax Analysis
    printf("\n--- [Stage 2/6] Syntax Analysis: Parsing ---\n");
    // Reset lexer to start of file for parsing
    lexer_reset();
    current_line = 1; 
    
    if (yyparse() != 0) {
        fprintf(stderr, "\nParsing Failed! Stopping compilation.\n");
        fclose(input_file);
        return 1;
    }
    printf("-> Syntax Analysis completed successfully.\n");
    fclose(input_file);

    if (!final_root) {
        fprintf(stderr, "\nNo Parse Tree generated.\n");
        return 1;
    }

    // Stage 3: Parse Tree Construction
    printf("\n--- [Stage 3/6] Parse Tree Construction ---\n");
    ast_print(final_root, 0);
    printf("-> Parse Tree constructed successfully.\n");

    // Stage 4: Semantic Analysis
    printf("\n--- [Stage 4/6] Semantic Analysis ---\n");
    if (!semantic_analyze(final_root)) {
        fprintf(stderr, "\nSemantic Analysis Failed! Stopping compilation.\n");
        ast_free(final_root);
        return 1;
    }
    printf("-> Semantic Analysis passed successfully.\n");

    // Stage 5: Code Generation (stdout preview)
    printf("\n--- [Stage 5/6] Code Generation: C (Preview) ---\n");
    printf("#include <stdio.h>\n\n");
    codegen_generate(final_root, stdout);
    printf("\n-> Code generation preview complete.\n");

    // Stage 6: Final Output (File writing)
    printf("\n--- [Stage 6/6] Final Output: Writing to File ---\n");
    FILE* out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_path);
        ast_free(final_root);
        return 1;
    }
    fprintf(out, "#include <stdio.h>\n\n");
    codegen_generate(final_root, out);
    fclose(out);

    printf("-> Successfully wrote compiled code to: %s\n", output_path);
    printf("\n--- Compilation Summary ---\n");
    printf("Source: %s\nTarget: %s\nStatus: Success\n", input_path, output_path);

    // Cleanup
    ast_free(final_root);

    return 0;
}
