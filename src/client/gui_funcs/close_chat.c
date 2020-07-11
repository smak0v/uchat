#include "client.h"

gboolean mx_close_chat(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    if (e->keyval == GDK_KEY_Escape) {
        gtk_widget_hide(g->messages_area);
        gtk_widget_hide(g->box_message);
        gtk_widget_show(g->l_select_chat);

        g->group = false;
        g->dgid = MX_MISTERY;
    }

    (void)w;

    return FALSE;
}
