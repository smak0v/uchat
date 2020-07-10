#include "client.h"

void mx_load_messages(t_glade *g, int duid, bool group, time_t timestamp) {
    char *request = mx_json_string_load_messages(g, timestamp, duid, group);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_parse_load_messages_response(response, g);

    mx_strdel(&request);
    mx_strdel(&response);
}
