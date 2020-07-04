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
