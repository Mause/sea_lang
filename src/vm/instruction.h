#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "src/bool.h"

enum instruction {
    PUSH,
    POP,
    ADD,
    IFEQ,
    JUMP,       // jump to the specified value, or to the value on top of the stack if none is provided
    PRINT,      // prints val on top of stack
    DUP,        // duplicates item on top of stack
    NOP,        // does nothing
};

bool startswith(char* poss, char* chunk);
int to_instruction(char* line);
char* repr_instruction(int instruction);

#endif
