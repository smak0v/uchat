#include "uchat.h"

static bool check_number(char *str) {
    for (size_t i = 0; i < strlen(str); ++i)
        if (!isdigit(str[i]))
            return false;

    return true;
}

static bool check_ip(char *ip) {
    int dots = 0;
    char *ptr = strtok(ip, ".");

    if (!ptr)
        return false;

    while (ptr) {
        if (!check_number(ptr))
            return false;
        if (atoi(ptr) >= 0 && atoi(ptr) <= 255) {
            ptr = strtok(NULL, ".");
            if (ptr)
                dots++;
        }
        else
            return false;
    }

    return dots != 3 ? false : true;
}

bool mx_check_ip(char *ip) {
    bool checked = false;

    if (!ip)
        return false;

    if (!strcmp(ip, "localhost")) {
        mx_strdel(&ip);
        return true;
    }

    checked = check_ip(ip);

    mx_strdel(&ip);

    return checked;
}
