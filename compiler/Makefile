# Compiler settings
CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I.

# Project structure
SRC_DIR = .
OBJ_DIR = obj
BIN = ../algocompiler

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: $(BIN)

# Link the executable
$(BIN): $(OBJS)
	$(CXX) $(OBJS) -o $(BIN)

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build artifacts
clean:
	rm -rf $(OBJ_DIR) $(BIN)

# Run tests
test: all
	$(BIN) ../test_input.txt ../output.alg
	$(BIN) ../test_factorial.txt ../output_factorial.alg
	$(BIN) ../test_while.txt ../output_while.alg

.PHONY: all clean test
