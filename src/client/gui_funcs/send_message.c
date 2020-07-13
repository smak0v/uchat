#include "client.h"

static void add_message_to_gui(t_glade *g, t_msg *msg) {
    GtkWidget *event_box = gtk_event_box_new();
    GtkWidget *msg_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *l_msg = gtk_label_new(msg->msg);

    gtk_container_add(GTK_CONTAINER(event_box), msg_box);
    gtk_label_set_line_wrap(GTK_LABEL(l_msg), TRUE);
    gtk_box_pack_end(GTK_BOX(event_box), l_msg, FALSE, FALSE, 0);

    gtk_style_context_add_class(gtk_widget_get_style_context(msg_box), "msg");

    gtk_widget_realize(event_box);
    gtk_widget_add_events(event_box, GDK_BUTTON_PRESS_MASK);
    gtk_widget_show_all(event_box);
    (void)g;
}

static t_msg *build_msg(t_glade *g, char *user_message) {
    t_msg *msg = mx_memalloc(sizeof(t_msg));

    msg->group = g->group;
    msg->dgid = g->dgid;
    msg->uid2 = g->uid2;
    msg->msg = user_message;
    msg->time = time(NULL);
    msg->file = g->filename;

    return msg;
}

void mx_send_msg(GtkWidget *w, t_glade *g) {
    char *user_message = mx_get_input_text(g);
    t_msg *msg = NULL;
    char *request = NULL;
    char *response = NULL;

    msg = build_msg(g, user_message);
    // if (!user_message || strlen(user_message) <= 0))
    //     return;
    request = mx_json_string_send_message(g, msg);
    SSL_write(g->ssl, request, strlen(request));
    if (g->filename)
        mx_send_file(g->ssl, g->filename);
    response = mx_read_server_response(g);
    mx_printstr_endl(request);
    mx_printstr_endl(response);
    if (!mx_parse_send_message_response(response))
        add_message_to_gui(g, msg);
    mx_clear_input_text(g);
    free(msg);
    msg = NULL;
    mx_strdel(&user_message);
    mx_strdel(&request);
    mx_strdel(&response);
    mx_strdel(&g->filename);
    (void)w;
}
