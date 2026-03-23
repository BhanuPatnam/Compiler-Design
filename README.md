# Compiler Design Project: Pseudocode to C Compiler

This project is a modular compiler written in **C** that translates a custom pseudocode format into standard, compilable **C code**. It features a full 5-stage compilation pipeline with a specialized **Parse Tree** visualization.

## 🚀 Features

- **Standardized C Implementation**: The entire compiler is written in C for a low-level, performance-oriented design.
- **Full Translation Pipeline**:
  - **Stage 1: Lexical Analysis**: Converts raw text into tokens, supporting Unicode operators (`←`, `≠`, `≤`, `≥`).
  - **Stage 2: Syntax Analysis**: Validates input against formal BNF grammar.
  - **Stage 3: Parse Tree Construction**: Builds a hierarchical representation of the program's logic.
  - **Stage 4: Semantic Analysis**: Performs logical checks, such as detecting undefined variables and managing function scopes.
  - **Stage 5: Code Generation**: Emits valid C code with proper main-function wrapping and standard headers.
- **Language Constructs**:
  - Functions with return values.
  - Variables and assignments (`<-` or `←`).
  - `if...then...else...end if` conditionals.
  - `for...to` and `while...do` loops.
  - Standard `print()` statements (mapped to `printf`).

## 🛠 Project Structure

The project is organized in the `compiler/` directory:
- `lexer.h/c`: Lexical Analysis (Scanner).
- `parser.h/c`: Syntax Analysis & Parse Tree construction.
- `semantic.h/c`: Semantic Analysis (Logic & Scope checking).
- `codegen.h/c`: Code Generation and Parse Tree visualization.
- `main.c`: CLI entry point with verbose stage-by-stage terminal output.
- `grammar.txt`: Formal BNF grammar definition.

## 📋 Prerequisites

- C11 or higher
- `gcc` compiler
- `make` build tool

## 🔨 How to Build

Navigate to the `compiler` directory and use the `Makefile`:

```bash
cd compiler
make clean && make
```

By default, the compiler uses a **manual lexer** (`lexer.c`) with a **Bison parser** (`parser.y`). 
To use **Flex** for lexical analysis instead, run:
```bash
make clean && make LEXER=flex
```

This will create the `algocompiler` executable in the project root.

## 📖 Usage

### 1. Compile the Algorithm
Run the compiler on your pseudocode file:

```bash
./algocompiler input.txt output.c
```

### 2. Run the Resulting Program
Compile the generated C code with `gcc`:

```bash
gcc output.c -o my_program
./my_program
```

### Example

**Input (`try.txt`):**
```
function find_min(a,b)
    if a < b then 
        return a
    else
        return b
    end if
end function

x <- 10
y <- 20
print(find_min(x,y))
```

**Compiler Output (Terminal):**
- Shows the **Tokens** generated.
- Displays the **Parse Tree** structure.
- Confirms **Semantic Analysis** pass.
- Previews the **Generated C Code**.

## 🧪 Testing

Run the built-in test suite to verify all components:

```bash
cd compiler
make test
```

## 📜 License

This project is created for educational purposes in Compiler Design.
