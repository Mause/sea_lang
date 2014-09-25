#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "src/bool.h"
#include "src/dict/dict.h"
#include "src/dict/dict_entry.h"

static bool okFor(dict* d, char* key, int idx);
static bool overFull(dict* d);
static bool underFull(dict* d);
static double percentageFull(dict* d);
static int find(dict* d, char* key);
static int hash(dict* d, char* key);
static int hash(dict* d, char* key);
static int stepHash(int key);
static void dict_free_entries(dict_entry** entries, int num);
static void put_entry(dict* d, dict_entry* entry);
static void resize(dict* d, int size);
static void resizeIfNeeded(dict* d);

void  dict_set(dict* d, char* key, void* value) {
    dict_entry* entry = dict_create_entry();
    entry->key = key;
    entry->value = value;

    put_entry(d, entry);
    resizeIfNeeded(d);
}


dict* dict_create(int max_size) {
    dict* dc = calloc(1, sizeof(*dc));

    dc->count = 0;
    dc->max_size = max_size;
    dc->entries = calloc(max_size, sizeof(**dc->entries));

    return dc;
}

static void put_entry(dict* d, dict_entry* entry) {
    d->count++;
    entry->state = IN_USE;
    int idx = find(d, entry->key);
    d->entries[idx] = entry;
}

void* dict_get(dict* d, char* key) {
    dict_entry* entry = d->entries[find(d, key)];

    if (entry == NULL || entry->state == PREVIOUS_USED) {
        return NULL;
    } else {
        return entry->value;
    }
}

void* dict_remove(dict* d, char* key) {
    d->count--;
    void* value = dict_get(d, key);
    d->entries[find(d, key)]->state = PREVIOUS_USED;

    resizeIfNeeded(d);

    return value;
}

void dict_free(dict* d) {
    dict_free_entries(d->entries, d->max_size);
    free(d);
}

static void dict_free_entries(dict_entry** entries, int num) {
    int i;

    for (i=0; i<num; i++) {
        if (entries[i] != NULL) {
            free(entries[i]);
        }
    }
}

static bool okFor(dict* d, char* key, int idx) {
    dict_entry* cur_val = d->entries[idx];

    if (cur_val != NULL && cur_val->state == IN_USE && strcmp(cur_val->key, key) == 0) {
        // is already this key
        return TRUE;
    } else if (cur_val != NULL && cur_val->state == PREVIOUS_USED) {
        // has been used in the past, but has been cleared for use now
        return TRUE;
    } else if (cur_val == NULL) {
        // not as yet used
        return TRUE;
    } else {
        // already in use for another key
        return FALSE;
    }
}

static int MAX_STEP = 5;

static int stepHash(int key) {
    return MAX_STEP - (key % MAX_STEP);
}

static int find(dict* d, char* key) {
    int idx = hash(d, key);

    while (!okFor(d, key, idx)) {
        idx = (idx + stepHash(idx)) % d->max_size;
    }

    return idx;
}

bool contains_key(dict* d, char* key) {
    dict_entry* entry = d->entries[find(d, key)];

    if (entry == NULL || entry->state != IN_USE) {
        return FALSE;
    } else if (entry->state == PREVIOUS_USED) {
        return FALSE;
    } else {
        return TRUE;
    }
}

static void resize(dict* d, int size) {
    dict_entry** old_table = d->entries;

    int original_count = d->count,
        original_length = d->max_size,
        i;
    d->count = 0;

    d->entries = calloc(size, sizeof(**d->entries));
    d->max_size = size;

    for (i=0; i<original_length; i++) {
        if (old_table[i] != NULL && old_table[i]->state == IN_USE) {
            put_entry(d, old_table[i]);
        }
    }

    // dict_free_entries(old_table, original_count);

    assert(d->count == original_count);
}

// static int hash(dict* d, char* key) {
//     int hashIdx = 0, length = strlen(key), ii;
//     for (ii = 0; ii < length; ii++) {
//         hashIdx += key[ii];
//     }
//     return hashIdx % d->max_size; // We’ll discuss this line later
// }

static int hash(dict* d, char* key) {
    int hashIdx = 0,
        key_length = strlen(key),
        g, ii;

    for (ii = 0; ii < key_length; ii++) {
        hashIdx = (hashIdx << 4) + key[ii];
        g = hashIdx & 0xF0000000;
        if (g != 0) {
            hashIdx = hashIdx ^ (g >> 24); // ^ is the XOR operator
        }
        hashIdx = hashIdx & ~g;
    }
    return hashIdx % d->max_size;
}

static void resizeIfNeeded(dict* d) {
    int new_size = d->max_size;

    if (overFull(d)) {
        new_size = nextBiggestPrime(d->max_size);
    } else if (underFull(d)) {
        new_size = nextSmallestPrime(d->max_size);
    }

    if (new_size != d->max_size) {
        if (new_size < d->count) {
        } else {
            resize(d, new_size);
        }
    }
}

static double GLASS_HALF_FULL = 60;
static double GLASS_HALF_EMPTY = 30;

static double percentageFull(dict* d) {
    return ((double)d->count) / ((double)d->max_size) * 100.0;
}

static bool overFull(dict* d) {
    return percentageFull(d) > GLASS_HALF_FULL;
}

static bool underFull(dict* d) {
    return percentageFull(d) < GLASS_HALF_EMPTY;
}