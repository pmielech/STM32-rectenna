/*
 * dev_hash.c
 *
 *  Created on: Oct 2, 2022
 *      Author: patryk
 */
#include "dev/dev_hash.h"


uint32_t djb33_hash(const char* s, size_t len)
{
    uint32_t h = 5381;
    while (len--) {
        /* h = 33 * h ^ s[i]; */
        h += (h << 5);
        h ^= *s++;
    }
    return h;
}
