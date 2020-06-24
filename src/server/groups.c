#include "server.h"

static char *add_to_group(sqlite3 *db, array_list *ids, int grp_id) {
    int *array = (int *)*(ids->array);

    for (size_t i = 0; i < ids->length; i++)
        if (mx_add_group_member(db, array[i], grp_id) == -1)
            return "{\"code\": 500}";

    return "{\"code\": 200}";
}

char *mx_add_to_group(void *jobj, sqlite3 *db, int fd) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    int gid = -1;
    array_list *uid = NULL;

    fd = 0;
    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);

    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_array) {
        gid = json_object_get_int(j_gid);
        uid = json_object_get_array(j_uid);
    }
    else
        return mx_bad_request(NULL, NULL, 0);

    return add_to_group(db, uid, gid);
}

char *mx_new_group(void *jobj, sqlite3 *db, int fd) {
    json_object *j_name = NULL;
    json_object *j_id = NULL;
    const char *name = NULL;
    array_list *id = NULL;
    int grp_id = -1;

    fd = 0;
    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "id", &j_id);
    if (j_name && j_id && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_id) == json_type_array) {
        name = json_object_get_string(j_name);
        id = json_object_get_array(j_id);
    }
    else
        return mx_bad_request(NULL, NULL, 0);
    if ((grp_id = mx_add_grp(db, (char *)name)) == -1)
        return "{\"code\": 500}";
    else
        return add_to_group(db, id, grp_id);
}
