#include "client.h"

static void add_message_to_gui(t_glade *g, char *response) {

    (void)response;
    (void)g;
}

static t_msg *build_msg(t_glade *g, char *user_message) {
    t_msg *msg = mx_memalloc(sizeof(t_msg));

    msg->group = g->group;
    msg->dgid = g->dgid;
    msg->uid2 = g->uid2;
    msg->msg = user_message;
    msg->time = time(NULL);

    return msg;
}

void mx_send_msg(GtkWidget *w, t_glade *g) {
    char *user_message = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_message));
    t_msg *msg = NULL;
    char *request = NULL;
    char *response = NULL;

    if (!user_message || strlen(user_message) <= 0)
        return;
    msg = build_msg(g, user_message);
    request = mx_json_string_send_message(g, msg);
    SSL_write(g->ssl, request, strlen(request));
    response = mx_read_server_response(g);
    if (!mx_parse_send_message_response(response))
        add_message_to_gui(g, response);

    gtk_entry_set_text(GTK_ENTRY(g->e_message), "");
    free(msg);
    msg = NULL;
    mx_strdel(&request);
    mx_strdel(&response);
    (void)w;
}
