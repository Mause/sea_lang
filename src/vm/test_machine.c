#include <stdio.h>
#include <stdlib.h>

#include "./stack_machine.h"


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

    stack_machine* mach = calloc(1, sizeof(*mach));
    mach->prog = read_program(file);

    fclose(file);

    mach->stack = calloc(200, sizeof(int));
    mach->pc = 0;
    mach->sp = -1;


    run_machine(mach);

    // first_n(stack, 5);

    free_program(mach->prog);
    free(mach->stack);
    free(mach);

    return 0;
}
