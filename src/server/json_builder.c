#include "server.h"

char *mx_json_string_msg(t_msg *msg) {
    json_object *jobj = json_object_new_object();
    char *s_msg = msg->msg == NULL ? "" : msg->msg;
    char *file = msg->file == NULL ? "" : msg->file;

    json_object_object_add(jobj, "type", json_object_new_int(S_MES));
    json_object_object_add(jobj, "gid", json_object_new_int(msg->group_id));
    json_object_object_add(jobj, "did", json_object_new_int(msg->dialog_id));
    json_object_object_add(jobj, "uid", json_object_new_int(msg->sender));
    json_object_object_add(jobj, "uid2", json_object_new_int(msg->recepient));
    json_object_object_add(jobj, "msg", json_object_new_string(s_msg));
    json_object_object_add(jobj, "time", json_object_new_int(msg->time));
    json_object_object_add(jobj, "file", json_object_new_string(file));

    if (msg->forwarded)
        mx_j_o_o_a(jobj, "frwd", json_object_new_string(msg->forwarded));
    else
        mx_j_o_o_a(jobj, "frwd", json_object_new_null());

    return (char *)json_object_to_json_string(jobj);
}

char *mx_json_string_s_in(int uid, char *tok) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "code", json_object_new_int(200));
    json_object_object_add(jobj, "tok", json_object_new_string(tok));
    json_object_object_add(jobj, "uid", json_object_new_int(uid));

    return (char *)json_object_to_json_string(jobj);
}

char *mx_json_string_add_to_gr(int gid) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "code", json_object_new_int(200));
    json_object_object_add(jobj, "name", json_object_new_int(200));
    json_object_object_add(jobj, "id", json_object_new_int(gid));

    return (char *)json_object_to_json_string(jobj);
}

char *mx_json_string_load_dlg(t_ld_d *arrs, int len) {
    json_object *jobj = json_object_new_object();
    json_object *arr = NULL;

    json_object_object_add(jobj, "code", json_object_new_int(200));
    json_object_object_add(jobj, "len", json_object_new_int(len));
    if (len) {
        arr = json_object_new_array();
        mx_fill_array_int(arr, arrs->dialog_id, len);
        mx_j_o_o_a(jobj, "did", arr);

        arr = json_object_new_array();
        mx_fill_array_int(arr, arrs->user_id, len);
        mx_j_o_o_a(jobj, "uid", arr);

        arr = json_object_new_array();
        mx_fill_array_str(arr, arrs->username, len);
        mx_j_o_o_a(jobj, "names", arr);
    }

    return (char *)json_object_to_json_string(jobj);
}

char *mx_json_string_load_grp(t_ld_d *arrs, int len) {
    json_object *jobj = json_object_new_object();
    json_object *arr = NULL;

    json_object_object_add(jobj, "code", json_object_new_int(200));
    json_object_object_add(jobj, "len", json_object_new_int(len));
    if (len) {
        arr = json_object_new_array();
        mx_fill_array_int(arr, arrs->dialog_id, len);
        mx_j_o_o_a(jobj, "gid", arr);

        arr = json_object_new_array();
        mx_fill_array_str(arr, arrs->username, len);
        mx_j_o_o_a(jobj, "names", arr);
    }

    return (char *)json_object_to_json_string(jobj);
}
