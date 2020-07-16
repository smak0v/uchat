#include "client.h"

gboolean mx_hide_widget(gpointer w) {
    gtk_widget_hide(GTK_WIDGET(w));

    return 1;
}

gboolean mx_show_widget(gpointer w) {
    gtk_widget_show(GTK_WIDGET(w));

    return 1;
}

gboolean mx_show_all_widget(gpointer w) {
    gtk_widget_show_all(GTK_WIDGET(w));

    return 1;
}

gboolean mx_destroy_widget(gpointer w) {
    gtk_widget_destroy(GTK_WIDGET(w));

    return 1;
}
