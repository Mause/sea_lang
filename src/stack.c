#include <stdlib.h>

#include "src/stack.h"
#include "src/dict/dict.h"

stack_frame* stack_pop(callstack* cs) {
    stack_frame* old_top = cs->top;

    cs->top->next->previous = NULL;
    cs->top = cs->top->next;

    stack_free_frame(old_top);

    return cs->top;

    // stack_frame *current = sf;

    // while (current->next->next != NULL) {
    //     current = current->next;
    // }

    // stack_frame* to_cleanup = current->next;

    // current->next = NULL;

    // return to_cleanup;
}


void stack_free_frame(stack_frame* frame) {
    dict_free(frame->variables);
    free(frame);
}

void stack_free(callstack* cs) {
    stack_frame* current = cs->top;

    while (current != NULL) {
        stack_frame* next = current->next;
        stack_free_frame(current);
        current = next;
    }
}


stack_frame* stack_push(callstack* cs) {
    if (cs->top == NULL) {
        cs->top = calloc(1, sizeof(*cs->top));
    } else {
        stack_frame* old_top = cs->top;
        cs->top = calloc(1, sizeof(*cs->top));
        cs->top->next = old_top;
        old_top->previous = cs->top;
    }

    cs->top->variables = dict_create(100);

    return cs->top;
}


callstack* stack_create(void) {
    callstack* cs = calloc(1, sizeof(*cs));

    cs->top = NULL;

    return cs;
}
