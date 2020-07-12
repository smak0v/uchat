#include "server.h"

static char *send_group_message(t_msg *message, sqlite3 *db, int fd) {
    char *js_str = mx_json_string_msg(message);
    int *group_members = mx_get_all_id_group_members(db, message->group_id);
    int socket_fd = -1;

    mx_add_msg(db, message);

    for (int i = 0; group_members[i] != -1; i++) {
        socket_fd = mx_get_sock_by_user_id(db, group_members[i]);
        if (socket_fd != -1 && socket_fd != fd) {
            printf("Sending message to fd = %d\n", socket_fd);
            write(socket_fd, js_str, (sizeof(char) * strlen(js_str)));
        }
    }

    mx_strdel(&js_str);
    return mx_msg_json_builder(message);;
}

// Dialog_id == -1: message is a group message; Dialog_id == -2: message is a
// private message, but the chat doesn't exist yet;
static char *send_private_message(t_msg *msg, sqlite3 *db) {
    int d_id = msg->dialog_id;
    int socket_fd = -1;
    char *json_string = NULL;

    if (msg->dialog_id == -2) {
        if ((d_id = mx_add_dialog(db, msg->sender, msg->recepient)) == -1)
            return "{\"code\": 500}";
        msg->dialog_id = d_id;
    }

    mx_add_msg(db, msg);
    if ((socket_fd = mx_get_sock_by_user_id(db, msg->recepient)) != -1) {
        json_string = mx_json_string_msg(msg);
        write(socket_fd, json_string, (sizeof(char) * strlen(json_string)));
        mx_strdel(&json_string);
    }
    return mx_msg_json_builder(msg);;
}

char *mx_send_message(void *jobj, t_comm *connect) {
    t_msg *message = mx_extract_message(jobj);
    char *res = NULL;

    if (!message)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, message->sender, (json_object *)jobj))
        return "{\"code\": 401}";
    if (message->group_id != -1)
        res = send_group_message(message, connect->db, connect->fd);
    else
        res = send_private_message(message, connect->db);

    if (message->file)
        mx_recv_file(connect->ssl, message->file);

    return res;
}

char *mx_edit_message(void *jobj, t_comm *connect) {
    int uid = -1;
    int msg_id = -1;
    char *msg = NULL;

    if (mx_extract_edit_msg(jobj, &uid, &msg_id, &msg) == -1)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    if (mx_update_msg_by_id(connect->db, msg, msg_id) == -1)
        return "{\"code\": 500}";

    return "{\"code\": 200}";
}

char *mx_delete_message(void *jobj, t_comm *connect) {
    t_msg *msg = NULL;
    int uid = -1;
    int mid = -1;

    if (mx_extract_delete_message(jobj, &uid, &mid) != 0)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    msg = mx_get_msg_by_id(connect->db, mid);

    if (msg->sender != uid)
        return "{\"code\": 403}";

    if (mx_delete_msg_by_id(connect->db, mid) != 0)
        return "{\"code\": 500}";

    return "{\"code\": 201}";
}
