#include "client.h"

void mx_create_error_modal_window(char *first, char *second, GtkWidget *win) {
    GtkStyleContext *context = NULL;
    GtkWidget *error = gtk_message_dialog_new(
        NULL, GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", first);

    context = gtk_widget_get_style_context(error);
    gtk_style_context_add_class(context, "error_modal");
    gtk_message_dialog_format_secondary_text(
        GTK_MESSAGE_DIALOG(error), "%s", second);
    gtk_window_set_transient_for(GTK_WINDOW(error), GTK_WINDOW(win));
    gtk_window_set_position(GTK_WINDOW(error), GTK_WIN_POS_CENTER_ON_PARENT);
    gtk_dialog_run(GTK_DIALOG(error));
    g_idle_add(mx_destroy_widget, error);
}
