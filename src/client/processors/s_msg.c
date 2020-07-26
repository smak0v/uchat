#include "client.h"

static gboolean s_msg(gpointer data) {
    if (!mx_parse_send_message_response(((t_main_thread *)data)->response,
        ((t_main_thread *)data)->g))
        mx_add_message_to_gui(((t_main_thread *)data)->g,
            ((t_main_thread *)data)->response);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_s_msg(char *response, t_glade *g) {
    gdk_threads_add_idle(s_msg, mx_create_main_thread_struct(response, g));
}
