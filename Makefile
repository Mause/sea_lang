CC = gcc

all: build/main

build/lexer.o: src/lexer.h src/lexer.c
	$(CC) -g src/lexer.c -c -o build/lexer.o


build/parser.o: src/parser.h src/parser.c
	$(CC) -g src/parser.c -c -o build/parser.o


build/execute.o: src/execute.h src/execute.c
	$(CC) -g src/execute.c -c -o build/execute.o	


build/main: build/lexer.o build/parser.o build/execute.o src/main.c
	$(CC) -g src/main.c build/lexer.o build/parser.o build/execute.o -o build/main

clean:
	rm -f build/lexer.o build/parser.o build/execute.o build/main
