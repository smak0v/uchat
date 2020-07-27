#include "server.h"

int mx_j_o_o_a(json_object *jso, const char *key, json_object *val) {
    return json_object_object_add(jso, key, val);
}

enum json_type mx_j_o_g_t(json_object *jso) {
    return json_object_get_type(jso);
}

char *mx_j_s_c_t(int code, int type) {
    return mx_json_string_code_type(code, type);
}
