#include "client.h"

gboolean delete_account(gpointer data) {
    json_object *jobj = json_tokener_parse(((t_main_thread *)data)->response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);

        if (j_code && json_object_get_type(j_code) == json_type_int)
            if (json_object_get_int(j_code) == 200)
                mx_open_logwin(((t_main_thread *)data)->g->b_delete_account,
                    ((t_main_thread *)data)->g);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}

void mx_finish_deleting_account(char *response, t_glade *g) {
    gdk_threads_add_idle(delete_account,
        mx_create_main_thread_struct(response, g));
}
