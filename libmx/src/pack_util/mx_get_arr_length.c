#include "libmx.h"

int mx_get_arr_length(char **arr) {
    int length = 0;
    char **a = arr;

    while (*a) {
        length++;
        a++;
    }
    return length;
}
