#include "server.h"

static json_object *create_profile_object(t_profile *prof) {
    json_object *j_prof = json_object_new_object();

    mx_j_o_o_a(j_prof, "uid", json_object_new_int(prof->user_id));
    mx_j_o_o_a(j_prof, "name", json_object_new_string(prof->name));
    mx_j_o_o_a(j_prof, "dob", json_object_new_string(prof->birth));
    mx_j_o_o_a(j_prof, "mail", json_object_new_string(prof->email));
    mx_j_o_o_a(j_prof, "status", json_object_new_string(prof->status));
    mx_j_o_o_a(j_prof, "country", json_object_new_string(prof->country));

    return j_prof;
}

static char *json_str_builder(t_profile *prof) {
    json_object *jobj = json_object_new_object();
    json_object *j_prof = NULL;

    mx_j_o_o_a(j_prof, "code", json_object_new_int(200));
    if (!prof)
        mx_j_o_o_a(jobj, "prof", json_object_new_null());
    else
        mx_j_o_o_a(jobj, "prof", create_profile_object(prof));

    return (char *)json_object_to_json_string(jobj);
}

// TODO: REFACTOR
char *mx_get_user(void *jobj, t_comm *connect) {
    json_object *j_name = NULL;
    json_object *j_uid = NULL;
    char *name = NULL;
    int uid = -1;
    t_profile *prof = NULL;
    t_user *user = NULL;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "name", &j_uid);
    if (j_name && j_uid && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_uid) == json_type_int) {
        name = (char *)json_object_get_string(j_name);
        uid = json_object_get_int(j_uid);
    }
    else
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    if ((user = mx_get_user_by_login(connect->db, name)) == NULL)
        return "{\"code\": 500}";
    prof = mx_get_profile_by_id(connect->db, user->user_id);

    return json_str_builder(prof);
}
