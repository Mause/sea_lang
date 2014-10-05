#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "src/sea.h"
#include "build/grammar.h"

#include "src/ast/ast.h"

sea* create_sea(void) {
    sea* s = malloc(sizeof(*s));

    return s;
}


void free_sea(sea* s) {
    free(s);
}

extern FILE* yyin;
extern void* res;

int execute_file(sea* s, FILE* file) {
    yyin = file;

    int return_val = yyparse();

    if (return_val != 0)  return return_val;

    ASTNode* ast = res;
    printf("\n---------------------------\n");
    render_ast(ast, -1);
    printf("\n---------------------------\n");

    // free_ast(ast);

    int stat = eval(ast);

    if (stat != 0) {
        printf("An error occured during evaluation\n");
    }

    // assert(0);

    return 0;
}
