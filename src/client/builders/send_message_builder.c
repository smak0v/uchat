#include "client.h"

static void get_file_and_frwd(json_object *jobj, t_msg *msg) {
    if (msg->file)
        json_object_object_add(jobj, "file",
            json_object_new_string(msg->file));
    else
        json_object_object_add(jobj, "file", json_object_new_string(""));

    if (msg->fwrd)
        json_object_object_add(jobj, "frwd",
            json_object_new_string(msg->fwrd));
    else
        json_object_object_add(jobj, "frwd", json_object_new_null());
}

static void get_dgid_uuid2(json_object *jobj, t_msg *msg, t_glade *g) {
    if (g->group) {
        json_object_object_add(jobj, "gid", json_object_new_int(msg->dgid));
        json_object_object_add(jobj, "did", json_object_new_int(-1));
        json_object_object_add(jobj, "uid2", json_object_new_int(-1));
    }
    else {
        json_object_object_add(jobj, "gid", json_object_new_int(-1));
        json_object_object_add(jobj, "did", json_object_new_int(-2));
        json_object_object_add(jobj, "uid2", json_object_new_int(msg->uid2));
    }
}

char *mx_json_string_send_message(t_glade *g, t_msg *msg) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type", json_object_new_int((int)S_MES));
    json_object_object_add(jobj, "uid", json_object_new_int(g->uid));
    json_object_object_add(jobj, "tok", json_object_new_string(g->token));
    json_object_object_add(jobj, "msg", json_object_new_string(msg->msg));
    json_object_object_add(jobj, "time", json_object_new_int(msg->time));

    get_file_and_frwd(jobj, msg);
    get_dgid_uuid2(jobj, msg, g);

    return (char *)json_object_to_json_string(jobj);
}
