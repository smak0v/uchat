#include "client.h"

void mx_open_profile(GtkWidget *w, t_glade *g) {
    g_idle_add(mx_hide_widget, g->messages_area);
    g_idle_add(mx_hide_widget, g->box_message);
    g_idle_add(mx_hide_widget, g->e_search);
    g_idle_add(mx_hide_widget, g->l_select_chat);
    g_idle_add(mx_hide_widget, g->box5);

    g_idle_add(mx_show_widget, g->profile_area);
    g_idle_add(mx_show_widget, g->b_close_profile);

    (void)w;
}

void mx_close_profile(GtkWidget *w, t_glade *g) {
    g_idle_add(mx_hide_widget, g->profile_area);
    g_idle_add(mx_hide_widget, g->b_close_profile);

    g_idle_add(mx_show_widget, g->e_search);
    g_idle_add(mx_show_widget, g->l_select_chat);

    (void)w;
}
