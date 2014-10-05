#include <stdio.h>
#include <assert.h>

#include "src/dict/dict.h"
#include "src/bool.h"

int testBasic(void) {
    dict* table = dict_create(100);

    dict_set(table, "World", "Hello");

    bool res = strcmp(
        dict_get(table, "World"),
        "Hello"
    ) == 0;

    dict_free(table);

    return res;
}

int testRemoveValue(void) {
    dict* table = dict_create(20);

    dict_set(table, "World", "Hello");

    dict_remove(table, "World");

    bool res = dict_get(table, "World") == NULL;

    dict_free(table);

    return res;
}

int testResizeLarger(void) {
    dict* table = dict_create(3);

    dict_set(table, "World", "Hello");
    dict_set(table, "Worle", "Hello");

    // "the bucket number should have increased since the table was initialized",
    bool res = table->max_size > 3;

    dict_free(table);

    return res;
}

int testResizeSmaller(void) {
    dict* table = dict_create(7);

    dict_set(table, "Hello", "World");

    // "the bucket number should have decreased since the table was initialized",
    bool res = table->max_size < 7;

    dict_free(table);

    return res;
}

int testContainsKey(void) {
    bool res = TRUE;
    dict* table = dict_create(7);

    // "An empty hashtable shouldn't contain the key",
    res = res && (!contains_key(table, "world"));

    dict_set(table, "world", "hello");

    // "hashtable should contain a key after it has been put in",
    res = res && contains_key(table, "world");

    dict_remove(table, "world");

    // "hashtable shouldn't contain a key that has been removed",
    res = res && (!contains_key(table, "world"));

    dict_free(table);

    return res;
}

#define test(func, name) \
    printf("%s running\n-----------------------------\n\n", name);\
    assert(func());\
    printf("%s succeeded\n-----------------------------\n\n", name); \

int main(int argc, char const *argv[]) {
    test(testBasic, "testBasic");
    test(testRemoveValue, "testRemoveValue");
    test(testContainsKey, "testContainsKey");
    test(testResizeLarger, "testResizeLarger");
    test(testResizeSmaller, "testResizeSmaller");

    /* code */
    return 0;
}
