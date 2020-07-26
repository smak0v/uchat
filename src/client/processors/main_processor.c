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

static void check_response_type_1(int type, char *response, t_glade *g) {
    if (type == EDIT_PROFILE)
        mx_edit_profile(response, g);
    else if (type == FIND_USER)
        mx_search_users(response, g);
    else if (type == S_MES)
        mx_s_msg(response, g);
    else if (type == FILE_TRANSFER)
        mx_cli_file_transfer(response, g);
    else if (type == INV)
        mx_inv(response, g);
    else if (type == NOTIF_ADD_TO_GR)
        mx_notif_add_to_gr(response, g);
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
    else if (type == GET_USER)
        mx_find_user(response, g);
    else if (type == N_GRP)
        mx_n_grp(response, g);
    else if (type == LOAD_MESSAGES)
        mx_load_messages(response, g);
    else
        check_response_type_1(type, response, g);
}
