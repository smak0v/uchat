#include "client.h"

static void add_chat(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_chat_search), "");
    gtk_widget_hide(GTK_WIDGET(g->d_add_chat));

    (void)w;
}

static void cancel_add_chat(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_chat_search), "");
    gtk_widget_hide(GTK_WIDGET(g->d_add_chat));

    (void)w;
}

static void destroy_dialog(GtkWidget *w, t_glade *g) {
    (void)w;
    (void)g;

    return;
}

void mx_add_chat(GtkWidget *w, t_glade *g) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(g->gc_notebook), 0);
    gtk_window_set_transient_for(GTK_WINDOW(g->d_add_chat),
        GTK_WINDOW(g->w_chat));
    gtk_window_set_position(GTK_WINDOW(g->d_add_chat),
        GTK_WIN_POS_CENTER_ON_PARENT);

    g->b_add_chat_ok = mx_get_gtk_obj(g, "b_add_chat_ok");
    g->b_add_chat_cancel = mx_get_gtk_obj(g, "b_add_chat_cancel");
    g->e_chat_search = mx_get_gtk_obj(g, "e_chat_search");

    g_signal_connect(g->b_add_chat_ok, "clicked", G_CALLBACK(add_chat), g);
    g_signal_connect(g->b_add_chat_cancel, "clicked",
        G_CALLBACK(cancel_add_chat), g);
    g_signal_connect(g->d_add_chat, "delete-event",
        G_CALLBACK(destroy_dialog), g);

    gtk_widget_show_all(g->d_add_chat);
    gtk_dialog_run(GTK_DIALOG(g->d_add_chat));

    (void)w;
}
