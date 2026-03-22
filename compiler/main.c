#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "semantic.h"

char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input.txt> <output.c>\n", argv[0]);
        return 1;
    }

    const char* input_path = argv[1];
    const char* output_path = argv[2];

    printf("--- Reading Source File: %s ---\n", input_path);
    char* source = read_file(input_path);
    if (!source) {
        fprintf(stderr, "Error: Could not read file %s\n", input_path);
        return 1;
    }
    printf("%s\n", source);

    // Stage 1: Lexical Analysis
    printf("\n--- [Stage 1/5] Lexical Analysis: Tokens ---\n");
    Lexer lexer = lexer_init(source);
    int token_count = 0;
    Token* tokens = lexer_tokenize(&lexer, &token_count);
    for (int i = 0; i < token_count; i++) {
        printf("[%s] ", tokens[i].value);
    }
    printf("\n-> Found %d tokens.\n", token_count);

    // Stage 2 & 3: Parsing & Parse Tree Construction
    printf("\n--- [Stage 2-3/5] Parsing & Parse Tree Construction ---\n");
    Parser parser = parser_init(tokens, token_count);
    ASTNode* root = parser_parse(&parser);
    ast_print(root, 0);
    printf("-> Parse Tree constructed successfully.\n");

    // Stage 4: Semantic Analysis
    printf("\n--- [Stage 4/5] Semantic Analysis ---\n");
    if (!semantic_analyze(root)) {
        fprintf(stderr, "\nSemantic Analysis Failed! Stopping compilation.\n");
        return 1;
    }
    printf("-> Semantic Analysis passed successfully.\n");

    // Stage 5: Code Generation
    printf("\n--- [Stage 5/5] Code Generation: C ---\n");
    FILE* out = fopen(output_path, "w");
    if (!out) {
        fprintf(stderr, "Error: Could not open output file %s\n", output_path);
        return 1;
    }
    
    // Add standard header to the output C file
    fprintf(out, "#include <stdio.h>\n\n");
    
    // Generate C code
    codegen_generate(root, out);
    
    // Also print to stdout for visibility
    printf("#include <stdio.h>\n\n");
    codegen_generate(root, stdout);
    
    fclose(out);
    printf("\n-> Code generation complete.\n");

    printf("\n--- Writing to File: %s ---\n", output_path);
    printf("\nSuccess! Compiled %s to %s\n", input_path, output_path);

    // Cleanup
    ast_free(root);
    tokens_free(tokens, token_count);
    free(source);

    return 0;
}
