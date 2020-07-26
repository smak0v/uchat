#include "client.h"

static int check_response_code(int code, t_glade *g) {
    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else if (code == 666) {
        gtk_widget_hide(g->l_invite_user_success);
        gtk_widget_show(g->l_invite_user_error);
        return MX_SUCCESS;
    }
    else {
        gtk_widget_hide(g->l_invite_user_error);
        gtk_widget_show(g->l_invite_user_success);
        return MX_SUCCESS;
    }
}

gboolean mx_invite_user_to_group_response(gpointer data) {
    json_object *jobj = json_tokener_parse(((t_main_thread *)data)->response);
    json_object *j_code = NULL;

    json_object_object_get_ex(jobj, "code", &j_code);

    if (j_code && json_object_get_type(j_code) == json_type_int)
        if (!check_response_code(json_object_get_int(j_code),
            ((t_main_thread *)data)->g))
            mx_clear_jobj(&jobj, MX_SUCCESS);

    mx_clear_jobj(&jobj, MX_SUCCESS);

    mx_delete_main_thread_struct((t_main_thread **)&data);

    return G_SOURCE_REMOVE;
}
