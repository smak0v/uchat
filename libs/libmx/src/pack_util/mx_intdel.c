#include "libmx.h"

void mx_intdel(int **arr) {
    free(*arr);
    *arr = NULL;
}
