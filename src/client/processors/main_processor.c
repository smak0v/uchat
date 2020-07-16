#include "client.h"

static int get_response_type(char *response) {
    json_object *jobj = json_tokener_parse(response);
    json_object *j_type = NULL;
    int type = -1;

    json_object_object_get_ex(jobj, "type", &j_type);

    if (j_type && json_object_get_type(j_type) == json_type_int)
        type = json_object_get_int(j_type);

    mx_clear_jobj(&jobj, MX_SUCCESS);

    return type;
}

void mx_check_response_type(char *response, t_glade *g) {
    int type = get_response_type(response);

    type = S_IN;
    if (type == S_IN)
        mx_s_in(response, g);

}
