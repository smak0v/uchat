#include "libmx.h"

int mx_binary_search(char **arr, int size, const char *s, int *count) {
    int start = 0;
    int end = size - 1;
    int middle = 0;

    *count = 0;
    while (start <= end) {
        *count += 1;
        middle = (start + end) / 2;
        if (mx_strcmp(arr[middle], s) > 0)
            end = middle - 1;
        else if (mx_strcmp(arr[middle], s) < 0)
            start = middle + 1;
        else
            return middle;
    }
    *count = 0;
    return -1;
}
