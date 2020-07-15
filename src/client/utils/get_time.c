#include "client.h"

char *mx_get_time(time_t time) {
    char *buff = mx_strnew(20);

    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));

    return buff;
}
