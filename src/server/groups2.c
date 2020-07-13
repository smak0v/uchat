#include "server.h"

static int unpack_json(json_object *jobj, int *gid, int *uid) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_int) {
        *gid = json_object_get_int(j_gid);
        *uid = json_object_get_int(j_uid);
        return 0;
    }
    else
        return -1;
}

static int delete_messages(sqlite3 *db, int gid) {
    t_list *m_lst = mx_db_load_messages(db, gid, -1, 0);
    t_msg *msg = NULL;

    while (m_lst) {
        msg = m_lst->data;
        if (mx_delete_msg_by_id(db, msg->id) == -1)
            return -1;
        m_lst = m_lst->next;
    }

    return 0;
}

static int delete_members_and_group(sqlite3 *db, int gid) {
    int *members = mx_get_all_id_group_members(db, gid);

    for (int i = 0; members[i] != -1; i++) {
        // Notify online clients?
        if (mx_delete_user_from_group(db, members[i], gid) != 0)
            return -1;
    }

    if (mx_delete_grp_by_id(db, gid) == -1)
        return -1;

    return 0;
}

char *mx_del_group(void *jobj, t_comm *connect) {
    int gid = -1;
    int uid = -1;

    if (unpack_json((json_object *)jobj, &gid, &uid) != 0)
        return mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    // Check admin status
    // 403

    if (delete_messages(connect->db, gid) != 0)
        return "{\"code\": 500}";

    if (delete_members_and_group(connect->db, gid) != 0)
        return "{\"code\": 500}";

    return "{\"code\": 200}";
}

char *mx_leave_group(void *jobj, t_comm *connect) {
    json_object *j_gid = NULL;
    json_object *j_uid = NULL;
    int gid = -1;
    int uid = -1;

    json_object_object_get_ex(jobj, "gid", &j_gid);
    json_object_object_get_ex(jobj, "uid", &j_uid);
    if (j_gid && j_uid && json_object_get_type(j_gid) == json_type_int
        && json_object_get_type(j_uid) == json_type_int) {
        gid = json_object_get_int(j_gid);
        uid = json_object_get_int(j_uid);
    }
    else
        mx_bad_request(NULL, NULL);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";
    if (mx_delete_user_from_group(connect->db, uid, gid) == -1)
        return "{\"code\": 500}";
    return "{\"code\": 200}";
}
