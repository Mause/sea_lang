#include <stdlib.h>
#include <stdio.h>

#include "src/sea.h"


sea* create_sea() {
    sea* s = malloc(sizeof(*s));

    return s;
}


void free_sea(sea* s) {
    free(s);
}

extern FILE* yyin;
int execute_file(sea* s, FILE* file) {
    yyin = file;

    return yyparse();
}
