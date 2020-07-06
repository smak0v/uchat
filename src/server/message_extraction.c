#include "server.h"

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
    code = (!message->file && !message->msg) ? -1 : code;
    return code;
}

static void extract_entry(t_msg *message, char *key, json_object *val) {
    if (!mx_strcmp(key, "gid") && mx_j_o_g_t(val) == json_type_int)
        message->group_id = json_object_get_int(val);
    else if (!mx_strcmp(key, "did") && mx_j_o_g_t(val) == json_type_int)
        message->dialog_id = json_object_get_int(val);
    else if (!mx_strcmp(key, "uid") && mx_j_o_g_t(val) == json_type_int)
        message->sender = json_object_get_int(val);
    else if (!mx_strcmp(key, "uid2") && mx_j_o_g_t(val) == json_type_int)
        message->recepient = json_object_get_int(val);
    else if (!mx_strcmp(key, "msg") && mx_j_o_g_t(val) == json_type_string)
        message->msg = (char *)json_object_get_string(val);
    else if (!mx_strcmp(key, "time") && mx_j_o_g_t(val) == json_type_int)
        message->time = json_object_get_int(val);
    else if (!mx_strcmp(key, "file") && mx_j_o_g_t(val) == json_type_string)
        message->file = (char *)json_object_get_string(val);
    else if (!mx_strcmp(key, "frwd") && mx_j_o_g_t(val) == json_type_string)
        message->forwarded = (char *)json_object_get_string(val);
    else if (!mx_strcmp(key, "frwd") && mx_j_o_g_t(val) == json_type_null)
        message->forwarded = NULL;
}

t_msg *mx_extract_message(void *jobj) {
    t_msg *message = mx_memalloc(sizeof(t_msg));

    json_object_object_foreach((json_object *)jobj, key, val) {
        extract_entry(message, key, val);
    }

    message->read = 0;
    message->edited = 0;

    if (validate_message(message) == -1)
        return NULL;
    else
        return message;
}

int mx_extract_edit_msg(json_object *jobj, int *uid, int *mid, char **msg) {
    json_object *j_uid = NULL;
    json_object *j_mid = NULL;
    json_object *j_msg = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "uid", &j_uid);
        json_object_object_get_ex(jobj, "msg", &j_msg);
        json_object_object_get_ex(jobj, "mid", &j_mid);

        if (j_uid && j_mid && json_object_get_type(j_uid) == json_type_int
            && json_object_get_type(j_mid) == json_type_int && j_msg
            && json_object_get_type(j_msg) == json_type_string) {
                *uid = json_object_get_int(j_uid);
                *mid = json_object_get_int(j_mid);
                *msg = (char *)json_object_get_string(j_msg);
                return 0;
            }
    }

    return -1;
}

int mx_extract_delete_message(json_object *jobj, int *uid, int *mid) {
    json_object *j_uid = NULL;
    json_object *j_mid = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "uid", &j_uid);
        json_object_object_get_ex(jobj, "mid", &j_mid);

        if (j_uid && j_mid && json_object_get_type(j_uid) == json_type_int
            && json_object_get_type(j_mid) == json_type_int) {
                *uid = json_object_get_int(j_uid);
                *mid = json_object_get_int(j_mid);
                return 0;
            }
    }

    return -1;
}
