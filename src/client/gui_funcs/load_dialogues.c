#include "client.h"

void mx_load_dialogues_request(t_glade *g) {
    char *request = mx_json_string_load_dialogs_groups(LOAD_DIALOGUES,
        g->token, g->uid);

    SSL_write(g->ssl, request, strlen(request));

    g->dialogs_box = mx_get_gtk_obj(g, "dialogs_box");

    mx_strdel(&request);
}
