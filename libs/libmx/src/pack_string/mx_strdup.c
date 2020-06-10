#include "libmx.h"

char *mx_strdup(const char *str) {
    char *target = mx_strnew(mx_strlen(str));

    return mx_strcpy(target, str);
}
