#include <stdlib.h>
#include <string.h>

#include "./program.h"

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
