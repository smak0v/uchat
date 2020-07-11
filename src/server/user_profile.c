#include "server.h"

static int parse_json(json_object *jobj, t_profile *prof) {
    int code = -1;

    json_object_object_foreach(jobj, key, val) {
        if (mx_strcmp(key, "name") && !(code = 0))
            prof->name = (char *)json_object_get_string(val);
        else if (mx_strcmp(key, "dob") && !(code = 0))
            prof->birth = (char *)json_object_get_string(val);
        else if (mx_strcmp(key, "mail") && !(code = 0))
            prof->email = (char *)json_object_get_string(val);
        else if (mx_strcmp(key, "status") && !(code = 0))
            prof->status = (char *)json_object_get_string(val);
        else if (mx_strcmp(key, "country") && !(code = 0))
            prof->country = (char *)json_object_get_string(val);
    }

    return code;
}

char *mx_edit_profile(void *jobj, t_comm *connect) {
    t_profile *new_prof = NULL;
    json_object *j_uid = NULL;
    int uid = 0;

    json_object_object_get_ex((json_object *)jobj, "id", &j_uid);
    if (j_uid && json_object_get_type(j_uid) == json_type_int)
        uid = json_object_get_int(j_uid);
    else
        return mx_bad_request(NULL, NULL);

    if ((new_prof = mx_get_profile_by_id(connect->db, uid)) == NULL)
        return "{\"code\": 404}";
    if (parse_json(jobj, new_prof) == -1)
        return mx_bad_request(NULL, NULL);
    if (mx_change_profile_by_id(connect->db, new_prof, uid) == -1)
        return "{\"code\": 500}";

    return "{\"code\": 200}";
}

char *mx_del_user(void *jobj, t_comm *connect) {
    jobj = NULL;
    connect = NULL;
    return NULL;
}
