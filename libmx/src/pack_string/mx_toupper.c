#include "libmx.h"

int mx_toupper(int c) {
    if (mx_islower(c))
        return c - 32;
    return c;
}
