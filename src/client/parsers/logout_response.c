#include "client.h"

static int check_response_code(int code) {
    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else
        return MX_SUCCESS;
}

static void logout(gpointer data) {
    mx_clear_login_inputs(((t_main_thread *)data)->g);

    pthread_mutex_lock(&((t_main_thread *)data)->g->mutex);
    ((t_main_thread *)data)->g->dgid = MX_MISTERY;
    ((t_main_thread *)data)->g->group = false;
    pthread_mutex_unlock(&((t_main_thread *)data)->g->mutex);
}

gboolean mx_parse_logout_response(gpointer data) {
    json_object *jobj = json_tokener_parse(((t_main_thread *)data)->response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int)
            if (!check_response_code(json_object_get_int(j_code)))
                logout(data);
                mx_open_logwin(((t_main_thread *)data)->g->w_chat,
                    ((t_main_thread *)data)->g);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}
