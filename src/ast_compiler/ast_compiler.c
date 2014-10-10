#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "./ast_compiler.h"


char* condense_opcodes(char** orig_opcodes) {
    int num_opcodes = 0, idx = 0;
    char* cur_chunk, *opcodes, *last_pos;

    cur_chunk = orig_opcodes[0];
    while (cur_chunk != NULL) {
        num_opcodes += strlen(cur_chunk);
        cur_chunk = orig_opcodes[idx++];
    }


    opcodes = calloc(num_opcodes, sizeof(char*));
    last_pos = opcodes;

    cur_chunk = orig_opcodes[0];
    while (cur_chunk != NULL) {
        int chunk_len = strlen(cur_chunk);

        last_pos = strncpy(
            last_pos,
            cur_chunk,
            chunk_len
        );

        last_pos += strlen(cur_chunk);
    }
    opcodes[num_opcodes-1] = '\0';

    return opcodes;
}


SeaCodeObject* compile_ast(ASTNode* ast) {
    SeaCodeObject* obj = SeaCodeObject_New();

    char** opcodes = calloc(
        1024, // 1024 chunk of chunks of opcodes, to be condensed later
        sizeof(char*)
    );

    // obj->opcodes = opcodes;

    assert(ast->type == NODE_MANYNODES);


    condense_opcodes(opcodes);
    free(opcodes);

    return obj;
}
