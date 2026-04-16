#!/bin/bash

# Simple wrapper script for algocompiler
# Usage: ./alg <input.alg> [output.c]

if [ $# -lt 1 ]; then
    echo "Usage: $0 <input.alg> [output.c]"
    exit 1
fi

INPUT_FILE=$1
OUTPUT_C=$2
# Extract the binary name from the input file (e.g., test_arrays.alg -> test_arrays)
BASE_NAME=$(basename "$INPUT_FILE" .alg)
OUT_BIN="${BASE_NAME}"

if [ -z "$OUTPUT_C" ]; then
    OUTPUT_C="${BASE_NAME}.c"
fi

# Path to the actual compiler executable
COMPILER_BIN="./algocompiler"

# Ensure the compiler is built
if [ ! -f "$COMPILER_BIN" ]; then
    echo "Error: $COMPILER_BIN not found. Please run 'make' in the compiler directory."
    exit 1
fi

# Step 1: Translate .alg to .c
echo "--- Step 1: Translating $INPUT_FILE to $OUTPUT_C ---"
$COMPILER_BIN "$INPUT_FILE" "$OUTPUT_C"

if [ $? -ne 0 ]; then
    echo "Translation failed."
    exit 1
fi

# Step 2: Compile .c to binary using gcc
echo -e "\n--- Step 2: Compiling $OUTPUT_C to binary $OUT_BIN ---"
gcc "$OUTPUT_C" -o "$OUT_BIN"

if [ $? -eq 0 ]; then
    echo -e "Compilation successful! You can now run the program using: ./$OUT_BIN\n"
else
    echo "C compilation failed."
    exit 1
fi
