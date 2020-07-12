#include "client.h"

static void destroy_dialog(GtkWidget *w, t_glade *g) {
    (void)w;
    (void)g;

    return;
}

void mx_attach_file(GtkWidget *w, t_glade *g) {
    g_signal_connect(g->d_file_choose, "delete-event",
        G_CALLBACK(destroy_dialog), g);

    gtk_widget_show_all(g->d_file_choose);
    gtk_dialog_run(GTK_DIALOG(g->d_file_choose));

    (void)w;
}
