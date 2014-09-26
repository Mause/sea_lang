CC = gcc
CFLAGS = -g -I src -I .

all: build/main

build/flex_parser.o: src/flex_parser.l build/grammar.o
	flex --outfile=build/flex_parser.c src/flex_parser.l
	$(CC) $(CFLAGS) build/flex_parser.c -c -o build/flex_parser.o


build/grammar.o: src/grammar.y
	yacc --verbose --debug -d src/grammar.y -o build/grammar.c
	$(CC) $(CFLAGS) build/grammar.c -c -o build/grammar.o


build/sea.o: src/sea.h src/sea.c
	$(CC) $(CFLAGS) src/sea.c -c -o build/sea.o


SRC = build/grammar.o \
	  build/flex_parser.o \
	  build/sea.o \
	  src/main.c


build/main: $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o build/main

clean:
	rm -f \
		build/sea.o \
		build/main \
		build/flex_parser.c \
		build/flex_parser.o
