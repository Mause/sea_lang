#include <stdlib.h>

#include "src/dict/dict_entry.h"


dict_entry* dict_create_entry(void) {
    dict_entry* de = calloc(1, sizeof(*de));

    de->key = NULL;
    de->value = NULL;
    de->state = 0;

    return de;
}
