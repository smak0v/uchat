#include "server.h"

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
