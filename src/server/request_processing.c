#include "server.h"

char *mx_bad_request(void *jobj, t_list **clients, sqlite3 *db, int fd) {
    jobj = NULL;
    clients = NULL;
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
    // else if (!mx_strcmp(type, "NEW_GROUP"))
    //     return mx_new_group;
    // else if (!mx_strcmp(type, "INVITE"))
    //     return mx_invite_to_group;
    else
        return mx_bad_request;
}

char *mx_process_request(char *request, t_list **clients, sqlite3 *db, int fd) {
    json_object *jobj = json_tokener_parse(request);
    const char *type = NULL;

    if (json_object_get_type(jobj) == json_type_object) {
        // json_object_object_get_ex(object, "security-code", &secu_code);
        json_object_object_foreach(jobj, key, val) {
            if (json_object_get_type(val) == json_type_string
                && !mx_strcmp(key, "type"))
                type = json_object_get_string(val);
            else
                return mx_bad_request(NULL, NULL, NULL, 0);
            break;
        }
    }
    else
        return mx_bad_request(NULL, NULL, NULL, 0);

    return mx_select_method(type)((void *)jobj, clients, db, fd);
}
