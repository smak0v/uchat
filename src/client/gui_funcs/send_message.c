#include "client.h"

void mx_send_msg(GtkWidget *w, t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_message), "");

    (void)w;
}
