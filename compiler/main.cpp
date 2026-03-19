#include "lexer.h"
#include "parser.h"
#include "codegen.h"
#include "ast_printer.h"
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
    cout << "--- Reading Source File: " << inputPath << " ---" << endl;
    ifstream inputFile(inputPath);
    if (!inputFile.is_open()) {
        cerr << "Error: Could not open input file " << inputPath << endl;
        return 1;
    }

    stringstream buffer;
    buffer << inputFile.rdbuf();
    string source = buffer.str();
    inputFile.close();
    cout << source << endl;

    try {
        // Stage 1: Lexical Analysis
        cout << "\n--- [Stage 1/5] Lexical Analysis: Tokens ---" << endl;
        Lexer lexer(source);
        vector<Token> tokens = lexer.tokenize();
        for (const auto& token : tokens) {
            cout << "[" << token.value << "] ";
        }
        cout << "\n-> Found " << tokens.size() << " tokens." << endl;

        // Stage 2 & 3: Parsing and AST Construction
        cout << "\n--- [Stage 2-3/5] Parsing & AST Construction ---" << endl;
        Parser parser(tokens);
        auto program = parser.parse();
        ASTPrinter astPrinter;
        string astRepresentation = astPrinter.print(*program);
        cout << astRepresentation << endl;
        cout << "-> AST constructed successfully." << endl;

        // Stage 4: Semantic Analysis (Placeholder)
        cout << "\n--- [Stage 4/5] Semantic Analysis ---" << endl;
        cout << "-> (Skipped - No semantic analysis implemented yet)." << endl;

        // Stage 5: Code Generation
        cout << "\n--- [Stage 5/5] Code Generation: AlgoLang ---" << endl;
        CodeGenerator codegen;
        string algoLangCode = codegen.generate(*program);
        cout << algoLangCode << endl;
        cout << "-> Code generation complete." << endl;

        // Final Step: Writing to file
        cout << "\n--- Writing to File: " << outputPath << " ---" << endl;
        ofstream outputFile(outputPath);
        if (!outputFile.is_open()) {
            cerr << "Error: Could not open output file " << outputPath << endl;
            return 1;
        }
        outputFile << algoLangCode;
        outputFile.close();

        cout << "\nSuccess! Compiled " << inputPath << " to " << outputPath << endl;

    } catch (const exception& e) {
        cerr << "\nCompilation failed: " << e.what() << endl;
        return 1;
    }

    return 0;
}
