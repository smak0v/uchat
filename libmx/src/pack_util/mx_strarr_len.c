#include "libmx.h"

int mx_strarr_len(char **strarr) {
    int len = 0;

    if (!strarr)
        return len;
    while (strarr[len])
        len++;
    return len;
}
