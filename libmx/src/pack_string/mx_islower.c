#include "libmx.h"

bool mx_islower(char c) {
    if ((c >= 97) && (c <= 122))
        return true;
    return false;
}
