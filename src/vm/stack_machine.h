#ifndef STACK_MACHINE_H
#define STACK_MACHINE_H

#include "./program.h"

typedef struct {
    program* prog;
    int pc;
    int sp;
    int* stack;
} stack_machine;

#endif
