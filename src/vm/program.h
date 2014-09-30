#ifndef PROGRAM_H
#define PROGRAM_H

#include <stdio.h>
#include "src/dict/dict.h"

typedef struct {
    char** program;
    int num_lines;
    dict* symbol_table;
} program;


typedef struct {
    int pc;
} symbol;

void free_program(program* prog);
program* read_program(FILE* file);

#endif
