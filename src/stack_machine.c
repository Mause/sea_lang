#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "src/stack.h"
#include "src/bool.h"


enum Instruction {
    PUSH, POP, ADD, IFEQ, JUMP, PRINT, DUP, NOP
};

bool startswith(char* poss, char* chunk) {
    char* result = strstr(poss, chunk);

    return result != NULL && result == poss;
}

int to_instruction(char* line) {
         if (startswith(line, "push"))  return PUSH;
    else if (startswith(line, "pop"))   return POP;
    else if (startswith(line, "add"))   return ADD;
    else if (startswith(line, "ifeq"))  return IFEQ;
    else if (startswith(line, "jump"))  return JUMP;
    else if (startswith(line, "print")) return PRINT;
    else if (startswith(line, "dup"))   return DUP;
    else if (startswith(line, "nop"))   return NOP;
    else                                return -1;
}

void first_n(int* stack, int n) {
    int i;
    for (i=0; i<n; i++) {
        printf("%d, ", stack[i]);
    }
    printf("\n");
}

char* repr_instruction(int instruction) {
    switch(instruction) {
        case PUSH:  return "push";
        case POP:   return "pop";
        case ADD:   return "add";
        case IFEQ:  return "ifeq";
        case JUMP:  return "jump";
        case PRINT: return "print";
        case DUP:   return "dup";
        case NOP:   return "nop";
        default:    return "unknown";
    }
}

int count_lines(FILE* file) {
    int lines = 0;
    char buffer[1024];
    while (fgets(buffer, 1024, file)) lines++;
    rewind(file);
    return lines;
}

typedef struct {
    char** program;
    int num_lines;
    dict* symbol_table;
} program;

void free_program(program* prog) {
    int i;
    for (i=0; i<prog->num_lines; i++) {
        free(prog->program[i]);
    }
    free(prog->program);
    dict_free(prog->symbol_table);
    free(prog);
}


typedef struct {
    int pc;
} symbol;


program* read_program(FILE* file) {
    program* prog = calloc(1, sizeof(*prog));

    prog->num_lines = count_lines(file);
    prog->symbol_table = dict_create(10);
    prog->program = calloc(prog->num_lines, sizeof(char*));

    int cur_line=0;
    char buffer[1024];
    while (fgets(buffer, 1024, file)) {
        strtok(buffer, "\n");

        if (buffer[0] == ':') {
            symbol* sim = calloc(1, sizeof(*sim));
            sim->pc = cur_line;

            dict_set(
                prog->symbol_table,
                strdup(buffer+1), // everything bar the colon
                sim
            );
            prog->program[cur_line++] = strdup("nop");

        } else if (buffer[0] == '#') {
            prog->program[cur_line++] = strdup("nop");

        } else {
            prog->program[cur_line++] = strdup(buffer);

        }
    }

    // dict_keys(prog->symbol_table);

    return prog;
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
