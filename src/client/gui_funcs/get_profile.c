#include "client.h"

void mx_get_profile(t_glade *g, bool current) {
    char *request = mx_json_string_search_user(g->token, g->uid, g->log);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_parse_get_profile_response(response, g, current);

    mx_strdel(&request);
    mx_strdel(&response);
}
