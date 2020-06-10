#include "libmx.h"

void mx_print_error(const char *error) {
    write(2, error, mx_strlen(error));
}
