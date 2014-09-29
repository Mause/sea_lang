#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "src/ast/ast.h"
#include "src/bool.h"
#include "src/compile.h"
#include "src/dict/dict.h"
#include "src/object/object.h"
#include "src/object/none.h"
#include "src/stack.h"

int run_function(ASTNode* func, ASTNode* program, callstack* stack);


int eval(ASTNode* ast) {
    assert(ast->type == NODE_MANYNODES);

    callstack* stack = compile(ast);

    ASTNode* main = dict_get(stack->top->variables, "main");
    if (main == NULL) return -1;

    run_function(
        main,
        ast,
        stack
    );

    return 0;
}


SeaObject* get_variable(char* name, stack_frame* stack) {
    SeaObject* obj = dict_get(stack->variables, name);

    if (obj == NULL) {
        if (stack->next == NULL) {
            return NULL;
        } else {
            return get_variable(name, stack->next);
        }
    } else {
        return obj;
    }
}

int run_function(ASTNode* func, ASTNode* program, callstack* cs) {
    assert(func->type == NODE_FUNCTION);

    stack_frame* cur_frame = stack_push(cs);
    int pc = 0;

    many_nodes* body = func->func->body;
    assert(body);
    assert(body->num_nodes > 0);

    printf("(Looping)\n");
    while (TRUE) {
        if (pc == body->num_nodes) break;

        ASTNode* current = body->nodes[pc];
        printf("%s\n", repr(current->type));

        switch(current->type) {
        case NODE_DECLARATION: {
            if (current->declare->expr == NULL) {
                dict_set(
                    cur_frame->variables,
                    current->declare->ident,
                    SeaNone
                );
            } else {
                fprintf(stderr, "var's with values not yet supported\n");
            }
            break;
        }
        case NODE_ASSIGNMENT: {
            void* val = get_variable(
                current->assign->ident,
                cs->top
            );

            if (val == NULL) {
                fprintf(stderr, "No such variable as %s\n", current->assign->ident);
            }
            break;
        }
        case NODE_FUNCTION_CALL: {
            // run_function(current->call->function, program, stack);
            break;
        }
        default: {
            fprintf(stderr, "Unknown; %s\n", repr(current->type));
            assert(0);
        }
        }

        pc++;
    }
    dict_repr(cs->top->variables);
}
