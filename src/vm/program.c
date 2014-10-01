#include <stdlib.h>
#include <string.h>

#include "./program.h"

static int count_lines(FILE* file) {
    int lines = 0;
    char buffer[1024];
    while (fgets(buffer, 1024, file)) lines++;
    rewind(file);
    return lines;
}

void free_program(program* prog) {
    int i;
    for (i=0; i<prog->num_lines; i++) {
        free(prog->program[i]);
    }
    free(prog->program);
    for (i=0; i<prog->symbol_table->max_size; i++) {
        if (dict_valid_entry(prog->symbol_table, i)) {
            dict_entry* entry = prog->symbol_table->entries[i];

            free(entry->key);
            free(entry->value);
        }
    }
    dict_free(prog->symbol_table);
    free(prog);
}


program* read_program(FILE* file) {
    program* prog = calloc(1, sizeof(*prog));

    prog->num_lines = count_lines(file);
    prog->symbol_table = dict_create(10);
    prog->program = calloc(prog->num_lines, sizeof(char*));

    int cur_line=0;
    char buffer[1024];
    while (fgets(buffer, 1024, file)) {
        strtok(buffer, "\n");

        if (buffer[0] == '\n' || buffer[0] == '#') {
            // don't do anything for a comment or an empty line

        } else if (buffer[0] == ':') {
            symbol* sim = calloc(1, sizeof(*sim));
            sim->pc = cur_line;

            dict_set(
                prog->symbol_table,
                strdup(buffer+1), // everything bar the colon
                sim
            );

        } else {
            prog->program[cur_line++] = strdup(buffer);

        }
    }

    // rebuild program, discarding lines that ended up being comments or empty
    prog->num_lines = cur_line;
    char** program = calloc(prog->num_lines, sizeof(char*));
    for (cur_line=0; cur_line<prog->num_lines; cur_line++) {
        program[cur_line] = prog->program[cur_line];
    }
    free(prog->program);
    prog->program = program;

    return prog;
}
