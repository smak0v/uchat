#include "client.h"

void mx_load_groups_request(t_glade *g) {
    char *request = mx_json_string_load_dialogs_groups(LOAD_GROUPS, g->token,
        g->uid);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}
