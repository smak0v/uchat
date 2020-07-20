#include "server.h"

void mx_send_to_all_clients(sqlite3 *db, char *j_str, int uid, SSL *ssl) {
    int len = 0;
    int *sockets = mx_parse_sock_str(db, uid, &len);
    int fd = SSL_get_fd(ssl);;

    for (int i = 0; i < len; ++i) {
        SSL_set_fd(ssl, sockets[i]);
        SSL_write(ssl, j_str, (sizeof(char) * strlen(j_str)));
    }

    SSL_set_fd(ssl, fd);
}
