#include "client.h"

static void add_message_to_gui() {

}

static t_msg *build_msg(t_glade *g) {
    t_msg *msg = mx_memalloc(sizeof(t_msg));

    msg->group = g->group;
    msg->dgid = g->dgid;
    msg->uid2 = g->uid2;
    msg->msg = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_message));
    msg->time = time(NULL);

    return msg;
}

void mx_send_msg(GtkWidget *w, t_glade *g) {
    t_msg *msg = build_msg(g);
    char *request = mx_json_string_send_message(g, msg);
    char *response = NULL;

    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);

    mx_printstr_endl(request);
    mx_printstr_endl(response);

    if (!mx_parse_send_message_response(response)) {
        add_message_to_gui();
    }

    gtk_entry_set_text(GTK_ENTRY(g->e_message), "");

    free(msg);
    msg = NULL;
    mx_strdel(&request);
    mx_strdel(&response);

    (void)w;
}
