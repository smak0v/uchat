#include "client.h"

static void parse_messages(json_object *j_msgs, int len, t_glade *g) {
    json_object *j_msg_val = NULL;

    for (int i = 0; i < len; ++i) {
        j_msg_val = json_object_array_get_idx(j_msgs, i);

        (void)j_msg_val;
    }

    (void)g;
}

static int check_response_code(int code, json_object *jobj, t_glade *g) {
    json_object *j_msgs = NULL;
    int len = 0;

    if (code == 400 || code == 401 || code == 500)
        return MX_FAILURE;
    else {
        j_msgs = json_object_object_get(jobj, "msg");
        len = json_object_array_length(j_msgs);
        parse_messages(j_msgs, len, g);
        return MX_SUCCESS;
    }
}

void mx_parse_load_messages_response(char *response, t_glade *g) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_code = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "code", &j_code);
        if (j_code && json_object_get_type(j_code) == json_type_int) {
            if (!check_response_code(json_object_get_int(j_code), jobj, g))
                mx_clear_jobj(&jobj, MX_SUCCESS);
            else
                mx_clear_jobj(&jobj, MX_FAILURE);
        }

        mx_clear_jobj(&jobj, MX_FAILURE);
    }

    mx_clear_jobj(&jobj, MX_FAILURE);
}