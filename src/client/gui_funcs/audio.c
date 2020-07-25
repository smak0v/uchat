#include "client.h"

gboolean mx_send_audio(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    pthread_mutex_lock(&g->recorder_mutex);
    g->record_audio_pressed = false;
    pthread_mutex_unlock(&g->recorder_mutex);

    (void)w;
    (void)e;

    return FALSE;
}

void mx_play(GtkWidget *w, t_glade *g) {

    (void)w;
    (void)g;
}
