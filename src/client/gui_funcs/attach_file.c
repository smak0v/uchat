#include "client.h"

void mx_attach_file(GtkWidget *w, t_glade *g) {
    char *filename = NULL;
    GtkWidget *dialog = gtk_file_chooser_dialog_new ("Choose file",
        GTK_WINDOW(g->w_chat), GTK_FILE_CHOOSER_ACTION_OPEN, "_Cancel",
        GTK_RESPONSE_CANCEL, "_Open", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        g->filename = mx_strdup(filename);
        g_free(filename);
    }

    gtk_widget_destroy(dialog);

    (void)w;
}
