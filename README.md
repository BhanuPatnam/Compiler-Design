# Compiler Design Project: Pseudocode to AlgoLang

This project is a mini-compiler written in C++ that converts algorithms written in a specific pseudocode format into a new programming language called **AlgoLang** (which uses a clean, Python-like syntax).

## 🚀 Features

The compiler supports:
- **Variables & Assignment**: Use `←` for assignment (e.g., `x ← 10`).
- **Arithmetic Operations**: `+`, `-`, `*`, `/` with operator precedence and parentheses.
- **Control Flow**:
  - `if...then...else...end if`
  - `for...←...to...end for`
  - `while...do...end while`
- **Functions**: Define with `function name(params)...end function`.
- **I/O**: Standard `print(expression)` statements.

## 🛠 Project Structure

The project is organized into several modular stages:
- `lexer.h/cpp`: Lexical Analysis (Tokenization).
- `parser.h/cpp`: Syntax Analysis (Recursive Descent Parsing) & AST Construction.
- `ast.h`: Abstract Syntax Tree node hierarchy using the Visitor pattern.
- `codegen.h/cpp`: Code Generation (AST traversal to AlgoLang).
- `main.cpp`: CLI tool entry point.
- `grammar.txt`: Formal BNF grammar definition.

## 📋 Prerequisites

- C++14 or higher
- `g++` compiler
- `make` build tool

## 🔨 How to Build

Navigate to the `compiler` directory and use the provided `Makefile`:

```bash
cd compiler
make
```

This will create the `algocompiler` executable in the project root.

## 📖 Usage

To compile an algorithm:

```bash
./algocompiler <input_file.txt> <output_file.alg>
```

### Example

**Input (`input.txt`):**
```
function factorial(n)
    if n = 0 then
        return 1
    else
        return n * factorial(n - 1)
    end if
end function

print(factorial(5))
```

**Run:**
```bash
./algocompiler input.txt output.alg
```

**Output (`output.alg`):**
```python
def factorial(n):
    if (n == 0):
        return 1
    else:
        return (n * factorial((n - 1)))

print(factorial(5))
```

## 🧪 Testing

You can run the built-in test suite:

```bash
cd compiler
make test
```

## 📜 License

This project is created for educational purposes in Compiler Design.
