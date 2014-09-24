#ifndef SEA_H
#define SEA_H

#include "src/parser.h"


typedef struct {
    parser* ps;
} sea;

sea* create_sea();
void free_sea(sea* s);
#endif
