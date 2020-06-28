#include "server.h"

static char *add_to_group(sqlite3 *db, json_object *arr, int gr_id, int adm) {
    char *grp_id_str = mx_itoa(gr_id);
    json_object *id = NULL;

    for (size_t i = 0; i < json_object_array_length(arr); i++) {
        id = json_object_array_get_idx(arr, i);
        if (json_object_get_type(id) != json_type_int)
            return mx_bad_request(NULL, NULL);
        if (mx_add_group_member(db, json_object_get_int(id), gr_id, adm) == -1)
            return "{\"code\": 500}";
    }

    return mx_json_builder(4, "\"code\":", "200", "\"id\":", grp_id_str);;
}

char *mx_rename_group(void *jobj, t_comm *connect) {
    json_object *j_name = NULL;
    json_object *j_id = NULL;
    const char *name = NULL;
    int grp_id = -1;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_name && j_id && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_id) == json_type_int) {
        name = json_object_get_string(j_name);
        grp_id = json_object_get_int(j_id);
    }
    else
        return mx_bad_request(NULL, NULL);

    if (mx_rename_grp_by_id(connect->db, grp_id, (char *)name) == -1)
        return "{\"code\": 500}";
    //send the update to all online members of the group
    return "{\"code\": 200}";
}

char *mx_add_to_group(void *jobj, t_comm *connect) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    int gid = -1;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);

    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_array)
        gid = json_object_get_int(j_gid);
    else
        return mx_bad_request(NULL, NULL);

    return add_to_group(connect->db, j_uid, gid, 0);
}

char *mx_new_group(void *jobj, t_comm *connect) {
    json_object *j_name = NULL;
    json_object *j_id = NULL;
    const char *name = NULL;
    int grp_id = -1;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_name && j_id && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_id) == json_type_array)
        name = json_object_get_string(j_name);
    else
        return mx_bad_request(NULL, NULL);
    if ((grp_id = mx_add_grp(connect->db, (char *)name)) == -1)
        return "{\"code\": 500}";
    else
        return add_to_group(connect->db, j_id, grp_id, 1);
}
