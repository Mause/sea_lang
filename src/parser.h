#ifndef PARSER_H
#define PARSER_H

#include "src/lexer.h"

typedef struct {
    lexer* lx;
} parser;

parser* create_parser();

#endif
