#include "server.h"

// struct s_msg {
// 	int id;
// 	int group_id;
// 	int dialog_id;
// 	int sender;
// 	int recepient;
// 	char *msg;
// 	int time;
// 	bool edited;
// 	bool read;
// 	char *file;
// };

static char *send_group_message(t_msg *message, sqlite3 *db) {
    char *json_string = NULL;//create_json_string(message);
    int *group_members = mx_get_all_id_group_members(db, message->group_id);
    int socket_fd = -1;

    mx_add_msg(db, message);

    for (int i = 0; group_members[i] != -1; i++) {
        socket_fd = mx_get_sock_by_user_id(db, group_members[i]);
        if (socket_fd != -1)
            write(socket_fd, json_string, (sizeof(char) * strlen(json_string)));
    }

    return "{\"code\": 200}";
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
    json_string = msg->msg;//create_json_string(msg);
    if ((socket_fd = mx_get_sock_by_user_id(db, msg->recepient)) != -1) {
        printf("About to send message to the user with fd = %d:\n%s\n", socket_fd, msg->msg);
        write(socket_fd, json_string, (sizeof(char) * strlen(json_string)));
        // mx_send_response(json_string, socket_fd);
        }
    else {
        mx_print_db(db, "SOCKETS");
    }

    return "{\"code\": 200}";
}

static int validate_message(t_msg *message) {
    int code = 0;

    if (message->group_id == 0)
        code = -1;
    else if (message->dialog_id == 0)
        code = -1;
    else if (message->sender == 0)
        code = -1;
    else if (message->recepient == 0)
        code = -1;
    else if (message->msg == NULL)
        code = -1;
    else if (message->time == 0)
        code = -1;
    else if (message->file == NULL)
        code = -1;

    message->msg = !mx_strcmp(message->msg, "") ? NULL : message->msg;
    message->file = !mx_strcmp(message->file, "") ? NULL : message->file;
    return code;
}

static void extract_entry(t_msg *message, char *key, json_object *val) {
    if (!mx_strcmp(key, "gid") && json_object_get_type(val) == json_type_int)
        message->group_id = json_object_get_int(val);
    else if (!mx_strcmp(key, "did")
                && json_object_get_type(val) == json_type_int)
        message->dialog_id = json_object_get_int(val);
    else if (!mx_strcmp(key, "uid")
                && json_object_get_type(val) == json_type_int)
        message->sender = json_object_get_int(val);
    else if (!mx_strcmp(key, "uid2")
            && json_object_get_type(val) == json_type_int)
        message->recepient = json_object_get_int(val);
    else if (!mx_strcmp(key, "msg")
        && json_object_get_type(val) == json_type_string)
        message->msg = (char *)json_object_get_string(val);
    else if (!mx_strcmp(key, "time")
             && json_object_get_type(val) == json_type_int)
        message->time = json_object_get_int(val);
    else if (!mx_strcmp(key, "file")
            && json_object_get_type(val) == json_type_string)
        message->file = (char *)json_object_get_string(val);
}

static t_msg *extract_message(json_object *jobj) {
    t_msg *message = mx_memalloc(sizeof(t_msg));

    json_object_object_foreach(jobj, key, val) {
        extract_entry(message, key, val);
    }

    message->read = 0;
    message->edited = 0;

    return message;
}

char *mx_send_message(void *jobj, t_comm *connect) {
    t_msg *message = extract_message(jobj);

    if (!message || validate_message(message) == -1)
        return mx_bad_request(NULL, NULL);

    if (message->group_id != -1)
        return send_group_message(message, connect->db);
    else
        return send_private_message(message, connect->db);
}

