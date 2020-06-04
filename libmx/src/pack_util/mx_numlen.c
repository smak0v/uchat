#include "libmx.h"

int mx_numlen(long long value) {
    int length = 0;
    int copy_value = value;

    while (copy_value) {
        ++length;
        copy_value /= 10;
    }
    return value < 0 ? length + 1 : length;
}
