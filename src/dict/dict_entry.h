#ifndef DICT_ENTRY_H
#define DICT_ENTRY_H

enum dict_entry_state {
    PREVIOUS_USED, IN_USE
};

typedef struct {
    char* key;
    void* value;
    int state;
} dict_entry;

dict_entry* dict_create_entry();

#endif
