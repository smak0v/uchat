#include "libmx.h"

void *mx_memmove(void *dst, const void *src, size_t len) {
    unsigned char *p1 = dst;
    unsigned char *p2 = (unsigned char *)src;
    char *buffer = (char *)malloc(len * sizeof(char));

    mx_memcpy(buffer, p2, len);
    mx_memcpy(p1, buffer, len);
    free(buffer);
    return dst;
}
