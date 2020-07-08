#include "server.h"

char *mx_leave_group(void *jobj, t_comm *connect) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    int gid = -1;
    int uid = -1;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_int) {
        gid = json_object_get_int(j_gid);
        uid = json_object_get_int(j_uid);
    }
    else
        mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";
    if (mx_delete_user_from_group(connect->db, uid, gid) == -1)
        return "{\"code\": 500}";
    return "{\"code\": 200}";
}
