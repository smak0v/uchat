#include "client.h"

static void connect_signals(t_glade *g) {
    g_signal_connect(g->w_chat, "destroy", G_CALLBACK(mx_gtk_quit), g);
    g_signal_connect(g->b_send_msg, "clicked", G_CALLBACK(mx_send_msg), g);
    g_signal_connect(g->b_logout, "clicked", G_CALLBACK(mx_b_logout), g);
    g_signal_connect(g->b_username, "clicked", G_CALLBACK(mx_open_profile), g);
    g_signal_connect(g->b_close_profile, "clicked",
        G_CALLBACK(mx_close_profile), g);
    g_signal_connect(g->b_add_chat, "clicked", G_CALLBACK(mx_add_chat), g);
    g_signal_connect(g->b_add_group, "clicked", G_CALLBACK(mx_add_group), g);
    g_signal_connect(g->b_save_profile, "clicked",
        G_CALLBACK(mx_save_profile), g);
    g_signal_connect(g->w_chat, "key_press_event",
        G_CALLBACK(mx_close_chat), g);
    g_signal_connect(g->b_attach_file, "clicked",
        G_CALLBACK(mx_attach_file), g);
    g_signal_connect(g->messages_area, "size-allocate",
        G_CALLBACK(mx_scroll_to_bottom), g);
}

static void win_chat_utils(t_glade *g) {
    gtk_button_set_label(GTK_BUTTON(g->b_username), g->log);

    mx_delete_childs(g->dialogs_box);
    mx_delete_childs(g->groups_box);
    mx_load_dialogues_request(g);
    mx_load_groups_request(g);
    mx_get_profile(g);
}

void mx_create_win_chat(t_glade *g) {
    mx_find_gtk_objects_1(g);
    mx_find_gtk_objects_2(g);

    connect_signals(g);

    gtk_widget_realize(g->w_chat);
}

void mx_show_win_chat(GtkWidget *v, t_glade *g) {
    int w = 0;
    int h = 0;
    int x = 0;
    int y = 0;

    gtk_window_get_position(GTK_WINDOW(v), &x, &y);
    gtk_window_get_size(GTK_WINDOW(v), &w, &h);
    g_idle_add(mx_hide_widget, v);

    gtk_window_resize(GTK_WINDOW(g->w_chat), w, h);
    gtk_window_move(GTK_WINDOW(g->w_chat), x, y + 22.35);
    g_idle_add(mx_show_all_widget, g->w_chat);

    g_idle_add(mx_hide_widget, g->profile_area);
    g_idle_add(mx_hide_widget, g->messages_area);
    g_idle_add(mx_hide_widget, g->box_message);
    g_idle_add(mx_hide_widget, g->b_close_profile);
    g_idle_add(mx_hide_widget, g->box5);

    win_chat_utils(g);
}
