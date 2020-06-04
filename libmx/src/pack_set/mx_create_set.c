#include "libmx.h"

char **mx_create_set(char **arr) {
    int count = mx_count_unequal(arr) + 1;
    char **set = (char **)malloc(sizeof(char *) * count);
    int j = 0;

    for (int i = 0; i < count; i++)
        set[i] = NULL;
    for (int i = 0; i < mx_get_arr_length(arr); i++)
        if (!mx_check_set(set, arr[i], count))
            set[j++] = mx_strdup(arr[i]);
    set[j] = NULL;
    return set;
}
