#include "libmx.h"

int mx_sqrt(int x) {
    int start = 1;
    int end = x / 2;
    int middle = 0;

    if (x <= 0)
        return 0;
    if (x == 1)
        return x;
    if (x / 2 > 46340)
        end = 46340;
    while (start <= end) {
        middle = (start + end) / 2;
        if (middle * middle == x)
            return middle;
        if (middle * middle < x)
            start = middle + 1;
        else
            end = middle - 1;
    }
    return 0;
}
