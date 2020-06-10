#include "libmx.h"

char *mx_strncpy(char *dst, const char *src, int len) {
    char *tmp = dst;

    while ((len > 0) && (*src != '\0')) {
        *tmp++ = *src++;
        --len;
    }
    while (len > 0) {
        *tmp++ = '\0';
        --len;
    }
    return dst;
}
