#include <string.h>

#include "src/bool.h"
#include "./instruction.h"

bool startswith(char* poss, char* chunk) {
    char* result = strstr(poss, chunk);

    return result != NULL && result == poss;
}

int to_instruction(char* line) {
         if (startswith(line, "push"))  return PUSH;
    else if (startswith(line, "pop"))   return POP;
    else if (startswith(line, "add"))   return ADD;
    else if (startswith(line, "ifeq"))  return IFEQ;
    else if (startswith(line, "jump"))  return JUMP;
    else if (startswith(line, "print")) return PRINT;
    else if (startswith(line, "dup"))   return DUP;
    else if (startswith(line, "nop"))   return NOP;
    else if (startswith(line, "show"))  return SHOW;
    else if (startswith(line, "sleep")) return SLEEP;
    else if (startswith(line, "subroutine")) return JSR;
    else                                return -1;
}

char* repr_instruction(int instruction) {
    switch(instruction) {
        case PUSH:  return "push";
        case POP:   return "pop";
        case ADD:   return "add";
        case IFEQ:  return "ifeq";
        case JUMP:  return "jump";
        case PRINT: return "print";
        case DUP:   return "dup";
        case NOP:   return "nop";
        case SHOW:  return "show";
        case SLEEP: return "sleep";
        case JSR:   return "subroutine";
        default:    return "unknown";
    }
}
