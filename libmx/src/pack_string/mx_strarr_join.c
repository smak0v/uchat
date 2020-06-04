#include "libmx.h"

char **mx_strarr_join(char **arr, char **push) {
    char **new_arr = mx_strarr_dup(arr);

    if (push && *push) {
        int arr_len = mx_strarr_len(arr);
        int new_len = mx_strarr_len(push);
        int i = arr_len;

        new_arr = realloc(new_arr, sizeof(char *)
                          * (arr_len + new_len + 1));
        for (int j = 0; i < arr_len + new_len; ++j)
            new_arr[i++] = mx_strdup(push[j]);
        new_arr[i] = NULL;
    }
    return new_arr;
}
