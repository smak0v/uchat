#include "client.h"

static int check_response_code(int code, t_glade *g) {
    if (code == 400)
        return MX_FAILURE;
    else if (code == 409) {
        mx_create_error_modal_window("Username taken!",
            "User with this username already exists. Try another!", g->w_reg);
        return MX_FAILURE;
    }
    else if (code == 500) {
        mx_create_error_modal_window("Something were wrong!",
            "Something were wrong!. Try again later!", g->w_reg);
        return MX_FAILURE;
    }
    else
        return MX_SUCCESS;
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
