#include "client.h"

void mx_change_theme(GtkWidget *w, t_glade *g) {
    if (!g->light_theme) {
        mx_connect_css("light.css");
        gtk_button_set_label(GTK_BUTTON(g->b_change_theme), "Dark theme");
        g->light_theme = true;
    }

    else {
        mx_connect_css("dark.css");
        gtk_button_set_label(GTK_BUTTON(g->b_change_theme), "Light theme");
        g->light_theme = false;
    }

    (void)w;
}
