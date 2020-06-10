#include "libmx.h"

void mx_printchar_ntimes(char ch, int count) {
    for (int i = 0; i < count; ++i)
        mx_printchar(ch);
}
