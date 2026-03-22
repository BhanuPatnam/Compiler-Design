CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = main.c lexer.c parser.c codegen.c semantic.c
OBJ = $(SRC:.c=.o)
TARGET = ../algocompiler_c

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

test: all
	$(TARGET) ../try.txt ../try_output_c.alg

.PHONY: all clean test
