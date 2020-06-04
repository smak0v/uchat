#include "libmx.h"

int mx_count_unequal(char **arr) {
    int unequal_count = 0;
    int count = 0;

    for (int i = 0; i < mx_get_arr_length(arr); i++) {
        unequal_count++;
        for (int j = i + 1; j < mx_get_arr_length(arr); j++)
            if (mx_strcmp(arr[j], arr[i]) == 0) {
                count++;
                break;
            }
    }
    return unequal_count - count;
}
