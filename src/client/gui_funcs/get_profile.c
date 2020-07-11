#include "client.h"

void mx_get_profile(t_glade *g) {
    char *request = mx_json_string_search_user(g->token, g->uid, g->log);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_printstr_endl(request);
    mx_printstr_endl("profile");
    mx_printstr_endl(response);

    mx_strdel(&request);
    mx_strdel(&response);
}
