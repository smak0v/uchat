#include "server.h"

int mx_j_o_o_a(json_object *jso, const char *key, json_object *val) {
    return json_object_object_add(jso, key, val);
}
