#include "src/execute.h"
#include "src/lexer.h"

int token_callback(token* tk, lexer* lx) {
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
