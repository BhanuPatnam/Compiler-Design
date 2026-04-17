# Root Makefile for AlgoCompiler (.c -> static algocompiler)

CC = gcc
CFLAGS = -Wall

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
LDFLAGS = -static
else
LDFLAGS =
endif

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

COMPILER_DIR = compiler
BINARY = algocompiler

PARSER_Y = $(COMPILER_DIR)/parser.y
PARSER_C = $(COMPILER_DIR)/y.tab.c
PARSER_H = $(COMPILER_DIR)/y.tab.h

SRCS = $(wildcard $(COMPILER_DIR)/*.c)

all: $(BINARY)

$(PARSER_C): $(PARSER_Y)
	bison -d $(PARSER_Y) -o $(PARSER_C)

$(BINARY): $(PARSER_C) $(SRCS)
	# Build a single static binary (as requested)
	cd $(COMPILER_DIR) && $(CC) $(CFLAGS) $(LDFLAGS) -o ../$(BINARY) *.c

clean:
	rm -f $(BINARY)
	rm -f $(PARSER_C) $(PARSER_H)

install: $(BINARY)
	install -d "$(DESTDIR)$(BINDIR)"
	install -m 755 "$(BINARY)" "$(DESTDIR)$(BINDIR)/$(BINARY)"

uninstall:
	rm -f "$(DESTDIR)$(BINDIR)/$(BINARY)"

.PHONY: all clean install uninstall
