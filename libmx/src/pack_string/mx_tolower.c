#include "libmx.h"

int mx_tolower(int c) {
    if (mx_isupper(c))
        return c + 32;
    return c;
}
