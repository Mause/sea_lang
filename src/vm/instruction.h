#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "src/bool.h"

enum instruction {
    PUSH,       // push's the specified number onto the stack
    POP,        // pops the top items on the stack
    ADD,        // pop the top items from the stack, add them together, and push the result back on top
    IFEQ,       // if the value on top of the stack is zero, jump the specified address
    JUMP,       // jump to the specified value, or to the value on top of the stack if none is provided
    PRINT,      // prints val on top of stack
    DUP,        // duplicates item on top of stack
    NOP,        // does nothing
    SHOW,       // pretty print the stack
    SLEEP,      // sleep for one second, for debugging purposes
    JSR,        // (jump subroutine) pushes the current address+1 onto the stack, jump to the specified position
};

bool startswith(char* poss, char* chunk);
int to_instruction(char* line);
char* repr_instruction(int instruction);

#endif
