#include "server.h"

char *mx_delete_user(void *jobj, t_comm *connect) {
    json_object *j_id = NULL;
    int uid = -1;
    t_profile *prof = NULL;

    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_id && json_object_get_type(j_id) == json_type_int)
        uid = json_object_get_int(j_id);
    else
        return mx_bad_request(NULL, NULL);

    if (mx_change_user_username(connect->db, uid, "Deleted User"))
        return mx_json_string_code_type(500, DELETE_USER);
    prof = mx_memalloc(sizeof(t_profile));
    prof->name = mx_strdup("Deleted User");
    if (mx_change_profile_by_id(connect->db, prof, uid) == -1)
        return mx_json_string_code_type(500, DELETE_USER);

    return mx_json_string_code_type(200, DELETE_USER);
}

