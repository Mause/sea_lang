#include <stdlib.h>
#include <string.h>

#include "src/lexer.h"
#include "src/bool.h"

void* value(lexer* lx);
token* word(lexer* lx);
token* number(lexer* lx);
char* digit(lexer* lx);

lexer* create_lexer() {
    lexer* lx = malloc(sizeof(*lx));

    lx->token_callback = NULL;

    lx->text = NULL;
    lx->ch = '\0';
    lx->at = 0;
    lx->len = 0;
    lx->depth = 0;

    return lx;
}

void free_lexer(lexer* lx) {
    free(lx);
}

token* create_token() {
    token* tk = malloc(sizeof(*tk));

    tk->contents = "";
    tk->type = 0;

    return tk;
}

void free_token(token* tk) {
    free(tk->contents);
    free(tk);
}

void* lexer_feedline(lexer* lx, char* line) {
    lx->text = line;
    lx->at = 0;
    lx->ch = '\0';
    lx->depth = 0;
    lx->len = strlen(lx->text);
    next_chr(lx);

    while (TRUE) {
        token* tk = value(lx);
        if (tk == NULL) break;
        lx->token_callback(tk, lx);
    }
}

void white(lexer* lx) {
    while (lx->ch != '\0') {
        if (isspace(lx->ch)) {
            next_chr(lx);
        } else {
            break;
        }
    }
}

int next_chr(lexer* lx) {
    if (lx->at >= lx->len) {
        lx->ch = '\0';
        return FALSE;
    }

    lx->ch = lx->text[lx->at];
    lx->at++;
    return TRUE;
}


void* value(lexer* lx) {
    white(lx);
    if (lx->ch == '\0') {
        return NULL;
    }

    /*
    if (lx->ch == '{') {
//        return object(lx);
    }

    if (lx->ch, "[") == 0) {
        next_chr(lx);
    }*/


/*    if (lx->ch in ['"',  "'",  '[']) {
        return string(lx, lx->ch);
    }*/

    if (isdigit(lx->ch) || lx->ch == '-') {
        return number(lx);
    }

    return word(lx);
}

/*
def string(self, end=None):
    s = ''
    start = self.ch
    if end == '[':
        end = ']'
    if start in ['"',  "'",  '[']:
        while self.next_chr():
            if self.ch == end:
                self.next_chr()
                if start != "[" or self.ch == ']':
                    return s
            if self.ch == '\\' and start == end:
                self.next_chr()
                if self.ch != end:
                    s += '\\'
            s += self.ch
    print ERRORS['unexp_end_string']
*/

/*
def object(self):
    o = {}
    k = ''
    idx = 0
    numeric_keys = False
    self.depth += 1
    self.next_chr()
    self.white()
    if self.ch and self.ch == '}':
        self.depth -= 1
        self.next_chr()
        return o #Exit here
    else:
        while self.ch:
            self.white()
            if self.ch == '{':
                o[idx] = self.object()
                idx += 1
                continue
            elif self.ch == '}':
                self.depth -= 1
                self.next_chr()
                if k:
                   o[idx] = k
                if not numeric_keys and len([ key for key in o if type(key) in (str,  float,  bool,  tuple)]) == 0:
                    ar = []
                    for key in o:
                       ar.insert(key, o[key])
                    o = ar
                return o #or here
            else:
                if self.ch == ',':
                    self.next_chr()
                    continue
                else:
                    k = self.value()
                    if self.ch == ']':
                        numeric_keys = True
                        self.next_chr()
                self.white()
                if self.ch == '=':
                    self.next_chr()
                    self.white()
                    o[k] = self.value()
                    idx += 1
                    k = ''
                elif self.ch == ',':
                    self.next_chr()
                    self.white()
                    o[idx] = k
                    idx += 1
                    k = ''
    print ERRORS['unexp_end_table'] #Bad exit here

*/
token* word(lexer* lx) {
    char* s = calloc(20, sizeof(char*));
    int idx = 0;

    if (lx->ch != '\n') {
        s[idx++] = lx->ch;
    }

    while (next_chr(lx)) {
        if (isalnum(lx->ch)) {
            s[idx++] = lx->ch;
        } else {
            break;
        }
    }

    token* tk = create_token();
    tk->contents = s;
    tk->type = IDENTIFIER;
    return tk;
}


token* number(lexer* lx) {
    token* tk = create_token();

    tk->contents = digit(lx);
    tk->type = INTEGER;

    return tk;
}

char* digit(lexer* lx) {
    char* n = calloc(20, sizeof(char*));
    int idx = 0;

    while (lx->ch != '\0' && isdigit(lx->ch)) {
        n[idx] = lx->ch;
        next_chr(lx);
    }

    return n;
}

/*
def hex(self):
    n = ''
    while self.ch and \
        (self.ch in 'ABCDEFabcdef' or self.ch.isdigit()):
        n += self.ch
        self.next_chr()
    return n
    */
