#include <stdlib.h>

#include "src/parser.h"

parser* create_parser() {
    parser* ps = malloc(sizeof(*ps));

    ps->lx = create_lexer();

    return ps;
}


void free_parser(parser* ps) {
    free_lexer(ps->lx);
    free(ps);
}
