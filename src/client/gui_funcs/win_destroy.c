#include "client.h"

void mx_gtk_quit(GtkWidget *w, t_glade *g) {
    (void)w;

    if (g && g->token && g->uid)
        mx_logout(g);

    gtk_main_quit();
}
