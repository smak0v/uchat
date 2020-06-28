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

// {"type": "S_MES", "gid": -1, "did": -2, "uid": 2, "uid2": 1, "msg": "Hello I'm Geralt", "time": 3819524, "file": ""}

static char *create_json_string(t_msg *message) {
    char *gid = mx_itoa(message->group_id);
    char *did = mx_itoa(message->dialog_id);
    char *uid = mx_itoa(message->sender);
    char *uid2 = mx_itoa(message->recepient);
    char *msg = mx_str_builder(message->msg);
    char *time = mx_itoa(message->time);
    char *file = mx_str_builder(message->file);
    char *str = mx_json_builder(16, "\"type\":", "\"S_MES\"", "\"gid\":", gid,
                                "\"did\":", did, "\"uid\":", uid, "\"uid2\":",
                                uid2, "\"msg\":", msg, "\"time\":", time,
                                "\"file\":", file);

    mx_strdel(&gid);
    mx_strdel(&did);
    mx_strdel(&uid);
    mx_strdel(&uid2);
    mx_strdel(&msg);
    mx_strdel(&time);
    mx_strdel(&file);
    return str;
}

static char *send_group_message(t_msg *message, sqlite3 *db, int fd) {
    printf("1\n");
    char *js_str = create_json_string(message);
    printf("2\n");
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
    json_string = create_json_string(msg);
    if ((socket_fd = mx_get_sock_by_user_id(db, msg->recepient)) != -1)
        write(socket_fd, json_string, (sizeof(char) * strlen(json_string)));
    else {
        mx_print_db(db, "SOCKETS");
    }
    return "{\"code\": 200}";
}

char *mx_send_message(void *jobj, t_comm *connect) {
    t_msg *message = mx_extract_message(jobj);

    if (!message)
        return mx_bad_request(NULL, NULL);

    if (message->group_id != -1)
        return send_group_message(message, connect->db, connect->fd);
    else
        return send_private_message(message, connect->db);
}

