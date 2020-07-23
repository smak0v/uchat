#include "server.h"

void mx_fill_array_int(json_object *jobj, int *arr, int len) {
    json_object *j_int = NULL;

    for (int i = 0; i < len; i++) {
        j_int = json_object_new_int(arr[i]);
        json_object_array_add(jobj, j_int);
    }
}

void mx_fill_array_str(json_object *jobj, char **arr, int len) {
    json_object *j_int = NULL;

    for (int i = 0; i < len; i++) {
        j_int = json_object_new_string(arr[i]);
        json_object_array_add(jobj, j_int);
    }
}

json_object *mx_json_builder_msg(t_msg *msg) {
    json_object *jobj = json_object_new_object();
    char *s_msg = msg->msg == NULL ? "" : msg->msg;
    char *file = msg->file == NULL ? "" : msg->file;

    json_object_object_add(jobj, "type", json_object_new_int(S_MES));
    json_object_object_add(jobj, "gid", json_object_new_int(msg->group_id));
    json_object_object_add(jobj, "did", json_object_new_int(msg->dialog_id));
    json_object_object_add(jobj, "mid", json_object_new_int(msg->id));
    json_object_object_add(jobj, "uid", json_object_new_int(msg->sender));
    json_object_object_add(jobj, "uid2", json_object_new_int(msg->recepient));
    json_object_object_add(jobj, "msg", json_object_new_string(s_msg));
    json_object_object_add(jobj, "time", json_object_new_int(msg->time));
    json_object_object_add(jobj, "file", json_object_new_string(file));
    json_object_object_add(jobj, "nme", json_object_new_string(msg->username));
    if (msg->forwarded)
        mx_j_o_o_a(jobj, "frwd", json_object_new_string(msg->forwarded));
    else
        mx_j_o_o_a(jobj, "frwd", json_object_new_null());

    return jobj;
}

void mx_fill_array_msg(json_object *jobj, t_list *msg_list) {
    json_object *j_msg = NULL;
    t_msg *msg = NULL;

    while (msg_list) {
        msg = msg_list->data;
        j_msg = mx_json_builder_msg(msg);
        json_object_array_add(jobj, j_msg);
        msg_list = msg_list->next;
    }
}

void mx_fill_array_user(json_object *jobj, t_list *user_list) {
    json_object *j_user = NULL;
    t_user *user = NULL;

    while (user_list) {
        user = user_list->data;
        j_user = json_object_new_object();
        mx_j_o_o_a(j_user, "id", json_object_new_int(user->user_id));
        mx_j_o_o_a(j_user, "name", json_object_new_string(user->user_login));
        json_object_array_add(jobj, j_user);
        user_list = user_list->next;
    }
}
