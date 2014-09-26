CC = gcc
CFLAGS = -g -I src -I .

all: build/main build/test_dict
.PHONY: all

build/flex_parser.o: build/grammar.o src/flex_parser.l
	flex --outfile=build/flex_parser.c src/flex_parser.l
	$(CC) $(CFLAGS) build/flex_parser.c -c -o build/flex_parser.o


build/grammar.o: src/grammar.y
	yacc --verbose --debug -d src/grammar.y -o build/grammar.c
	$(CC) $(CFLAGS) build/grammar.c -c -o build/grammar.o


build/sea.o: src/sea.h src/sea.c build/grammar.o
	$(CC) $(CFLAGS) src/sea.c -c -o build/sea.o


build/stack.o: src/stack.h src/stack.c
	$(CC) $(CFLAGS) src/stack.c -c -o build/stack.o


build/eval.o: src/eval.h src/eval.c
	$(CC) $(CFLAGS) src/eval.c -c -o build/eval.o


build/compile.o: src/compile.h src/compile.c
	$(CC) $(CFLAGS) src/compile.c -c -o build/compile.o


build/dict_entry.o: src/dict/dict_entry.c src/dict/dict_entry.h
	$(CC) $(CFLAGS) src/dict/dict_entry.c -c -o build/dict_entry.o


build/primes.o: src/dict/primes.h src/dict/primes.c
	$(CC) $(CFLAGS) src/dict/primes.c -c -o build/primes.o


build/dict.o: src/dict/dict.h src/dict/dict.c build/dict_entry.o build/primes.o
	$(CC) $(CFLAGS) src/dict/dict.c -c -o build/dict.o


build/grammar_handlers.o: src/grammar_handlers.h src/grammar_handlers.c
	$(CC) $(CFLAGS) src/grammar_handlers.c -c -o build/grammar_handlers.o


SRC = build/grammar.o \
	  build/grammar_handlers.o \
	  build/flex_parser.o \
	  build/dict_entry.o \
	  build/primes.o \
	  build/stack.o \
	  build/eval.o \
	  build/compile.o \
	  build/sea.o \
	  build/dict.o \
	  src/ast/ast.c \
	  src/ast/render_ast.c \
	  src/main.c

build/test_dict: build/dict.o build/primes.o build/dict_entry.o src/dict/test_dict.c
	$(CC) $(CFLAGS) src/dict/test_dict.c build/dict_entry.o build/primes.o build/dict.o -o build/test_dict


build/main: $(SRC) src/ast/ast.h src/ast/render_ast.h
	$(CC) $(CFLAGS) $(SRC) -o build/main

clean:
	rm -f \
		build/*.o \
		build/flex_parser.c \
		build/grammar.c \
		build/grammar.h \
		build/grammar.output \
		build/main \
		build/test_dict
