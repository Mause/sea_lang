#include <assert.h>

#include "src/stack.h"

#include "./stack_machine.h"
#include "./program.h"
#include "./instruction.h"


void first_n(int* stack, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d", stack[i]);

        if (i == (n-1)) {
            printf("\n");
        } else {
            printf(", ");
        }
    }
}


int pop(stack_machine* mach) {
    int val = mach->stack[mach->sp];
    mach->stack[mach->sp--] = 0;
    return val;
}

void push(stack_machine* mach, int val) {
    mach->stack[++mach->sp] = val;
}

void jump(stack_machine* mach, char* dest) {
    if (isalpha(dest[0])) {
        symbol* sim = dict_get(
            mach->prog->symbol_table,
            dest
        );
        if (sim == NULL) {
            printf("No such label as %s\n", dest);
            assert(sim != NULL);
        } else {
            printf("warping to %s\n", dest);
        }
        mach->pc = sim->pc;
    assert(mach->pc >= 0);

    } else {
        mach->pc = atoi(dest) - 1;
        printf("%s\n", dest);
        assert(mach->pc >= 0);
    }
}


int run_machine(stack_machine* mach) {
    assert(mach->pc != mach->prog->num_lines);
    while (mach->pc != mach->prog->num_lines) {
        assert(mach->pc >= 0);
        assert(mach->prog->program[mach->pc] != NULL);

        int instruction = to_instruction(mach->prog->program[mach->pc]);
        if (instruction == -1) {
            fprintf(
                stderr,
                "%s:%d@%s: Invalid instruction: \"%s\"\n",
                __FILE__, __LINE__, __PRETTY_FUNCTION__,
                mach->prog->program[mach->pc]
            );
            return -1;
        }

        switch(instruction) {
            case NOP: {
                mach->pc++;
                break;
            }
            case PUSH: {
                push(
                    mach,
                    atoi(mach->prog->program[mach->pc]+5)
                );
                mach->pc++;

                break;
            }
            case JSR: {
                push(mach, mach->pc+1);
                jump(mach, mach->prog->program[mach->pc]+11);
                assert(mach->pc >= 0);
                // pc is assigned in jump
                break;
            }
            case POP: {
                assert(mach->sp >= 0);
                pop(mach);

                mach->pc++;
                break;
            }
            case ADD: {
                assert(mach->sp >= 1);
                int num_1 = pop(mach),
                    num_2 = pop(mach);

                mach->stack[++mach->sp] = num_1 + num_2;
                mach->pc++;
                break;
            }
            case IFEQ: {
                assert(mach->sp >= 1);
                int top = mach->stack[mach->sp];
                if (top == 0) {
                    mach->pc++;
                } else {
                    mach->pc = atoi(mach->prog->program[mach->pc]+5) - 1;
                    assert(mach->pc >= 0);
                }
                break;
            }
            case JUMP: {
                char* jump_pos = mach->prog->program[mach->pc]+5;

                if (mach->prog->program[mach->pc][4] == '\0') {
                    printf("warping back to %d\n", mach->stack[mach->sp]);
                    mach->pc = pop(mach); assert(mach->pc >= 0);

                } else {
                    jump(mach, jump_pos);
                }

                break;
            }
            case PRINT: {
                printf("line %d -> ", mach->pc+1);
                if (mach->sp == -1) {
                    printf("stack empty\n");
                } else {
                    printf("%d\n", mach->stack[mach->sp]);
                }

                mach->pc++;
                break;
            }
            case DUP: {
                mach->stack[++mach->sp] = mach->stack[mach->sp];
                mach->pc++;
                break;
            }
            case SHOW: {
                int i;
                printf("[ ");
                for (i=0; i<(mach->sp+1); i++) {
                    printf("%d", mach->stack[i]);

                    if (i != mach->sp) {
                        printf(", ");
                    }
                }
                printf(" ]\n");
                mach->pc++; assert(mach->pc >= 0);
                break;
            }
            case SLEEP: {
                sleep(1);
                mach->pc++;
                break;
            }
            default: {
                printf("%s\n", mach->prog->program[mach->pc]);
                assert("unknown instruction" == 0);
            }
        }
        // sleep(1);
    }

    return 0;
}
