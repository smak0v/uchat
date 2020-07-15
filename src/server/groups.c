#include "server.h"

static char *add_to_group(sqlite3 *db, json_object *arr, int gr_id, int adm) {
    json_object *id = NULL;

    for (size_t i = 0; i < json_object_array_length(arr); i++) {
        id = json_object_array_get_idx(arr, i);
        if (json_object_get_type(id) != json_type_int)
            return NULL;
        if (mx_add_group_member(db, json_object_get_int(id), gr_id, adm) == -1)
            return NULL;
    }

    return mx_json_string_add_to_gr(gr_id);
}

static int extract_data(json_object *jobj, char **name, int *gid, int *uid) {
    json_object *j_name = NULL;
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_name && j_gid && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_gid) == json_type_int && j_uid
        && json_object_get_type(j_uid) == json_type_int) {
        *name = (char *)json_object_get_string(j_name);
        *gid = json_object_get_int(j_gid);
        *uid = json_object_get_int(j_uid);
        return 0;
    }
    return 1;
}

char *mx_rename_group(void *jobj, t_comm *connect) {
    const char *name = NULL;
    int grp_id = -1;
    int uid = -1;

    if (extract_data(jobj, (char **)&name, &grp_id, &uid) != 0)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    if (mx_rename_grp_by_id(connect->db, grp_id, (char *)name) == -1)
        return "{\"code\": 500}";

    mx_notify_group_renamed(connect->db, grp_id, (char *)name);

    return "{\"code\": 200}";
}

char *mx_add_to_group(void *jobj, t_comm *connect) {
    int gid = -1;
    int uid = -1;
    json_object *j_add = mx_unpack_addtogroup((json_object *)jobj, &gid, &uid);
    char *res = NULL;

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";
    res = add_to_group(connect->db, j_add, gid, 0);

    if (res)
        mx_notify_add_to_group(connect->db, j_add, gid);

    return res;
}

char *mx_new_group(void *jobj, t_comm *connect) {
    json_object *j_name = NULL;
    json_object *j_id = NULL;
    const char *name = NULL;
    int grp_id = -1;
    int uid = -1;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_name && j_id && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_id) == json_type_array)
        name = json_object_get_string(j_name);
    else
        return mx_bad_request(NULL, NULL);
    j_name = json_object_array_get_idx(j_id, 0);
    uid = json_object_get_int(j_name);
    if (mx_validate_token(connect->db, uid, (json_object *)jobj) != 0)
        return "{\"code\": 401}";
    if ((grp_id = mx_add_grp(connect->db, (char *)name)) == -1)
        return "{\"code\": 500}";
    return add_to_group(connect->db, j_id, grp_id, 1);
}
