#include "client.h"

char *mx_json_string_send_message() {
    json_object *jobj = json_object_new_object();

    return (char *)json_object_to_json_string(jobj);
}
