#include "server.h"

static int parse_json(json_object *jobj, t_profile *prof) {
    json_object *j_tmp = NULL;
    int code = -1;

    json_object_object_get_ex(jobj, "name", &j_tmp);
    if (j_tmp && !(code = 0))
        prof->name = (char *)json_object_get_string(j_tmp);
    json_object_object_get_ex(jobj, "dob", &j_tmp);
    if (j_tmp && !(code = 0))
        prof->birth = (char *)json_object_get_string(j_tmp);
    json_object_object_get_ex(jobj, "mail", &j_tmp);
    if (j_tmp && !(code = 0))
        prof->email = (char *)json_object_get_string(j_tmp);
    json_object_object_get_ex(jobj, "status", &j_tmp);
    if (j_tmp && !(code = 0))
        prof->status = (char *)json_object_get_string(j_tmp);
    json_object_object_get_ex(jobj, "country", &j_tmp);
    if (j_tmp && !(code = 0))
        prof->country = (char *)json_object_get_string(j_tmp);

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
        return mx_json_string_code_type(404, EDIT_PROFILE);
    if (parse_json(jobj, new_prof) == -1)
        return mx_bad_request(NULL, NULL);
    if (mx_change_profile_by_id(connect->db, new_prof, uid) == -1)
        return mx_json_string_code_type(500, EDIT_PROFILE);

    return mx_json_string_code_type(200, EDIT_PROFILE);
}

char *mx_del_user(void *jobj, t_comm *connect) {
    jobj = NULL;
    connect = NULL;
    return NULL;
}
