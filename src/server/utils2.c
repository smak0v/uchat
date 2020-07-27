#include "server.h"

void mx_process_new_ssl(SSL_CTX *ctx, int connect_fd, t_meta *trd_data) {
    SSL *ssl = SSL_new(ctx);
    t_list *ssl_list = NULL;

    SSL_set_fd(ssl, connect_fd);
    if (!trd_data->ssl_list) {
        ssl_list = mx_create_node((void *)ssl);
        trd_data->ssl_list = &ssl_list;
    }
    else
        mx_push_back(trd_data->ssl_list, ssl);

    trd_data->ssl = ssl;
}
