#include <stdlib.h>
#include <string.h>

#include "src/lexer.h"

typedef int bool;
#define TRUE 1
#define FALSE !TRUE

void* value(lexer* lx);
void* word(lexer* lx);

lexer* create_lexer() {
    lexer* lx = malloc(sizeof(*lx));

    lx->token_callback = NULL;

    lx->text = calloc(1024, sizeof(char*));
    lx->ch = NULL;
    lx->at = 0;
    lx->len = 0;
    lx->depth = 0;

    return lx;
}

void* lexer_feedline(lexer* lx, char* line) {
    // FIXME: only short comments removed
    // reg = re.compile('--.*$', re.M)
//    text = reg.sub('', text, 0)
    lx->text = line;
    lx->at = 0;
    lx->ch = NULL;
    lx->depth = 0;
    lx->len = strlen(lx->text);
    next_chr(lx);
    return value(lx);
}

void white(lexer* lx) {
    while (lx->ch != NULL) {
        if (isspace(lx->ch)) {
            next_chr(lx);
        } else {
            break;
        }
    }
}

int next_chr(lexer* lx) {
    if (lx->at >= lx->len) {
        lx->ch = NULL;
        return FALSE;
    }

    lx->ch = lx->text[lx->at];
    lx->ch[1] = 
    lx->at++;
    return TRUE;
}


void* value(lexer* lx) {
    white(lx);
    if (lx->ch == NULL) {
        return NULL;
    }

    if (strcmp(lx->ch, "{") == 0) {
//        return object(lx);
    }

    if (strcmp(lx->ch, "[") == 0) {
        next_chr(lx);
    }

/*    if (lx->ch in ['"',  "'",  '[']) {
        return string(lx, lx->ch);
    }*/

    if (isdigit(lx->ch) || (strcmp(lx->ch, "-") == 0)) {
//        return number(lx);
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

void* word(lexer* lx) {
    char* s = "";

    if (strcmp(lx->ch, "\n") != 0) {
        s = lx->ch;
    }

    while (next_chr(lx)) {
        if (isalnum(lx->ch)) {
            strcat(s, lx->ch);
        } else {
                   if (strcmp(s, "true") == 0) {
                return TRUE;
            } else if (strcmp(s, "false") == 0) {
                return FALSE;
            }
/*                    else if s == 'nil':
                return NULL;
            }*/
            return s;
        }
    }
}


/*
void* number(lexer* lx):
    def next_digit(err):
        n = self.ch
        self.next_chr()
        if not self.ch or not self.ch.isdigit():
            raise ParseError(err)
        return n
    n = ''
    try:
        if self.ch == '-':
            n += next_digit(ERRORS['mfnumber_minus'])
        n += self.digit()
        if n == '0' and self.ch in ['x', 'X']:
            n += self.ch
            self.next_chr()
            n += self.hex()
        else:
            if self.ch and self.ch == '.':
                n += next_digit(ERRORS['mfnumber_dec_point'])
                n += self.digit()
            if self.ch and self.ch in ['e', 'E']:
                n += self.ch
                self.next_chr()
                if not self.ch or self.ch not in ('+', '-'):
                    raise ParseError(ERRORS['mfnumber_sci'])
                n += next_digit(ERRORS['mfnumber_sci'])
                n += self.digit()
    except ParseError as e:
        print e
        return 0
    try:
        return int(n, 0)
    except:
        pass
    return float(n)

def digit(self):
    n = ''
    while self.ch and self.ch.isdigit():
        n += self.ch
        self.next_chr()
    return n

def hex(self):
    n = ''
    while self.ch and \
        (self.ch in 'ABCDEFabcdef' or self.ch.isdigit()):
        n += self.ch
        self.next_chr()
    return n
    */
