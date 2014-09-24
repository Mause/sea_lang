#include <stdio.h>

#include "src/execute.h"
#include "src/sea.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Must provide an input file\n");
        return -1;
    }

    FILE* input_file;
    int is_stdin = strcmp(argv[1], "-") == 0;

    if (is_stdin) {
        input_file = stdin;
    } else {
        input_file = fopen(argv[1], "rb");
    }

    if (!input_file) {
        perror("An error occured whilst reading from the input file");
        return -1;
    }

    sea* s = create_sea();
    execute_file(s, input_file);

    if (!is_stdin) {
        if (ferror(input_file)) {
            fprintf(stderr, "An error occured during reading from the file\n");
        }
        fclose(input_file);
    }

    return 0;
}
