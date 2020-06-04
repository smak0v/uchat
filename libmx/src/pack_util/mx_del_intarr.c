#include "libmx.h"

void mx_del_intarr(int ***array, int length) {
    int **arr = *array;

    for (int i = 0; i < length; i++) {
        free(arr[i]);
        arr[i] = NULL;
    }
    free(arr);
    arr = NULL;
}
