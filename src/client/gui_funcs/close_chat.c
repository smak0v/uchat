#include "client.h"

gboolean mx_close_chat(GtkWidget *w, GdkEventKey *e, t_glade *g) {
    if (e->keyval == GDK_KEY_Escape) {
        gdk_threads_add_idle(mx_hide_widget, g->messages_area);
        gdk_threads_add_idle(mx_hide_widget, g->box_message);
        gdk_threads_add_idle(mx_hide_widget, g->profile_area);
        gdk_threads_add_idle(mx_hide_widget, g->box9);
        gdk_threads_add_idle(mx_hide_widget, g->messages_area);
        gdk_threads_add_idle(mx_hide_widget, g->b_add_user);

        gdk_threads_add_idle(mx_show_widget, g->l_select_chat);

        gtk_label_set_text(GTK_LABEL(g->l_chat_name), "");

        mx_delete_childs(g->messages_area, true);

        g->group = false;
        g->dgid = MX_MISTERY;
    }

    (void)w;
    return FALSE;
}
