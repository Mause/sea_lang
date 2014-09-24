#include "src/execute.h"
#include "src/lexer.h"


char* repr(int type) {
    switch(type) {
        case IDENTIFIER: return "IDENTIFIER";
        case INTEGER   : return "INTEGER";
    }
    
    return "UNKNOWN";
}


int token_callback(token* tk, lexer* lx) {
    printf("Token: \"%s\" of type %s\n", tk->contents, repr(tk->type));
    return 0;
}


int execute_file(FILE* file) {
    lexer* lx = create_lexer();

    lx->token_callback = token_callback;

    char buffer[1024];
    while (fgets(buffer, 1024, file) != NULL) {
        lexer_feedline(lx, buffer);
    }

    return 0;
}
