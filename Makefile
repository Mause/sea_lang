CC = gcc
ARCHIVER = ar
AFLAGS += -cq
# AFLAGS = -cvq
CFLAGS += -g -I src -I .

# build/test_dict build/test_machine

all: build/sea
.PHONY: all

build/flex_parser.o: build/grammar.o src/parser/flex_parser.l
	flex --outfile=build/flex_parser.c src/parser/flex_parser.l
	$(CC) $(CFLAGS) build/flex_parser.c -c -o $@


build/grammar.o: src/parser/grammar.y
	bison --verbose --debug -d src/parser/grammar.y -o build/parser/grammar.c
	$(CC) $(CFLAGS) build/parser/grammar.c -c -o $@


build/sea.o: src/sea.h src/sea.c build/grammar.o
	$(CC) $(CFLAGS) src/sea.c -c -o $@


build/stack.o: src/stack.h src/stack.c
	$(CC) $(CFLAGS) src/stack.c -c -o $@


build/eval.o: src/eval.h src/eval.c
	$(CC) $(CFLAGS) src/eval.c -c -o $@


build/compile.o: src/compile.h src/compile.c
	$(CC) $(CFLAGS) src/compile.c -c -o $@


build/none.o: src/object/none.h src/object/none.c
	$(CC) $(CFLAGS) src/object/none.c -c -o $@


build/object.o: src/object/object.h src/object/object.c
	$(CC) $(CFLAGS) src/object/object.c -c -o $@


build/code_object.o: src/object/code_object.h src/object/code_object.c
	$(CC) $(CFLAGS) src/object/code_object.c -c -o $@


build/dict_entry.o: src/dict/dict_entry.c src/dict/dict_entry.h
	$(CC) $(CFLAGS) src/dict/dict_entry.c -c -o $@


build/primes.o: src/dict/primes.h src/dict/primes.c
	$(CC) $(CFLAGS) src/dict/primes.c -c -o $@


build/dict.o: src/dict/dict.h src/dict/dict.c build/dict_entry.o build/primes.o
	$(CC) $(CFLAGS) src/dict/dict.c -c -o $@


build/grammar_handlers.o: src/parser/grammar_handlers.h src/parser/grammar_handlers.c
	$(CC) $(CFLAGS) src/parser/grammar_handlers.c -c -o $@


build/ast.o: src/ast/ast.h src/ast/ast.c
	$(CC) $(CFLAGS) src/ast/ast.c -c -o $@


build/render_ast.o: src/ast/render_ast.h src/ast/render_ast.c
	$(CC) $(CFLAGS) src/ast/render_ast.c -c -o $@


build/ast_compiler.o: src/ast_compiler/ast_compiler.h src/ast_compiler/ast_compiler.c
	$(CC) $(CFLAGS) src/ast_compiler/ast_compiler.c -c -o $@


LIBDICT_SRC := build/dict.o \
			   build/primes.o \
			   build/dict_entry.o \
			   build/none.o

build/libdict.a: $(LIBDICT_SRC)
	$(ARCHIVER) $(AFLAGS) build/libdict.a $(LIBDICT_SRC)


SRC = build/grammar.o \
	  build/grammar_handlers.o \
	  build/flex_parser.o \
	  build/ast_compiler.o \
	  build/none.o \
	  build/object.o \
	  build/stack.o \
	  build/code_object.o \
	  build/eval.o \
	  build/compile.o \
	  build/ast.o \
	  build/render_ast.o \
	  build/sea.o \
	  build/libdict.a


build/libsea.a: $(SRC) $(LIBDICT_SRC)
	$(ARCHIVER) $(AFLAGS) build/libsea.a $(SRC) $(LIBDICT_SRC)


build/test_dict: src/dict/test_dict.c build/libdict.a
	$(CC) $(CFLAGS) src/dict/test_dict.c build/libdict.a -o $@


build/sea: build/libsea.a src/main.c
	$(CC) $(CFLAGS) src/main.c build/libsea.a -o $@


build/vm/instruction.o: src/vm/instruction.h src/vm/instruction.c
	$(CC) $(CFLAGS) src/vm/instruction.c -c -o $@


build/vm/program.o: src/vm/program.h src/vm/program.c
	$(CC) $(CFLAGS) src/vm/program.c -c -o $@


build/vm/stack_machine.o: src/vm/stack_machine.h src/vm/stack_machine.c
	$(CC) $(CFLAGS) src/vm/stack_machine.c -c -o $@

LIBVM_SRC = build/vm/instruction.o build/vm/program.o build/vm/stack_machine.o

build/test_machine: $(LIBVM_SRC) $(LIBDICT_SRC)
	$(CC) $(CFLAGS) src/vm/test_machine.c $(LIBDICT_SRC) $(LIBVM_SRC) -o $@

clean:
	find build  | grep "\.o" | xargs rm
	rm -f \
		build/*.a \
		build/*.exe \
		build/flex_parser.c \
		build/grammar.c \
		build/grammar.h \
		build/grammar.output \
		build/main \
		build/test_dict \
		build/test_machine
