#include "client.h"

gboolean mx_send_by_enter(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    if (e->keyval == GDK_KEY_Return)
        mx_send_msg(w, g);

    return FALSE;
}
