#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "src/bool.h"

enum instruction {
    PUSH, POP, ADD, IFEQ, JUMP, PRINT, DUP, NOP
};

bool startswith(char* poss, char* chunk);
int to_instruction(char* line);
char* repr_instruction(int instruction);

#endif
