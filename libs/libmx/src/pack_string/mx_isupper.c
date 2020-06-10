#include "libmx.h"

bool mx_isupper(char c) {
    if ((c >= 65) && (c <= 90))
        return true;
    return false;
}
