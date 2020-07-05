#include "server.h"

int *mx_get_all_user_groups_member(sqlite3 *db, int user_id) { 
    sqlite3_stmt *stmt;
    int *groups = malloc(2 * sizeof(int));
    int n = 0;

    for (int i = 0; i < 2; i++)
        groups[i] = -1;
    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS " \
                        "WHERE USER_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        groups = realloc(groups, (sizeof(int) * (n + 1)));
        groups[n++] = sqlite3_column_int(stmt, 2);
    }
    sqlite3_finalize(stmt);
    if (n < 1)
        return groups;
    groups[n] = -1;
    return groups;
}

t_list *mx_get_groups(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt;
    t_list *groups = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS " \
                        "WHERE USER_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_all_gr_member *tmp = malloc(sizeof(t_all_gr_member));

        tmp->group_id = sqlite3_column_int(stmt, 2);
        tmp->group_name = mx_get_group_name_by_id(db, tmp->group_id);
        mx_push_back(&groups, tmp);
    }

    sqlite3_finalize(stmt);

    return groups;
}
