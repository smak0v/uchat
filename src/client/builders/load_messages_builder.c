#include "client.h"

char *mx_json_string_load_messages(t_glade *g, int time, int dgid,
    bool group) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type",
        json_object_new_int((int)LOAD_MESSAGES));

    json_object_object_add(jobj, "uid", json_object_new_int(g->uid));
    json_object_object_add(jobj, "tok", json_object_new_string(g->token));
    json_object_object_add(jobj, "time", json_object_new_int(time));

    if (group) {
        json_object_object_add(jobj, "gid", json_object_new_int(dgid));
        json_object_object_add(jobj, "did", json_object_new_int(-1));
    }
    else {
        json_object_object_add(jobj, "gid", json_object_new_int(-1));
        json_object_object_add(jobj, "did", json_object_new_int(dgid));
    }

    return (char *)json_object_to_json_string(jobj);
}
