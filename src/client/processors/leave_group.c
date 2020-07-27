#include "client.h"

gboolean levae_group(gpointer data) {
    mx_delete_childs(((t_main_thread *)data)->g->groups_box);

    mx_load_groups_request(((t_main_thread *)data)->g);

    mx_close(((t_main_thread *)data)->g);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_leave_group_processor(char *response, t_glade *g) {
    gdk_threads_add_idle(levae_group,
        mx_create_main_thread_struct(response, g));
}

