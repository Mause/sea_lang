#include <stdlib.h>
#include <assert.h>

#include "src/compile.h"

callstack* compile(ASTNode* ast) {
    callstack* stack = stack_create();

    stack_frame* cur_frame = stack_push(stack);

    assert(ast->type == NODE_MANYNODES);
    int i;

    for (i=0; i<ast->nodes->num_nodes; i++) {
        ASTNode* current = ast->nodes->nodes[i];

        if (current->type == NODE_FUNCTION) {
            dict_set(
                cur_frame->variables,
                current->func->name,
                current
            );
        } else if (current->type == NODE_IMPORT) {
            dict_set(
                cur_frame->variables,
                current->string,
                NULL
            );
        }
    }

    dict_repr(cur_frame->variables);

    return stack;
}
