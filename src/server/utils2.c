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

char *mx_add_dialog_name(sqlite3 *db, int code, char *j_str, int uid) {
    t_user *u = NULL;

    if (code) {
        u = mx_get_user_by_user_id(db, uid);
        if (!u)
            return mx_json_string_code_type(500, S_MES);
        j_str = mx_add_field(j_str, 1, "dnme", (void *)u->user_login);
    }

    return j_str;
}
