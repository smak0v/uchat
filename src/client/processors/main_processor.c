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

    if (type == S_IN)
        mx_s_in_s_up(response, g, S_IN);
    else if (type == REG)
        mx_s_in_s_up(response, g, REG);
    else if (type == S_OUT)
        mx_s_out(response, g);
    else if (type == LOAD_DIALOGUES)
        mx_load_dialogues(response, g);
    else if (type == LOAD_GROUPS)
        mx_load_groups(response, g);
    else if (type == FIND_USER)
        mx_find_user(response, g);
    else if (type == N_GRP)
        mx_n_grp(response, g);
    else if (type == LOAD_MESSAGES)
        mx_load_messages(response, g);
    else if (type == EDIT_PROFILE)
        mx_edit_profile(response, g);
}