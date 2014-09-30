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


int run_machine(stack_machine* mach) {
    assert(mach->pc != mach->prog->num_lines);
    while (mach->pc != mach->prog->num_lines) {
        first_n(mach->stack, 5);
        assert(mach->prog->program[mach->pc] != NULL);

        int instruction = to_instruction(mach->prog->program[mach->pc]);
        if (instruction == -1) {
            fprintf(
                stderr,
                "Invalid instruction: \"%s\"\n", mach->prog->program[mach->pc]
            );
            return -1;
        }

        switch(instruction) {
            case NOP: {
                mach->pc++;
                break;
            }
            case PUSH: {
                char* push_val = mach->prog->program[mach->pc]+5;

                if (push_val[0] == '\0') {
                    mach->stack[++mach->sp] = mach->pc++;

                } else {
                    mach->stack[++mach->sp] = atoi(push_val);
                    mach->pc++;
                }

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
                // int num_1 = mach->stack[mach->sp--],
                //     num_2 = mach->stack[mach->sp--];
                // mach->stack[mach->sp+1] = 0;
                // mach->stack[mach->sp+2] = 0;
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
                }
                break;
            }
            case JUMP: {
                char* jump_pos = mach->prog->program[mach->pc]+5;

                if (jump_pos[0] == '\0') {
                    mach->pc = pop(mach);

                } else if (isalpha(jump_pos[0])) {
                    symbol* sim = dict_get(
                        mach->prog->symbol_table,
                        jump_pos
                    );
                    if (sim == NULL) {
                        printf("No such label as %s\n", jump_pos);
                        assert(sim != NULL);
                    }
                    mach->pc = sim->pc;

                } else {
                    mach->pc = atoi(jump_pos) - 1;
                }

                break;
            }
            case PRINT: {
                printf("%d\n", mach->stack[mach->sp]);
                mach->pc++;
                break;
            }
            case DUP: {
                mach->stack[++mach->sp] = mach->stack[mach->sp];
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
}
