#include "server.h"

char *mx_bad_request(void *jobj, sqlite3 *db, int fd) {
    jobj = NULL;
    db = NULL;
    fd = 0;

    return "{\"code\": 400}";
}

api_function mx_select_method(const char *type) {
    if (!mx_strcmp(type, "REG"))
        return mx_register_user;
    else if (!mx_strcmp(type, "S_IN"))
        return mx_sign_in;
    else if (!mx_strcmp(type, "S_OUT"))
        return mx_sign_out;
    else if (!mx_strcmp(type, "N_GRP"))
        return mx_new_group;
    else if (!mx_strcmp(type, "INV"))
        return mx_add_to_group;
    else
        return mx_bad_request;
}

char *mx_process_request(char *request, sqlite3 *db, int fd) {
    json_object *jobj = json_tokener_parse(request);
    json_object *j_type = NULL;
    const char *type = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        json_object_object_get_ex(jobj, "type", &j_type);
        if (j_type && json_object_get_type(j_type) == json_type_string)
            type = json_object_get_string(j_type);
        else
            return mx_bad_request(NULL, NULL, 0);
    }
    else
        return mx_bad_request(NULL, NULL, 0);

    return mx_select_method(type)((void *)jobj, db, fd);
}
