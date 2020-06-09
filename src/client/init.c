#include "uchat.h"

static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window = gtk_application_window_new(app);

    gtk_window_set_title(GTK_WINDOW(window), "UCHAT");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 200);
    gtk_widget_show_all(window);

    user_data = NULL;
}

int mx_init_client(int argc, char **argv) {
    int status = MX_SUCCESS;
    GtkApplication *app = gtk_application_new("com.ucode.uchat", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
