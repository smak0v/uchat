#include "uchat.h"

static t_gr_members *for_get_member(sqlite3_stmt *stmt) {
    t_gr_members *mem = NULL;
    int rv = 0;

    if ((rv = sqlite3_step(stmt)) != SQLITE_ROW) {
        if (rv == SQLITE_ERROR)
            return NULL;
        sqlite3_finalize(stmt);
        return NULL;
    }
    mem = malloc(sizeof(t_gr_members));
    mem->gr_members_id = sqlite3_column_int(stmt, 0);
    mem->user_id = sqlite3_column_int(stmt, 1);
    mem->group_id = sqlite3_column_int(stmt, 2);
    mem->adm = sqlite3_column_int(stmt, 3);
    sqlite3_finalize(stmt);

    return mem;
}

int mx_add_group_member(sqlite3 *db, int user_id, int group_id, bool adm) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    if (mx_get_group_member_by_user_id(db, user_id) > 0)
        return -2;
    rv = sqlite3_prepare_v2(db,
        "INSERT INTO GROUP_MEMBERS(USER_ID, GROUP_ID, ADMIN)"\
        "VALUES(?1, ?2, ?3);", -1, &stmt, NULL);
    if (rv == SQLITE_ERROR)
        return -2;

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, group_id);
    sqlite3_bind_int(stmt, 3, adm);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;
    sqlite3_finalize(stmt);
    return mx_get_group_member_by_user_id(db, user_id);
}

t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, int gr_member_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    sqlite3_prepare_v2(db,
                       "SELECT * FROM GROUP_MEMBERS \
                       WHERE GROUP_MEMBERS_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, gr_member_id);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_member(stmt);
}

int mx_get_group_member_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int grp_mem = -1;
    int rv = sqlite3_prepare_v2(db,
            "SELECT * FROM GROUP_MEMBERS WHERE USER_ID = ?1",
            -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

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
    if (g == NULL)
        return NULL;

    return g;
}
