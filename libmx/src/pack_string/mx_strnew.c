#include "libmx.h"

char *mx_strnew(const int size) {
    char *target = (char *)malloc((size + 1) * sizeof(char));

    for (int i = 0; i <= size; i++)
        target[i] = '\0';
    return target;
}
