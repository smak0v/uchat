#include "libmx.h"

char *mx_itoa(int number) {
    int length = mx_numlen(number);
    char *str_integer = NULL;

    if ((number != 0) && (number != -2147483648)) {
        str_integer = mx_strnew(length);
        if (number < 0)
            --length;
        for (int i = 0; i < length; ++i) {
            if (number < 0) {
                str_integer[length] = '-';
                number *= -1;
            }
            str_integer[i] = (number % 10) + 48;
            number /= 10;
        }
        mx_str_reverse(str_integer);
        return str_integer;
    }
    return (number == 0) ? mx_strdup("0") : mx_strdup("-2147483648");
}
