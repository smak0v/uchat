#include "server.h"

json_object *mx_unpack_addtogroup(json_object *jobj, int *gid, int *uid) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    json_object *j_add = NULL;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    json_object_object_get_ex(jobj, "add", &j_add);
    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_add) == json_type_array && j_add
        && json_object_get_type(j_uid) == json_type_int) {
        *gid = json_object_get_int(j_gid);
        *uid = json_object_get_int(j_uid);

        return j_add;
    }

    return NULL;
}
