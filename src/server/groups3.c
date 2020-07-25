#include "server.h"

char *mx_load_group_members(void *jobj, t_comm *connect) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    int uid = 0;
    int gid = 0;
    t_list *members = NULL;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_int) {
        uid = json_object_get_int(j_uid);
        gid = json_object_get_int(j_gid);
    }
    else
        return mx_bad_request(NULL, NULL);
    
    members = mx_get_all_group_members(connect->db, gid);
    return mx_json_str_builder_get_members(connect->db, members, gid);
}

int mx_validate_invite(sqlite3 *db, json_object *arr, int gid) {
    t_list *members = mx_get_all_group_members(db, gid);
    t_list *head = NULL;
    t_gr_members *member = NULL;
    int id = 0;

    if (!members)
        return -2;
    for (size_t i = 0; i < json_object_array_length(arr); i++) {
        head = members;
        id = json_object_get_int(json_object_array_get_idx(arr, i));

        while (head) {
            member = head->data;
            if (member->user_id == id)
                return -1;
            head = head->next;
        }
    }

    return 0;
}
