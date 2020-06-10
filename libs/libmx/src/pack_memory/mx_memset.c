#include "libmx.h"

void *mx_memset(void *b, int c, size_t len) {
    unsigned char *p = b;

    while (len--)
        *p++ = c;
    return b;
}
