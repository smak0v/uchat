#include "server.h"

static void fill_array_group_mem(sqlite3 *db, json_object *jobj, t_list *list) {
    json_object *j_user = NULL;
    t_gr_members *member= NULL;
    t_user *user = NULL;

    while (list) {
        member = list->data;
        user = mx_get_user_by_user_id(db, member->user_id);
        j_user = json_object_new_object();
        mx_j_o_o_a(j_user, "id", json_object_new_int(user->user_id));
        mx_j_o_o_a(j_user, "name", json_object_new_string(user->user_login));
        json_object_array_add(jobj, j_user);
        list = list->next;
    }
}

char *mx_json_str_builder_get_members(sqlite3 *db, t_list *list, int gid) {
    json_object *jobj = json_object_new_object();
    json_object *j_arr = NULL;

    mx_j_o_o_a(jobj, "type", json_object_new_int(LOAD_GR_MEMBERS));
    mx_j_o_o_a(jobj, "code", json_object_new_int(200));
    mx_j_o_o_a(jobj, "gid", json_object_new_int(gid));
    if (!list)
        mx_j_o_o_a(jobj, "mem", json_object_new_null());
    else {
        j_arr = json_object_new_array();
        fill_array_group_mem(db, j_arr, list);
        mx_j_o_o_a(jobj, "mem", j_arr);
    }
    return (char *)json_object_to_json_string(jobj);
}
