#include "client.h"

void mx_delete_account(GtkWidget *w, t_glade *g) {
    char *request = mx_json_string_delete_account(g->token, g->uid);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);

    (void)w;
}
