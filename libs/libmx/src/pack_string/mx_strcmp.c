#include "libmx.h"

int mx_strcmp(const char *s1, const char *s2) {
    int i = 0;

    for (; (s1[i] != '\0') || (s2[i] != '\0'); i++)
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
    return 0;
}
