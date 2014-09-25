#ifndef DICT_H
#define DICT_H

#include "src/dict/dict_entry.h"

typedef struct {
    int count;
    int max_size;
    dict_entry** entries;
} dict;

dict* dict_create(int max_size);
void  dict_set(dict* d, char* key, void* value);
void* dict_get(dict* d, char* key);
void* dict_remove(dict* d, char* key);

#endif
