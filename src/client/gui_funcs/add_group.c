#include "client.h"

static void add_new_group(t_glade *g, char *name) {
    char *request = mx_json_string_new_group(g->token, g->uid, name);

    SSL_write(g->ssl, request, strlen(request));

    mx_strdel(&request);
}

static void prepare_add_group(GtkWidget *w, t_glade *g) {
    char *name = (char *)gtk_entry_get_text(GTK_ENTRY(g->e_new_group_name));

    if (name && strlen(name) >= 1)
        add_new_group(g, name);
    else {
        gtk_label_set_text(GTK_LABEL(g->err_group_name_label),
            "The name of the group must be at least\n1 character long." \
            "Try another name!");
        mx_widget_visible(g->err_group_name_label, true);
    }

    (void)w;
}

static void cancel_add_group(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_new_group_name), "");
    gtk_label_set_text(GTK_LABEL(g->err_group_name_label), "");
    gtk_widget_hide(GTK_WIDGET(g->d_add_group));

    (void)w;
}

static void destroy_dialog(GtkWidget *w, t_glade *g) {
    (void)w;
    (void)g;

    return;
}

void mx_add_group(GtkWidget *w, t_glade *g) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(g->gc_notebook), 1);
    gtk_window_set_transient_for(GTK_WINDOW(g->d_add_group),
        GTK_WINDOW(g->w_chat));
    gtk_window_set_position(GTK_WINDOW(g->d_add_group),
        GTK_WIN_POS_CENTER_ON_PARENT);

    g->b_add_group_ok = mx_get_gtk_obj(g, "b_add_group_ok");
    g->b_add_group_cancel = mx_get_gtk_obj(g, "b_add_group_cancel");
    g->e_new_group_name = mx_get_gtk_obj(g, "e_new_group_name");
    g->err_group_name_label = mx_get_gtk_obj(g, "err_group_name_label");
    g_signal_connect(g->b_add_group_ok, "clicked",
        G_CALLBACK(prepare_add_group), g);
    g_signal_connect(g->b_add_group_cancel, "clicked",
        G_CALLBACK(cancel_add_group), g);
    g_signal_connect(g->d_add_group, "delete-event",
        G_CALLBACK(destroy_dialog), g);
    gtk_widget_show_all(g->d_add_group);
    gtk_dialog_run(GTK_DIALOG(g->d_add_group));

    (void)w;
}
