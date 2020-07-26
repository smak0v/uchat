#include "client.h"

static gboolean login(gpointer data) {
    if (!mx_parse_login_response(((t_main_thread *)data)->response,
        ((t_main_thread *)data)->g))
        mx_show_win_chat(((t_main_thread *)data)->g);

    mx_clear_login_inputs(((t_main_thread *)data)->g);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

static gboolean signup(gpointer data) {
    if (!mx_parse_signup_response(((t_main_thread *)data)->response,
        ((t_main_thread *)data)->g))
        mx_show_win_chat(((t_main_thread *)data)->g);

    mx_clear_signup_inputs(((t_main_thread *)data)->g);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_s_in_s_up(char *response, t_glade *g, int type) {
    if (type == S_IN)
        gdk_threads_add_idle(login, mx_create_main_thread_struct(response, g));

    else if (type == REG)
        gdk_threads_add_idle(signup,
            mx_create_main_thread_struct(response, g));
}
