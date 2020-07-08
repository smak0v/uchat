#include "client.h"

void add_group(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_group_search), "");
    gtk_widget_destroy(GTK_WIDGET(g->d_add_group));

    (void)w;
}

void cancel_add_group(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_group_search), "");
    gtk_widget_destroy(GTK_WIDGET(g->d_add_group));

    (void)w;
}

void mx_add_group(GtkWidget *w, t_glade *g) {
    gtk_window_set_transient_for(GTK_WINDOW(g->d_add_group),
        GTK_WINDOW(g->w_chat));
    gtk_window_set_position(GTK_WINDOW(g->d_add_group),
        GTK_WIN_POS_CENTER_ON_PARENT);

    g->b_add_group_ok = mx_get_gtk_obj(g, "b_add_group_ok");
    g->b_add_group_cancel = mx_get_gtk_obj(g, "b_add_group_cancel");
    g->e_group_search = mx_get_gtk_obj(g, "e_search_group");

    g_signal_connect(g->b_add_group_ok, "clicked", G_CALLBACK(add_group), g);
    g_signal_connect(g->b_add_group_cancel, "clicked",
        G_CALLBACK(cancel_add_group), g);

    gtk_dialog_run(GTK_DIALOG(g->d_add_group));

    (void)w;
}
