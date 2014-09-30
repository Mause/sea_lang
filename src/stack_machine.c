#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "src/stack.h"

#include "src/stack_machine/program.h"
#include "src/stack_machine/instruction.h"


void first_n(int* stack, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d, ", stack[i]);
    }
    printf("\n");
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Must provide an input file\n");
        return -1;
    }

    FILE* file;
    file = fopen(argv[1], "r");
    if (!file) {
        perror("Couldn't open input file");
        return -1;
    }

    program* prog = read_program(file);

    fclose(file);

    int* stack = calloc(200, sizeof(int)),
         pc = 0,
         sp = -1;

    assert(pc != prog->num_lines);
    while (pc != prog->num_lines) {
        // first_n(stack, 5);
        assert(prog->program[pc] != NULL);

        // printf("%s\n", prog->program[pc]);
        int instruction = to_instruction(prog->program[pc]);
        if (instruction == -1) {
            fprintf(stderr, "Invalid instruction: %s\n", prog->program[pc]);
            return -1;
        }

        switch(instruction) {
            case NOP: {
                pc++;
                break;
            }
            case PUSH: {
                stack[++sp] = atoi(prog->program[pc]+5);
                pc++;
                break;
            }
            case POP: {
                assert(sp >= 0);
                stack[sp] = 0;
                sp--;
                pc++;
                break;
            }
            case ADD: {
                assert(sp >= 1);
                int num_1, num_2;
                num_1 = stack[sp--];
                num_2 = stack[sp--];
                stack[sp+1] = 0;
                stack[sp+2] = 0;

                stack[++sp] = num_1 + num_2;
                pc++;
                break;
            }
            case IFEQ: {
                assert(sp >= 1);
                int top = stack[sp];
                if (top == 0) {
                    pc++;
                } else {
                    pc = atoi(prog->program[pc]+5) - 1;
                }
                break;
            }
            case JUMP: {
                char* jump_pos = prog->program[pc]+5;

                if (isalpha(jump_pos[0])) {
                    symbol* sim = dict_get(
                        prog->symbol_table,
                        jump_pos
                    );
                    if (sim == NULL) {
                        printf("No such label as %s\n", jump_pos);
                        assert(sim != NULL);
                    }
                    pc = sim->pc;

                } else {
                    pc = atoi(jump_pos) - 1;
                }

                break;
            }
            case PRINT: {
                printf("%d\n", stack[sp]);
                pc++;
                break;
            }
            case DUP: {
                stack[++sp] = stack[sp];
                pc++;
                break;
            }
            default: {
                printf("%s\n", prog->program[pc]);
                assert("unknown instruction" == 0);
            }
        }
        // sleep(1);
    }

    // first_n(stack, 5);

    free_program(prog);

    free(stack);

    /* code */
    return 0;
}
