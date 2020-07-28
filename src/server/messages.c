#include "server.h"

static char *send_group_message(t_comm *connect, t_msg *message, sqlite3 *db) {
    char *js_str = mx_json_string_msg(message);
    int *group_members = mx_get_all_id_group_members(db, message->group_id);

    message->id = mx_add_msg(db, message);

    for (int i = 0; group_members[i] != -1; i++)
        if (group_members[i] != message->sender)
            mx_send_to_all_clients(connect, js_str, group_members[i]);

    mx_strdel(&js_str);
    return mx_msg_json_builder(message);
}

// Dialog_id == -1: message is a group message; Dialog_id == -2: message is a
// private message, but the chat doesn't exist yet;
static char *send_private_message(t_comm *connect, t_msg *msg, sqlite3 *db) {
    int d_id = msg->dialog_id;
    char *j_str = NULL;
    t_dialog *dialogue = NULL;
    int code = 0;

    if (msg->dialog_id == -2) {
        if ((dialogue = mx_get_dialog_by_id1_id2(connect->db, msg->sender,
                                                 msg->recepient)) != NULL)
            msg->dialog_id = dialogue->dialog_id;
        else if ((d_id = mx_add_dialog(db, msg->sender, msg->recepient)) == -1)
            return mx_json_string_code_type(500, S_MES);
        msg->dialog_id = d_id;
        code = 1;
    }
    msg->id = mx_add_msg(db, msg);
    j_str = mx_msg_json_builder(msg);
    mx_send_to_all_clients(connect, j_str, msg->recepient);
    j_str = mx_add_dialog_name(connect->db, code, j_str, msg->recepient);

    return j_str;
}

char *mx_send_message(void *jobj, t_comm *connect) {
    t_msg *msg = mx_extract_message(jobj);
    char *res = NULL;
    char *tmp = msg ? msg->file : NULL;

    if (!msg)
        return mx_bad_request(NULL, NULL);
    if (mx_validate_token(connect->db, msg->sender, (json_object *)jobj))
         return mx_json_string_code_type(401, S_MES);
    if (!(msg->username = mx_get_user_login_by_id(connect->db, msg->sender)))
        return mx_json_string_code_type(500, S_MES);
    if (msg->file)
        msg->file = (char *)mx_memrchr(tmp, '/', mx_strlen(tmp)) + 1;
    if (msg->group_id != -1)
        res = send_group_message(connect, msg, connect->db);
    else
        res = send_private_message(connect, msg, connect->db);
    if (msg->file)
        return mx_file_transfer(connect, tmp, res, msg->id, 0);
    return res;
}

char *mx_edit_message(void *jobj, t_comm *connect) {
    int uid = -1;
    int msg_id = -1;
    char *msg = NULL;
    t_msg *struct_msg = NULL;

    if (mx_extract_edit_msg(jobj, &uid, &msg_id, &msg) == -1)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return mx_json_string_code_type(401, EDIT_MESSAGE);

    struct_msg = mx_get_msg_by_id(connect->db, msg_id);
    if (struct_msg->sender != uid)
        return mx_json_string_code_type(403, EDIT_MESSAGE);

    if (mx_update_msg_by_id(connect->db, msg, msg_id) == -1)
        return mx_json_string_code_type(500, EDIT_MESSAGE);

    return mx_json_string_code_type(200, EDIT_MESSAGE);
}

char *mx_delete_message(void *jobj, t_comm *connect) {
    t_msg *msg = NULL;
    int uid = -1;
    int mid = -1;

    if (mx_extract_delete_message(jobj, &uid, &mid) != 0)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return mx_json_string_code_type(401, DELETE_MESSAGE);

    msg = mx_get_msg_by_id(connect->db, mid);
    if (msg->sender != uid)
        return mx_json_string_code_type(403, DELETE_MESSAGE);

    if (mx_delete_msg_by_id(connect->db, mid) != 0)
        return mx_json_string_code_type(500, DELETE_MESSAGE);

    return mx_json_string_code_type(201, DELETE_MESSAGE);
}
