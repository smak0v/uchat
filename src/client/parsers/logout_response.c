#include "client.h"

static int check_response_code(int code) {
    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else
        return MX_SUCCESS;
}

void mx_parse_logout_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code))) {
                mx_clear_jobj(&jobj, MX_SUCCESS);
                mx_clear_login_inputs(g);
                g->dgid = MX_MISTERY;
                g->group = false;
                mx_open_logwin(g->w_chat, g);
            }
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }
    mx_clear_jobj(&jobj, MX_FAILURE);
}
