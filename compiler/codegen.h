#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

void codegen_generate(ASTNode* node, FILE* out);
void ast_print(ASTNode* node, int indent); // Prints the Parse Tree

#endif
