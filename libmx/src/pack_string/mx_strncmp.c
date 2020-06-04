#include "libmx.h"

int mx_strncmp(const char *s1, const char *s2, size_t n) {
    if ((!s1) && (!s2))
        return 0;
    if (!s1)
        return *s2;
    if (!s2)
        return *s1;
    for (size_t i = 0; i < n; i++)
        if (s1[i] != s2[i])
            return s1[i] - s2[i];
    return 0;
}
