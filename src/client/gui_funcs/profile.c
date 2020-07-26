#include "client.h"

void mx_open_profile(GtkWidget *w, t_glade *g) {
    gtk_widget_hide(g->messages_area);
    gtk_widget_hide(g->box_message);
    gtk_widget_hide(g->e_search);
    gtk_widget_hide(g->l_select_chat);
    gtk_widget_hide(g->box9);
    gtk_widget_hide(g->b_add_user);
    gtk_widget_hide(g->b_leave_group);

    gtk_widget_show(g->profile_area);
    gtk_widget_show(g->b_close_profile);

    g->group = false;
    g->dgid = MX_MISTERY;

    (void)w;
}

void mx_close_profile(GtkWidget *w, t_glade *g) {
    gtk_widget_hide(g->profile_area);
    gtk_widget_hide(g->b_close_profile);

    gtk_widget_show(g->e_search);
    gtk_widget_show(g->l_select_chat);

    g->group = false;
    g->dgid = MX_MISTERY;

    (void)w;
}
