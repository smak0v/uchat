#include "server.h"

static api_function select_method3(enum e_types type) {
    if (type == LOAD_GR_MEMBERS)
        return mx_load_group_members;
    else if (type == DOWNLOAD)
        return mx_download;
    else
        return mx_bad_request;
}

static api_function select_method2(enum e_types type) {
    if (type == EDIT_MESSAGE)
        return mx_edit_message;
    else if (type == DELETE_MESSAGE)
        return mx_delete_message;
    else if (type == LOAD_DIALOGUES)
        return mx_load_dialogues;
    else if (type == LOAD_GROUPS)
        return mx_load_groups;
    else if (type == LOAD_MESSAGES)
        return mx_load_messages;
    // else if (type == DELETE_USER)
    //     return mx_del_user;
    else if (type == GET_USER)
        return mx_get_user;
    else if (type == FIND_USER)
        return mx_find_user;
    else if (type == EDIT_PROFILE)
        return mx_edit_profile;
    else
        return select_method3(type);
}

static api_function select_method(enum e_types type) {
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
    else if (type == DEL_GROUP)
        return mx_del_group;
    else if (type == LEAVE_GROUP)
        return mx_leave_group;
    else if (type == S_MES)
        return mx_send_message;
    else
        return select_method2(type);
}

char *mx_bad_request(void *jobj, t_comm *connect) {
    jobj = NULL;
    connect = NULL;

    return mx_json_string_code_type(400, BAD_REQUEST);
}

char *mx_process_request(char *request, t_comm *connect) {
    json_object *jobj = json_tokener_parse(request);
    json_object *j_type = NULL;
    int type = 0;
    char *output = NULL;

    printf("%s\n", request);

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "type", &j_type);
        if (j_type && json_object_get_type(j_type) == json_type_int)
            type = json_object_get_int(j_type);
        else
            return mx_bad_request(NULL, NULL);
    }
    else
        return mx_bad_request(NULL, NULL);

    output = select_method((enum e_types)type)((void *)jobj, connect);
    json_object_put(jobj);

    return output;
}
