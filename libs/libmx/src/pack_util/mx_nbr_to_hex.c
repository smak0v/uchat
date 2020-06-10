#include "libmx.h"

static int get_nbr_length(unsigned long nbr);

char *mx_nbr_to_hex(unsigned long nbr) {
    int i = 0;
    int reminder = 0;
    char *hex_number = NULL;

    if (nbr == 0)
        return mx_strdup("0");
    hex_number = mx_strnew(get_nbr_length(nbr));
    while (nbr) {
        reminder = nbr % 16;
        nbr /= 16;
        if (reminder >= 0 && reminder <= 9)
            hex_number[i] = reminder + 48;
        else if (reminder >= 10 && reminder <= 15)
            hex_number[i] = reminder + 87;
        i++;
    }
    mx_str_reverse(hex_number);
    return hex_number;
}

static int get_nbr_length(unsigned long nbr) {
    unsigned long nbr_copy = nbr;
    int hex_length = 0;

    while (nbr_copy) {
        hex_length++;
        nbr_copy /= 10;
    }
    return hex_length;
}
