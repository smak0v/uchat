#include "client.h"

static gboolean s_msg(gpointer data) {
    t_main_thread *maint_thread = (t_main_thread *)data;

    if (!mx_parse_send_message_response(maint_thread->response,
        maint_thread->g)) {

        mx_add_message_to_gui(maint_thread->g,
            maint_thread->response);
    }

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_s_msg(char *response, t_glade *g) {
    gdk_threads_add_idle(s_msg, mx_create_main_thread_struct(response, g));
}
