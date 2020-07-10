#include "server.h"

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

t_gr_members *mx_get_by_group_mem_id(sqlite3 *db, int gr_member_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS \
                       WHERE GROUP_MEMBERS_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, gr_member_id);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_member(stmt);
}

t_gr_members *mx_get_by_user_id(sqlite3 *db, int user_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS \
                       WHERE USER_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_member(stmt);
}

t_gr_members *mx_get_by_group_id(sqlite3 *db, int group_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS \
                       WHERE GROUP_ID = ?1", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, group_id);

    if (rv != SQLITE_OK)
        return NULL;

    return for_get_member(stmt);
}