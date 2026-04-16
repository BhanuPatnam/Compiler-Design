#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "codegen.h"
#include "semantic.h"
#include "icg.h"
#include "optimization.h"
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
        case TOK_ADDR_OF:    return "ADDR_OF";
        case TOK_LBRACKET:   return "LBRACKET";
        case TOK_RBRACKET:   return "RBRACKET";
        case TOK_INT_TYPE:   return "INT_TYPE";
        case TOK_FLOAT_TYPE: return "FLOAT_TYPE";
        case TOK_CHAR_TYPE:  return "CHAR_TYPE";
        case TOK_STRUCT:     return "STRUCT";
        case TOK_DOT:        return "DOT";
        case TOK_UNKNOWN:    return "UNKNOWN";
        default:             return "EOF";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.alg> <output.c>\n", argv[0]);
        return 1;
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];

    // Extract base name for executable
    char base_name[256]; // Assuming max path length
    const char* last_slash = strrchr(input_path, '/');
    const char* filename_start = last_slash ? last_slash + 1 : input_path;
    char* dot = strrchr(filename_start, '.');
    if (dot) {
        strncpy(base_name, filename_start, dot - filename_start);
        base_name[dot - filename_start] = '\0';
    } else {
        strcpy(base_name, filename_start);
    }

    // Check for .alg extension
    const char* ext = strrchr(input_path, '.');
    if (!ext || strcmp(ext, ".alg") != 0) {
        fprintf(stderr, "Error: Input file must have a .alg extension (found '%s')\n", ext ? ext : "none");
        return 1;
    }

    printf("--- Reading Source File: %s ---\n", input_path);
    FILE* input_file = fopen(input_path, "r");
    if (!input_file) {
        fprintf(stderr, "Error: Could not open file %s\n", input_path);
        return 1;
    }

    // Phase 1: Lexical Analysis
    printf("\n--- [Phase 1/6] Lexical Analysis: Scanning Tokens ---\n");
    yyin = input_file;
    int token;
    while ((token = yylex()) != 0) {
        printf("Token: %-12s | Value: %s\n", get_token_name(token), yytext);
    }
    printf("-> Lexical Analysis completed.\n");

    // Phase 2: Syntax Analysis
    printf("\n--- [Phase 2/6] Syntax Analysis: Parsing ---\n");
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
    ast_print(final_root, 0);

    // Phase 3: Semantic Analysis
    printf("\n--- [Phase 3/6] Semantic Analysis ---\n");
    if (!semantic_analyze(final_root)) {
        fprintf(stderr, "\nSemantic Analysis Failed! Stopping compilation.\n");
        ast_free(final_root);
        return 1;
    }
    printf("-> Semantic Analysis passed successfully.\n");

    // Phase 4: Intermediate Code Generation
    printf("\n--- [Phase 4/6] Intermediate Code Generation (TAC) ---\n");
    generate_icg(final_root);
    printf("-> Intermediate Code Generation complete.\n");

    // Phase 5: Code Optimization
    printf("\n--- [Phase 5/6] Code Optimization ---\n");
    int opt_success = 1;
    final_root = optimize_ast(final_root, &opt_success);
    if (!opt_success) {
        fprintf(stderr, "\nOptimization Failed due to critical errors!\n");
        printf("\n--- Compilation Summary ---\n");
        printf("Source: %s\n", input_path);
        printf("Target: %s\n", output_path);
        printf("Status: Failed (Optimization Phase)\n");
        ast_free(final_root);
        return 1;
    }
    printf("-> Code Optimization complete.\n");

    // Phase 6: Code Generation
    printf("\n--- [Phase 6/6] Code Generation: C ---\n");
    
    FILE* out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_path);
        ast_free(final_root);
        return 1;
    }
    fprintf(out, "#include <stdio.h>\n#include <math.h>\n\n"); // Add math.h
    codegen_generate(final_root, out);
    fclose(out);
    printf("\n-> Code generation complete. Output saved to: %s\n", output_path);

    // Phase 7: Compilation and Linking
    printf("\n--- [Phase 7/7] Compiling and Linking C Code ---\n");
    char compile_command[512];
    snprintf(compile_command, sizeof(compile_command), "gcc %s -o %s -lm", output_path, base_name);
    printf("[AlgoCompiler] Linking: %s\n", compile_command);
    if (system(compile_command) != 0) {
        fprintf(stderr, "Error: C compilation failed.\n");
        ast_free(final_root);
        return 1;
    }
    printf("[AlgoCompiler] Success! Run with: ./%s\n", base_name);

    printf("\n--- Compilation Summary ---\n");
    printf("Source: %s\nTarget C File: %s\nExecutable: %s\nStatus: Success\n", input_path, output_path, base_name);

    // Cleanup
    ast_free(final_root);

    return 0;
}
