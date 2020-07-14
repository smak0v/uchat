#include "server.h"

static int count_commas(char *sock) {
    int counter = 0;

    for (int i = 0; sock[i]; i++) {
        if (sock[i] == ',')
            counter++;
    }
    counter += 1;
    return counter;
}

static int *parse_sock_str(char *sock, int *sock_array, int len) {
    int atoi = 0;
    int arr_counter = 0;
    int j = 0;

    for (int i = 0; arr_counter < len;) {
        while (sock[j] && sock[j] != ',')
            j++;
        if (j != i) {
            char str[j - i + 1];

            mx_strncpy(str, &sock[i], j - i);
            str[j - i] = '\0';
            atoi = mx_atoi(str);
            sock_array[arr_counter] = atoi;
            arr_counter++;
            i += (j - i + 1);
            j += 1;
        }
    }
    return sock_array;
}

int *mx_parse_sock_str(sqlite3 *db, int uid, int *len) {
    char *sock = mx_get_sock_by_user_id(db, uid);
    int *sock_array = NULL;

    if (!sock || sock[0] == '\0')
        return NULL;

    *len = count_commas(sock);
    sock_array = malloc(sizeof(int) * (*len));

    if (*len == 1) {
        sock_array[0] = mx_atoi(sock);
        return sock_array;
    }
    else
        return parse_sock_str(sock, sock_array, *len);

    return sock_array;
}

char *mx_add_socket(char *sock, int fd) {
    char *tmp = NULL;
    char *itoa_str = NULL;

    if (!sock)
        return mx_itoa(fd);
    else if (sock[0] == '\0')
        return NULL;
    else {
        tmp = mx_strjoin(sock, ",");
        mx_strdel(&sock);
        itoa_str = mx_itoa(fd);
        sock = mx_strjoin(tmp, itoa_str);
        mx_strdel(&itoa_str);
        mx_strdel(&tmp);
        tmp = mx_strjoin(sock, "\0");
        mx_strdel(&sock);

        return tmp;
    }
}
