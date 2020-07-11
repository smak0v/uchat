#include "client.h"

char *mx_json_string_load_dialogs_groups(enum e_types type, char *token,
    int uid) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type", json_object_new_int((int)type));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "id", json_object_new_int(uid));

    return (char *)json_object_to_json_string(jobj);
}
