#include <stdlib.h>

#include "src/parser.h"

parser* create_parser() {
    parser* ps = malloc(sizeof(*ps));

    return ps;
}


void free_parser(parser* ps) {
    free(ps);
}
