#include "libmx.h"

bool mx_check_set(char **set, char *element, int set_size) {
    int i = 0;

    while (i <= set_size) {
        if (set[i] == NULL)
            return false;
        if (mx_strcmp(set[i], element) == 0)
            return true;
        i++;
    }
    return false;
}
