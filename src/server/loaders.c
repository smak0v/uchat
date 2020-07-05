#include "server.h"

// static void free_arrays(t_ld_d *arrays) {
//     free(arrays->dialog_id);
//     free(arrays->user_id);
//     free(arrays->username);
//     free(arrays);
// }

static t_ld_d *create_arrays(t_list *lst, int len) {
    t_ld_d *arrays = malloc(sizeof(t_ld_d));
    t_dialog_users *dialogue = lst->data;
    int *dids = malloc(sizeof(int) * len);
    int *uids = malloc(sizeof(int) * len);
    char **names = malloc(sizeof(char *) * len);

    for (int i = 0; i < len; i++) {
        dids[i] = dialogue->dialog_id;
        uids[i] = dialogue->user_id;
        names[i] = dialogue->username;
        lst = lst->next;
        dialogue = lst->data;
    }

    arrays->dialog_id = dids;
    arrays->user_id = uids;
    arrays->username = names;

    return arrays;
}

char *mx_load_dialogues(void *jobj, t_comm *connect) {
    json_object *j_uid = NULL;
    t_list *d_lst = NULL;
    t_ld_d *arrays = NULL;
    char *res = NULL;
    int len = 0;
    int uid = 0;

    json_object_object_get_ex(jobj, "id", &j_uid);
    if (!j_uid || json_object_get_type(j_uid) != json_type_int)
        return mx_bad_request(NULL, NULL);

    uid = json_object_get_int(j_uid);

    if (mx_validate_token(connect->db, uid, (json_object *)jobj))
        return "{\"code\": 401}";

    d_lst = mx_get_dialog_users(connect->db, uid, &len);
    arrays = create_arrays(d_lst, len);
    // if (d_lst)
    //     mx_delete_list(d_lst);
    res = mx_json_string_dlg(arrays, len);
    // free_arrays(arrays);
    return res;
}
