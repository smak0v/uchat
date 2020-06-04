#include "libmx.h"

bool mx_isdigit(char c) {
    if ((c >= 48) && (c <= 57))
        return true;
    return false;
}
