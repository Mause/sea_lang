#include <stdlib.h>
#include <stdio.h>

#include "src/sea.h"
#include "src/parser.h"


sea* create_sea() {
    sea* s = malloc(sizeof(*s));

    s->ps = create_parser();

    return s;
}


void free_sea(sea* s) {
    free_parser(s->ps);
    free(s);
}

extern FILE* yyin;
int execute_file(sea* s, FILE* file) {
    yyin = file;

    return yyparse();
}
