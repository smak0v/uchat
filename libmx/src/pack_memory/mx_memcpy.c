#include "libmx.h"

void *mx_memcpy(void *restrict dst, const void *restrict src, size_t n) {
    unsigned char *p1 = dst;
    unsigned char *p2 = (unsigned char *)src;

    while (n--)
        *p1++ = *p2++;
    return dst;
}
