#include <stdlib.h>

#include "./code_object.h"
#include "./object.h"
#include "./type_object.h"

static SeaTypeObject SeaCodeObject_Type = {
    SeaObject_HEAD_INIT(&SeaCodeObject_Type)
};

SeaCodeObject* SeaCodeObject_New() {
    SeaCodeObject* obj = SeaObject_New(SeaCodeObject, &SeaCodeObject_Type);

    return obj;
}


void SeaCodeObject_Free(SeaCodeObject* obj) {
    // if (obj == NULL) return;
    // SeaObject_Free(obj->ob_base);

    // if (obj->opcodes == NULL) return;

    // int idx = 0;
    // opcode* op = obj->opcodes[idx];
    // while (op != NULL) {
    //     SeaObject_Opcode_Free(op);
    //     op = obj->opcodes[idx++];
    // }
}
