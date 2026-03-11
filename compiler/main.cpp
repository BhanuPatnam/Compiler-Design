#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: ./algocompiler input.txt output.alg" << endl;
        return 1;
    }

    string inputPath = argv[1];
    string outputPath = argv[2];

    // Read input file
    ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file " << inputPath << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string source = buffer.str();
    inputFile.close();

    try {
        // Stage 1: Lexical Analysis
        cout << "Stage 1: Lexical Analysis..." << endl;
        Lexer lexer(source);
        vector<Token> tokens = lexer.tokenize();

        // Stage 2: Syntax Analysis (Parsing) & AST Construction
        cout << "Stage 2: Syntax Analysis & AST Construction..." << endl;
        Parser parser(tokens);
        auto program = parser.parse();

        // Stage 3: Code Generation
        cout << "Stage 3: Code Generation..." << endl;
        CodeGenerator codegen;
        string algoLangCode = codegen.generate(*program);

        // Write output file
        ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
            cerr << "Error: Could not open output file " << outputPath << endl;
            return 1;
        }
        outputFile << algoLangCode;
        outputFile.close();

        cout << "Success! Compiled " << inputPath << " to " << outputPath << endl;

    } catch (const exception& e) {
        cerr << "Compilation failed: " << e.what() << endl;
        return 1;
    }

    return 0;
}
