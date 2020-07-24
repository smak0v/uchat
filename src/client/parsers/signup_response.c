#include "client.h"

static void save_response_data(json_object *jobj, t_glade *g) {
    json_object *j_token = NULL;
    json_object *j_uid = NULL;

    pthread_mutex_lock(&g->mutex);
    json_object_object_get_ex(jobj, "tok", &j_token);
    if (j_token && json_object_get_type(j_token) == json_type_string)
        g->token = (char *)json_object_get_string(j_token);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_uid && json_object_get_type(j_uid) == json_type_int)
        g->uid = json_object_get_int(j_uid);
    pthread_mutex_unlock(&g->mutex);
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    if (code == 400)
        return MX_FAILURE;
    else if (code == 409) {
        gtk_label_set_text(GTK_LABEL(g->l_signup_error), "Username taken!\n" \
            "User with this username already exists. Try another!");
        gdk_threads_add_idle(mx_show_widget, g->l_signup_error);
        return MX_FAILURE;
    }
    else if (code == 500) {
        gtk_label_set_text(GTK_LABEL(g->l_signup_error), "Error!\n" \
            "Something were wrong! Try again later!");
        gdk_threads_add_idle(mx_show_widget, g->l_signup_error);
        return MX_FAILURE;
    }
    else {
        gtk_label_set_text(GTK_LABEL(g->l_signup_error), "");
        gdk_threads_add_idle(mx_hide_widget, g->l_signup_error);
        save_response_data(jobj, g);
        return MX_SUCCESS;
    }
}

int mx_parse_signup_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), jobj, g))
                return mx_clear_jobj(&jobj, MX_SUCCESS);
            else
                return mx_clear_jobj(&jobj, MX_FAILURE);
        }

        return mx_clear_jobj(&jobj, MX_FAILURE);
    }

    return mx_clear_jobj(&jobj, MX_FAILURE);
}
