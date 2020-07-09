#include "client.h"

void mx_load_groups(t_glade *g) {
    char *request = mx_json_string_load_groups(g->token, g->uid);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    g->groups_box = mx_get_gtk_obj(g, "groups_box");

    mx_parse_load_groups_response(response, g);

    mx_strdel(&request);
    mx_strdel(&response);
}
