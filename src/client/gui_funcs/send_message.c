#include "client.h"

// gtk_box_set_baseline_position!!!!!

static void add_message_to_gui(t_glade *g, t_msg *msg) {
    GtkWidget *l_msg = gtk_label_new(msg->msg);
    GtkWidget *l_username = gtk_label_new("username");

    char buff[20];
    time_t time = msg->time;
    strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&time));
    GtkWidget *l_time = gtk_label_new(buff);

    GtkWidget *msg_v_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *u_t_h_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *msg_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    gtk_label_set_xalign(GTK_LABEL(l_username), 0.0);
    gtk_label_set_xalign(GTK_LABEL(l_time), 1);
    gtk_label_set_xalign(GTK_LABEL(l_msg), 0.0);
    gtk_box_pack_end(GTK_BOX(g->messages_area), msg_v_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), u_t_h_box, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(msg_v_box), msg_vbox, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(u_t_h_box), l_username, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(u_t_h_box), l_time, FALSE, FALSE, 0);
    gtk_box_pack_end(GTK_BOX(msg_vbox), l_msg, FALSE, FALSE, 0);

    gtk_widget_set_halign(GTK_WIDGET(msg_v_box), GTK_ALIGN_END);

    gtk_style_context_add_class(gtk_widget_get_style_context(msg_v_box), "msg");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_username), "username");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_time), "time");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_msg), "msgtxt");
    gtk_widget_show_all(msg_v_box);
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
