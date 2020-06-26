#include "uchat.h"

int mx_delete_user_from_group(sqlite3 *db, int user_id, int group_id) {
    sqlite3_stmt *stmt = NULL;
    int rv = sqlite3_prepare_v2(db,
            "DELETE FROM GROUP_MEMBERS WHERE USER_ID = ?1"\
            " AND GROUP_ID = ?2", -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, user_id);
    sqlite3_bind_int(stmt, 2, group_id);

    if (rv == SQLITE_ERROR)
        return -1;

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);

    return 0;
}

int mx_change_admin_status(sqlite3 *db, int user_id, int group_id, bool adm) {
    sqlite3_stmt *stmt = NULL;
    int rv = 0;

    if (mx_get_group_member_by_user_id(db, user_id) < 0)
        return -2;
    rv = sqlite3_prepare_v2(db, "UPDATE GROUP_MEMBERS SET ADMIN = ?1 "\
        "WHERE USER_ID = ?2 AND GROUP_ID = ?3;", -1, &stmt, NULL);
    if (rv == SQLITE_ERROR)
        return -1;
    sqlite3_bind_int(stmt, 1, adm);
    sqlite3_bind_int(stmt, 2, user_id);
    sqlite3_bind_int(stmt, 3, group_id);

    if ((rv = sqlite3_step(stmt)) != SQLITE_DONE)
        return -1;

    sqlite3_finalize(stmt);
    return 0;
}

int mx_get_size_table(sqlite3 *db, char *table) {
    sqlite3_stmt *stmt;
    int size = 0;
    char *str = mx_strjoin("SELECT * FROM ", table);
    int rv;

    rv = sqlite3_prepare_v2(db, str, -1, &stmt, NULL);

    while (sqlite3_step(stmt) == SQLITE_ROW)
        size++;

    sqlite3_finalize(stmt);
    sqlite3_free(str);

    return size;
}

int *mx_get_all_id_group_members(sqlite3 *db, int group_id) {
    sqlite3_stmt *stmt;
    int size = mx_get_size_table(db, "GROUP_MEMBERS");
    int *members = malloc(size * sizeof(int));
    int i = 0;

    sqlite3_prepare_v2(db, "SELECT * FROM GROUP_MEMBERS \
                       WHERE GROUP_ID = ?1", -1, &stmt, NULL);
    sqlite3_bind_int(stmt, 1, group_id);
    while(sqlite3_step(stmt) == SQLITE_ROW)
        members[i++] = sqlite3_column_int(stmt, 1);
    members[i] = -1;
    sqlite3_finalize(stmt);

    return members;
}
