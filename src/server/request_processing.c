#include "server.h"

char *mx_bad_request(void *jobj, t_comm *connect) {
    jobj = NULL;
    connect = NULL;

    return "{\"code\": 400}";
}

api_function mx_select_method(enum e_types type) {
    if (type == REG)
        return mx_register_user;
    else if (type == S_IN)
        return mx_sign_in;
    else if (type == S_OUT)
        return mx_sign_out;
    else if (type == N_GRP)
        return mx_new_group;
    else if (type == INV)
        return mx_add_to_group;
    else if (type == RN_GR)
        return mx_rename_group;
    else if (type == S_MES)
        return mx_send_message;
    else if (type == EDIT_MESSAGE)
        return mx_edit_message;
    // else if (type == DEL_USER_PROFILE)
    //     return mx_del_user;
    else
        return mx_bad_request;
}

char *mx_process_request(char *request, t_comm *connect) {
    json_object *jobj = json_tokener_parse(request);
    json_object *j_type = NULL;
    int type = 0;
    char *output = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "type", &j_type);
        if (j_type && json_object_get_type(j_type) == json_type_int)
            type = json_object_get_int(j_type);
        else
            return mx_bad_request(NULL, NULL);
    }
    else
        return mx_bad_request(NULL, NULL);

    output = mx_select_method((enum e_types)type)((void *)jobj, connect);
    json_object_put(jobj);

    return output;
}
