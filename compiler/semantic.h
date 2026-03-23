#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

// Semantic Analysis: Check for undefined variables and functions
int semantic_analyze(ASTNode* root);

#endif
