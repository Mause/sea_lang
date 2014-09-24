#include <stdlib.h>

#include "src/parser.h"

parser* create_parser() {
    parser* ps = malloc(sizeof(*ps));

    ps->lx = create_lexer();

    return ps;
}

