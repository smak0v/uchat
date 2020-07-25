#include "client.h"

gboolean mx_record_audio(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    mx_printstr_endl("record");

    (void)w;
    (void)e;
    (void)g;

    return FALSE;
}

gboolean mx_send_audio(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    mx_printstr_endl("send");

    (void)w;
    (void)e;
    (void)g;

    return FALSE;
}
