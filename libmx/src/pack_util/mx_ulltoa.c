#include "libmx.h"

char *mx_ulltoa(unsigned long long value) {
    int length = mx_unumlen(value);
    char *str_value = NULL;

    if (value) {
        str_value = mx_strnew(length);
        for (int i = 0; i < length; ++i) {
            str_value[i] = (value % 10) + 48;
            value /= 10;
        }
        mx_str_reverse(str_value);
        return str_value;
    }
    return mx_strdup("0");
}
