CC=g++
CFLAGS=-I

main: io hoffman composer parser util
	$(CC) -o bin/main.o main.cpp

io:
	mkdir bin
	$(CC) -o bin/io.o io.hpp

hoffman: tree
	$(CC) -o bin/hoffman.o hoffman.hpp

tree: node byte
	$(CC) -o bin/tree.o tree.hpp

node:
	$(CC) -o bin/node.o node.hpp

byte:
	$(CC) -o bin/byte.o byte.hpp

composer:
	$(CC) -o bin/composer.o composer.hpp

filestruct:
	$(CC) -o bin/filestruct.o filestruct.hpp

parser:
	$(CC) -o bin/parser.o parser.hpp

util:
	$(CC) -o bin/util.o util.hpp

	$(CC) -o bin/util.o util.hpp
