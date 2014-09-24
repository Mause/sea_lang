#include "src/execute.h"
#include "src/lexer.h"
#include "src/parser.h"
#include "src/bool.h"

char* repr(int type) {
    switch(type) {
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER   : return "INTEGER";
        case STATEMENT : return "STATEMENT"; 
    }
    
    return "UNKNOWN";
}

int is_statement(token* tk) {
           if (strcmp(tk->contents, "var") == 0) {
        return TRUE;
    } else if (strcmp(tk->contents, "func") == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}


int token_callback(token* tk, lexer* lx) {
    if (is_statement(tk)) {
        tk->type = STATEMENT;
    }

    printf("Token: \"%s\" of type %s\n", tk->contents, repr(tk->type));

    return 0;
}


int execute_file(sea* s, FILE* file) {
    s->ps->lx->token_callback = token_callback;

    char buffer[1024];
    while (fgets(buffer, 1024, file) != NULL) {
        lexer_feedline(s->ps->lx, buffer);
    }

    return 0;
}
