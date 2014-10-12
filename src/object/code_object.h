#ifndef CODE_OBJECT_H
#define CODE_OBJECT_H

#include "./../vm/program.h"
#include "./../vm/opcodes.h"
#include "./object.h"

typedef struct {
    SeaObject_HEAD
    opcode** opcodes;
} SeaCodeObject;

SeaCodeObject* SeaCodeObject_New();
void           SeaCodeObject_Free(SeaCodeObject* obj);

#endif
