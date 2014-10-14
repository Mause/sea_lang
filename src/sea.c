#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "src/sea.h"
#include "build/parser/grammar.h"

#include "src/ast/ast.h"

sea* create_sea(void) {
    sea* s = malloc(sizeof(*s));

    return s;
}


void free_sea(sea* s) {
    free(s);
}

bool show_errors(ASTNode* ast, FILE* file) {
    many_nodes* errors = grab_errors(ast);

    int i;
    for (i=0; i<errors->num_nodes; i++) {
        error* err = errors->nodes[i]->err;

        rewind(file);

        int j;
        char buffer[1024];
        for (j=0; j<err->lloc.last_line; j++) {
            fgets(buffer, 1024, file);
        }

        printf("Syntax error on line %d:", err->lloc.last_line);
        if (err->message != NULL) printf(" %s\n", err->message);
        else                      printf("\n");

        strtok(buffer, "\n");
        printf("\t%s\n", buffer);
        printf("\t^\n");

        free(err);
    }

    int was_errors = errors->num_nodes != 0;

    free(errors->nodes);
    free(errors);

    printf("\n");

    return was_errors;
}

int execute_file(sea* s, FILE* file) {
    extern FILE* yyin;
    yyin = file;

    int return_val = yyparse();

    if (return_val != 0) return return_val;

    extern void* res;
    ASTNode* ast = res;
    printf("\n---------------------------\n");
    render_ast(ast, -1);
    printf("\n---------------------------\n");

    bool was_errors = show_errors(ast, file);
    if (was_errors) return -1;

    int stat = eval(ast);

    if (stat != 0) {
        printf("An error occured during evaluation\n");
    }

    free_ast(ast);
    yylex_destroy();

    return 0;
}
