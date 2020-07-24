#include "client.h"

void mx_open_profile(GtkWidget *w, t_glade *g) {
    gdk_threads_add_idle(mx_hide_widget, g->messages_area);
    gdk_threads_add_idle(mx_hide_widget, g->box_message);
    gdk_threads_add_idle(mx_hide_widget, g->e_search);
    gdk_threads_add_idle(mx_hide_widget, g->l_select_chat);
    gdk_threads_add_idle(mx_hide_widget, g->box9);
    gdk_threads_add_idle(mx_hide_widget, g->b_add_user);

    gdk_threads_add_idle(mx_show_widget, g->profile_area);
    gdk_threads_add_idle(mx_show_widget, g->b_close_profile);

    g->group = false;
    g->dgid = MX_MISTERY;

    (void)w;
}

void mx_close_profile(GtkWidget *w, t_glade *g) {
    gdk_threads_add_idle(mx_hide_widget, g->profile_area);
    gdk_threads_add_idle(mx_hide_widget, g->b_close_profile);

    gdk_threads_add_idle(mx_show_widget, g->e_search);
    gdk_threads_add_idle(mx_show_widget, g->l_select_chat);

    g->group = false;
    g->dgid = MX_MISTERY;

    (void)w;
}
