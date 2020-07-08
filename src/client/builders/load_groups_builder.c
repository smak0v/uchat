#include "client.h"

char *mx_json_string_load_groups(char *token, int uid) {
    json_object *jobj = json_object_new_object();

    // json_object_object_add(jobj, "type", json_object_new_int((int)type));
    // json_object_object_add(jobj, "tok", json_object_new_string(log));
    // json_object_object_add(jobj, "uid", json_object_new_string(passw));

    return (char *)json_object_to_json_string(jobj);
}
