CC = gcc
LEX = flex
YACC = bison
CFLAGS = -Wall -Wextra -std=c11 -g

# Default to manual lexer (lexer.c) instead of Flex (lexer.l)
# To use Flex, run: make LEXER=flex
LEXER ?= manual

ifeq ($(LEXER), flex)
    LEX_SRC = lex.yy.c
    LEX_GEN = lex.yy.c
else
    LEX_SRC = lexer.c
    LEX_GEN = 
endif

SRC = main.c $(LEX_SRC) y.tab.c ast.c codegen.c semantic.c
OBJ = $(SRC:.c=.o)
TARGET = ../algocompiler

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

y.tab.c y.tab.h: parser.y
	$(YACC) -d parser.y -o y.tab.c

lex.yy.c: lexer.l y.tab.h
	$(LEX) lexer.l

%.o: %.c y.tab.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h $(TARGET)

test: all
	$(TARGET) ../try.txt ../try_output.c

.PHONY: all clean test
