#include "libmx.h"

static void set_rem(char **rem, int fd, char *buffer, int count) {
    char *tmp = NULL;

    buffer[count] = '\0';
    tmp = mx_strjoin(rem[fd], buffer);
    mx_strdel(&rem[fd]);
    rem[fd] = tmp;
}

static int copy_before_delim(char **ptr, int delim, char **rem, int fd) {
    char *tmp = NULL;

    *ptr = mx_strndup(rem[fd], delim);
    tmp = mx_strdup(rem[fd] + delim + 1);
    mx_strdel(&rem[fd]);
    rem[fd] = tmp;
    return delim;
}

static int copy_end(char **lineptr, int fd, char **rem) {
    *lineptr = mx_strdup(rem[fd]);
    mx_strdel(&rem[fd]);
    return -1;
}

int mx_read_line(char **lineptr, size_t buf_size, char delim, const int fd) {
    static char *rem[255];
    char buffer[buf_size];
    int index = 0;
    int count = 0;

    if ((fd < 0) || (fd > 255))
        return -2;
    if (!rem[fd])
        rem[fd] = mx_strnew(0);
    while ((count = read(fd, buffer, buf_size)) > -1) {
        set_rem(rem, fd, buffer, count);
        index = mx_get_char_index(rem[fd], delim);
        if ((index > -1) && (count > 0))
            break;
        if ((index > -1) && (count == 0))
            return copy_before_delim(lineptr, index, rem, fd);
        if ((index == -1) && (count == 0))
            return copy_end(lineptr, fd, rem);
    }
    return (count == -1) ? -2 : copy_before_delim(lineptr, index, rem, fd);
}
