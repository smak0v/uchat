#include "client.h"

void mx_load_messages_request(t_glade *g, time_t timestamp) {
    char *request = mx_json_string_load_messages(g, timestamp, g->dgid,
        g->group);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}
