#include "libmx.h"

void mx_print_error_endl(const char *error) {
    char endl = '\n';

    write(2, error, mx_strlen(error));
    write(2, &endl, 1);
}
