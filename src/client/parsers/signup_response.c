#include "client.h"

static int check_response_code(int code, t_glade *g) {
    if (code == 400)
        return MX_FAILURE;
    else if (code == 409) {
        gtk_label_set_text(GTK_LABEL(g->l_signup_error),
            "Username taken!\n" \
            "User with this username already exists. Try another!");
        g_idle_add(mx_show_widget, g->l_login_error);
        return MX_FAILURE;
    }
    else if (code == 500) {
        gtk_label_set_text(GTK_LABEL(g->l_signup_error), "Error!\n" \
            "Something were wrong! Try again later!");
        g_idle_add(mx_show_widget, g->l_login_error);
        return MX_FAILURE;
    }
    else {
        gtk_label_set_text(GTK_LABEL(g->l_login_error), "");
        g_idle_add(mx_hide_widget, g->l_login_error);
        return MX_SUCCESS;
    }
}

int mx_parse_signup_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), g))
                return mx_clear_jobj(&jobj, MX_SUCCESS);
            else
                return mx_clear_jobj(&jobj, MX_FAILURE);
        }

        return mx_clear_jobj(&jobj, MX_FAILURE);
    }

    return mx_clear_jobj(&jobj, MX_FAILURE);
}
