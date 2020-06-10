#include "libmx.h"

static int get_str_length_without_extra_spaces(const char *str) {
    int length = 0;
    int str_lenght = mx_strlen(str);

    for (int i = 0; i < str_lenght; i++) {
        if (mx_isspace(str[i])) {
            length++;
            while (mx_isspace(str[i]))
                i++;
        }
        if (!mx_isspace(str[i]))
            length++;
    }
    return length;
}

static void skip_spaces(char *new_str, char *trimmed_str, int *j, int *i) {
    new_str[*j] = ' ';
    (*j)++;
    while (mx_isspace(trimmed_str[*i]))
        (*i)++;
}

char *mx_del_extra_spaces(const char *str) {
    char *new_str = NULL;
    char *trimmed_str = NULL;

    if (str) {
        trimmed_str = mx_strtrim(str);
        new_str = mx_strnew(get_str_length_without_extra_spaces(trimmed_str));
        for (int i = 0, j = 0; i < mx_strlen(trimmed_str); ) {
            if (!mx_isspace(trimmed_str[i])) {
                new_str[j] = trimmed_str[i];
                i++;
                j++;
            }
            if (mx_isspace(trimmed_str[i]))
                skip_spaces(new_str, trimmed_str, &j, &i);
        }
        mx_strdel(&trimmed_str);
    }
    return new_str;
}
