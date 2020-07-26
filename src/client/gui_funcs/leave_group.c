#include "client.h"

void mx_leave_group(GtkWidget *w, t_glade *g) {
    char *request = mx_json_string_leave_group(g->token, g->uid, g->dgid);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);

    (void)w;
}
