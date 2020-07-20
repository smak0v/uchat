#include "client.h"

char *mx_json_string_search_user(char *token, int uid, char *name) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type", json_object_new_int((int)FIND_USER));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "uid", json_object_new_int(uid));
    json_object_object_add(jobj, "name", json_object_new_string(name));

    return (char *)json_object_to_json_string(jobj);
}

