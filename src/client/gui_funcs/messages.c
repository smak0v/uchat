#include "client.h"

void mx_set_message_style(GtkWidget *msg_v_box, GtkWidget *l_username,
    GtkWidget *l_time, GtkWidget *l_msg) {
    gtk_style_context_add_class(gtk_widget_get_style_context(msg_v_box),
        "msg");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_username),
        "username");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_time), "time");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_msg), "msgtxt");

    gtk_widget_show_all(msg_v_box);
}

void mx_show_hide_chat_group_utils(t_glade *g) {
    gtk_widget_show(g->messages_area);
    gtk_widget_show(g->box_message);
    gtk_widget_show(g->e_search);
    gtk_widget_show(g->box9);

    if (g->group) {
        gtk_widget_show(g->b_group_members);
        gtk_widget_show(g->b_add_user);
        gtk_widget_show(g->b_leave_group);
    }

    gtk_widget_hide(g->l_select_chat);
    gtk_widget_hide(g->profile_area);
}

void mx_set_message_wrap(GtkWidget *l_msg) {
    gtk_label_set_line_wrap(GTK_LABEL(l_msg), TRUE);
    gtk_label_set_max_width_chars(GTK_LABEL(l_msg), 60);
    gtk_label_set_line_wrap_mode(GTK_LABEL(l_msg), PANGO_WRAP_CHAR);
}
