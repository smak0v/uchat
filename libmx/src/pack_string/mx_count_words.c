#include "libmx.h"

static int get_word_length(const char *str, char c) {
    int word_length = 0;
    char *copy_str = (char *)str;

    while ((*copy_str != c) && (*copy_str != '\0')) {
        word_length++;
        copy_str++;
    }
    return word_length;
}

int mx_count_words(const char *str, char c) {
    int words_count = 0;
    char *s = (char *)str;

    if (!s)
        return -1;
    while ((*s) && (*s != '\0')) {
        if (*s == c) {
            s++;
            continue;
        }
        words_count++;
        s += get_word_length(s, c);
    }
    return words_count;
}
