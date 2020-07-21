#include "client.h"

void mx_get_profile(t_glade *g) {
    char *request = mx_json_string_get_profile(g->token, g->uid, g->log);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}
