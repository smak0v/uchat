#include "libmx.h"

bool mx_isprime(long int number) {
    int j = mx_sqrt(number);

    for(int i = 2; i <= j; ++i)
        if(number % i == 0)
            return false;

    return true;
}
