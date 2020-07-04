#include "client.h"

char *mx_json_string_login_signup(enum e_types type, char *log, char *passw) {
    json_object *jobj = json_object_new_object();

    json_object_object_add(jobj, "type", json_object_new_int((int)type));
    json_object_object_add(jobj, "name", json_object_new_string(log));
    json_object_object_add(jobj, "passw", json_object_new_string(passw));

    return (char *)json_object_to_json_string(jobj);
}
