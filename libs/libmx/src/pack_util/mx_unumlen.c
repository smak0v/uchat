#include "libmx.h"

int mx_unumlen(unsigned long long value) {
    int length = 0;

    while (value) {
        ++length;
        value /= 10;
    }
    return length;
}
