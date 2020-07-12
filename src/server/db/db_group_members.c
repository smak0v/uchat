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

t_list *mx_get_groups(sqlite3 *db, int user_id, int *len) {
    sqlite3_stmt *stmt;
    t_list *groups = NULL;
    t_all_gr_member *tmp = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS " \
                       "WHERE USER_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, user_id);

    while(sqlite3_step(stmt) == SQLITE_ROW) {
        tmp = malloc(sizeof(t_all_gr_member));

        tmp->group_id = sqlite3_column_int(stmt, 2);
        tmp->group_name = mx_get_group_name_by_id(db, tmp->group_id);
        mx_push_back(&groups, tmp);
        *len += 1;
    }

    sqlite3_finalize(stmt);

    return groups;
}

int mx_add_group_member(sqlite3 *db, int user_id, int group_id, bool adm) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    if (mx_check_group_member(db, user_id, group_id) > 0)
        return -2;
    rv = sqlite3_prepare_v2(db,
        "INSERT INTO GROUP_MEMBERS(USER_ID, GROUP_ID, ADMIN)"\
        "VALUES(?1, ?2, ?3);", -1, &stmt, NULL);
    if (rv == SQLITE_ERROR)
        return -1;

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, group_id);
    sqlite3_bind_int(stmt, 3, adm);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;
    sqlite3_finalize(stmt);
    return mx_check_group_member(db, user_id, group_id);
}

int mx_check_group_member(sqlite3 *db, int user_id, int group_id) {
    sqlite3_stmt *stmt = NULL;
    int grp_mem = -1;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS WHERE "
            "USER_ID = ?1 AND GROUP_ID = ?2", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, group_id);

    if (rv != SQLITE_OK)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW)
        if (rv == SQLITE_ERROR)
            return -1;

    grp_mem = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    return grp_mem == 0 ? -1 : grp_mem;
}

t_list *mx_get_all_group_members(sqlite3 *db, int group_mem_id) {
    sqlite3_stmt *stmt;
    t_list *g = NULL;

    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS " \
                       "WHERE GROUP_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, group_mem_id);
    while(sqlite3_step(stmt) == SQLITE_ROW) {
        t_gr_members *d = malloc(sizeof(t_gr_members));

        d->gr_members_id = sqlite3_column_int(stmt, 0);
        d->user_id = sqlite3_column_int(stmt, 1);
        d->group_id = sqlite3_column_int(stmt, 2);
        d->adm = sqlite3_column_int(stmt, 3);
        mx_push_back(&g, d);
    }
    sqlite3_finalize(stmt);
    return g;
}
