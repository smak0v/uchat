#include "libmx.h"

void mx_str_reverse(char *s) {
    int str_len = 0;

    if (s) {
        str_len = mx_strlen(s);
        for (int i = 0, j = str_len - 1; i < str_len / 2; i++, j--)
            mx_swap_char(&s[i], &s[j]);
    }
}
