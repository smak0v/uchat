#include "client.h"

gboolean notif_add_to_gr_response(gpointer data) {
    mx_delete_childs(((t_main_thread *)data)->g->groups_box);

    mx_load_groups_request(((t_main_thread *)data)->g);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_notif_add_to_gr(char *response, t_glade *g) {
    gdk_threads_add_idle(notif_add_to_gr_response,
        mx_create_main_thread_struct(response, g));
}
