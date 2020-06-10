#include "libmx.h"

void mx_terminate(char *error_message) {
    mx_print_error(error_message);
    mx_print_error("\n");
    exit(1);
}
