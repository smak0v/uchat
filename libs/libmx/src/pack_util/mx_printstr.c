#include "libmx.h"

void mx_printstr(const char *s) {
    if (s)
        write(1, s, mx_strlen(s));
}

void mx_printstrn(const char *s, int n) {
    if (s)
        write(1, s, n);
}
