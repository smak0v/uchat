#include "client.h"

void mx_clear_signup_inputs(t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->r_ename), "");
    gtk_entry_set_text(GTK_ENTRY(g->r_epass), "");
    gtk_entry_set_text(GTK_ENTRY(g->r_repass), "");
}
