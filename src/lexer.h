#ifndef LEXER_H
#define LEXER_H

enum token_type {
    IDENTIFIER, INTEGER
};

typedef struct {
    char* contents;
    int type;
} token;

typedef struct lexer {
    int (*token_callback)(token* tk, struct lexer* lx);

    char* text;
    char ch;
    int at;
    int len;
    int depth;
} lexer;

lexer* create_lexer();
void* lexer_feedline(lexer* lx, char* line);
    
#define NEWLINE '\n'
#define TAB     '\t'

#endif
