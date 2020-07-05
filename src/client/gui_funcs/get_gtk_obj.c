#include "client.h"

GtkWidget *mx_get_gtk_obj(t_glade *g, char *name) {
    return GTK_WIDGET(gtk_builder_get_object(g->bd, name));
}
