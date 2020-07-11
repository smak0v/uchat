#include "client.h"

static void find_gtk_objects(t_glade *g) {
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
    g->e_message = mx_get_gtk_obj(g, "e_message");
    g->b_attach_file = mx_get_gtk_obj(g, "b_attach_file");
    g->l_select_chat = mx_get_gtk_obj(g, "l_select_chat");
    g->b_add_chat = mx_get_gtk_obj(g, "b_add_chat");
    g->b_add_group = mx_get_gtk_obj(g, "b_add_group");
    g->b_save_profile = mx_get_gtk_obj(g, "b_save_profile");
    g->d_add_chat = mx_get_gtk_obj(g, "d_add_chat");
    g->d_add_group = mx_get_gtk_obj(g, "d_add_group");
    g->d_file_choose = mx_get_gtk_obj(g, "d_file_choose");
    g->gc_notebook = mx_get_gtk_obj(g, "gc_notebook");
    g->e_country = mx_get_gtk_obj(g, "e_country");
    g->e_birthday = mx_get_gtk_obj(g, "e_birthday");
    g->e_email = mx_get_gtk_obj(g, "e_email");
    g->e_profile_name = mx_get_gtk_obj(g, "e_profile_name");
    g->e_status = mx_get_gtk_obj(g, "e_status");
    g->l_chat_name = mx_get_gtk_obj(g, "l_chat_name");
    g->scrolledwindow1 = mx_get_gtk_obj(g, "scrolledwindow1");
}

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
}

static void win_chat_utils(t_glade *g) {
    gtk_button_set_label(GTK_BUTTON(g->b_username), g->log);

    mx_delete_childs(g->dialogs_box);
    mx_delete_childs(g->groups_box);
    mx_load_dialogs(g);
    mx_load_groups(g);
    mx_get_profile(g, true);
}

void mx_create_win_chat(t_glade *g) {
    find_gtk_objects(g);
    connect_signals(g);
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
    gtk_widget_hide(GTK_WIDGET(g->messages_area));
    gtk_widget_hide(GTK_WIDGET(g->box_message));
    gtk_widget_hide(GTK_WIDGET(g->b_close_profile));

    win_chat_utils(g);
}
