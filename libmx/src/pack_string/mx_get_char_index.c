#include "libmx.h"

int mx_get_char_index(const char *str, char c) {
    if (str) {
        for (int i = 0; i < mx_strlen(str); i++)
            if (str[i] == c)
                return i;
        return -1;
    }
    return -2;
}

int mx_get_char_index_r(const char *str, char c) {
    if (str) {
        for (int i = mx_strlen(str) - 1; i >= 0; --i)
            if (str[i] == c)
                return i;
        return -1;
    }
    return -2;
}
