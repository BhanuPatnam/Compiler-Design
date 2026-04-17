# Root Makefile for AlgoCompiler

all:
	$(MAKE) -C compiler all

clean:
	$(MAKE) -C compiler clean

test:
	$(MAKE) -C compiler test

install:
	$(MAKE) -C compiler install

deb:
	$(MAKE) -C compiler deb

.PHONY: all clean test install deb
