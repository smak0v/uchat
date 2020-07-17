#include "server.h"

char *mx_del_user(void *jobj, t_comm *connect) {
    json_object *j_id = NULL;
    int uid = -1;

    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_id && json_object_get_type(j_id) == json_type_int)
        uid = json_object_get_int(j_id);
    else
        return mx_bad_request(NULL, NULL);

    return mx_json_string_code_type(200, DELETE_USER);
}
