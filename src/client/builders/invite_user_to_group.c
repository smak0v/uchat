#include "client.h"

char *mx_json_string_invite_user_to_group(char *token, int uid, int gid,
    int uid2) {
    json_object *jobj = json_object_new_object();
    json_object *j_arr = json_object_new_array();
    json_object *j_int = json_object_new_int(uid2);

    json_object_array_add(j_arr, j_int);

    json_object_object_add(jobj, "type", json_object_new_int((int)INV));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "uid", json_object_new_int(uid));
    json_object_object_add(jobj, "gid", json_object_new_int(gid));
    json_object_object_add(jobj, "add", j_arr);

    return (char *)json_object_to_json_string(jobj);
}
