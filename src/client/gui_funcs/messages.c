#include "client.h"

void mx_set_message_style(GtkWidget *msg_v_box, GtkWidget *l_username,
    GtkWidget *l_time, GtkWidget *l_msg) {
    gtk_style_context_add_class(gtk_widget_get_style_context(msg_v_box),
        "msg");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_username),
        "username");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_time), "time");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_msg), "msgtxt");

    gdk_threads_add_idle(mx_show_all_widget, msg_v_box);
}

void mx_show_hide_chat_group_utils(t_glade *g) {
    gdk_threads_add_idle(mx_show_widget, g->messages_area);
    gdk_threads_add_idle(mx_show_widget, g->box_message);
    gdk_threads_add_idle(mx_show_widget, g->e_search);
    gdk_threads_add_idle(mx_show_widget, g->box9);

    if (g->group)
        gdk_threads_add_idle(mx_show_widget, g->b_add_user);

    gdk_threads_add_idle(mx_hide_widget, g->l_select_chat);
    gdk_threads_add_idle(mx_hide_widget, g->profile_area);
}
