#include "server.h"

static int parse_request(void *jobj, int *uid, int *gid, int *did) {
    json_object *j_uid = NULL;
    json_object *j_gid = NULL;
    json_object *j_did = NULL;
    json_object *j_time = NULL;
    int time = -1;

    json_object_object_get_ex((json_object *)jobj, "uid", &j_uid);
    json_object_object_get_ex((json_object *)jobj, "gid", &j_gid);
    json_object_object_get_ex((json_object *)jobj, "did", &j_did);
    json_object_object_get_ex((json_object *)jobj, "time", &j_time);
    if (!j_uid || !j_gid || !j_did || mx_j_o_g_t(j_uid) != json_type_int
        || mx_j_o_g_t(j_gid) != json_type_int || !j_time
        || mx_j_o_g_t(j_did) != json_type_int
        || mx_j_o_g_t(j_time) != json_type_int)
        return -1;
    *uid = json_object_get_int(j_uid);
    *gid = json_object_get_int(j_gid);
    *did = json_object_get_int(j_did);
    time = json_object_get_int(j_time);
    return time;
}

static char *process_t_list(t_list *m_list) {
    json_object *jobj = json_object_new_object();
    json_object *arr = json_object_new_array();
    char *res = NULL;

    json_object_object_add(jobj, "code", json_object_new_int(200));
    mx_fill_array_msg(arr, m_list);
    mx_j_o_o_a(jobj, "msg", arr);
    res = (char *)json_object_to_json_string(jobj);

    return res;
}

char *mx_load_messages(void *jobj, t_comm *connect) {
    t_list *m_list = NULL;
    int time = 0;
    int uid = 0;
    int gid = 0;
    int did = 0;

    if ((time = parse_request(jobj, &uid, &gid, &did)) == -1)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    m_list = mx_db_load_next_messages(connect->db, gid, did, 20, time);

    return process_t_list(m_list);
}
