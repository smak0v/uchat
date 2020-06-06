#include "uchat.h"

bool mx_check_port(char *port) {
    if (!port)
        return false;

    for (size_t i = 0; i < strlen(port); ++i)
        if (!isdigit(port[i]))
            return false;

    if (!strcmp(port, "0"))
        return false;

    return true;
}
