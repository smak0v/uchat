#include "client.h"

static gboolean n_grp(gpointer data) {
    if (!mx_parse_new_group_response(((t_main_thread *)data)->response,
        ((t_main_thread *)data)->g)) {
        gtk_entry_set_text(
            GTK_ENTRY(((t_main_thread *)data)->g->e_new_group_name), "");
        gtk_label_set_text(
            GTK_LABEL(((t_main_thread *)data)->g->err_group_name_label), "");

        gtk_widget_hide(((t_main_thread *)data)->g->d_add_group);
    }

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_n_grp(char *response, t_glade *g) {
    gdk_threads_add_idle(n_grp, mx_create_main_thread_struct(response, g));
}
