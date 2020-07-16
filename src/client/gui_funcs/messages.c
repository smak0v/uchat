#include "client.h"

void mx_set_message_style(GtkWidget *msg_v_box, GtkWidget *l_username,
    GtkWidget *l_time, GtkWidget *l_msg) {
    gtk_style_context_add_class(gtk_widget_get_style_context(msg_v_box),
        "msg");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_username),
        "username");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_time), "time");
    gtk_style_context_add_class(gtk_widget_get_style_context(l_msg), "msgtxt");

    g_idle_add(mx_show_all_widget, msg_v_box);
}
