#include "libmx.h"

int mx_ctoi(char c) {
    if (mx_isdigit(c))
        return c - '0';
    return -1;
}
