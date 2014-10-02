#ifndef CODE_OBJECT_H
#define CODE_OBJECT_H

#include "./../vm/program.h"
#include "./object.h"

typedef struct {
    SeaObject_HEAD
    program* prog;
} SeaCodeObject;

#endif
