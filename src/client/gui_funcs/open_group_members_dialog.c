#include "client.h"

static void destroy_dialog(GtkWidget *w, t_glade *g) {
    (void)w;
    (void)g;

    return;
}

void mx_close_members_window(GtkWidget *w, t_glade *g) {
    gtk_widget_hide(g->d_show_group_members);

    mx_delete_childs(g->box13);

    (void)w;
}

void mx_open_group_members_dialog(GtkWidget *w, t_glade *g) {
    char *request = mx_json_string_load_group_members(g->token, g->uid,
        g->dgid);

    gtk_window_set_transient_for(GTK_WINDOW(g->d_show_group_members),
        GTK_WINDOW(g->window));
    gtk_window_set_position(GTK_WINDOW(g->d_show_group_members),
        GTK_WIN_POS_CENTER_ON_PARENT);

    g_signal_connect(g->d_show_group_members, "delete-event",
        G_CALLBACK(destroy_dialog), g);

    mx_delete_childs(g->box13);

    gtk_widget_show_all(g->d_show_group_members);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);

    (void)w;
}
