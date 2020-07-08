#include "client.h"

static int check_response_code(int code, t_glade *g) {
    if (code == 400)
        return MX_FAILURE;
    else if (code == 401 || code == 500) {
        gtk_label_set_text(GTK_LABEL(g->err_group_name_label),
            "Something were wrong! Try again later!");
        mx_widget_visible(GTK_WIDGET(g->err_group_name_label), true);
        return MX_FAILURE;
    }
    else
        return MX_SUCCESS;
}

int mx_parse_new_group_response(char *response, t_glade *g, char *name) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), g)) {
                // TODO add group in GUI
                mx_printstr_endl(name);

                return mx_clear_jobj(&jobj, MX_SUCCESS);
            }
            else
                return mx_clear_jobj(&jobj, MX_FAILURE);
        }

        return mx_clear_jobj(&jobj, MX_FAILURE);
    }

    return mx_clear_jobj(&jobj, MX_FAILURE);
}
