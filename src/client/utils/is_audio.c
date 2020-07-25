#include "client.h"

bool mx_is_audio(char *filename) {
    char *type = mx_memrchr(filename, '.', strlen(filename));

    if (!strcmp(type + 1, "aif"))
        return true;

    return false;
}
