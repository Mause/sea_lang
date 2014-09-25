#include <assert.h>

static int PRIMES[] = {
    1, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,

    // what follows are the apparently optimal primes
    53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317,
    196613, 393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
    50331653, 100663319, 201326611, 402653189, 805306457, 1610612741
};
#define PRIME_NUM 42

int nextBiggestPrime(int old) {
    int i;
    for (i=0; i<PRIME_NUM; i++){
        if (PRIMES[i] > old) return PRIMES[i];
    }

    return PRIMES[PRIME_NUM-1];
}

int nextSmallestPrime(int old) {
    int next_smallest = 0, i;

    // for (int prime : PRIMES) {
    for (i=0; i<PRIME_NUM; i++) {
        if (PRIMES[i] < old) {
            next_smallest = PRIMES[i];
        }
    }

    assert(next_smallest != 0);

    return next_smallest;
}
