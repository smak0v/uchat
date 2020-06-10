#include "libmx.h"

char *mx_lltoa(long long value) {
    int length = mx_numlen(value);
    char *str_value = NULL;

    if ((value != 0) && (value != (-9223372036854775807LL - 1))) {
        str_value = mx_strnew(length);
        if (length < 0)
            --length;
        for (int i = 0; i < length; ++i) {
            if (value < 0) {
                str_value[length] = '-';
                value *= -1;
            }
            str_value[i] = (value % 10) + 48;
            value /= 10;
        }
        mx_str_reverse(str_value);
        return str_value;
    }
    return (value == 0) ? mx_strdup("0") : mx_strdup("-9223372036854775808");
}
