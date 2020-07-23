#include "client.h"

void mx_gtk_quit(t_glade *g) {
    (void)pthread_join(g->listener, NULL);

    gtk_main_quit();
    exit(0);
}
