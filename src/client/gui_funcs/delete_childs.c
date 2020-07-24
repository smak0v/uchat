#include "client.h"

static void delete_child_1(void *child, void *data) {
    if (child)
        gtk_widget_destroy(GTK_WIDGET(child));

    (void)data;
}

static void delete_child_2(void *child, void *data) {
    if (child)
        gdk_threads_add_idle(mx_destroy_widget, GTK_WIDGET(child));

    (void)data;
}

void mx_delete_childs(GtkWidget *w, bool gtk_thread) {
    GList *childs = NULL;

    if (w) {
        childs = gtk_container_get_children(GTK_CONTAINER(w));

        if (childs) {
            if (gtk_thread)
                g_list_foreach(childs, (GFunc)delete_child_1, NULL);
            else
                g_list_foreach(childs, (GFunc)delete_child_2, NULL);

            g_list_free(childs);
            childs = NULL;
        }
    }
}
