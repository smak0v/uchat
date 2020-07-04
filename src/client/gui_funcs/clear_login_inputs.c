#include "client.h"

void mx_clear_login_inputs(t_glade *g) {
    gtk_entry_set_text(GTK_ENTRY(g->e_name), "");
    gtk_entry_set_text(GTK_ENTRY(g->e_pass), "");
}
