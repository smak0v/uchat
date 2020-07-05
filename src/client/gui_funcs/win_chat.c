#include "client.h"

void mx_send_msg(GtkWidget *w, t_glade *g) {
    (void)w;

    g++;
}

void mx_create_win_chat(t_glade *g) {
    g->w_chat = mx_get_gtk_obj(g, "win_chat");
    g->b_logout = mx_get_gtk_obj(g, "b_logout");
    g->b_send_msg = mx_get_gtk_obj(g, "b_send_msg");
    g->b_username = mx_get_gtk_obj(g, "b_username");
    g->common_area = mx_get_gtk_obj(g, "common_area");
    g->messages_area = mx_get_gtk_obj(g, "messages_area");
    g->profile_area = mx_get_gtk_obj(g, "profile_area");
    g->box_message = mx_get_gtk_obj(g, "box_message");
    g->b_close_profile = mx_get_gtk_obj(g, "b_close_profile");
    g->e_search = mx_get_gtk_obj(g, "e_search");
    g->i_search = mx_get_gtk_obj(g, "i_search");
    g->b_attach_file = mx_get_gtk_obj(g, "b_attach_file");

    g_signal_connect(g->w_chat, "destroy", G_CALLBACK(mx_gtk_quit), NULL);
    g_signal_connect(g->b_send_msg, "clicked", G_CALLBACK(mx_send_msg), g);
    g_signal_connect(g->b_logout, "clicked", G_CALLBACK(mx_b_logout), g);
    g_signal_connect(g->b_username, "clicked", G_CALLBACK(mx_open_profile), g);
    g_signal_connect(g->b_close_profile, "clicked",
        G_CALLBACK(mx_close_profile), g);
}

void mx_show_win_chat(GtkWidget *v, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    gtk_window_get_position(GTK_WINDOW(v), &x, &y);
    gtk_window_get_size(GTK_WINDOW(v), &w, &h);
    gtk_widget_hide(GTK_WIDGET(v));

    gtk_window_resize(GTK_WINDOW(g->w_chat), w, h);
    gtk_window_move(GTK_WINDOW(g->w_chat), x, y + 22.35);
    gtk_widget_show_all(GTK_WIDGET(g->w_chat));

    gtk_widget_hide(GTK_WIDGET(g->profile_area));
    gtk_widget_hide(g->messages_area);
    gtk_widget_hide(g->box_message);
    gtk_widget_hide(g->b_close_profile);
}

void mx_open_win_chat(GtkWidget *w, t_glade *g) {
    gtk_button_set_label(GTK_BUTTON(g->b_username), g->log);
    mx_show_win_chat(w, g);
}
