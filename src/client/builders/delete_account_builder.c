#include "client.h"

char *mx_json_string_delete_account(char *token, int uid) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type",
        json_object_new_int((int)DELETE_USER));
    json_object_object_add(jobj, "tok", json_object_new_string(token));
    json_object_object_add(jobj, "id", json_object_new_int(uid));

    return (char *)json_object_to_json_string(jobj);
}
