#include "server.h"

FILE *mx_open_file(char *filename, char *mode) {
    FILE *file = NULL;
    struct stat st;
    char *path = mx_strjoin(MX_DOWNLOADS, filename);

    if (stat(MX_DOWNLOADS, &st) == -1)
        mkdir(MX_DOWNLOADS, 0700);
    if (path)
        file = fopen(path, mode);
    mx_strdel(&path);

    return file;
}
