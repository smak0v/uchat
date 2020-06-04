#include "libmx.h"

char **mx_push_to_strarr(char **strarr, char *str) {
    int len = mx_strarr_len(strarr) + 1;
    char **new_strarr = realloc(strarr, sizeof(char *) * len + 1);

    new_strarr[len - 1] = mx_strdup(str);
    new_strarr[len] = NULL;

    return new_strarr;
}
