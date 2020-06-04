#include "libmx.h"

void *mx_memmem(const void *big, size_t big_len, const void *little,
                size_t little_len) {
    const char *big_p = big;
    const char *little_p = little;

    if ((big_len < little_len) || (!big_len) || (!little_len))
        return NULL;
    for (size_t i = 0; i < big_len;) {
        if (big_p[i] == little_p[0]) {
            for (size_t j = 1, k = i + 1; j < little_len
			&& big_p[k] == little_p[j]; j++, k++) {
                if (j + 1 == little_len) {
                    big = big_p + k - j;
                    return (void *)big;
                }
            }
        }
        i++;
    }
    return NULL;
}
