#include "libmx.h"

char **mx_strarr_dup(char **strarr) {
    if (!strarr || !(*strarr))
        return NULL;

    int strarr_len = mx_get_arr_length(strarr);
    char **new_arr = malloc(sizeof(char *) * (strarr_len + 1));
    int i = 0;

    while (strarr[i]) {
        new_arr[i] = mx_strdup(strarr[i]);
        i++;
    }

    new_arr[i] = NULL;
    return new_arr;
}
