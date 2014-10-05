#include <stdlib.h>
#include <stdio.h>

#include "src/sea.h"

void print_usage(char** argv) {
    printf("Usage:   %s [-d] input_filename\n", argv[0]);
    printf("Options: -h     Print this help message\n");
    printf("         -d     Print lexing debug messages\n");

    printf("\n");
    printf("         [input_filename] can be a dash (-) for reading a program\n");
    printf("                          from standard in\n");
}


int main(int argc, char* argv[]) {
    // parse arguments
    char opt;
    extern int optind;
    while((opt = getopt(argc, argv, "dh")) != -1) {
        switch(opt) {
            case 'd': {
                extern int yydebug;
                yydebug = 1;
                break;
            }
            case 'h':
            case '?': {
                print_usage(argv);
                exit(EXIT_FAILURE);
            }
        }
    }

    if (optind >= argc) {
        fprintf(stderr, "Expected argument after options\n");
        print_usage(argv);
        exit(EXIT_FAILURE);
    }

    // use arguments
    char* filename = argv[optind];
    FILE* input_file;
    int is_stdin = strcmp(filename, "-") == 0;

    input_file = is_stdin ? stdin : fopen(filename, "rb");
    if (!input_file) {
        perror("An error occured whilst reading from the input file");
        return -1;
    }

    // main part of program
    sea* s = create_sea();
    int stat = execute_file(s, input_file);
    free_sea(s);

    // cleanup
    if (!is_stdin) {
        if (ferror(input_file)) {
            fprintf(stderr, "An error occured during reading from the file\n");
        }
        fclose(input_file);
    }

    if (stat != 0) {
        fprintf(stderr, "An occured during parsing\n");
        return stat;
    } else {
        return 0;
    }
}
