#include "client.h"

static void search_user(GtkWidget *w, t_glade *g) {
    char *input = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_chat_search));
    char *request = NULL;

    mx_delete_childs(g->box8);

    if (input && strlen(input) > 0) {
        request = mx_json_string_search_user(g->token, g->uid, input,
            FIND_USER);

        SSL_write(g->ssl, request, strlen(request));

        mx_strdel(&request);
    }

    (void)w;
}

static void cancel_add_chat(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_chat_search), "");

    gtk_widget_hide(g->d_add_chat);

    mx_delete_childs(g->box8);

    (void)w;
}

static void destroy_dialog(GtkWidget *w, t_glade *g) {
    (void)w;
    (void)g;

    return;
}

static void reset_grop_subwindow(t_glade *g) {
    gtk_widget_hide(g->b_add_user);
    gtk_widget_hide(g->b_leave_group);
    gtk_widget_hide(g->box9);

    g->dgid = MX_MISTERY;
    g->group = false;
}

void mx_add_chat(GtkWidget *w, t_glade *g) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(g->gc_notebook), 0);
    gtk_window_set_transient_for(GTK_WINDOW(g->d_add_chat),
        GTK_WINDOW(g->window));
    gtk_window_set_position(GTK_WINDOW(g->d_add_chat),
        GTK_WIN_POS_CENTER_ON_PARENT);
    g->b_add_chat_cancel = mx_get_gtk_obj(g, "b_add_chat_cancel");
    g->e_chat_search = mx_get_gtk_obj(g, "e_chat_search");
    g_signal_connect(g->b_add_chat_cancel, "clicked",
        G_CALLBACK(cancel_add_chat), g);
    g_signal_connect(g->d_add_chat, "delete-event",
        G_CALLBACK(destroy_dialog), g);
    g_signal_connect(g->e_chat_search, "changed",
        G_CALLBACK(search_user), g);
    mx_delete_childs(g->box8);
    gtk_entry_set_text(GTK_ENTRY(g->e_chat_search), "");
    gtk_widget_show_all(g->d_add_chat);

    reset_grop_subwindow(g);

    (void)w;
}
