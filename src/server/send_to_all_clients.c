#include "server.h"

void mx_send_to_all_clients(t_comm *connect, char *j_str, int uid) {
    int len = 0;
    int *sockets = mx_parse_sock_str(connect->db, uid, &len);
    SSL *ssl = NULL;

    for (int i = 0; i < len; ++i) {
        if (sockets[i] != connect->fd) {
            ssl = mx_find_ssl(connect->ssl_list, sockets[i]);
            SSL_write(ssl, j_str, (sizeof(char) * strlen(j_str)));
        }
    }
}
