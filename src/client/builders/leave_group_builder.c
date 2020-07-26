#include "client.h"

char *mx_json_string_leave_group(char *token, int uid, int gid) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type",
        json_object_new_int((int)LEAVE_GROUP));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "uid", json_object_new_int(uid));
    json_object_object_add(jobj, "gid", json_object_new_int(gid));

    return (char *)json_object_to_json_string(jobj);
}
