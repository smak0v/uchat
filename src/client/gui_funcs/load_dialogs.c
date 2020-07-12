#include "client.h"

void mx_load_dialogs(t_glade *g) {
    char *request = mx_json_string_load_dialogs_groups(LOAD_DIALOGUES,
        g->token, g->uid);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_printstr_endl(response);

    g->dialogs_box = mx_get_gtk_obj(g, "dialogs_box");

    mx_parse_load_dialogs_response(response, g);

    mx_strdel(&request);
    mx_strdel(&response);
}
