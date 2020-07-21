#include "server.h"

static char *json_str_builder_get(t_profile *prof) {
    json_object *jobj = json_object_new_object();

    mx_j_o_o_a(jobj, "type", json_object_new_int(GET_USER));
    mx_j_o_o_a(jobj, "code", json_object_new_int(200));
    if (!prof)
        mx_j_o_o_a(jobj, "prof", json_object_new_null());
    else
        mx_j_o_o_a(jobj, "prof", mx_create_profile_object(prof));

    return (char *)json_object_to_json_string(jobj);
}

static char *json_str_builder_find(t_list *users) {
    json_object *jobj = json_object_new_object();
    json_object *arr = NULL;

    mx_j_o_o_a(jobj, "type", json_object_new_int(FIND_USER));
    mx_j_o_o_a(jobj, "code", json_object_new_int(200));
    if (!users)
        mx_j_o_o_a(jobj, "users", json_object_new_null());
    else {
        arr = json_object_new_array();
        mx_fill_array_user(arr, users);
        mx_j_o_o_a(jobj, "users", arr);
    }
    return (char *)json_object_to_json_string(jobj);
}

static int parse_get_user(json_object *jobj, char **name, int *uid) {
    json_object *j_name = NULL;
    json_object *j_uid = NULL;

    json_object_object_get_ex(jobj, "name", &j_name);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_name && j_uid && json_object_get_type(j_name) == json_type_string
        && json_object_get_type(j_uid) == json_type_int) {
        *name = (char *)json_object_get_string(j_name);
        *uid = json_object_get_int(j_uid);
        return 0;
    }
    else
        return -1;
}

char *mx_get_user(void *jobj, t_comm *connect) {
    char *name = NULL;
    int uid = -1;
    t_profile *prof = NULL;
    t_user *user = NULL;

    if (parse_get_user((json_object *)jobj, &name, &uid))
        return mx_bad_request(NULL, NULL);
    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return mx_json_string_code_type(401, GET_USER);

    if ((user = mx_get_user_by_login(connect->db, name)) == NULL)
        return mx_json_string_code_type(401, GET_USER);

    prof = mx_get_profile_by_id(connect->db, user->user_id);

    return json_str_builder_get(prof);
}

char *mx_find_user(void *jobj, t_comm *connect) {
    char *name = NULL;
    int uid = -1;
    t_list *prof = NULL;

    if (parse_get_user((json_object *)jobj, &name, &uid))
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return mx_json_string_code_type(401, FIND_USER);

    prof = mx_find_user_by_char(connect->db, name);

    return json_str_builder_find(prof);
}
