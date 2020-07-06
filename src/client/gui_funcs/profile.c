#include "client.h"

void mx_open_profile(GtkWidget *w, t_glade *g) {
    (void)w;

    mx_widget_visible(g->messages_area, false);
    mx_widget_visible(g->box_message, false);
    mx_widget_visible(g->e_search, false);
    mx_widget_visible(g->i_search, false);
    mx_widget_visible(g->l_select_chat, false);

    mx_widget_visible(g->profile_area, true);
    mx_widget_visible(g->b_close_profile, true);
}

void mx_close_profile(GtkWidget *w, t_glade *g) {
    (void)w;

    mx_widget_visible(g->profile_area, false);
    mx_widget_visible(g->b_close_profile, false);

    mx_widget_visible(g->e_search, true);
    mx_widget_visible(g->i_search, true);
    mx_widget_visible(g->l_select_chat, true);
}

