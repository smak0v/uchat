#include "libmx.h"

int mx_count_substr(const char *str, const char *sub) {
    const char *s = str;
    int count = 0;

    if ((!str) || (!sub))
        return -1;
    for (s = mx_strstr(s, sub); s; s = mx_strstr(s + mx_strlen(sub), sub))
        count++;
    return count;
}
