#include "libmx.h"

void *mx_memchr(const void *s, int c, size_t n) {
    unsigned char *ptr = (unsigned char *)s;

    while ((n--) && (*ptr)) {
        if (*ptr == c)
            return ptr;
        ptr++;
    }
    return NULL;
}
