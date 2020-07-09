#include "client.h"

char *mx_json_string_new_group(char *token, int uid, char *group_name) {
    json_object *jobj = json_object_new_object();
    json_object *jarray = json_object_new_array();

    json_object_object_add(jobj, "type", json_object_new_int((int)N_GRP));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "name", json_object_new_string(group_name));

    json_object_array_add(jarray, json_object_new_int(uid));

    json_object_object_add(jobj, "id", jarray);

    return (char *)json_object_to_json_string(jobj);
}
