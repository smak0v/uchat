#include "client.h"

void mx_n_grp(char *response, t_glade *g) {
    if (!mx_parse_new_group_response(response, g)) {
        gtk_entry_set_text(GTK_ENTRY(g->e_new_group_name), "");
        gtk_label_set_text(GTK_LABEL(g->err_group_name_label), "");

        gtk_widget_hide(g->d_add_group);
    }
}
