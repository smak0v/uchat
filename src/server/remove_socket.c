#include "server.h"

static char *process_strarr(char **split, int fd) {
    char *sock = NULL;

    for (int i = 0; split[i]; i++) {
        if (fd == mx_atoi(split[i]))
            continue;
        sock = mx_add_socket(sock, mx_atoi(split[i]));
    }

    return sock;
}

char *mx_remove_socket(sqlite3 *db, int fd, int uid) {
    char *sock = mx_get_sock_by_user_id(db, uid);
    char **split = NULL;

    if (!sock || sock[0] == '\0')
        return NULL;

    split = mx_strsplit(sock, ',');
    mx_strdel(&sock);
    sock = process_strarr(split, fd);
    mx_del_strarr(&split);

    if (!sock)
        mx_delete_sock_by_user_id(db, uid);
    else
        mx_update_socket_by_user_id(db, sock, uid);
    return sock;
}
// i should check for a mistake in the last lines and handle it but idgaf

