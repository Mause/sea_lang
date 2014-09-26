#ifndef STACK_H
#define STACK_H

#include "src/dict/dict.h"

typedef struct stack_frame {
    dict* variables;
    struct stack_frame* next;
    struct stack_frame* previous;
} stack_frame;

typedef struct {
    stack_frame* top;
} callstack;

stack_frame* stack_pop(callstack* cs);
stack_frame* stack_push(callstack* cs);
void stack_free_frame(stack_frame* frame);
callstack* stack_create();


#endif
